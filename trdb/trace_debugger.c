/*
 * trdb - Trace Debugger Software for the PULP platform
 *
 * Copyright (C) 2018 ETH Zurich and University of Bologna
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <endian.h> /* Non-standard (posix), use <arpa/inet.h> instead? */
#include <stdbool.h>
#include <inttypes.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include <errno.h>
#include "trace_debugger.h"
#include "disassembly.h"
#include "serialize.h"
#include "list.h"
#include "utils.h"

struct disassembler_unit;

struct trdb_config;
struct trdb_state;
struct branch_map_state;
struct filter_state;

/* Configuration state of the trace debugger, used to guide the compression and
 * decompression routine
 */
struct trdb_config {
    /* TODO: Unused, inspect full-address, iaddress-lsb-p, implicit-except,
     * set-trace
     */
    uint64_t resync_max;
    /* bool iaddress_lsb_p; */
    /* bool implicit_except; */
    /* bool set_trace; */

    /* collect as much information as possible, but slower execution */
    bool full_statistics;

    /* set to true to always use absolute addresses in packets, this is used for
     * decompression and compression
     */
    bool full_address;
    /* do the sign extension of addresses like PULP. Only relevant if
     * full_address = false
     */
    bool use_pulp_sext;
    /* Don't regard ret's as unpredictable discontinuity */
    bool implicit_ret;
    /* Use additional packets to jump over vector table */
    bool pulp_vector_table_packet;
    /* whether we compress full branch maps */
    bool compress_full_branch_map;

    /* TODO: move this */
    /* set to true to always diassemble to most general representation */
    bool no_aliases;
};

/* Records the state of the CPU. The compression routine looks at a sequence of
 * recorded or streamed instructions (struct tr_instr) to figure out the
 * entries of this struct. Based on those it decides when to emit packets.
 */
struct trdb_state {

    bool halt;           /* TODO: update this */
    bool unhalted;       /* TODO: handle halt? */
    bool context_change; /* TODO: ?? */

    bool qualified;
    bool unqualified;
    bool exception;
    bool unpred_disc; /* unpredicted discontinuity*/
    bool emitted_exception_sync;

    uint32_t privilege;
    bool privilege_change;

    struct tr_instr instr;
};

/* Responsible to hold current branch state, that is the sequence of taken/not
 * taken branches so far. The bits field keeps track of that by setting the
 * cnt'th bit to 1 or 0 for a taken or not taken branch respectively. There can
 * be at most 31 entries in the branch map. A full branch map has the full flag
 * set.
 */
struct branch_map_state {
    bool full;
    uint32_t bits;
    uint32_t cnt;
};

/* We don't want to record all the time and this struct is used to indicate when
 * we do.
 */
struct filter_state {

    /* if we should output periodic timestamps (not implemented) */
    bool enable_timestamps;

    /* for tracing only certain privilege levels (not implemented) */
    bool trace_privilege;
    uint32_t privilege;

    /* TODO: look at those variables */
    uint64_t resync_cnt;
    bool resync_pend;
    /* uint32_t resync_nh = 0;  */
};

/* Current state of the cpu during decompression. Allows one to
 * precisely emit a sequence  tr_instr. Handles the exception stack
 * (well here we assume that programs actually do nested exception
 * handling properly) and hardware loops (only the TODO: immediate
 * version for now).
 */
/* TODO: Note on hw loops interrupted by an interrupt: look at after
 * the interrupt how many instructions of the loop are executed to
 * figure out in which loop number we got interrupted
 */
struct trdb_dec_state {
    /* absolute loop addresses */
    /* uint32_t lp_start0; */
    /* uint32_t lp_end0; */
    /* uint32_t lp_cnt0; */
    /* uint32_t lp_start1; */
    /* uint32_t lp_end1; */
    /* uint32_t lp_cnt1; */
    /* nested interrupt stacks for each privilege mode*/
    /* uint32_t m_exc_stack; */
    /* uint32_t s_exc_stack; */
    /* uint32_t u_exc_stack; */
    /* record current privilege level */
    uint32_t privilege : PRIVLEN;
    uint32_t last_packet_addr;
    struct branch_map_state branch_map;
};

struct trdb_stats {
    size_t payloadbits;
    size_t packetbits;
    size_t pulpbits;
    size_t instrbits;
    size_t instrs;
    size_t packets;
    size_t zo_addresses;  /**< all zero or all ones addresses */
    size_t zo_branchmaps; /**< all zero or all ones branchmaps */
    size_t addr_only_packets;
    size_t exception_packets;
    size_t start_packets;
    size_t diff_packets;
    size_t abs_packets;
    size_t bmap_full_packets;
    size_t bmap_full_addr_packets;
    uint32_t sext_bits[32];
};

/* Library context, needs to be passed to most function calls.
 * TODO: don't pass everything via stack
 * TODO: add comments
 */
struct trdb_ctx {
    /* specific settings for compression/decompression */
    struct trdb_config config;
    /* state used for the compression/decompression */
    struct trdb_state lastc;
    struct trdb_state thisc;
    struct trdb_state nextc;
    struct branch_map_state branch_map;
    struct filter_state filter;
    uint32_t last_iaddr; /* TODO: make this work with 64 bit */
    /* state used for disassembling */
    struct tr_instr *dis_instr;
    struct disassembler_unit *dunit;
    /* compression statistics */
    struct trdb_stats stats;
    /* desired logging level and custom logging hook*/
    int log_priority;
    void (*log_fn)(struct trdb_ctx *ctx, int priority, const char *file,
                   int line, const char *fn, const char *format, va_list args);
};

void trdb_log(struct trdb_ctx *ctx, int priority, const char *file, int line,
              const char *fn, const char *format, ...)
{
    va_list args;

    va_start(args, format);
    ctx->log_fn(ctx, priority, file, line, fn, format, args);
    va_end(args);
}

static void log_stderr(struct trdb_ctx *ctx, int priority, const char *file,
                       int line, const char *fn, const char *format,
                       va_list args)
{
    fprintf(stderr, "trdb: %s:%d:0: %s(): ", file, line, fn);
    vfprintf(stderr, format, args);
}

static int log_priority(const char *priority)
{
    char *endptr;
    int prio;

    prio = strtol(priority, &endptr, 10);
    if (endptr[0] == '\0' || isspace(endptr[0]))
        return prio;
    if (strncmp(priority, "err", 3) == 0)
        return LOG_ERR;
    if (strncmp(priority, "info", 4) == 0)
        return LOG_INFO;
    if (strncmp(priority, "debug", 5) == 0)
        return LOG_DEBUG;
    return 0;
}

void trdb_reset_compression(struct trdb_ctx *ctx)
{
    ctx->config = (struct trdb_config){.resync_max = UINT64_MAX,
                                       .full_address = true,
                                       .no_aliases = true,
                                       .pulp_vector_table_packet = true,
                                       .full_statistics = true};

    ctx->lastc = (struct trdb_state){.privilege = 7};
    ctx->thisc = (struct trdb_state){.privilege = 7};
    ctx->nextc = (struct trdb_state){.privilege = 7};
    ctx->branch_map = (struct branch_map_state){0};
    ctx->filter = (struct filter_state){0};
    ctx->last_iaddr = 0;
    ctx->stats = (struct trdb_stats){0};
}

void trdb_reset_decompression(struct trdb_ctx *ctx)
{
    ctx->config = (struct trdb_config){.resync_max = UINT64_MAX,
                                       .full_address = true,
                                       .no_aliases = true,
                                       .pulp_vector_table_packet = true,
                                       .full_statistics = true};

    ctx->branch_map = (struct branch_map_state){0};
    ctx->filter = (struct filter_state){0};
    ctx->stats = (struct trdb_stats){0};
}

struct trdb_ctx *trdb_new()
{
    const char *env;
    struct trdb_ctx *ctx = malloc(sizeof(*ctx));
    if (!ctx)
        return NULL;

    ctx->config = (struct trdb_config){.resync_max = UINT64_MAX,
                                       .full_address = true,
                                       .no_aliases = true,
                                       .pulp_vector_table_packet = true,
                                       .full_statistics = true};

    ctx->lastc = (struct trdb_state){.privilege = 7};
    ctx->thisc = (struct trdb_state){.privilege = 7};
    ctx->nextc = (struct trdb_state){.privilege = 7};
    ctx->branch_map = (struct branch_map_state){0};
    ctx->filter = (struct filter_state){0};
    ctx->last_iaddr = 0;
    ctx->stats = (struct trdb_stats){0};

    ctx->log_fn = log_stderr;
    ctx->log_priority = LOG_ERR; // TODO: change that

    /* environment overwrites config */
    env = secure_getenv("TRDB_LOG");
    if (env != NULL)
        trdb_set_log_priority(ctx, log_priority(env));

    info(ctx, "ctx %p created\n", ctx);
    dbg(ctx, "log_priority=%d\n", ctx->log_priority);

    return ctx;
}

void trdb_free(struct trdb_ctx *ctx)
{
    if (!ctx)
        return;
    info(ctx, "context %p released\n", ctx);
    free(ctx);
}

void trdb_set_log_fn(struct trdb_ctx *ctx,
                     void (*log_fn)(struct trdb_ctx *ctx, int priority,
                                    const char *file, int line, const char *fn,
                                    const char *format, va_list args))
{
    ctx->log_fn = log_fn;
    info(ctx, "custom logging function %p registered\n", log_fn);
}

int trdb_get_log_priority(struct trdb_ctx *ctx)
{
    return ctx->log_priority;
}

void trdb_set_log_priority(struct trdb_ctx *ctx, int priority)
{
    ctx->log_priority = priority;
}

void trdb_set_dunit(struct trdb_ctx *ctx, struct disassembler_unit *dunit)
{
    ctx->dunit = dunit;
}

struct disassembler_unit *trdb_get_dunit(struct trdb_ctx *ctx)
{
    return ctx->dunit;
}

void trdb_set_full_address(struct trdb_ctx *ctx, bool v)
{
    ctx->config.full_address = v;
}

bool trdb_is_full_address(struct trdb_ctx *ctx)
{
    return ctx->config.full_address;
}

void trdb_set_implicit_ret(struct trdb_ctx *ctx, bool implicit_ret)
{
    ctx->config.implicit_ret = implicit_ret;
}

bool trdb_get_implicit_ret(struct trdb_ctx *ctx)
{
    return ctx->config.implicit_ret;
}

void trdb_set_pulp_extra_packet(struct trdb_ctx *ctx, bool extra_packet)
{
    ctx->config.pulp_vector_table_packet = extra_packet;
}

bool trdb_get_pulp_extra_packet(struct trdb_ctx *ctx)
{
    return ctx->config.pulp_vector_table_packet;
}

void trdb_set_compress_branch_map(struct trdb_ctx *ctx, bool compress)
{
    ctx->config.compress_full_branch_map = compress;
}

bool trdb_get_compress_branch_map(struct trdb_ctx *ctx)
{

    return ctx->config.compress_full_branch_map;
}

size_t trdb_get_payloadbits(struct trdb_ctx *ctx)
{
    return ctx->stats.payloadbits;
}

size_t trdb_get_pulpbits(struct trdb_ctx *ctx)
{
    return ctx->stats.pulpbits;
}

size_t trdb_get_packetcnt(struct trdb_ctx *ctx)
{
    return ctx->stats.packets;
}

size_t trdb_get_instrcnt(struct trdb_ctx *ctx)
{
    return ctx->stats.instrs;
}

size_t trdb_get_instrbits(struct trdb_ctx *ctx)
{
    return ctx->stats.instrbits;
}

void trdb_get_packet_stats(struct trdb_ctx *ctx,
                           struct trdb_packet_stats *stats)
{
    stats->packets = ctx->stats.packets;
    stats->addr_only_packets = ctx->stats.addr_only_packets;
    stats->exception_packets = ctx->stats.exception_packets;
    stats->start_packets = ctx->stats.start_packets;
    stats->diff_packets = ctx->stats.diff_packets;
    stats->abs_packets = ctx->stats.abs_packets;
    stats->bmap_full_packets = ctx->stats.bmap_full_packets;
    stats->bmap_full_addr_packets = ctx->stats.bmap_full_addr_packets;
}

static bool is_branch(uint32_t instr)
{
    bool is_riscv_branch = is_beq_instr(instr) || is_bne_instr(instr) ||
                           is_blt_instr(instr) || is_bge_instr(instr) ||
                           is_bltu_instr(instr) || is_bgeu_instr(instr);
    bool is_pulp_branch = is_p_bneimm_instr(instr) || is_p_beqimm_instr(instr);
    bool is_riscv_compressed_branch =
        is_c_beqz_instr(instr) || is_c_bnez_instr(instr);
    return is_riscv_branch || is_pulp_branch || is_riscv_compressed_branch;
    /* auipc */
}

static bool branch_taken(bool before_compressed, uint32_t addr_before,
                         uint32_t addr_after)
{
    /* TODO: this definitely doens't work for 64 bit instructions */
    /* since we have already decompressed instructions, but still compressed
     * addresses we need this additional flag to tell us what the isntruction
     * originally was. So we can't tell by looking at the lower two bits of
     * instr.
     */
    return before_compressed ? !(addr_before + 2 == addr_after)
                             : !(addr_before + 4 == addr_after);
}

static uint32_t branch_map_len(uint32_t branches)
{
    if (branches == 0) {
        return 31;
    } else if (branches <= 1) {
        return 1;
    } else if (branches <= 9) {
        return 9;
    } else if (branches <= 17) {
        return 17;
    } else if (branches <= 25) {
        return 25;
    } else if (branches <= 31) {
        return 31;
    }
    return -1;
}

/* Some jumps can't be predicted i.e. the jump address can only be figured out
 * at runtime. That happens e.g. if the target address depends on some register
 * entries. For plain RISC-V I this is just the jalr instruction. For the PULP
 * extensions, the hardware loop instruction have to be considered too.
 */
static bool is_unpred_discontinuity(uint32_t instr, bool implicit_ret)
{
    bool jump = is_jalr_instr(instr) || is_really_c_jalr_instr(instr) ||
                is_really_c_jr_instr(instr);
    bool exception_ret =
        is_mret_instr(instr) || is_sret_instr(instr) || is_uret_instr(instr);

    /* this allows us to mark ret's as not being discontinuities, if we want */
    bool not_ret = true;
    if (implicit_ret)
        not_ret = !(is_c_ret_instr(instr) || is_ret_instr(instr));

    return (jump || exception_ret) && not_ret;
}

/* Just crash and error if we hit one of those */
static bool is_unsupported(uint32_t instr)
{
    return is_lp_setup_instr(instr) || is_lp_counti_instr(instr) ||
           is_lp_count_instr(instr) || is_lp_endi_instr(instr) ||
           is_lp_starti_instr(instr) || is_lp_setupi_instr(instr);
}

static uint32_t sign_extendable_bits64(uint64_t addr)
{
    int clz = __builtin_clzll(addr);
    int clo = __builtin_clzll(~addr);
    return clz > clo ? clz : clo;
}

static uint32_t sign_extendable_bits(uint32_t addr)
{
    int clz = __builtin_clz(addr);
    int clo = __builtin_clz(~addr);
    return clz > clo ? clz : clo;
}
/* Sometimes we have to decide whether to put in the absolute or differential
 * address into the packet. We choose the one which has the least amount of
 * meaningfull bits, i.e. the bits that can't be inferred by sign-extension.
 */
static bool differential_addr(int *lead, uint32_t absolute,
                              uint32_t differential)
{
    int abs = sign_extendable_bits(absolute);
    int diff = sign_extendable_bits(differential);

    /* /\* on tie we probe which one would be better *\/ */
    /* if ((abs == 32) && (diff == 32)) { */
    /*     if ((abs & 1) == last) { */
    /*         *lead = 0; */
    /*         return prefer_abs; */
    /*     } else if ((diff & 1) == last) { */
    /*         *lead = 0; */
    /*         return prefer_diff; */
    /*     } else { */
    /*         *lead = 1; */
    /*         return prefer_abs; */
    /*     } */
    /* } */

    /* /\* check if we can sign extend from the previous byte *\/ */
    /* if (abs == 32) { */
    /*     if ((abs & 1) == last_bit) */
    /*         *lead = 0; */
    /*     else */
    /*         *lead = 1; */
    /*     return prefer_abs; */
    /* } */

    /* if (diff == 32) { */
    /*     if ((diff & 1) == last_bit) */
    /*         *lead = 0; */
    /*     else */
    /*         *lead = 1; */
    /*     return prefer_diff; */
    /* } */

    /* general case */
    *lead = diff > abs ? diff : abs;
    return diff > abs; /* on tie we prefer absolute */
}

static int quantize_clz(int x)
{

    if (x < 9)
        return 0;
    else if (x < 17)
        return 9;
    else if (x < 25)
        return 17;
    else
        return 25;
}

/* Does the same as differential_addr() but only considers byte boundaries */
static bool pulp_differential_addr(int *lead, uint32_t absolute,
                                   uint32_t differential)
{
    int abs = sign_extendable_bits(absolute);
    int diff = sign_extendable_bits(differential);

    /* /\* on tie we probe which one would be better *\/ */
    /* if ((abs == 32) && (diff == 32)) { */
    /*     if ((abs & 1) == last) { */
    /*         *lead = 0; */
    /*         return prefer_abs; */
    /*     } else if ((diff & 1) == last) { */
    /*         *lead = 0; */
    /*         return prefer_diff; */
    /*     } else { */
    /*         *lead = 1; */
    /*         return prefer_abs; */
    /*     } */
    /* } */

    /* /\* check if we can sign extend from the previous byte *\/ */
    /* if (abs == 32) { */
    /*     if ((abs & 1) == last_bit) */
    /*         *lead = 0; */
    /*     else */
    /*         *lead = 1; */
    /*     return prefer_abs; */
    /* } */

    /* if (diff == 32) { */
    /*     if ((diff & 1) == last_bit) */
    /*         *lead = 0; */
    /*     else */
    /*         *lead = 1; */
    /*     return prefer_diff; */
    /* } */
    /* we are only interested in sign extension for byte boundaries */
    abs = quantize_clz(abs);
    diff = quantize_clz(diff);
    assert(abs != 32); /* there is always a one or zero leading */

    /* general case */
    *lead = diff > abs ? diff : abs;
    return diff > abs; /* on tie we prefer absolute */
}

static void emit_exception_packet(struct trdb_ctx *c, struct tr_packet *tr,
                                  struct tr_instr *lc_instr,
                                  struct tr_instr *tc_instr,
                                  struct tr_instr *nc_instr)
{
    tr->format = F_SYNC;          /* sync */
    tr->subformat = SF_EXCEPTION; /* exception */
    tr->context = 0;              /* TODO: what comes here? */
    tr->privilege = tc_instr->priv;

    if (is_branch(tc_instr->instr) &&
        !branch_taken(tc_instr->compressed, tc_instr->iaddr, nc_instr->iaddr))
        tr->branch = 1;
    else
        tr->branch = 0;

    tr->address = tc_instr->iaddr;
    /* With this packet we record last cycles exception
     * information. It's not possible for (i==0 &&
     * lastc_exception) to be true since it takes one cycle
     * for lastc_exception to change
     */
    tr->ecause = lc_instr->cause;
    tr->interrupt = lc_instr->interrupt;
    tr->tval = lc_instr->tval;
    tr->length = FORMATLEN + FORMATLEN + PRIVLEN + 1 + XLEN + CAUSELEN + 1;
    c->stats.exception_packets++;
}

static void emit_start_packet(struct trdb_ctx *c, struct tr_packet *tr,
                              struct tr_instr *tc_instr,
                              struct tr_instr *nc_instr)
{
    tr->format = F_SYNC;      /* sync */
    tr->subformat = SF_START; /* start */
    tr->context = 0;          /* TODO: what comes here? */
    tr->privilege = tc_instr->priv;
    if (is_branch(tc_instr->instr) &&
        !branch_taken(tc_instr->compressed, tc_instr->iaddr, nc_instr->iaddr))
        tr->branch = 1;
    else
        tr->branch = 0;
    tr->address = tc_instr->iaddr;
    tr->length = FORMATLEN + FORMATLEN + PRIVLEN + 1 + XLEN;
    c->stats.start_packets++;
}

static uint32_t sext32(uint32_t val, uint32_t bit)
{
    if (bit == 0)
        return 0;

    if (bit == 32)
        return val;

    int m = 1U << (bit - 1);

    val = val & ((1U << bit) - 1);
    return (val ^ m) - m;
}

static int emit_branch_map_flush_packet(struct trdb_ctx *ctx,
                                        struct tr_packet *tr,
                                        struct branch_map_state *branch_map,
                                        struct tr_instr *tc_instr,
                                        uint32_t last_iaddr, bool full_address,
                                        bool is_u_discontinuity)
{
    if (!ctx || !tr || !branch_map || !tc_instr)
        return -trdb_internal;

    if (branch_map->cnt == 0) {
        tr->format = F_ADDR_ONLY;
        tr->branches = branch_map->cnt;

        if (full_address) {
            tr->address = tc_instr->iaddr;
            tr->length = FORMATLEN + XLEN;
        } else {
            /* always differential in F_ADDR_ONLY*/
            uint32_t diff = last_iaddr - tc_instr->iaddr;
            uint32_t lead = ctx->config.use_pulp_sext
                                ? quantize_clz(sign_extendable_bits(diff))
                                : sign_extendable_bits(diff);

            uint32_t keep = XLEN - lead + 1;
            /* should only be relevant for serialization */
            /* tr->address = MASK_FROM(keep) & diff; */
            tr->address = diff;
            tr->length = FORMATLEN + keep;
            /* record distribution */
            ctx->stats.sext_bits[keep - 1]++;
            if (tr->address == 0 || tr->address == -1)
                ctx->stats.zo_addresses++;
        }
        ctx->stats.addr_only_packets++;
        assert(branch_map->bits == 0);
    } else {
        if (branch_map->full && is_u_discontinuity)
            dbg(ctx, "full branch map and discontinuity edge case\n");

        tr->branches = branch_map->cnt;

        if (full_address) {
            tr->format = F_BRANCH_FULL;
            tr->address = tc_instr->iaddr;
            tr->length =
                FORMATLEN + BRANCHLEN + branch_map_len(branch_map->cnt);
            if (branch_map->full) {
                if (is_u_discontinuity) {
                    tr->length += XLEN;
                    ctx->stats.bmap_full_addr_packets++;
                } else {
                    /* we don't need to record the address, indicate by settings
                     * branchcnt to 0
                     */
                    tr->length += 0;
                    tr->branches = 0;
                }
                ctx->stats.bmap_full_packets++;
            } else {
                tr->length += XLEN;
                ctx->stats.abs_packets++;
            }
        } else {
            /* In this mode we try to compress the instruction address by taking
             * the difference address or the absolute address, whichever has
             * more bits which can be inferred by signextension.
             */
            uint32_t diff = last_iaddr - tc_instr->iaddr;
            uint32_t full = tc_instr->iaddr;
            uint32_t keep = 0;
            int lead = 0;
            bool use_differential =
                ctx->config.use_pulp_sext
                    ? pulp_differential_addr(&lead, full, diff)
                    : differential_addr(&lead, full, diff);

            if (use_differential) {
                keep = XLEN - lead + 1;
                tr->format = F_BRANCH_DIFF;
                /* this should only be relevant for serialization */
                /* tr->address = MASK_FROM(keep) & diff; */
                tr->address = diff;
                ctx->stats.sext_bits[keep - 1]++;
            } else {
                keep = XLEN - lead + 1;
                tr->format = F_BRANCH_FULL;
                /* this should only be relevant for serialization */
                /* tr->address = MASK_FROM(keep) & full; */
                tr->address = full;
                ctx->stats.sext_bits[keep - 1]++;
            }

            if (tr->address == 0 || tr->address == -1)
                ctx->stats.zo_addresses++;
            uint32_t sext = sign_extendable_bits64(
                ((uint64_t)tr->address << XLEN) |
                ((uint64_t)branch_map->bits
                 << (XLEN - branch_map_len(branch_map->cnt))));
            if (sext > XLEN + branch_map_len(branch_map->cnt))
                sext = XLEN + branch_map_len(branch_map->cnt);
            uint32_t ext = XLEN + branch_map_len(branch_map->cnt) - sext + 1;
            tr->length =
                FORMATLEN + BRANCHLEN + branch_map_len(branch_map->cnt);
            /* tr->length = FORMATLEN + BRANCHLEN */
            /* 	+ext; */

            if (branch_map->full) {
                if (is_u_discontinuity) {
                    tr->length += keep;
                    ctx->stats.bmap_full_addr_packets++;
                } else {
                    /* we don't need to record the address, indicate by settings
                     * branchcnt to 0
                     */
                    tr->length += 0;
                    tr->branches = 0;
                    ctx->stats.bmap_full_packets++;
                }
            } else {
                tr->length += keep;
                if (use_differential)
                    ctx->stats.diff_packets++;
                else
                    ctx->stats.abs_packets++;
            }
        }
        tr->branch_map = branch_map->bits;
        *branch_map = (struct branch_map_state){0};
    }

    return 0;
}

static void emit_full_branch_map(struct trdb_ctx *ctx, struct tr_packet *tr,
                                 struct branch_map_state *branch_map)
{
    assert(branch_map->cnt == 31);
    tr->format = F_BRANCH_FULL;
    /* full branch map withouth address is indicated by settings branches to 0*/
    tr->branches = 0;
    tr->branch_map = branch_map->bits;
    /* No address needed */
    uint32_t sext = sign_extendable_bits(branch_map->bits << 1);
    if (sext > 31)
        sext = 31;
    if (ctx->config.compress_full_branch_map)
        tr->length = FORMATLEN + BRANCHLEN + (31 - sext + 1);
    else
        tr->length = FORMATLEN + BRANCHLEN + branch_map_len(31);
    *branch_map = (struct branch_map_state){0};

    ctx->stats.bmap_full_packets++;
}

/* helper macro to reduce boilerplate in trdb_compress_trace_step */
#define ALLOC_PACKET(name)                                                     \
    name = malloc(sizeof(*name));                                              \
    if (!name) {                                                               \
        status = -trdb_nomem;                                                  \
        goto fail_malloc;                                                      \
    }                                                                          \
    *name = (struct tr_packet){.msg_type = W_TRACE};

int trdb_compress_trace_step(struct trdb_ctx *ctx,
                             struct list_head *packet_list,
                             struct tr_instr *instr)
{
    int status = 0;
    int generated_packet = 0;
    bool full_address = ctx->config.full_address;
    bool pulp_vector_table_packet = ctx->config.pulp_vector_table_packet;
    bool implicit_ret = ctx->config.implicit_ret;

    /* for each cycle */
    // TODO: fix this hack by doing unqualified instead
    struct trdb_state *lastc = &ctx->lastc;
    struct trdb_state *thisc = &ctx->thisc;
    struct trdb_state *nextc = &ctx->nextc;

    nextc->instr = *instr;

    struct tr_instr *nc_instr = &ctx->nextc.instr;
    struct tr_instr *tc_instr = &ctx->thisc.instr;
    struct tr_instr *lc_instr = &ctx->lastc.instr;

    struct branch_map_state *branch_map = &ctx->branch_map;
    struct filter_state *filter = &ctx->filter;

    thisc->halt = false;
    /* test for qualification by filtering */
    /* TODO: implement filtering logic */

    nextc->qualified = true;
    nextc->unqualified = !nextc->qualified;
    nextc->exception = instr->exception;
    nextc->unpred_disc = is_unpred_discontinuity(instr->instr, implicit_ret);
    nextc->privilege = instr->priv;
    nextc->privilege_change = (thisc->privilege != nextc->privilege);

    /* last address we emitted in packet, needed to compute differential
     * addresses
     */
    uint32_t *last_iaddr = &ctx->last_iaddr;

    /* TODO: clean this up, proper initial state per round required */
    thisc->emitted_exception_sync = false;
    nextc->emitted_exception_sync = false;

    bool firstc_qualified = !lastc->qualified && thisc->qualified;

    /* Start of one cycle */
    if (!instr->valid) {
        /* Invalid interface data, just freeze state*/
        return 0;
    }

    if (!thisc->qualified) {
        /* check if we even need to record anything */
        *lastc = *thisc;
        *thisc = *nextc;
        return 0; /* end of cycle */
    }

    if (is_unsupported(tc_instr->instr)) {
        err(ctx,
            "Instruction is not supported for compression: 0x%" PRIx32
            " at addr: 0x%" PRIx32 "\n",
            tc_instr->instr, tc_instr->iaddr);
        status = -trdb_bad_instr;
        goto fail;
    }

    if (filter->resync_cnt++ == ctx->config.resync_max) {
        filter->resync_pend = true;
        filter->resync_cnt = 0;
    }

    if (is_branch(tc_instr->instr)) {
        /* update branch map */
        if (!branch_taken(tc_instr->compressed, tc_instr->iaddr,
                          nc_instr->iaddr))
            branch_map->bits = branch_map->bits | (1u << branch_map->cnt);
        branch_map->cnt++;
        if (branch_map->cnt == 31) {
            branch_map->full = true;
        }
    }

    struct tr_packet *tr = NULL;

    /* We trace the packet before the trapped instruction and the
     * first one of the exception handler
     */
    if (lastc->exception) {
        /* Send te_inst:
         * format 3
         * subformat 1 (exception -> all fields present)
         * resync_pend = 0
         */
        ALLOC_PACKET(tr);
        emit_exception_packet(ctx, tr, lc_instr, tc_instr, nc_instr);
        *last_iaddr = tc_instr->iaddr;

        list_add(&tr->list, packet_list);

        thisc->emitted_exception_sync = true;
        filter->resync_pend = false; /* TODO: how to handle this */

        generated_packet = 1;
        /* end of cycle */

    } else if (lastc->emitted_exception_sync && pulp_vector_table_packet) {
        /* First we assume that the vector table entry is a jump. Since that
         * entry can change during runtime, we need to emit the jump
         * destination address, which is the second instruction of the trap
         * handler. This a bit hacky and made to work for the PULP. If
         * someone puts something else than a jump instruction there then
         * all bets are off. This is a custom change.
         */
        /* Start packet */
        /* Send te_inst:
         * format 3
         * subformat 0 (start, no ecause, interrupt and tval)
         * resync_pend = 0
         */
        ALLOC_PACKET(tr);
        emit_start_packet(ctx, tr, tc_instr, nc_instr);
        *last_iaddr = tc_instr->iaddr;

        list_add(&tr->list, packet_list);

        filter->resync_pend = false;
        generated_packet = 1;

    } else if (firstc_qualified || thisc->unhalted || thisc->privilege_change ||
               (filter->resync_pend && branch_map->cnt == 0)) {

        /* Start packet */
        /* Send te_inst:
         * format 3
         * subformat 0 (start, no ecause, interrupt and tval)
         * resync_pend = 0
         */
        ALLOC_PACKET(tr);
        emit_start_packet(ctx, tr, tc_instr, nc_instr);
        *last_iaddr = tc_instr->iaddr;

        list_add(&tr->list, packet_list);

        filter->resync_pend = false;
        generated_packet = 1;

    } else if (lastc->unpred_disc) {
        /* Send te_inst:
         * format 0/1/2
         */
        ALLOC_PACKET(tr);
        status = emit_branch_map_flush_packet(ctx, tr, branch_map, tc_instr,
                                              *last_iaddr, full_address, true);
        if (status < 0) {
            generated_packet = 0;
            goto fail;
        }

        *last_iaddr = tc_instr->iaddr;

        list_add(&tr->list, packet_list);

        generated_packet = 1;

    } else if (filter->resync_pend && branch_map->cnt > 0) {
        /* we treat resync_pend && branches == 0 before */
        /* Send te_inst:
         * format 0/1/2
         */
        ALLOC_PACKET(tr);
        status = emit_branch_map_flush_packet(ctx, tr, branch_map, tc_instr,
                                              *last_iaddr, full_address, false);
        if (status < 0) {
            generated_packet = 0;
            goto fail;
        }

        *last_iaddr = tc_instr->iaddr;

        list_add(&tr->list, packet_list);

        generated_packet = 1;

    } else if (nextc->halt || nextc->exception || nextc->privilege_change ||
               nextc->unqualified) {
        /* Send te_inst:
         * format 0/1/2
         */
        ALLOC_PACKET(tr);
        status = emit_branch_map_flush_packet(ctx, tr, branch_map, tc_instr,
                                              *last_iaddr, full_address, false);
        if (status < 0) {
            generated_packet = 0;
            goto fail;
        }

        *last_iaddr = tc_instr->iaddr;

        list_add(&tr->list, packet_list);

        generated_packet = 1;

    } else if (branch_map->full) {
        /* Send te_inst:
         * format 0
         * no address
         */
        ALLOC_PACKET(tr);
        emit_full_branch_map(ctx, tr, branch_map);

        list_add(&tr->list, packet_list);

        generated_packet = 1;

    } else if (thisc->context_change) {
        /* Send te_inst:
         * format 3
         * subformat 2
         */
        err(ctx, "context_change not supported\n");
        status = -trdb_unimplemented;
        goto fail;
        /* ALLOC_INIT_PACKET(tr); */
        /* tr->format = F_SYNC; */
        /* tr->subformat = SF_CONTEXT; */
        /* tr->context = 0; /\* TODO: what comes here? *\/ */
        /* *last_iaddr = tr->address; */
        /* tr->privilege = tc_instr->priv; */
        /* /\* tr->branch *\/ */
        /* /\* tr->address *\/ */
        /* /\* tr->ecause *\/ */
        /* /\* tr->interrupt *\/ */
        /* /\* tr->tval *\/ */
        /* tr->length =  */
        /* list_add(&tr->list, packet_list); */

        /* generated_packet = 1; */
    }

    /* update last cycle state */
    *lastc = *thisc;
    *thisc = *nextc;

    /* TODO: no 64 bit instr support */
    ctx->stats.instrbits += instr->compressed ? 16 : 32;
    ctx->stats.instrs++;
    if (generated_packet) {
        *branch_map = (struct branch_map_state){0};
        ctx->stats.payloadbits += (tr->length);
        ctx->stats.packets++;

        if (ctx->config.full_statistics) {
            /* figure out pulp payload by serializing and couting bits */
            uint8_t bin[16] = {0};
            size_t bitcnt = 0;
            if (trdb_pulp_serialize_packet(ctx, tr, &bitcnt, 0, bin)) {
                dbg(ctx, "failed to count bits of pulp packet\n");
            }
            /* we have to round up to the next full byte */
            ctx->stats.pulpbits += ((bitcnt / 8) + (bitcnt % 8 != 0)) * 8;
        }
    }

    if (generated_packet) {
        trdb_log_packet(
            ctx, list_entry(packet_list->next, typeof(struct tr_packet), list));
    }

    if (ctx->dunit) {
        /* TODO: unfortunately this ignores log_fn */
        if (trdb_get_log_priority(ctx) == LOG_DEBUG) {
            trdb_disassemble_instr(instr, ctx->dunit);
        }
    } else {
        dbg(ctx, "0x%08jx  0x%08jx\n", (uintmax_t)instr->iaddr,
            (uintmax_t)instr->instr);
    }

fail:
fail_malloc:
    if (status == trdb_ok) {
        return generated_packet;
    } else
        return status;
}

static int read_memory_at_pc(bfd_vma pc, uint64_t *instr, unsigned int len,
                             struct disassemble_info *dinfo)
{
    bfd_byte packet[2];
    *instr = 0;
    bfd_vma n;
    int status;

    /* Instructions are a sequence of 2-byte packets in little-endian order.  */
    for (n = 0; n < sizeof(uint64_t) && n < riscv_instr_len(*instr); n += 2) {
        status = (*dinfo->read_memory_func)(pc + n, packet, 2, dinfo);
        if (status != 0) {
            /* Don't fail just because we fell off the end.  */
            if (n > 0)
                break;
            (*dinfo->memory_error_func)(status, pc, dinfo);
            return status;
        }

        (*instr) |= ((uint64_t)bfd_getl16(packet)) << (8 * n);
    }
    return status;
}

/* The decoding algorithm tries to recover as much information as possible from
 * the packets. Additionaly it also disassembles the instruction bits given the
 * context (address, binary).
 */
static int disassemble_at_pc(struct trdb_ctx *c, bfd_vma pc,
                             struct tr_instr *instr,
                             struct disassembler_unit *dunit, int *status)
{
    *status = 0;

    if (!c || !instr || !dunit) {
        *status = -trdb_invalid;
        return 0;
    }

    /* make sure start froma a clean slate */
    *instr = (struct tr_instr){0};

    struct disassemble_info *dinfo = dunit->dinfo;
    /* Important to set for internal calls to fprintf */
    c->dis_instr = instr;
    dinfo->stream = c;

    /* print instr address */
    (*dinfo->fprintf_func)(c, "0x%08jx  ", (uintmax_t)pc);

    /* check if insn_info works */
    dinfo->insn_info_valid = 0;

    int instr_size = (*dunit->disassemble_fn)(pc, dinfo);
    (*dinfo->fprintf_func)(c, "\n");
    if (instr_size <= 0) {
        err(c, "encountered instruction with %d bytes, stopping\n", instr_size);
        *status = -trdb_bad_instr;
        return 0;
    }
    if (!dinfo->insn_info_valid) {
        err(c, "encountered invalid instruction info\n");
        *status = -trdb_bad_instr;
        return 0;
    }
    uint64_t instr_bits = 0;
    if (read_memory_at_pc(pc, &instr_bits, 0, dinfo)) {
        err(c, "reading instr at pc failed\n");
        *status = -trdb_bad_instr;
        return 0;
    }

    instr->valid = true;
    instr->iaddr = pc;
    instr->instr = instr_bits;
    instr->compressed = instr_size == 2;
    /* instr->priv = 0 */
    return instr_size;
}

/* libopcodes only knows how to call a fprintf based callback function. We abuse
 * it by passing through the void pointer our custom data (instead of a stream).
 * This ugly hack doesn't seem to be used by just me.
 */
static int build_instr_fprintf(void *stream, const char *format, ...)
{
    struct trdb_ctx *c = stream;
    char tmp[INSTR_STR_LEN];
    va_list args;
    va_start(args, format);
    int rv = vsnprintf(tmp, INSTR_STR_LEN - 1, format, args);
    if (rv >= INSTR_STR_LEN) {
        err(c, "build_instr_fprintf output truncated, adjust buffer size\n");
    }
    if (rv < 0) {
        err(c, "Encountered an error in vsnprintf\n");
    }
    va_end(args);

    dbg(c, tmp);

    return rv;
}

static void free_section_for_debugging(struct disassemble_info *dinfo)
{
    if (!dinfo)
        return;
    free(dinfo->buffer);
    dinfo->buffer_vma = 0;
    dinfo->buffer_length = 0;
    dinfo->section = NULL;
}

static int alloc_section_for_debugging(struct trdb_ctx *c, bfd *abfd,
                                       asection *section,
                                       struct disassemble_info *dinfo)
{
    if (!dinfo || !section)
        return -trdb_invalid;

    bfd_size_type section_size = bfd_get_section_size(section);

    bfd_byte *section_data = malloc(section_size);
    if (!section_data)
        return -trdb_nomem;

    if (!bfd_get_section_contents(abfd, section, section_data, 0,
                                  section_size)) {
        err(c, "bfd_get_section_contents: %s\n", bfd_errmsg(bfd_get_error()));
        free(section_data);
        return -trdb_section_empty;
    }

    dinfo->buffer = section_data;
    dinfo->buffer_vma = section->vma;
    dinfo->buffer_length = section_size;
    dinfo->section = section;
    return 0;
}

static int add_trace(struct trdb_ctx *c, struct list_head *instr_list,
                     struct tr_instr *instr)
{
    struct tr_instr *add = malloc(sizeof(*add));
    if (!add)
        return -trdb_nomem;

    memcpy(add, instr, sizeof(*add));
    list_add(&add->list, instr_list);
    return 0;
}

/* TODO: packet wise decompression with error events */
int trdb_decompress_trace(struct trdb_ctx *c, bfd *abfd,
                          struct list_head *packet_list,
                          struct list_head *instr_list)
{
    int status = 0;
    if (!c || !abfd || !packet_list || !instr_list)
        return -trdb_invalid;

    /* We assume our hw block in the pulp generated little endian
     * addresses, thus we should make sure that before we interact
     * with bfd addresses to convert this foreign format to the local
     * host format
     */
    /* TODO: supports only statically linked elf executables */
    bool full_address = c->config.full_address;
    bool implicit_ret = c->config.implicit_ret;
    bool no_aliases = c->config.no_aliases;

    /* find section belonging to start_address */
    bfd_vma start_address = abfd->start_address;
    asection *section = trdb_get_section_for_vma(abfd, start_address);
    if (!section) {
        err(c, "VMA not pointing to any section\n");
        status = -trdb_bad_vma;
        goto fail;
    }
    info(c, "Section of start_address:%s\n", section->name);

    struct disassembler_unit dunit = {0};
    struct disassemble_info dinfo = {0};
    /*TODO: move this into trdb_ctx so that we can continuously decode pieces?*/
    struct trdb_dec_state dec_ctx = {.privilege = 7};
    struct tr_instr dis_instr = {0};

    dunit.dinfo = &dinfo;
    trdb_init_disassembler_unit(&dunit, abfd, no_aliases ? "no-aliases" : NULL);
    /* advanced fprintf output handling */
    dunit.dinfo->fprintf_func = build_instr_fprintf;
    /* dunit.dinfo->stream = &instr; */

    /* Alloc and config section data for disassembler */
    bfd_vma stop_offset = bfd_get_section_size(section) / dinfo.octets_per_byte;

    if ((status = alloc_section_for_debugging(c, abfd, section, &dinfo)) < 0)
        goto fail;

    bfd_vma pc = start_address; /* TODO: well we get a sync packet anyway... */

    /* we need to be able to look at two packets to make the right
     * decompression decision
     */
    struct tr_packet *last_packet = NULL;
    struct tr_packet *packet = NULL;
    struct tr_packet *next_packet = NULL;

    struct list_head *p;

    list_for_each_prev (p, packet_list) {
        packet = list_entry(p, typeof(*packet), list);
        /* this is a bit ugly, we determine the next packet if it exists. Also
         * we have to iterate the list in reverse order
         */
        if (p->prev != packet_list) {
            next_packet = list_entry(p->prev, typeof(*next_packet), list);
        } else {
            next_packet = NULL;
        }
        if (p->next != packet_list) {
            last_packet = list_entry(p->next, typeof(*last_packet), list);
        } else {
            last_packet = NULL;
        }

        /* we ignore unknown or unused packets (TIMER, SW) */
        if (packet->msg_type != W_TRACE) {
            info(c, "skipped a packet\n");
            continue;
        }

        /* Sometimes we leave the current section (e.g. changing from
         * the .start to the .text section), so let's load the
         * appropriate section and remove the old one
         */
        if (pc >= section->vma + stop_offset || pc < section->vma) {
            section = trdb_get_section_for_vma(abfd, pc);
            if (!section) {
                err(c, "VMA (PC) not pointing to any section\n");
                status = -trdb_bad_vma;
                goto fail;
            }
            stop_offset = bfd_get_section_size(section) / dinfo.octets_per_byte;

            free_section_for_debugging(&dinfo);
            if ((status =
                     alloc_section_for_debugging(c, abfd, section, &dinfo)) < 0)
                goto fail;

            info(c, "switched to section:%s\n", section->name);
        }

        switch (packet->format) {
        case F_BRANCH_FULL:
        case F_BRANCH_DIFF:
            dec_ctx.branch_map.cnt = packet->branches;
            dec_ctx.branch_map.bits = packet->branch_map;
            dec_ctx.branch_map.full =
                (packet->branches == 31) || (packet->branches == 0);
            break;
        case F_SYNC:
            break;
        case F_ADDR_ONLY:
            break;
        default:
            /* impossible */
            status = -trdb_bad_packet;
            goto fail;
        }

        trdb_log_packet(c, packet);

        if (packet->format == F_BRANCH_FULL) {
            bool hit_address = false; /* TODO: for now we don't allow
                                       * resync packets, see todo.org
                                       */
            /* We don't need to care about the address field if the branch map
             * is full,(except if full and instr before last branch is
             * discontinuity)
             */
            bool hit_discontinuity = dec_ctx.branch_map.full;

            /* Remember last packet address to be able to compute differential
             * address. Careful, a full branch map doesn't always have an
             * address field
             * TODO: edgecase where we sign extend from msb of branchmap
             */

            uint32_t absolute_addr = packet->address;

            if (dec_ctx.branch_map.cnt > 0)
                dec_ctx.last_packet_addr = absolute_addr;

            /* this indicates we don't have a valid address but still a full
             * branch map
             */
            if (dec_ctx.branch_map.cnt == 0)
                dec_ctx.branch_map.cnt = 31;

            while (!(dec_ctx.branch_map.cnt == 0 &&
                     (hit_discontinuity || hit_address))) {

                int size =
                    disassemble_at_pc(c, pc, &dis_instr, &dunit, &status);
                if (status < 0)
                    goto fail;

                /* TODO: test if packet addr valid first */
                if (dec_ctx.branch_map.cnt == 0 && pc == absolute_addr)
                    hit_address = true;

                dis_instr.priv = dec_ctx.privilege;
                if ((status = add_trace(c, instr_list, &dis_instr)) < 0)
                    goto fail;

                /* advance pc */
                pc += size;

                /* we hit a conditional branch, follow or not and update map */
                switch (dinfo.insn_type) {
                case dis_nonbranch:
                    /* TODO: we need this hack since {m,s,u} ret are not
                     * classified
                     */
                    if (!is_unpred_discontinuity(dis_instr.instr,
                                                 implicit_ret)) {
                        break;
                    }
                    info(c, "detected mret, uret or sret\n");
                case dis_jsr:    /* There is not real difference ... */
                case dis_branch: /* ... between those two */
                    /* we know that this instruction must have its jump target
                     * encoded in the binary else we would have gotten a
                     * non-predictable discontinuity packet. If
                     * branch_map.cnt == 0 + jump target unknown and we are here
                     * then we know that its actually a branch_map
                     * flush + discontinuity packet.
                     */
                    if (dec_ctx.branch_map.cnt > 1 && dinfo.target == 0)
                        err(c,
                            "can't predict the jump target, never happens\n");

                    if (dec_ctx.branch_map.cnt == 1 && dinfo.target == 0) {
                        if (!dec_ctx.branch_map.full) {
                            info(
                                c,
                                "we hit the not-full branch_map + address edge case, "
                                "(branch following discontinuity is included in this "
                                "packet)\n");
                            /* TODO: should I poison addr? */
                            pc = absolute_addr;
                        } else {
                            info(
                                c,
                                "we hit the full branch_map + address edge case\n");
                            pc = absolute_addr;
                        }
                        hit_discontinuity = true;
                    } else if (dec_ctx.branch_map.cnt > 0 ||
                               dinfo.target != 0) {
                        /* we should not hit unpredictable
                         * discontinuities
                         */
                        pc = dinfo.target;
                    } else {
                        /* we finally hit a jump with unknown  destination,
                         * thus the information in this packet  is used up
                         */
                        pc = absolute_addr;
                        hit_discontinuity = true;
                        info(c, "found discontinuity\n");
                    }
                    break;

                case dis_condbranch:
                    /* this case allows us to exhaust the branch bits */
                    {
                        /* 32 would be undefined */
                        bool branch_taken = !(dec_ctx.branch_map.bits & 1);
                        dec_ctx.branch_map.bits >>= 1;
                        dec_ctx.branch_map.cnt--;
                        if (dinfo.target == 0)
                            err(c,
                                "can't predict the jump target, never happens\n");
                        if (branch_taken)
                            pc = dinfo.target;
                        break;
                    }
                case dis_dref:
                    /* err(c, "Don't know what to do with this type\n"); */
                    break;

                case dis_dref2:
                case dis_condjsr:
                case dis_noninsn:
                    err(c, "invalid insn_type: %d\n", dinfo.insn_type);
                    status = -trdb_bad_instr;
                    goto fail;
                }
            }
        } else if (packet->format == F_BRANCH_DIFF) {
            if (full_address) {
                err(c,
                    "F_BRANCH_DIFF shouldn't happen, decoder configured with full_address\n");
                status = -trdb_bad_config;
                goto fail;
            }

            /* We have to find the instruction where we can apply the address
             * information to. This might either be a discontinuity information
             * or a sync up address. Furthermore we have to calculate the
             * absolute address we are referring to.
             */
            bool hit_address = false;
            /* We don't need to care about the address field if the branch map
             * is full,(except if full and instr before last branch is
             * discontinuity)
             */
            bool hit_discontinuity = dec_ctx.branch_map.full;

            uint32_t absolute_addr = dec_ctx.last_packet_addr - packet->address;

            dbg(c,
                "F_BRANCH_DIFF resolved address:%" PRIx32 " from %" PRIx32
                " - %" PRIx32 "\n",
                absolute_addr, dec_ctx.last_packet_addr, packet->address);

            /* Remember last packet address to be able to compute differential
             * address. Careful, a full branch map doesn't always have an
             * address field
             * TODO: edgecase where we sign extend from msb of branchmap
             */
            if (dec_ctx.branch_map.cnt > 0)
                dec_ctx.last_packet_addr = absolute_addr;
            /* this indicates we don't have a valid address but still a full
             * branch map
             */
            if (dec_ctx.branch_map.cnt == 0)
                dec_ctx.branch_map.cnt = 31;

            while (!(dec_ctx.branch_map.cnt == 0 &&
                     (hit_discontinuity || hit_address))) {

                int status = 0;
                int size =
                    disassemble_at_pc(c, pc, &dis_instr, &dunit, &status);
                if (status < 0)
                    goto fail;

                if (dec_ctx.branch_map.cnt == 0 && pc == absolute_addr)
                    hit_address = true;

                dis_instr.priv = dec_ctx.privilege;
                if ((status = add_trace(c, instr_list, &dis_instr)) < 0)
                    goto fail;

                /* advance pc */
                pc += size;

                /* we hit a conditional branch, follow or not and update map */
                switch (dinfo.insn_type) {
                case dis_nonbranch:
                    /* TODO: we need this hack since {m,s,u} ret are not
                     * classified
                     */
                    if (!is_unpred_discontinuity(dis_instr.instr,
                                                 implicit_ret)) {
                        break;
                    }
                    info(c, "detected mret, uret or sret\n");
                case dis_jsr:    /* There is not real difference ... */
                case dis_branch: /* ... between those two */
                    /* we know that this instruction must have its jump target
                     * encoded in the binary else we would have gotten a
                     * non-predictable discontinuity packet. If
                     * branch_map.cnt == 0 + jump target unknown and we are here
                     * then we know that its actually a branch_map
                     * flush + discontinuity packet.
                     */
                    if (dec_ctx.branch_map.cnt > 1 && dinfo.target == 0)
                        err(c,
                            "can't predict the jump target, never happens\n");

                    if (dec_ctx.branch_map.cnt == 1 && dinfo.target == 0) {
                        if (!dec_ctx.branch_map.full) {
                            info(
                                c,
                                "we hit the not-full branch_map + address edge case, "
                                "(branch following discontinuity is included in this "
                                "packet)\n");
                            /* TODO: should I poison addr? */
                            pc = absolute_addr;
                        } else {
                            info(
                                c,
                                "we hit the full branch_map + address edge case\n");
                            pc = absolute_addr;
                        }
                        hit_discontinuity = true;
                    } else if (dec_ctx.branch_map.cnt > 0 ||
                               dinfo.target != 0) {
                        /* we should not hit unpredictable discontinuities */
                        pc = dinfo.target;
                    } else {
                        /* we finally hit a jump with unknown destination, thus
                         * the information in this packet is used up
                         */
                        pc = absolute_addr;
                        hit_discontinuity = true;
                        info(c, "found discontinuity\n");
                    }
                    break;

                case dis_condbranch:
                    /* this case allows us to exhaust the branch bits */
                    {
                        /* 32 would be undefined */
                        bool branch_taken = !(dec_ctx.branch_map.bits & 1);
                        dec_ctx.branch_map.bits >>= 1;
                        dec_ctx.branch_map.cnt--;
                        if (dinfo.target == 0)
                            err(c,
                                "can't predict the jump target, never happens\n");
                        if (branch_taken)
                            pc = dinfo.target;
                        break;
                    }
                case dis_dref:
                    /* err(c, "Don't know what to do with this type\n"); */
                    break;

                case dis_dref2:
                case dis_condjsr:
                case dis_noninsn:
                    err(c, "invalid insn_type: %d\n", dinfo.insn_type);
                    status = -trdb_bad_instr;
                    goto fail;
                }
            }

        } else if (packet->format == F_SYNC) {
            /* Sync pc. */
            dec_ctx.privilege = packet->privilege;
            pc = packet->address;

            /* Remember last packet address to be able to compute differential
             * addresses
             */
            dec_ctx.last_packet_addr = packet->address;

            /* since we are abruptly changing the pc we have to check if we
             * leave the section before we can disassemble. This is really ugly
             * TODO: fix
             */
            if (pc >= section->vma + stop_offset || pc < section->vma) {
                section = trdb_get_section_for_vma(abfd, pc);
                if (!section) {
                    err(c, "VMA (PC) not pointing to any section\n");
                    status = -trdb_bad_vma;
                    goto fail;
                }
                stop_offset =
                    bfd_get_section_size(section) / dinfo.octets_per_byte;

                free_section_for_debugging(&dinfo);
                if ((status = alloc_section_for_debugging(c, abfd, section,
                                                          &dinfo)) < 0)
                    goto fail;

                info(c, "switched to section:%s\n", section->name);
            }

            int status = 0;
            int size = disassemble_at_pc(c, pc, &dis_instr, &dunit, &status);
            if (status < 0)
                goto fail;

            dis_instr.priv = dec_ctx.privilege;
            if ((status = add_trace(c, instr_list, &dis_instr)) < 0)
                goto fail;

            pc += size;

            switch (dinfo.insn_type) {
            case dis_nonbranch:
                /* TODO: we need this hack since {m,s,u} ret are not
                 * classified in libopcodes
                 */
                if (!is_unpred_discontinuity(dis_instr.instr, implicit_ret)) {
                    break;
                }
                info(c, "detected mret, uret or sret\n");
            case dis_jsr:    /* There is not real difference ... */
            case dis_branch: /* ... between those two */
                if (dinfo.target == 0)
                    err(c, "can't predict the jump target, never happens\n");
                pc = dinfo.target;
                break;

            case dis_condbranch:
                if (dinfo.target == 0)
                    err(c, "can't predict the jump target, never happens\n");
                if (packet->branch == 0) {
                    err(c, "doing a branch from a F_SYNC packet\n");
                    pc = dinfo.target;
                }
                break;
            case dis_dref: /* TODO: is this useful? */
                break;
            case dis_dref2:
            case dis_condjsr:
            case dis_noninsn:
                err(c, "invalid insn_type: %d\n", dinfo.insn_type);
                status = -trdb_bad_instr;
                goto fail;
            }
        } else if (packet->format == F_ADDR_ONLY) {
            /* We have to find the instruction where we can apply the address
             * information to. This might either be a discontinuity infromation
             * or a sync up address. So we stop either at the given address or
             * use it on a discontinuity, whichever comes first. There can be an
             * ambiguity between whether a packet is meant for an address for
             * for a jump target. This could happen if we have jr infinite loop
             * with the only way out being exception. Related to that there is
             * also that
             *
             * Resync packets have an issue, namely that we can't distinguishing
             * between address sync and unpredictable discontinuities. We can
             * distinguish the case of advance to next address with advance to
             * unpredictable discontinuitiy by inspecting the next packet. The
             * events halt and unqualified happen if we don't have any more
             * packets (next_packet == NULL, TODO: make this better). Exception
             * and privilege change events always happen in packet pairs, which,
             * again, we can figure out by looking at next_packet. If any test
             * so far failed, then we know, since we are disallowing resync
             * packets for now, that we are dealing with a unpredictable
             * discontinuity.
             */
            bool hit_address = false;
            bool hit_discontinuity = false;

            uint32_t absolute_addr;
            if (full_address) {
                absolute_addr = packet->address;
            } else {
                /* absolute_addr = dec_ctx.last_packet_addr - sext_addr; */
                absolute_addr = dec_ctx.last_packet_addr - packet->address;
            }

            dbg(c,
                "F_ADDR_ONLY resolved address:%" PRIx32 " from %" PRIx32
                " - %" PRIx32 "\n",
                absolute_addr, dec_ctx.last_packet_addr, packet->address);

            /* Remember last packet address to be able to compute differential
             * addresses
             */
            dec_ctx.last_packet_addr = absolute_addr;

            while (!(hit_address || hit_discontinuity)) {
                int status = 0;
                int size =
                    disassemble_at_pc(c, pc, &dis_instr, &dunit, &status);
                if (status < 0)
                    goto fail;

                if (pc == absolute_addr)
                    hit_address = true;

                dis_instr.priv = dec_ctx.privilege;
                if ((status = add_trace(c, instr_list, &dis_instr)) < 0)
                    goto fail;

                /* advance pc */
                pc += size;

                switch (dinfo.insn_type) {
                case dis_nonbranch:
                    /* TODO: we need this hack since {m,s,u} ret are not
                     * classified
                     */
                    if (!is_unpred_discontinuity(dis_instr.instr,
                                                 implicit_ret)) {
                        break;
                    }
                    info(c, "detected mret, uret or sret\n");
                case dis_jsr:    /* There is not real difference ... */
                case dis_branch: /* ... between those two */
                    if (dinfo.target) {
                        pc = dinfo.target;
                    } else {
                        info(c, "found the discontinuity\n");
                        pc = absolute_addr;
                        hit_discontinuity = true;
                    }
                    break;

                case dis_condbranch:
                    err(c,
                        "we shouldn't hit conditional branches with F_ADDRESS_ONLY\n");
                    break;

                case dis_dref: /* TODO: is this useful? */
                    break;
                case dis_dref2:
                case dis_condjsr:
                case dis_noninsn:
                    err(c, "invalid insn_type: %d\n", dinfo.insn_type);
                    status = -trdb_bad_instr;
                    goto fail;
                }
            }
        }
    }
    return status;

fail:
    free_section_for_debugging(&dinfo);
    return status;
}

void trdb_disassemble_trace(size_t len, struct tr_instr trace[len],
                            struct disassembler_unit *dunit)
{
    struct disassemble_info *dinfo = dunit->dinfo;
    for (size_t i = 0; i < len; i++) {
        (*dinfo->fprintf_func)(dinfo->stream, "0x%08jx  ",
                               (uintmax_t)trace[i].iaddr);
        (*dinfo->fprintf_func)(dinfo->stream, "0x%08jx  ",
                               (uintmax_t)trace[i].instr);
        (*dinfo->fprintf_func)(dinfo->stream, "%s",
                               trace[i].exception ? "TRAP!  " : "");
        trdb_disassemble_single_instruction(trace[i].instr, trace[i].iaddr,
                                            dunit);
    }
}

void trdb_disassemble_trace_with_bfd(struct trdb_ctx *c, size_t len,
                                     struct tr_instr trace[len], bfd *abfd,
                                     struct disassembler_unit *dunit)
{
    struct disassemble_info *dinfo = dunit->dinfo;
    for (size_t i = 0; i < len; i++) {
        (*dinfo->fprintf_func)(dinfo->stream, "%s",
                               trace[i].exception ? "TRAP!  " : "");
        trdb_disassemble_instruction_with_bfd(c, abfd, trace[i].iaddr, dunit);
    }
}

void trdb_disassemble_instr(struct tr_instr *instr,
                            struct disassembler_unit *dunit)
{
    struct disassemble_info *dinfo = dunit->dinfo;
    (*dinfo->fprintf_func)(dinfo->stream, "0x%08jx  ", (uintmax_t)instr->iaddr);
    (*dinfo->fprintf_func)(dinfo->stream, "0x%08jx  ", (uintmax_t)instr->instr);
    (*dinfo->fprintf_func)(dinfo->stream, "%s",
                           instr->exception ? "TRAP!  " : "");

    trdb_disassemble_single_instruction(instr->instr, instr->iaddr, dunit);
}

void trdb_disassemble_instr_with_bfd(struct trdb_ctx *c, struct tr_instr *instr,
                                     bfd *abfd, struct disassembler_unit *dunit)
{
    struct disassemble_info *dinfo = dunit->dinfo;
    (*dinfo->fprintf_func)(dinfo->stream, "%s",
                           instr->exception ? "TRAP!  " : "");
    trdb_disassemble_instruction_with_bfd(c, abfd, instr->iaddr, dunit);
}

void trdb_dump_packet_list(FILE *stream, const struct list_head *packet_list)
{
    struct tr_packet *packet;
    list_for_each_entry_reverse (packet, packet_list, list) {
        trdb_print_packet(stream, packet);
    }
}

void trdb_log_packet(struct trdb_ctx *c, const struct tr_packet *packet)
{
    if (!c)
        return;

    if (!packet) {
        dbg(c, "error printing packet\n");
        return;
    }

    switch (packet->msg_type) {
    case W_TRACE:
        switch (packet->format) {
        case F_BRANCH_FULL:
        case F_BRANCH_DIFF:
            dbg(c, "PACKET ");
            if (packet->format == F_BRANCH_FULL)
                dbg(c, "0: F_BRANCH_FULL\n");
            else
                dbg(c, "1: F_BRANCH_DIFF\n");

            dbg(c, "    branches  : %" PRIu32 "\n", packet->branches);
            dbg(c, "    branch_map: 0x%" PRIx32 "\n", packet->branch_map);
            dbg(c, "    address   : 0x%" PRIx32 "\n", packet->address);
            /* TODO: that special full branch map behaviour */
            break;

        case F_ADDR_ONLY:
            dbg(c, "PACKET 2: F_ADDR_ONLY\n");
            dbg(c, "    address   : 0x%" PRIx32 "\n", packet->address);
            break;
        case F_SYNC:
            dbg(c, "PACKET 3: F_SYNC\n");
            const char *subf[4];
            subf[0] = "SF_START";
            subf[1] = "SF_EXCEPTION";
            subf[2] = "SF_CONTEXT";
            subf[3] = "RESERVED";
            dbg(c, "    subformat : %s\n", subf[packet->subformat]);

            /* TODO fix this */
            dbg(c, "    context   :\n");
            dbg(c, "    privilege : 0x%" PRIx32 "\n", packet->privilege);
            if (packet->subformat == SF_CONTEXT)
                return;

            dbg(c, "    branch    : %s\n", packet->branch ? "true" : "false");
            dbg(c, "    address   : 0x%" PRIx32 "\n", packet->address);
            if (packet->subformat == SF_START)
                return;

            dbg(c, "    ecause    : 0x%" PRIx32 "\n", packet->ecause);
            dbg(c, "    interrupt : %s\n",
                packet->interrupt ? "true" : "false");
            dbg(c, "    tval      : 0x%" PRIx32 "\n", packet->tval);
            /* SF_EXCEPTION */
        }
        break;

    case W_SOFTWARE:
        dbg(c, "PACKET W_SOFTWARE\n");
        dbg(c, "    userdata  : 0x%" PRIx32 "\n", packet->userdata);
        break;

    case W_TIMER:
        dbg(c, "PACKET W_TIMER\n");
        dbg(c, "    time : %" PRIu64 "\n", packet->time);
        break;
    }
}

void trdb_print_packet(FILE *stream, const struct tr_packet *packet)
{
    if (!stream)
        return;

    if (!packet) {
        fprintf(stream, "error printing packet\n");
        return;
    }

    switch (packet->msg_type) {
    case W_TRACE:
        switch (packet->format) {
        case F_BRANCH_FULL:
        case F_BRANCH_DIFF:
            fprintf(stream, "PACKET ");
            packet->format == F_BRANCH_FULL
                ? fprintf(stream, "0: F_BRANCH_FULL\n")
                : fprintf(stream, "1: F_BRANCH_DIFF\n");
            fprintf(stream, "    branches  : %" PRIu32 "\n", packet->branches);
            fprintf(stream, "    branch_map: 0x%" PRIx32 "\n",
                    packet->branch_map);
            fprintf(stream, "    address   : 0x%" PRIx32 "\n", packet->address);
            /* TODO: that special full branch map behaviour */
            break;
        case F_ADDR_ONLY:
            fprintf(stream, "PACKET 2: F_ADDR_ONLY\n");
            fprintf(stream, "    address   : 0x%" PRIx32 "\n", packet->address);
            break;
        case F_SYNC:
            fprintf(stream, "PACKET 3: F_SYNC\n");
            const char *subf[4];
            subf[0] = "SF_START";
            subf[1] = "SF_EXCEPTION";
            subf[2] = "SF_CONTEXT";
            subf[3] = "RESERVED";
            fprintf(stream, "    subformat : %s\n", subf[packet->subformat]);

            /* TODO fix this */
            fprintf(stream, "    context   :\n");
            fprintf(stream, "    privilege : 0x%" PRIx32 "\n",
                    packet->privilege);
            if (packet->subformat == SF_CONTEXT)
                return;

            fprintf(stream, "    branch    : %s\n",
                    packet->branch ? "true" : "false");
            fprintf(stream, "    address   : 0x%" PRIx32 "\n", packet->address);
            if (packet->subformat == SF_START)
                return;

            fprintf(stream, "    ecause    : 0x%" PRIx32 "\n", packet->ecause);
            fprintf(stream, "    interrupt : %s\n",
                    packet->interrupt ? "true" : "false");
            fprintf(stream, "    tval      : 0x%" PRIx32 "\n", packet->tval);
            /* SF_EXCEPTION */
        }
        break;

    case W_SOFTWARE:
        fprintf(stream, "PACKET W_SOFTWARE\n");
        fprintf(stream, "    userdata  : 0x%" PRIx32 "\n", packet->userdata);
        break;

    case W_TIMER:
        fprintf(stream, "PACKET W_TIMER\n");
        fprintf(stream, "    time : %" PRIu64 "\n", packet->time);
        break;
    }
}

void trdb_log_instr(struct trdb_ctx *c, const struct tr_instr *instr)
{
    if (!c)
        return;

    if (!instr) {
        dbg(c, "error logging instruction\n");
        return;
    }

    dbg(c, "INSTR\n");
    dbg(c, "    iaddr     : 0x%08" PRIx32 "\n", instr->iaddr);
    dbg(c, "    instr     : 0x%08" PRIx32 "\n", instr->instr);
    dbg(c, "    priv      : 0x%" PRIx32 "\n", instr->priv);
    dbg(c, "    exception : %s\n", instr->exception ? "true" : "false");
    dbg(c, "    cause     : 0x%" PRIx32 "\n", instr->cause);
    dbg(c, "    tval      : 0x%" PRIx32 "\n", instr->tval);
    dbg(c, "    interrupt : %s\n", instr->interrupt ? "true" : "false");
    dbg(c, "    compressed: %s\n", instr->compressed ? "true" : "false");
}

void trdb_print_instr(FILE *stream, const struct tr_instr *instr)
{
    if (!stream)
        return;

    if (!instr) {
        fprintf(stream, "error printing instruction\n");
        return;
    }

    fprintf(stream, "INSTR\n");
    fprintf(stream, "    iaddr     : 0x%08" PRIx32 "\n", instr->iaddr);
    fprintf(stream, "    instr     : 0x%08" PRIx32 "\n", instr->instr);
    fprintf(stream, "    priv      : 0x%" PRIx32 "\n", instr->priv);
    fprintf(stream, "    exception : %s\n",
            instr->exception ? "true" : "false");
    fprintf(stream, "    cause     : 0x%" PRIx32 "\n", instr->cause);
    fprintf(stream, "    tval      : 0x%" PRIx32 "\n", instr->tval);
    fprintf(stream, "    interrupt : %s\n",
            instr->interrupt ? "true" : "false");
    fprintf(stream, "    compressed: %s\n",
            instr->compressed ? "true" : "false");
}

bool trdb_compare_packet()
{
    return false;
}

bool trdb_compare_instr(struct trdb_ctx *c, const struct tr_instr *instr0,
                        const struct tr_instr *instr1)
{
    if (!instr0 || !instr1)
        return false;

    bool sum = true;
    sum |= instr0->valid == instr1->valid;
    sum |= instr0->exception == instr1->exception;
    sum |= instr0->interrupt == instr1->interrupt;
    sum |= instr0->cause == instr1->cause;
    sum |= instr0->tval == instr1->tval;
    sum |= instr0->priv == instr1->priv;
    sum |= instr0->iaddr == instr1->iaddr;
    sum |= instr0->instr == instr1->instr;
    sum |= instr0->compressed == instr1->compressed;

    return sum;
}

void trdb_free_packet_list(struct list_head *packet_list)
{
    if (list_empty(packet_list))
        return;

    struct tr_packet *packet;
    struct tr_packet *packet_next;
    list_for_each_entry_safe (packet, packet_next, packet_list, list) {
        free(packet);
    }
}

void trdb_free_instr_list(struct list_head *instr_list)
{
    if (list_empty(instr_list))
        return;

    struct tr_instr *instr;
    struct tr_instr *instr_next;
    list_for_each_entry_safe (instr, instr_next, instr_list, list) {
        free(instr);
    }
}
