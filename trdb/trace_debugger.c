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
#include "list.h"
#include "utils.h"

struct trdb_config;
struct trdb_state;
struct branch_map_state;
struct filter_state;

/* Configuration state of the trace debugger, used to guide the compression and
 * decompression routine
 */
struct trdb_config {
    /* TODO: inspect full-address, iaddress-lsb-p, implicit-except, set-trace */
    uint64_t resync_max;
    bool full_address;
    bool iaddress_lsb_p;
    bool implicit_except;
    bool set_trace;
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
    size_t instrbits;
    size_t instrs;
    size_t packets;
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
    ctx->config = (struct trdb_config){
        .resync_max = UINT64_MAX, .full_address = true, .no_aliases = true};

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
    ctx->config = (struct trdb_config){
        .resync_max = UINT64_MAX, .full_address = true, .no_aliases = true};

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

    ctx->config = (struct trdb_config){
        .resync_max = UINT64_MAX, .full_address = true, .no_aliases = true};

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


static bool is_branch(uint32_t instr)
{
    /* static bool is_##code##_instr(long insn) */
    /* TODO: add rvc instr */
    assert((instr & 3) == 3);
    bool is_riscv_branch = is_beq_instr(instr) || is_bne_instr(instr)
                           || is_blt_instr(instr) || is_bge_instr(instr)
                           || is_bltu_instr(instr) || is_bgeu_instr(instr);
    bool is_pulp_branch = is_p_bneimm_instr(instr) || is_p_beqimm_instr(instr);
    return is_riscv_branch || is_pulp_branch;
    /* auipc */
}


static bool branch_taken(bool before_compressed, uint32_t addr_before,
                         uint32_t addr_after)
{
    /* can this cause issues with RVC + degenerate jump (+2)? -> yes*/
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
    if (branches <= 1) {
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
static bool is_unpred_discontinuity(uint32_t instr)
{
    /* Should also work just fine with rvc instructions */
    return is_jalr_instr(instr) || is_mret_instr(instr) || is_sret_instr(instr)
           || is_uret_instr(instr);
}


/* Just crash and error if we hit one of those */
static bool is_unsupported(uint32_t instr)
{
    return is_lp_setup_instr(instr) || is_lp_counti_instr(instr)
           || is_lp_count_instr(instr) || is_lp_endi_instr(instr)
           || is_lp_starti_instr(instr) || is_lp_setupi_instr(instr);
}


/* Sometimes we have to decide whether to put in the absolute or differential
 * address into the packet. We choose the one which has the least amount of
 * meaningfull bits, i.e. the bits that can't be inferred by sign-extension.
 */
static bool differential_addr(int *lead, uint32_t absolute,
                              uint32_t differential)
{
    int aclz = __builtin_clz(absolute);
    int aclo = __builtin_clz(~absolute);
    int dclz = __builtin_clz(differential);
    int dclo = __builtin_clz(~differential);

    int abs = aclz > aclo ? aclz : aclo;
    int diff = dclz > dclo ? dclz : dclo;
    *lead = diff > abs ? diff : abs;
    return diff > abs;
}


static void emit_exception_packet(struct tr_packet *tr,
                                  struct tr_instr *lc_instr,
                                  struct tr_instr *tc_instr,
                                  struct tr_instr *nc_instr)
{
    tr->format = F_SYNC;          /* sync */
    tr->subformat = SF_EXCEPTION; /* exception */
    tr->context = 0;              /* TODO: what comes here? */
    tr->privilege = tc_instr->priv;

    if (is_branch(tc_instr->instr)
        && !branch_taken(tc_instr->compressed, tc_instr->iaddr,
                         nc_instr->iaddr))
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
    tr->length = 2 + 2 + PRIVLEN + 1 + XLEN + CAUSELEN + 1;
}


static void emit_start_packet(struct tr_packet *tr, struct tr_instr *tc_instr,
                              struct tr_instr *nc_instr)
{
    tr->format = F_SYNC;      /* sync */
    tr->subformat = SF_START; /* start */
    tr->context = 0;          /* TODO: what comes here? */
    tr->privilege = tc_instr->priv;
    if (is_branch(tc_instr->instr)
        && !branch_taken(tc_instr->compressed, tc_instr->iaddr,
                         nc_instr->iaddr))
        tr->branch = 1;
    else
        tr->branch = 0;
    tr->address = tc_instr->iaddr;
    tr->length = 2 + 2 + PRIVLEN + 1 + XLEN;
}


static int emit_branch_map_flush_packet(struct trdb_ctx *ctx,
                                        struct tr_packet *tr,
                                        struct branch_map_state *branch_map,
                                        struct tr_instr *tc_instr,
                                        uint32_t last_iaddr, bool full_address,
                                        bool is_u_discontinuity)
{
    if (branch_map->cnt == 0) {
        tr->format = F_ADDR_ONLY;
        if (full_address) {
            tr->address = tc_instr->iaddr;
        } else {
            /* always differential in F_ADDR_ONLY*/
            tr->address = last_iaddr - tc_instr->iaddr;
        }
        /* TODO: compress length with differential */
        tr->length = FORMATLEN + XLEN;
        assert(branch_map->bits == 0);
    } else {
        if (branch_map->full && is_u_discontinuity)
            dbg(ctx, "full branch map and discontinuity edge case\n");

        if (full_address) {
            tr->format = F_BRANCH_FULL;
            tr->address = tc_instr->iaddr;
            tr->length =
                FORMATLEN + BRANCHLEN + branch_map_len(branch_map->cnt);
            if (branch_map->full) {
                if (is_u_discontinuity)
                    tr->length += XLEN;
                else
                    tr->length += 0;
            } else {
                tr->length += XLEN;
            }
        } else {
            /* In this mode we try to compress the instruction address by taking
             * the difference address or the absolute address, whichever has
             * more bits which can be inferred by signextension.
             */
            uint32_t diff = last_iaddr - tc_instr->iaddr;
            uint32_t full = tc_instr->iaddr;
            int lead = 0;
            if (differential_addr(&lead, full, diff)) {
                tr->format = F_BRANCH_DIFF;
                tr->address = diff;
            } else {
                tr->format = F_BRANCH_FULL;
                tr->address = full;
            }
            tr->length =
                FORMATLEN + BRANCHLEN + branch_map_len(branch_map->cnt);
            if (branch_map->full) {
                if (is_u_discontinuity)
                    tr->length += XLEN;
                else
                    tr->length += 0;
            } else {
                tr->length += XLEN;
            }
        }
        tr->branches = branch_map->cnt;
        tr->branch_map = branch_map->bits;
        /* TODO: compress length with differential */
        *branch_map = (struct branch_map_state){0};
    }

    return 0;
}


static void emit_full_branch_map(struct tr_packet *tr,
                                 struct branch_map_state *branch_map)
{
    assert(branch_map->cnt == 31);
    tr->format = F_BRANCH_FULL;
    tr->branches = branch_map->cnt;
    tr->branch_map = branch_map->bits;
    /* No address needed */
    tr->length = FORMATLEN + BRANCHLEN + branch_map_len(31);
    *branch_map = (struct branch_map_state){0};
}

int trdb_compress_trace_step(struct trdb_ctx *ctx,
                             struct list_head *packet_list,
                             struct tr_instr *instr)
{
    int generated_packet = 0;
    bool full_address = ctx->config.full_address;

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
    nextc->unpred_disc = is_unpred_discontinuity(instr->instr);
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
        goto fail;
    }

    if (filter->resync_cnt++ == ctx->config.resync_max) {
        filter->resync_pend = true;
        filter->resync_cnt = 0;
    }

    if (is_branch(tc_instr->instr)) {
        /* update branch map */
        /* in hardware maybe mask and compare is better ? */
        if (branch_taken(tc_instr->compressed, tc_instr->iaddr,
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
        emit_exception_packet(tr, lc_instr, tc_instr, nc_instr);
        *last_iaddr = tc_instr->iaddr;

        list_add(&tr->list, packet_list);

        thisc->emitted_exception_sync = true;
        filter->resync_pend = false; /* TODO: how to handle this */

        generated_packet = 1;
        /* end of cycle */

    } else if (lastc->emitted_exception_sync) {
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
        emit_start_packet(tr, tc_instr, nc_instr);
        *last_iaddr = tc_instr->iaddr;

        list_add(&tr->list, packet_list);

        filter->resync_pend = false;
        generated_packet = 1;
        /* end of cycle */

    } else if (firstc_qualified || thisc->unhalted || thisc->privilege_change
               || (filter->resync_pend && branch_map->cnt == 0)) {

        /* Start packet */
        /* Send te_inst:
         * format 3
         * subformat 0 (start, no ecause, interrupt and tval)
         * resync_pend = 0
         */
        ALLOC_PACKET(tr);
        emit_start_packet(tr, tc_instr, nc_instr);
        *last_iaddr = tc_instr->iaddr;

        list_add(&tr->list, packet_list);

        filter->resync_pend = false;
        generated_packet = 1;
        /* end of cycle */

    } else if (lastc->unpred_disc) {
        /* Send te_inst:
         * format 0/1/2
         */
        ALLOC_PACKET(tr);
        if (emit_branch_map_flush_packet(ctx, tr, branch_map, tc_instr,
                                         *last_iaddr, full_address, true)) {
            goto fail;
        }
        *last_iaddr = tc_instr->iaddr;

        list_add(&tr->list, packet_list);

        generated_packet = 1;
        /* end of cycle */

    } else if (filter->resync_pend && branch_map->cnt > 0) {
        /* we treat resync_pend && branches == 0 before */
        /* Send te_inst:
         * format 0/1/2
         */
        ALLOC_PACKET(tr);
        if (emit_branch_map_flush_packet(ctx, tr, branch_map, tc_instr,
                                         *last_iaddr, full_address, false)) {
            goto fail;
        }
        *last_iaddr = tc_instr->iaddr;

        list_add(&tr->list, packet_list);

        generated_packet = 1;
        /* end of cycle */

    } else if (nextc->halt || nextc->exception || nextc->privilege_change
               || nextc->unqualified) {
        /* Send te_inst:
         * format 0/1/2
         */
        ALLOC_PACKET(tr);
        if (emit_branch_map_flush_packet(ctx, tr, branch_map, tc_instr,
                                         *last_iaddr, full_address, false)) {
            goto fail;
        }
        *last_iaddr = tc_instr->iaddr;

        list_add(&tr->list, packet_list);

        generated_packet = 1;
        /* end of cycle */

    } else if (branch_map->full) {
        /* Send te_inst:
         * format 0
         * no address
         */
        ALLOC_PACKET(tr);
        emit_full_branch_map(tr, branch_map);

        list_add(&tr->list, packet_list);

        generated_packet = 1;
        /* end of cycle */

    } else if (thisc->context_change) {
        /* Send te_inst:
         * format 3
         * subformat 2
         */
        err(ctx, "context_change not supported\n");
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
        ctx->stats.packetbits += tr->length;
        ctx->stats.packets++;
    }

    if (generated_packet) {
        trdb_log_packet(
            ctx, list_entry(packet_list->next, typeof(struct tr_packet), list));
    }

    if (ctx->dunit) {
        /* TODO: unfortunately this ignores log_fn */
        if (trdb_get_log_priority(ctx) == LOG_DEBUG)
            trdb_disassemble_instr(instr, ctx->dunit);
    } else {
        dbg(ctx, "0x%016jx  0x%08jx\n", (uintmax_t)instr->iaddr,
            (uintmax_t)instr->instr);
    }

    /* also print packet */
    return generated_packet;
fail:
fail_malloc:
    return -1;
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
    /* make sure start froma a clean slate */
    *instr = (struct tr_instr){0};

    struct disassemble_info *dinfo = dunit->dinfo;
    /* Important to set for internal calls to fprintf */
    c->dis_instr = instr;
    dinfo->stream = c;

    /* print instr address */
    (*dinfo->fprintf_func)(c, "0x%016jx  ", (uintmax_t)pc);

    /* check if insn_info works */
    dinfo->insn_info_valid = 0;

    int instr_size = (*dunit->disassemble_fn)(pc, dinfo);
    (*dinfo->fprintf_func)(c, "\n");
    if (instr_size <= 0) {
        err(c, "Encountered instruction with %d bytes, stopping\n", instr_size);
        *status = -1;
        return 0;
    }
    if (!dinfo->insn_info_valid) {
        err(c, "Encountered invalid instruction info\n");
        *status = -1;
        return 0;
    }
    uint64_t instr_bits = 0;
    if (read_memory_at_pc(pc, &instr_bits, 0, dinfo)) {
        err(c, "Reading instr at pc failed\n");
        *status = -1;
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
    struct tr_instr *instr = c->dis_instr;
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
    if (strlen(instr->str) + rv + 1 > INSTR_STR_LEN) {
        err(c, "Can't append to buffer, truncating string\n");
        strncat(instr->str, tmp, INSTR_STR_LEN - 1 - strlen(instr->str));
    } else {
        strncat(instr->str, tmp, rv);
    }

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
    if (!dinfo || !section) {
        err(c, "disassemble_info or asection is NULL\n");
        return -1;
    }

    bfd_size_type section_size = bfd_get_section_size(section);

    bfd_byte *section_data = malloc(section_size);

    if (!section_data) {
        err(c, "malloc: %s\n", strerror(errno));
        return -1;
    }

    if (!bfd_get_section_contents(abfd, section, section_data, 0,
                                  section_size)) {
        err(c, "bfd_get_section_contents: %s\n", bfd_errmsg(bfd_get_error()));
        free(section_data);
        return -1;
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
    if (!add) {
        err(c, "malloc: %s\n", strerror(errno));
        return -1;
    }

    dbg(c, instr->str);

    memcpy(add, instr, sizeof(*add));
    list_add(&add->list, instr_list);
    return 0;
}


struct list_head *trdb_decompress_trace(struct trdb_ctx *c, bfd *abfd,
                                        struct list_head *packet_list,
                                        struct list_head *instr_list)
{
    /* We assume our hw block in the pulp generated little endian
     * addresses, thus we should make sure that before we interact
     * with bfd addresses to convert this foreign format to the local
     * host format
     */
    /* TODO: supports only statically linked elf executables */

    /* find section belonging to start_address */
    bfd_vma start_address = abfd->start_address;
    asection *section = get_section_for_vma(abfd, start_address);
    if (!section) {
        err(c, "VMA not pointing to any section\n");
        goto fail;
    }
    info(c, "Section of start_address:%s\n", section->name);

    struct disassembler_unit dunit = {0};
    struct disassemble_info dinfo = {0};
    /*TODO: move this into trdb_ctx so that we can continuously decode pieces?*/
    struct trdb_dec_state dec_ctx = {.privilege = 7};
    struct tr_instr dis_instr = {0};

    dunit.dinfo = &dinfo;
    init_disassembler_unit(&dunit, abfd,
                           c->config.no_aliases ? "no-aliases" : NULL);
    /* advanced fprintf output handling */
    dunit.dinfo->fprintf_func = build_instr_fprintf;
    /* dunit.dinfo->stream = &instr; */

    /* Alloc and config section data for disassembler */
    bfd_vma stop_offset = bfd_get_section_size(section) / dinfo.octets_per_byte;

    if (alloc_section_for_debugging(c, abfd, section, &dinfo)) {
        err(c, "Failed alloc_section_for_debugging\n");
        goto fail;
    }
    bfd_vma pc = start_address; /* TODO: well we get a sync packet anyway... */

    /* we need to be able to look at two packets to make the right
     * decompression decision
     */
    struct tr_packet *last_packet = NULL;
    struct tr_packet *packet = NULL;
    struct tr_packet *next_packet = NULL;

    struct list_head *p;

    list_for_each_prev(p, packet_list)
    {
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


        /* Sometimes we leave the current section (e.g. changing from
         * the .start to the .text section), so let's load the
         * appropriate section and remove the old one
         */
        if (pc >= section->vma + stop_offset || pc < section->vma) {
            free_section_for_debugging(&dinfo);

            section = get_section_for_vma(abfd, pc);
            if (!section) {
                err(c, "VMA (PC) not pointing to any section\n");
                goto fail;
            }
            stop_offset = bfd_get_section_size(section) / dinfo.octets_per_byte;

            if (alloc_section_for_debugging(c, abfd, section, &dinfo)) {
                err(c, "Failed alloc_section_for_debugging\n");
                goto fail;
            }
            info(c, "Switched to section:%s\n", section->name);
        }

        switch (packet->format) {
        case F_BRANCH_FULL:
        case F_BRANCH_DIFF:
            dec_ctx.branch_map.cnt = packet->branches;
            dec_ctx.branch_map.bits = packet->branch_map;
            dec_ctx.branch_map.full = packet->branches == 31;
            break;
        case F_SYNC:
            break;
        case F_ADDR_ONLY:
            break;
        default:
            err(c, "Unimplemented\n");
            goto fail;
        }

        if (trdb_get_log_priority(c) == LOG_DEBUG) {
            trdb_log_packet(c, packet);
        }

        if (packet->format == F_BRANCH_FULL) {
            /* We have to find the instruction where we can apply the address
             * information to. This might either be a discontinuity infromation
             * or a sync up address. The search_discontinuity
             * variable tells us which one is it
             */
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
            if ((dec_ctx.branch_map.full
                 && (packet->length
                     > FORMATLEN + BRANCHLEN
                           + branch_map_len(dec_ctx.branch_map.cnt)))
                || !dec_ctx.branch_map.full) {
                dec_ctx.last_packet_addr = packet->address;
            }


            while (!(dec_ctx.branch_map.cnt == 0
                     && (hit_discontinuity || hit_address))) {

                int status = 0;
                int size =
                    disassemble_at_pc(c, pc, &dis_instr, &dunit, &status);
                if (status != 0)
                    goto fail;

                if (dec_ctx.branch_map.cnt == 0 && pc == packet->address)
                    hit_address = true;

                dis_instr.priv = dec_ctx.privilege;
                add_trace(c, instr_list, &dis_instr);

                /* advance pc */
                pc += size;

                /* we hit a conditional branch, follow or not and update map */
                switch (dinfo.insn_type) {
                case dis_nonbranch:
                    /* TODO: we need this hack since {m,s,u} ret are not
                     * classified
                     */
                    if (!is_unpred_discontinuity(dis_instr.instr)) {
                        break;
                    }
                    info(c, "Detected mret, uret or sret\n");
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
                            "Can't predict the jump target, never happens\n");

                    if (dec_ctx.branch_map.cnt == 1 && dinfo.target == 0) {
                        if (!dec_ctx.branch_map.full) {
                            info(
                                c,
                                "We hit the not-full branch_map + address edge case, "
                                "(branch following discontinuity is included in this "
                                "packet)\n");
                            /* TODO: should I poison addr? */
                            pc = packet->address;
                        } else {
                            info(
                                c,
                                "We hit the full branch_map + address edge case\n");
                            pc = packet->address;
                        }
                        hit_discontinuity = true;
                    } else if (dec_ctx.branch_map.cnt > 0
                               || dinfo.target != 0) {
                        /* we should not hit unpredictable
                         * discontinuities
                         */
                        pc = dinfo.target;
                    } else {
                        /* we finally hit a jump with unknown  destination,
                         * thus the information in this packet  is used up
                         */
                        pc = packet->address;
                        hit_discontinuity = true;
                        info(c, "Found the discontinuity\n");
                    }
                    break;

                case dis_condbranch:
                    /* this case allows us to exhaust the branch bits */
                    {
                        /* 32 would be undefined */
                        bool branch_taken = dec_ctx.branch_map.bits & 1;
                        dec_ctx.branch_map.bits >>= 1;
                        dec_ctx.branch_map.cnt--;
                        if (dinfo.target == 0)
                            err(c,
                                "Can't predict the jump target, never happens\n");
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
                    err(c, "Invalid insn_type: %d\n", dinfo.insn_type);
                    goto fail;
                }
            }
        } else if (packet->format == F_BRANCH_DIFF) {
            if (c->config.full_address) {
                err(c,
                    "F_BRANCH_DIFF shouldn't happen, decoder configured with full_address\n");
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
            if ((dec_ctx.branch_map.full
                 && (packet->length
                     > FORMATLEN + BRANCHLEN
                           + branch_map_len(dec_ctx.branch_map.cnt)))
                || !dec_ctx.branch_map.full)
                dec_ctx.last_packet_addr = absolute_addr;

            while (!(dec_ctx.branch_map.cnt == 0
                     && (hit_discontinuity || hit_address))) {

                int status = 0;
                int size =
                    disassemble_at_pc(c, pc, &dis_instr, &dunit, &status);
                if (status != 0)
                    goto fail;

                if (dec_ctx.branch_map.cnt == 0 && pc == absolute_addr)
                    hit_address = true;

                dis_instr.priv = dec_ctx.privilege;
                add_trace(c, instr_list, &dis_instr);

                /* advance pc */
                pc += size;

                /* we hit a conditional branch, follow or not and update map */
                switch (dinfo.insn_type) {
                case dis_nonbranch:
                    /* TODO: we need this hack since {m,s,u} ret are not
                     * classified
                     */
                    if (!is_unpred_discontinuity(dis_instr.instr)) {
                        break;
                    }
                    info(c, "Detected mret, uret or sret\n");
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
                            "Can't predict the jump target, never happens\n");

                    if (dec_ctx.branch_map.cnt == 1 && dinfo.target == 0) {
                        if (!dec_ctx.branch_map.full) {
                            info(
                                c,
                                "We hit the not-full branch_map + address edge case, "
                                "(branch following discontinuity is included in this "
                                "packet)\n");
                            /* TODO: should I poison addr? */
                            pc = absolute_addr;
                        } else {
                            info(
                                c,
                                "We hit the full branch_map + address edge case\n");
                            pc = absolute_addr;
                        }
                        hit_discontinuity = true;
                    } else if (dec_ctx.branch_map.cnt > 0
                               || dinfo.target != 0) {
                        /* we should not hit unpredictable discontinuities */
                        pc = dinfo.target;
                    } else {
                        /* we finally hit a jump with unknown destination, thus
                         * the information in this packet is used up
                         */
                        pc = absolute_addr;
                        hit_discontinuity = true;
                        info(c, "Found the discontinuity\n");
                    }
                    break;

                case dis_condbranch:
                    /* this case allows us to exhaust the branch bits */
                    {
                        /* 32 would be undefined */
                        bool branch_taken = dec_ctx.branch_map.bits & 1;
                        dec_ctx.branch_map.bits >>= 1;
                        dec_ctx.branch_map.cnt--;
                        if (dinfo.target == 0)
                            err(c,
                                "Can't predict the jump target, never happens\n");
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
                    err(c, "Invalid insn_type: %d\n", dinfo.insn_type);
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
                free_section_for_debugging(&dinfo);

                section = get_section_for_vma(abfd, pc);
                if (!section) {
                    err(c, "VMA (PC) not pointing to any section\n");
                    goto fail;
                }
                stop_offset =
                    bfd_get_section_size(section) / dinfo.octets_per_byte;

                if (alloc_section_for_debugging(c, abfd, section, &dinfo)) {
                    err(c, "Failed alloc_section_for_debugging\n");
                    goto fail;
                }
                info(c, "Switched to section:%s\n", section->name);
            }

            int status = 0;
            int size = disassemble_at_pc(c, pc, &dis_instr, &dunit, &status);
            if (status != 0)
                goto fail;

            dis_instr.priv = dec_ctx.privilege;
            add_trace(c, instr_list, &dis_instr);

            pc += size;

            switch (dinfo.insn_type) {
            case dis_nonbranch:
                /* TODO: we need this hack since {m,s,u} ret are not
                 * classified in libopcodes
                 */
                if (!is_unpred_discontinuity(dis_instr.instr)) {
                    break;
                }
                info(c, "Detected mret, uret or sret\n");
            case dis_jsr:    /* There is not real difference ... */
            case dis_branch: /* ... between those two */
                if (dinfo.target == 0)
                    err(c, "Can't predict the jump target, never happens\n");
                pc = dinfo.target;
                break;

            case dis_condbranch:
                if (dinfo.target == 0)
                    err(c, "Can't predict the jump target, never happens\n");
                if (packet->branch == 0) {
                    err(c, "Doing a branch from a F_SYNC packet\n");
                    pc = dinfo.target;
                }
                break;
            case dis_dref: /* TODO: is this useful? */
                break;
            case dis_dref2:
            case dis_condjsr:
            case dis_noninsn:
                err(c, "Invalid insn_type: %d\n", dinfo.insn_type);
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
            if (c->config.full_address)
                absolute_addr = packet->address;
            else
                absolute_addr = dec_ctx.last_packet_addr - packet->address;

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
                if (status != 0)
                    goto fail;

                if (pc == absolute_addr)
                    hit_address = true;

                dis_instr.priv = dec_ctx.privilege;
                add_trace(c, instr_list, &dis_instr);

                /* advance pc */
                pc += size;

                switch (dinfo.insn_type) {
                case dis_nonbranch:
                    /* TODO: we need this hack since {m,s,u} ret are not
                     * classified
                     */
                    if (!is_unpred_discontinuity(dis_instr.instr)) {
                        break;
                    }
                    info(c, "Detected mret, uret or sret\n");
                case dis_jsr:    /* There is not real difference ... */
                case dis_branch: /* ... between those two */
                    if (dinfo.target) {
                        pc = dinfo.target;
                    } else {
                        info(c, "Found the discontinuity\n");
                        pc = absolute_addr;
                        hit_discontinuity = true;
                    }
                    break;

                case dis_condbranch:
                    err(c,
                        "We shouldn't hit conditional branches with F_ADDRESS_ONLY\n");
                    break;

                case dis_dref: /* TODO: is this useful? */
                    break;
                case dis_dref2:
                case dis_condjsr:
                case dis_noninsn:
                    err(c, "Invalid insn_type: %d\n", dinfo.insn_type);
                    goto fail;
                }
            }
        }
    }

fail:
    free_section_for_debugging(&dinfo);
    return NULL;
}


/* It's pretty annoying to pack our packets tightly, since our packet members
 * are not 8 bit aligned. We assure that each packet is smaller that 128 bits,
 * pack all packet members into a 128 bit integer and read it out to bytes
 * through the union. Since uint8_t doesn't count as a char the compiler assumes
 * a uint8_t pointer can't alias a __uint128_t, thus the union. Or we could use
 * -fno-strict-aliasing.
 */
union pack {
    __uint128_t bits; /* non-portable gcc stuff. TODO: fix */
    uint8_t bin[16];  /* since uint8_t =/= char strict aliasing might
                       * mess your shit up. Careful this is also
                       * endian dependent
                       */
};

/* bin must be an all zeros array */
int trdb_serialize_packet(struct trdb_ctx *c, struct tr_packet *packet,
                          size_t *bitcnt, uint8_t align, uint8_t bin[])
{
    if (!c->config.full_address) {
        err(c, "full_address false: not implemented yet\n");
        return -1;
    }
    if (align >= 8) {
        err(c, "bad alignment value: %" PRId8 "\n", align);
        return -1;
    }

    union pack data = {0};

    switch (packet->format) {
    case F_BRANCH_FULL: {
        uint32_t len = branch_map_len(packet->branches);

        /* we need enough space to do the packing it in uint128 */
        assert(128 > PACKETLEN + 2 + 5 + 31 + XLEN);
        /* TODO: assert branch map to overfull */
        /* TODO: branch map full logic handling */
        /* TODO: for now we put the address always */
        data.bits = (packet->length) | (packet->format << PACKETLEN)
                    | (packet->branches << (PACKETLEN + 2));
        data.bits |= ((__uint128_t)packet->branch_map & MASK_FROM(len))
                     << (PACKETLEN + 2 + 5);
        data.bits |=
            ((__uint128_t)packet->address << (PACKETLEN + 2 + 5 + len));

        data.bits <<= align;
        *bitcnt = PACKETLEN + 2 + 5 + len + XLEN;
        memcpy(bin, data.bin,
               (*bitcnt + align) / 8 + ((*bitcnt + align) % 8 != 0));

        return 0;
    }

    case F_BRANCH_DIFF:
        err(c, "F_BRANCH_DIFF not implemented yet\n");
        *bitcnt = 0;
        return -1;

    case F_ADDR_ONLY:
        assert(128 > PACKETLEN + 2 + XLEN);
        data.bits = (packet->length) | (packet->format << PACKETLEN)
                    | ((__uint128_t)packet->address << (PACKETLEN + 2));

        *bitcnt = PACKETLEN + 2 + XLEN;

        data.bits <<= align;
        memcpy(bin, data.bin,
               (*bitcnt + align) / 8 + ((*bitcnt + align) % 8 != 0));
        return 0;

    case F_SYNC:
        assert(PRIVLEN == 3);
        /* check for enough space to the packing */
        assert(128 > PACKETLEN + 4 + PRIVLEN + 1 + XLEN + CAUSELEN + 1);
        /* TODO: for now we ignore the context field since we have
         * only one hart
         */

        /* common part to all sub formats */
        data.bits = (packet->length) | (packet->format << PACKETLEN)
                    | (packet->subformat << (PACKETLEN + 2))
                    | (packet->privilege << (PACKETLEN + 4));
        *bitcnt = PACKETLEN + 4 + PRIVLEN;

        switch (packet->subformat) {
        case SF_START:
            data.bits |= (packet->branch << (PACKETLEN + 4 + PRIVLEN))
                         | ((__uint128_t)packet->address
                            << (PACKETLEN + 4 + PRIVLEN + 1));
            *bitcnt += 1 + XLEN;
            break;

        case SF_EXCEPTION:
            data.bits |= (packet->branch << (PACKETLEN + 4 + PRIVLEN))
                         | ((__uint128_t)packet->address
                            << (PACKETLEN + 4 + PRIVLEN + 1))
                         | ((__uint128_t)packet->ecause
                            << (PACKETLEN + 4 + PRIVLEN + 1 + XLEN))
                         | ((__uint128_t)packet->interrupt
                            << (PACKETLEN + 4 + PRIVLEN + 1 + XLEN + CAUSELEN));
            // going to be zero anyway in our case
            //  | ((__uint128_t)packet->tval
            //   << (PACKETLEN + 4 + PRIVLEN + 1 + XLEN + CAUSELEN + 1));
            *bitcnt += (1 + XLEN + CAUSELEN + 1);
            break;

        case SF_CONTEXT:
            /* TODO: we still ignore the context field */
            break;
        }

        data.bits <<= align;
        memcpy(bin, data.bin,
               (*bitcnt + align) / 8 + ((*bitcnt + align) % 8 != 0));
        return 0;
    }
    return -1;
}

int trdb_write_packets(struct trdb_ctx *c, const char *path,
                       struct list_head *packet_list)
{
    int status = 0;
    FILE *fp = fopen(path, "wb");
    if (!fp) {
        err(c, "fopen: %s\n", strerror(errno));
        status = -1;
        goto fail;
    }

    uint8_t bin[sizeof(struct tr_packet)] = {0};
    size_t bitcnt = 0;
    uint32_t alignment = 0;
    uint8_t carry = 0;
    size_t good = 0;
    size_t rest = 0;

    struct tr_packet *packet;
    /* TODO: do we need the rever version? I think we do*/
    list_for_each_entry(packet, packet_list, list)
    {
        if (trdb_serialize_packet(c, packet, &bitcnt, alignment, bin)) {
            status = -1;
            goto fail;
        }
        for (size_t i = 0; i < (bitcnt + alignment) / 8 + 1; i++) {
            /* printf("DUMP: %" PRIx8 "\n", bin[i]); */
        }
        /* stitch two consecutive packets together */
        bin[0] |= carry;
        rest = (bitcnt + alignment) % 8;
        /* printf("REST: %zu\n", rest); */
        good = (bitcnt + alignment) / 8;
        /* write as many bytes as we can i.e. withouth the potentially
         * intersecting ones
         */
        if (fwrite(bin, 1, good, fp) != good) {
            err(c, "fwrite: %s\n", strerror(errno));
            status = -1;
            goto fail;
        }
        /* we keep that for the next packet */
        carry = bin[good] & MASK_FROM(rest);
        alignment = rest;
    }
    /* done, write remaining carry */
    if (!fwrite(&bin[good], 1, 1, fp)) {
        err(c, "fwrite: %s\n", strerror(errno));
        status = -1;
    }
fail:
    if (fp)
        fclose(fp);
    return status;
}


size_t trdb_stimuli_to_trace_list(struct trdb_ctx *c, const char *path,
                                  int *status, struct list_head *instrs)
{
    *status = 0;
    struct tr_instr *samples = NULL;

    FILE *fp = fopen(path, "r");
    if (!fp) {
        err(c, "fopen: %s\n", strerror(errno));
        *status = -1;
        goto fail;
    }
    size_t scnt = 0;

    int ret = 0;
    int valid = 0;
    int exception = 0;
    int interrupt = 0;
    uint32_t cause = 0;
    uint32_t tval = 0;
    uint32_t priv = 0;
    uint32_t iaddr = 0;
    uint32_t instr = 0;
    int compressed = 0;

    size_t size = 128;
    samples = malloc(size * sizeof(*samples));
    if (!samples) {
        *status = -1;
        goto fail;
    }

    while (
        (ret = fscanf(fp,
                      "valid= %d exception= %d interrupt= %d cause= %" SCNx32
                      " tval= %" SCNx32 " priv= %" SCNx32
                      " compressed= %d addr= %" SCNx32 " instr= %" SCNx32 " \n",
                      &valid, &exception, &interrupt, &cause, &tval, &priv,
                      &compressed, &iaddr, &instr))
        != EOF) {
        // TODO: make this configurable so that we don't have to store so
        // much data
        /* if (!valid) { */
        /*     continue; */
        /* } */
        if (scnt >= size) {
            size = 2 * size;
            struct tr_instr *tmp = realloc(samples, size * sizeof(*samples));
            if (!tmp) {
                err(c, "realloc: %s\n", strerror(errno));
                *status = -1;
                goto fail;
            }
            samples = tmp;
        }
        (samples)[scnt] = (struct tr_instr){0};
        (samples)[scnt].valid = valid;
        (samples)[scnt].exception = exception;
        (samples)[scnt].interrupt = interrupt;
        (samples)[scnt].cause = cause;
        (samples)[scnt].tval = tval;
        (samples)[scnt].priv = priv;
        (samples)[scnt].iaddr = iaddr;
        (samples)[scnt].instr = instr;
        (samples)[scnt].compressed = compressed;
        scnt++;
    }

    /* free the remaining unused memory */
    struct tr_instr *tmp = realloc(samples, scnt * sizeof(*samples));
    if (!tmp) {
        err(c, "realloc: %s\n", strerror(errno));
        *status = -1;
        goto fail;
    }
    samples = tmp;

    /* add entries to list */
    for (size_t i = 0; i < scnt; i++) {
        list_add(&((samples)[scnt].list), instrs);
    }

    if (ferror(fp)) {
        err(c, "fscanf: %s\n", strerror(errno));
        *status = -1;
        goto fail;
    }

    if (fp)
        fclose(fp);
    return scnt;
fail:
    if (fp)
        fclose(fp);
    free(samples);
    return 0;
}


/* TODO: this double pointer mess is a bit ugly. Maybe use the list.h anyway?*/
size_t trdb_stimuli_to_trace(struct trdb_ctx *c, const char *path,
                             struct tr_instr **samples, int *status)
{
    *status = 0;
    FILE *fp = fopen(path, "r");
    if (!fp) {
        err(c, "fopen: %s\n", strerror(errno));
        *status = -1;
        goto fail;
    }
    size_t scnt = 0;

    int ret = 0;
    int valid = 0;
    int exception = 0;
    int interrupt = 0;
    uint32_t cause = 0;
    uint32_t tval = 0;
    uint32_t priv = 0;
    uint32_t iaddr = 0;
    uint32_t instr = 0;
    int compressed = 0;

    size_t size = 128;
    *samples = malloc(size * sizeof(**samples));
    if (!*samples) {
        *status = -1;
        goto fail;
    }

    while (
        (ret = fscanf(fp,
                      "valid= %d exception= %d interrupt= %d cause= %" SCNx32
                      " tval= %" SCNx32 " priv= %" SCNx32
                      " compressed= %d addr= %" SCNx32 " instr= %" SCNx32 " \n",
                      &valid, &exception, &interrupt, &cause, &tval, &priv,
                      &compressed, &iaddr, &instr))
        != EOF) {
        // TODO: make this configurable so that we don't have to store so much
        // data
        /* if (!valid) { */
        /*     continue; */
        /* } */
        if (scnt >= size) {
            size = 2 * size;
            struct tr_instr *tmp = realloc(*samples, size * sizeof(**samples));
            if (!tmp) {
                err(c, "realloc: %s\n", strerror(errno));
                *status = -1;
                goto fail;
            }
            *samples = tmp;
        }
        (*samples)[scnt] = (struct tr_instr){0};
        (*samples)[scnt].valid = valid;
        (*samples)[scnt].exception = exception;
        (*samples)[scnt].interrupt = interrupt;
        (*samples)[scnt].cause = cause;
        (*samples)[scnt].tval = tval;
        (*samples)[scnt].priv = priv;
        (*samples)[scnt].iaddr = iaddr;
        (*samples)[scnt].instr = instr;
        (*samples)[scnt].compressed = compressed;
        scnt++;
    }
    /* initialize the remaining unitialized memory */
    memset((*samples) + scnt, 0, size - scnt);

    if (ferror(fp)) {
        err(c, "fscanf: %s\n", strerror(errno));
        *status = -1;
        goto fail;
    }

    if (fp)
        fclose(fp);
    return scnt;
fail:
    if (fp)
        fclose(fp);
    free(*samples);
    *samples = NULL;
    return 0;
}


void trdb_disassemble_trace(size_t len, struct tr_instr trace[len],
                            struct disassembler_unit *dunit)
{
    struct disassemble_info *dinfo = dunit->dinfo;
    for (size_t i = 0; i < len; i++) {
        (*dinfo->fprintf_func)(dinfo->stream, "0x%016jx  ",
                               (uintmax_t)trace[i].iaddr);
        (*dinfo->fprintf_func)(dinfo->stream, "0x%08jx  ",
                               (uintmax_t)trace[i].instr);
        (*dinfo->fprintf_func)(dinfo->stream, "%s",
                               trace[i].exception ? "TRAP!  " : "");
        disassemble_single_instruction(trace[i].instr, trace[i].iaddr, dunit);
    }
}


void trdb_disassemble_instr(struct tr_instr *instr,
                            struct disassembler_unit *dunit)
{
    struct disassemble_info *dinfo = dunit->dinfo;
    (*dinfo->fprintf_func)(dinfo->stream, "0x%016jx  ",
                           (uintmax_t)instr->iaddr);
    (*dinfo->fprintf_func)(dinfo->stream, "0x%08jx  ", (uintmax_t)instr->instr);
    (*dinfo->fprintf_func)(dinfo->stream, "%s",
                           instr->exception ? "TRAP!  " : "");
    disassemble_single_instruction(instr->instr, instr->iaddr, dunit);
}


void trdb_dump_packet_list(FILE *stream, const struct list_head *packet_list)
{
    struct tr_packet *packet;
    list_for_each_entry_reverse(packet, packet_list, list)
    {
        trdb_print_packet(stream, packet);
    }
}


void trdb_log_packet(struct trdb_ctx *c, const struct tr_packet *packet)
{
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
        dbg(c, "    interrupt : %s\n", packet->interrupt ? "true" : "false");
        dbg(c, "    tval      : 0x%" PRIx32 "\n", packet->tval);
        /* SF_EXCEPTION */
    }
}


void trdb_print_packet(FILE *stream, const struct tr_packet *packet)
{
    switch (packet->format) {
    case F_BRANCH_FULL:
    case F_BRANCH_DIFF:
        fprintf(stream, "PACKET ");
        packet->format == F_BRANCH_FULL ? fprintf(stream, "0: F_BRANCH_FULL\n")
                                        : fprintf(stream, "1: F_BRANCH_DIFF\n");
        fprintf(stream, "    branches  : %" PRIu32 "\n", packet->branches);
        fprintf(stream, "    branch_map: 0x%" PRIx32 "\n", packet->branch_map);
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
        fprintf(stream, "    privilege : 0x%" PRIx32 "\n", packet->privilege);
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
}


void trdb_log_instr(struct trdb_ctx *c, const struct tr_instr *instr)
{
    dbg(c, "INSTR\n");
    dbg(c, "    exception : %s\n", instr->exception ? "true" : "false");
    dbg(c, "    interrupt : %s\n", instr->interrupt ? "true" : "false");
    dbg(c, "    cause     : 0x%" PRIx32 "\n", instr->cause);
    dbg(c, "    tval      : 0x%" PRIx32 "\n", instr->tval);
    dbg(c, "    priv      : 0x%" PRIx32 "\n", instr->priv);
    dbg(c, "    iaddr     : 0x%" PRIx32 "\n", instr->iaddr);
    dbg(c, "    instr     : 0x%" PRIx32 "\n", instr->instr);
    dbg(c, "    compressed: %s\n", instr->compressed ? "true" : "false");
}


void trdb_print_instr(FILE *stream, const struct tr_instr *instr)
{
    fprintf(stream, "INSTR\n");
    fprintf(stream, "    exception : %s\n",
            instr->exception ? "true" : "false");
    fprintf(stream, "    interrupt : %s\n",
            instr->interrupt ? "true" : "false");
    fprintf(stream, "    cause     : 0x%" PRIx32 "\n", instr->cause);
    fprintf(stream, "    tval      : 0x%" PRIx32 "\n", instr->tval);
    fprintf(stream, "    priv      : 0x%" PRIx32 "\n", instr->priv);
    fprintf(stream, "    iaddr     : 0x%" PRIx32 "\n", instr->iaddr);
    fprintf(stream, "    instr     : 0x%" PRIx32 "\n", instr->instr);
    fprintf(stream, "    compressed: %s\n",
            instr->compressed ? "true" : "false");
}


void trdb_free_packet_list(struct list_head *packet_list)
{
    struct tr_packet *packet;
    struct tr_packet *packet_next;
    list_for_each_entry_safe(packet, packet_next, packet_list, list)
    {
        free(packet);
    }
}

void trdb_free_instr_list(struct list_head *instr_list)
{
    struct tr_instr *instr;
    struct tr_instr *instr_next;
    list_for_each_entry_safe(instr, instr_next, instr_list, list)
    {
        free(instr);
    }
}


/* --------------------------------------------------------------------------*/
/* Below are just old, deprecated functions which are just used for regression
 * tests
 */
/* --------------------------------------------------------------------------*/
static struct trdb_config conf = {0};
static struct trdb_state lastc = {0};
static struct trdb_state thisc = {0};
static struct trdb_state nextc = {0};
static struct branch_map_state branch_map = {0};
static struct filter_state filter = {0};


void trdb_init()
{
    conf = (struct trdb_config){
        .resync_max = UINT64_MAX, .full_address = true, .no_aliases = true};
    lastc = (struct trdb_state){0};
    thisc = (struct trdb_state){0};
    nextc = (struct trdb_state){0};
    branch_map = (struct branch_map_state){0};
    filter = (struct filter_state){0};
}


void trdb_close()
{
}


static bool branch_taken_legacy(struct tr_instr before, struct tr_instr after)
{
    /* can this cause issues with RVC + degenerate jump (+2)? -> yes*/
    /* TODO: this definitely doens't work for 64 bit instructions */
    /* since we have already decompressed instructions, but still compressed
     * addresses we need this additional flag to tell us what the isntruction
     * originally was. So we can't tell by looking at the lower two bits of
     * instr.
     */
    return before.compressed ? !(before.iaddr + 2 == after.iaddr)
                             : !(before.iaddr + 4 == after.iaddr);
}


struct list_head *trdb_compress_trace_legacy(struct list_head *packet_list,
                                             size_t len,
                                             struct tr_instr instrs[len])
{
    bool full_address = conf.full_address;

    /* for each cycle */
    for (size_t i = 0; i < len - 1; i++) {
        thisc.halt = false;
        /* test for qualification by filtering */
        thisc.qualified = true; /* TODO: implement filtering logic */
        nextc.qualified = true;

        thisc.unqualified = !thisc.qualified;
        nextc.unqualified = !nextc.qualified;

        /* Update state TODO: maybe just ignore last sample instead?*/
        thisc.exception = instrs[i].exception;
        nextc.exception = i < len ? instrs[i + 1].exception : thisc.exception;

        thisc.unpred_disc = is_unpred_discontinuity(instrs[i].instr);
        nextc.unpred_disc =
            i < len ? is_unpred_discontinuity(instrs[i + 1].instr) : false;

        thisc.privilege = instrs[i].priv;
        nextc.privilege = i < len ? instrs[i + 1].priv : thisc.privilege;

        thisc.privilege_change = (thisc.privilege != lastc.privilege);
        nextc.privilege_change = (thisc.privilege != nextc.privilege);
        /* TODO: clean this up, proper initial state per round required */
        thisc.emitted_exception_sync = false;

        bool firstc_qualified = !lastc.qualified && thisc.qualified;

        /* Start of one cycle */
        if (!thisc.qualified) {
            /* check if we even need to record anything */
            lastc = thisc;
            thisc = nextc;
            continue; /* end of cycle */
        }

        if (is_unsupported(instrs[i].instr)) {
            LOG_ERRT("Instruction is not supported for compression: 0x%" PRIx32
                     " at addr: 0x%" PRIx32 "\n",
                     instrs[i].instr, instrs[i].iaddr);
            goto fail;
        }

        if (filter.resync_cnt++ == conf.resync_max) {
            filter.resync_pend = true;
            filter.resync_cnt = 0;
        }

        if (is_branch(instrs[i].instr)) {
            /* update branch map */
            /* in hardware maybe mask and compare is better ? */
            if ((i + 1 < len) && branch_taken_legacy(instrs[i], instrs[i + 1]))
                branch_map.bits = branch_map.bits | (1u << branch_map.cnt);
            branch_map.cnt++;
            if (branch_map.cnt == 31) {
                branch_map.full = true;
            }
        }
        /* We trace the packet before the trapped instruction and the
         * first one of the exception handler
         */
        if (lastc.exception) {
            /* Send te_inst:
             * format 3
             * subformat 1 (exception -> all fields present)
             * resync_pend = 0
             */
            ALLOC_INIT_PACKET(tr);
            tr->format = F_SYNC; /* sync */
            tr->subformat = 1;   /* exception */
            tr->context = 0;     /* TODO: what comes here? */
            tr->privilege = instrs[i].priv;
            /* TODO: actually we should clear the branch map if we really
             * fill out this entry, else we have recorded twice (in the next
             * packet)
             */
            if (is_branch(instrs[i].instr)
                && !branch_taken_legacy(instrs[i], instrs[i + 1]))
                tr->branch = 1;
            else
                tr->branch = 0;

            tr->address = instrs[i].iaddr;
            /* With this packet we record last cycles exception
             * information. It's not possible for (i==0 &&
             * lastc_exception) to be true since it takes one cycle
             * for lastc_exception to change
             */
            assert(i != 0);
            tr->ecause = instrs[i - 1].cause;
            tr->interrupt = instrs[i - 1].interrupt;
            tr->tval = instrs[i - 1].tval;
            list_add(&tr->list, packet_list);

            thisc.emitted_exception_sync = true;
            filter.resync_pend = false; /* TODO: how to handle this */
            /* end of cycle */

        } else if (lastc.emitted_exception_sync) {
            /* First we assume that the vector table entry is a jump. Since that
             * entry can change during runtime, we need to emit the jump
             * destination address, which is the second instruction of the trap
             * handler. This a bit hacky and made to work for the PULP. If
             * someone puts something else than a jump instruction there then
             * all bets are off. This is a custom change.
             * TODO: merge this with lastc.unpred_discon
             */
            /* Send te_inst:
             * format 0/1/2
             */
            ALLOC_INIT_PACKET(tr);
            /* TODO: for now only full address */
            if (!full_address) {
                LOG_ERRT("full_address false: Not implemented yet\n");
                goto fail;
            }
            if (branch_map.cnt == 0) {
                tr->format = F_ADDR_ONLY;
                tr->address = full_address ? instrs[i].iaddr : 0;

                assert(branch_map.bits == 0);
            } else {
                tr->format = full_address ? F_BRANCH_FULL : F_BRANCH_DIFF;
                tr->branches = branch_map.cnt;
                tr->branch_map = branch_map.bits;
                tr->address = full_address ? instrs[i].iaddr : 0;

                branch_map = (struct branch_map_state){0};
            }
            list_add(&tr->list, packet_list);

            /* end of cycle */

        } else if (firstc_qualified || thisc.unhalted || thisc.privilege_change
                   || (filter.resync_pend && branch_map.cnt == 0)) {

            /* Start packet */
            /* Send te_inst:
             * format 3
             * subformat 0 (start, no ecause, interrupt and tval)
             * resync_pend = 0
             */
            ALLOC_INIT_PACKET(tr);
            tr->format = F_SYNC; /* sync */
            tr->subformat = 0;   /* start */
            tr->context = 0;     /* TODO: what comes here? */
            tr->privilege = instrs[i].priv;
            if (is_branch(instrs[i].instr)
                && !branch_taken_legacy(instrs[i], instrs[i + 1]))
                tr->branch = 1;
            else
                tr->branch = 0;
            tr->address = instrs[i].iaddr;
            list_add(&tr->list, packet_list);

            filter.resync_pend = false;
            /* end of cycle */

        } else if (lastc.unpred_disc) {
            /* Send te_inst:
             * format 0/1/2
             */
            ALLOC_INIT_PACKET(tr);
            /* TODO: for now only full address */
            if (!full_address) {
                LOG_ERRT("full_address false: Not implemented yet\n");
                goto fail;
            }
            if (branch_map.cnt == 0) {
                tr->format = F_ADDR_ONLY;
                tr->address = full_address ? instrs[i].iaddr : 0;

                assert(branch_map.bits == 0);
            } else {
                tr->format = full_address ? F_BRANCH_FULL : F_BRANCH_DIFF;
                tr->branches = branch_map.cnt;
                tr->branch_map = branch_map.bits;
                tr->address = instrs[i].iaddr;

                branch_map = (struct branch_map_state){0};
            }
            list_add(&tr->list, packet_list);

            /* end of cycle */

        } else if (filter.resync_pend && branch_map.cnt > 0) {
            /* we treat resync_pend && branches == 0 before */
            /* Send te_inst:
             * format 0/1/2
             */
            ALLOC_INIT_PACKET(tr);
            /* TODO: for now only full address */
            if (!full_address) {
                LOG_ERRT("full_address false: Not implemented yet\n");
                goto fail;
            }
            tr->format = full_address ? F_BRANCH_FULL : F_BRANCH_DIFF;
            tr->branches = branch_map.cnt;
            tr->branch_map = branch_map.bits;
            tr->address = full_address ? instrs[i].iaddr : 0;
            list_add(&tr->list, packet_list);

            branch_map = (struct branch_map_state){0};
            /* end of cycle */

        } else if (nextc.halt || nextc.exception || nextc.privilege_change
                   || nextc.unqualified) {
            /* Send te_inst:
             * format 0/1/2
             */
            ALLOC_INIT_PACKET(tr);
            /* TODO: for now only full address */
            if (!full_address) {
                LOG_ERRT("full_address false: Not implemented yet\n");
                goto fail;
            }
            if (branch_map.cnt == 0) {
                tr->format = F_ADDR_ONLY;
                tr->address = full_address ? instrs[i].iaddr : 0;

                assert(branch_map.bits == 0);
            } else {
                tr->format = full_address ? F_BRANCH_FULL : F_BRANCH_DIFF;
                tr->branches = branch_map.cnt;
                tr->branch_map = branch_map.bits;
                tr->address = full_address ? instrs[i].iaddr : 0;

                branch_map = (struct branch_map_state){0};
            }
            list_add(&tr->list, packet_list);

            /* end of cycle */

        } else if (branch_map.full) {
            /* Send te_inst:
             * format 0
             * no address
             */
            assert(branch_map.cnt == 31);
            ALLOC_INIT_PACKET(tr);
            tr->format = F_BRANCH_FULL;
            tr->branches = branch_map.cnt;
            tr->branch_map = branch_map.bits;
            /* tr->address  TODO: no address, study explanation */
            list_add(&tr->list, packet_list);

            branch_map = (struct branch_map_state){0};
            /* end of cycle */

        } else if (thisc.context_change) {
            /* TODO: don't understand how to detect context change */
            /* Send te_inst:
             * format 3
             * subformat 2
             */
            ALLOC_INIT_PACKET(tr);
            tr->format = F_SYNC;
            tr->subformat = 2;
            tr->context = 0; /* TODO: what comes here? */
            tr->privilege = instrs[i].priv;
            /* tr->branch */
            /* tr->address */
            /* tr->ecause */
            /* tr->interrupt */
            /* tr->tval */
            list_add(&tr->list, packet_list);
        }

        /* update last cycle state */
        lastc = thisc;
        thisc = nextc;
    }
    return packet_list;
fail:
fail_malloc:
    trdb_free_packet_list(packet_list);
    return NULL;
}

struct list_head *trdb_compress_trace(struct list_head *packet_list, size_t len,
                                      struct tr_instr instrs[len])
{
    bool full_address = conf.full_address;

    /* for each cycle */
    // TODO: fix this hack by doing unqualified instead
    for (size_t i = 0; i < len + 3; i++) {
        thisc.halt = false;
        /* test for qualification by filtering */
        /* TODO: implement filtering logic */

        nextc.instr = i < len ? instrs[i] : nextc.instr;

        struct tr_instr *nc_instr = &nextc.instr;
        struct tr_instr *tc_instr = &thisc.instr;
        struct tr_instr *lc_instr = &lastc.instr;

        /* nextc.qualified = true; */
        /* thisc.qualified = true; */
        nextc.qualified = true;

        /* thisc.unqualified = !thisc.qualified; */
        /* nextc.unqualified = !nextc.qualified; */
        nextc.unqualified = !nextc.qualified;

        /* Update state TODO: maybe just ignore last sample instead?*/
        /* thisc.exception = instrs[i].exception; */
        /* nextc.exception = i < len ? instrs[i + 1].exception :
         * thisc.exception; */
        nextc.exception = i < len ? instrs[i].exception : nextc.exception;

        /* thisc.unpred_disc = is_unpred_discontinuity(instrs[i].instr); */
        /* nextc.unpred_disc = */
        /* i < len ? is_unpred_discontinuity(instrs[i + 1].instr) : false; */
        nextc.unpred_disc =
            i < len ? is_unpred_discontinuity(instrs[i].instr) : false;

        /* thisc.privilege = instrs[i].priv; */
        /* nextc.privilege = i < len ? instrs[i + 1].priv : thisc.privilege; */
        nextc.privilege = i < len ? instrs[i].priv : nextc.privilege;

        /* thisc.privilege_change = (thisc.privilege != lastc.privilege); */
        /* nextc.privilege_change = (thisc.privilege != nextc.privilege); */
        nextc.privilege_change = (thisc.privilege != nextc.privilege);

        /* TODO: clean this up, proper initial state per round required */
        thisc.emitted_exception_sync = false;

        bool firstc_qualified = !lastc.qualified && thisc.qualified;

        /* Start of one cycle */
        if (!nextc.instr.valid) {
            continue;
        }
        if (!thisc.qualified) {
            /* check if we even need to record anything */
            lastc = thisc;
            thisc = nextc;
            continue; /* end of cycle */
        }

        if (is_unsupported(tc_instr->instr)) {
            LOG_ERRT("Instruction is not supported for compression: 0x%" PRIx32
                     " at addr: 0x%" PRIx32 "\n",
                     tc_instr->instr, tc_instr->iaddr);
            goto fail;
        }

        if (filter.resync_cnt++ == conf.resync_max) {
            filter.resync_pend = true;
            filter.resync_cnt = 0;
        }

        if (is_branch(tc_instr->instr)) {
            /* update branch map */
            /* in hardware maybe mask and compare is better ? */
            if ((i + 1 < len)
                && branch_taken(tc_instr->compressed, tc_instr->iaddr,
                                nc_instr->iaddr))
                branch_map.bits = branch_map.bits | (1u << branch_map.cnt);
            branch_map.cnt++;
            if (branch_map.cnt == 31) {
                branch_map.full = true;
            }
        }
        /* We trace the packet before the trapped instruction and the
         * first one of the exception handler
         */
        if (lastc.exception) {
            /* Send te_inst:
             * format 3
             * subformat 1 (exception -> all fields present)
             * resync_pend = 0
             */
            ALLOC_INIT_PACKET(tr);
            tr->format = F_SYNC; /* sync */
            tr->subformat = 1;   /* exception */
            tr->context = 0;     /* TODO: what comes here? */
            tr->privilege = tc_instr->priv;
            /* TODO: actually we should clear the branch map if we really
             * fill out this entry, else we have recorded twice (in the next
             * packet)
             */
            if (is_branch(tc_instr->instr)
                && !branch_taken(tc_instr->compressed, tc_instr->iaddr,
                                 nc_instr->iaddr))
                tr->branch = 1;
            else
                tr->branch = 0;

            tr->address = tc_instr->iaddr;
            /* With this packet we record last cycles exception
             * information. It's not possible for (i==0 &&
             * lastc_exception) to be true since it takes one cycle
             * for lastc_exception to change
             */
            assert(i != 0);
            tr->ecause = lc_instr->cause;
            tr->interrupt = lc_instr->interrupt;
            tr->tval = lc_instr->tval;
            list_add(&tr->list, packet_list);

            thisc.emitted_exception_sync = true;
            filter.resync_pend = false; /* TODO: how to handle this */
            /* end of cycle */

        } else if (lastc.emitted_exception_sync) {
            /* First we assume that the vector table entry is a jump. Since that
             * entry can change during runtime, we need to emit the jump
             * destination address, which is the second instruction of the trap
             * handler. This a bit hacky and made to work for the PULP. If
             * someone puts something else than a jump instruction there then
             * all bets are off. This is a custom change.
             * TODO: merge this with lastc.unpred_discon
             */
            /* Send te_inst:
             * format 0/1/2
             */
            ALLOC_INIT_PACKET(tr);
            /* TODO: for now only full address */
            if (!full_address) {
                LOG_ERRT("full_address false: Not implemented yet\n");
                goto fail;
            }
            if (branch_map.cnt == 0) {
                tr->format = F_ADDR_ONLY;
                tr->address = full_address ? tc_instr->iaddr : 0;

                assert(branch_map.bits == 0);
            } else {
                tr->format = full_address ? F_BRANCH_FULL : F_BRANCH_DIFF;
                tr->branches = branch_map.cnt;
                tr->branch_map = branch_map.bits;
                tr->address = full_address ? tc_instr->iaddr : 0;

                branch_map = (struct branch_map_state){0};
            }
            list_add(&tr->list, packet_list);

            /* end of cycle */

        } else if (firstc_qualified || thisc.unhalted || thisc.privilege_change
                   || (filter.resync_pend && branch_map.cnt == 0)) {

            /* Start packet */
            /* Send te_inst:
             * format 3
             * subformat 0 (start, no ecause, interrupt and tval)
             * resync_pend = 0
             */
            ALLOC_INIT_PACKET(tr);
            tr->format = F_SYNC; /* sync */
            tr->subformat = 0;   /* start */
            tr->context = 0;     /* TODO: what comes here? */
            tr->privilege = tc_instr->priv;
            if (is_branch(tc_instr->instr)
                && !branch_taken(tc_instr->compressed, tc_instr->iaddr,
                                 nc_instr->iaddr))
                tr->branch = 1;
            else
                tr->branch = 0;
            tr->address = tc_instr->iaddr;
            list_add(&tr->list, packet_list);

            filter.resync_pend = false;
            /* end of cycle */

        } else if (lastc.unpred_disc) {
            /* Send te_inst:
             * format 0/1/2
             */
            ALLOC_INIT_PACKET(tr);
            /* TODO: for now only full address */
            if (!full_address) {
                LOG_ERRT("full_address false: Not implemented yet\n");
                goto fail;
            }
            if (branch_map.cnt == 0) {
                tr->format = F_ADDR_ONLY;
                tr->address = full_address ? tc_instr->iaddr : 0;

                assert(branch_map.bits == 0);
            } else {
                tr->format = full_address ? F_BRANCH_FULL : F_BRANCH_DIFF;
                tr->branches = branch_map.cnt;
                tr->branch_map = branch_map.bits;
                tr->address = tc_instr->iaddr;

                branch_map = (struct branch_map_state){0};
            }
            list_add(&tr->list, packet_list);

            /* end of cycle */

        } else if (filter.resync_pend && branch_map.cnt > 0) {
            /* we treat resync_pend && branches == 0 before */
            /* Send te_inst:
             * format 0/1/2
             */
            ALLOC_INIT_PACKET(tr);
            /* TODO: for now only full address */
            if (!full_address) {
                LOG_ERRT("full_address false: Not implemented yet\n");
                goto fail;
            }
            tr->format = full_address ? F_BRANCH_FULL : F_BRANCH_DIFF;
            tr->branches = branch_map.cnt;
            tr->branch_map = branch_map.bits;
            tr->address = full_address ? tc_instr->iaddr : 0;
            list_add(&tr->list, packet_list);

            branch_map = (struct branch_map_state){0};
            /* end of cycle */

        } else if (nextc.halt || nextc.exception || nextc.privilege_change
                   || nextc.unqualified) {
            /* Send te_inst:
             * format 0/1/2
             */
            ALLOC_INIT_PACKET(tr);
            /* TODO: for now only full address */
            if (!full_address) {
                LOG_ERRT("full_address false: Not implemented yet\n");
                goto fail;
            }
            if (branch_map.cnt == 0) {
                tr->format = F_ADDR_ONLY;
                tr->address = full_address ? tc_instr->iaddr : 0;

                assert(branch_map.bits == 0);
            } else {
                tr->format = full_address ? F_BRANCH_FULL : F_BRANCH_DIFF;
                tr->branches = branch_map.cnt;
                tr->branch_map = branch_map.bits;
                tr->address = full_address ? tc_instr->iaddr : 0;

                branch_map = (struct branch_map_state){0};
            }
            list_add(&tr->list, packet_list);

            /* end of cycle */

        } else if (branch_map.full) {
            /* Send te_inst:
             * format 0
             * no address
             */
            assert(branch_map.cnt == 31);
            ALLOC_INIT_PACKET(tr);
            tr->format = F_BRANCH_FULL;
            tr->branches = branch_map.cnt;
            tr->branch_map = branch_map.bits;
            /* tr->address  TODO: no address, study explanation */
            list_add(&tr->list, packet_list);

            branch_map = (struct branch_map_state){0};
            /* end of cycle */

        } else if (thisc.context_change) {
            /* TODO: don't understand how to detect context change */
            /* Send te_inst:
             * format 3
             * subformat 2
             */
            ALLOC_INIT_PACKET(tr);
            tr->format = F_SYNC;
            tr->subformat = 2;
            tr->context = 0; /* TODO: what comes here? */
            tr->privilege = tc_instr->priv;
            /* tr->branch */
            /* tr->address */
            /* tr->ecause */
            /* tr->interrupt */
            /* tr->tval */
            list_add(&tr->list, packet_list);
        }

        /* update last cycle state */
        lastc = thisc;
        thisc = nextc;
    }
    return packet_list;
fail:
fail_malloc:
    trdb_free_packet_list(packet_list);
    return NULL;
}

struct list_head *trdb_decompress_trace_guess(struct trdb_ctx *c, bfd *abfd,
                                              struct list_head *packet_list,
                                              struct list_head *instr_list)
{
    /* We assume our hw block in the pulp generated little endian
     * addresses, thus we should make sure that before we interact
     * with bfd addresses to convert this foreign format to the local
     * host format
     */
    /* TODO: supports only statically linked elf executables */

    /* find section belonging to start_address */
    bfd_vma start_address = abfd->start_address;
    asection *section = get_section_for_vma(abfd, start_address);
    if (!section) {
        err(c, "VMA not pointing to any section\n");
        goto fail;
    }
    info(c, "Section of start_address:%s\n", section->name);

    struct disassembler_unit dunit = {0};
    struct disassemble_info dinfo = {0};
    struct trdb_dec_state decomp_ctx = {.privilege = 7};
    struct tr_instr dis_instr = {0};

    dunit.dinfo = &dinfo;
    init_disassembler_unit(&dunit, abfd,
                           c->config.no_aliases ? "no-aliases" : NULL);
    /* advanced fprintf output handling */
    dunit.dinfo->fprintf_func = build_instr_fprintf;
    /* dunit.dinfo->stream = &instr; */

    /* Alloc and config section data for disassembler */
    bfd_vma stop_offset = bfd_get_section_size(section) / dinfo.octets_per_byte;

    if (alloc_section_for_debugging(c, abfd, section, &dinfo)) {
        err(c, "Failed alloc_section_for_debugging\n");
        goto fail;
    }
    bfd_vma pc = start_address; /* TODO: well we get a sync packet anyway... */

    struct branch_map_state branch_map = {0};
    /* we need to be able to look at two packets to make the right
     * decompression decision
     */
    struct tr_packet *last_packet = NULL;
    struct tr_packet *packet = NULL;
    struct tr_packet *next_packet = NULL;

    struct list_head *p;

    list_for_each_prev(p, packet_list)
    {
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

        /* Sometimes we leave the current section (e.g. changing from
         * the .start to the .text section), so let's load the
         * appropriate section and remove the old one
         */
        if (pc >= section->vma + stop_offset || pc < section->vma) {
            free_section_for_debugging(&dinfo);

            section = get_section_for_vma(abfd, pc);
            if (!section) {
                err(c, "VMA (PC) not pointing to any section\n");
                goto fail;
            }
            stop_offset = bfd_get_section_size(section) / dinfo.octets_per_byte;

            if (alloc_section_for_debugging(c, abfd, section, &dinfo)) {
                err(c, "Failed alloc_section_for_debugging\n");
                goto fail;
            }
            info(c, "Switched to section:%s\n", section->name);
        }

        switch (packet->format) {
        case F_BRANCH_FULL:
        case F_BRANCH_DIFF:
            branch_map.cnt = packet->branches;
            branch_map.bits = packet->branch_map;
            branch_map.full = packet->branches == 31;
            break;
        case F_SYNC:
            break;
        case F_ADDR_ONLY:
            break;
        default:
            err(c, "Unimplemented\n");
            goto fail;
        }

        if (trdb_get_log_priority(c) == LOG_DEBUG) {
            /* TODO: leverage log_fn */
            trdb_print_packet(stderr, packet);
        }

        if (last_packet && last_packet->format == F_SYNC
            && last_packet->subformat == SF_EXCEPTION) {
            /* exceptions come with three packets: flush, F_SYNC, and
             * F_ADDR_ONLY/F_BRANCH_*. The last packet is kinda of a hack to
             * bridge over the vector table jump instruction (which can change
             * at runtime)
             */
            assert(packet->format == F_BRANCH_FULL
                   || packet->format == F_ADDR_ONLY
                   || packet->format == F_BRANCH_DIFF);
            info(c, "Jumping over vector table\n");

            pc = packet->address;

            /* since we are abruptly changing the pc we have to check if we
             * leave the section before we can disassemble. This is really ugly
             * TODO: fix
             */
            if (pc >= section->vma + stop_offset || pc < section->vma) {
                free_section_for_debugging(&dinfo);

                section = get_section_for_vma(abfd, pc);
                if (!section) {
                    err(c, "VMA (PC) not pointing to any section\n");
                    goto fail;
                }
                stop_offset =
                    bfd_get_section_size(section) / dinfo.octets_per_byte;

                if (alloc_section_for_debugging(c, abfd, section, &dinfo)) {
                    err(c, "Failed alloc_section_for_debugging\n");
                    goto fail;
                }
                info(c, "Switched to section:%s\n", section->name);
            }

            int status = 0;
            int size = disassemble_at_pc(c, pc, &dis_instr, &dunit, &status);
            if (status != 0)
                goto fail;

            dis_instr.priv = decomp_ctx.privilege;
            add_trace(c, instr_list, &dis_instr);

            pc += size; /* normal case */

            switch (dinfo.insn_type) {
            case dis_nonbranch:
                /* TODO: we need this hack since {m,s,u} ret are not classified
                 * in libopcodes
                 */
                if (!is_unpred_discontinuity(dis_instr.instr)) {
                    break;
                }
                info(c, "Detected mret, uret or sret\n");
            case dis_jsr:    /* There is not real difference ... */
            case dis_branch: /* ... between those two */
                if (dinfo.target == 0) {
                    err(c,
                        "First instruction after vector table is unpredictable "
                        "discontinuity\n");
                    err(c, "TODO: fix this case by looking at next packet\n");
                    goto fail;
                }
                pc = dinfo.target;
                break;

            case dis_condbranch:
                if (dinfo.target == 0)
                    err(c, "Can't predict the jump target, never happens\n");
                if (packet->branches != 1)
                    err(c, "Wrong number of branch entries\n");
                if (packet->branch_map & 1) {
                    err(c,
                        "Doing a branch in the first intruction after vector table "
                        "jump\n");
                    pc = dinfo.target;
                }
                break;

            case dis_dref: /* TODO: is this useful? */
            case dis_dref2:
            case dis_condjsr:
            case dis_noninsn:
                err(c, "Invalid insn_type: %d\n", dinfo.insn_type);
                goto fail;
            }
        } else if (packet->format == F_BRANCH_FULL) {
            /* TODO: not true for discontinuous before last branch*/
            /* bool sync_consumed = branch_map.cnt == 31; */
            /* We have to find the instruction where we can apply the address
             * information to. This might either be a discontinuity infromation
             * or a sync up address. The search_discontinuity
             * variable tells us which one is it
             */
            bool hit_address = false; /* TODO: for now we don't allow
                                       * resync packets, see todo.org
                                       */
            /* We don't need to care about the address field if the branch map
             * is full,(except if full and instr before last branch is
             * discontinuity)
             */
            bool hit_discontinuity = branch_map.full;

            bool search_discontinuity = true;
            if (next_packet == NULL
                || (next_packet->format == F_SYNC
                    && (next_packet->subformat == SF_START
                        || next_packet->subformat == SF_EXCEPTION))) {
                /* advance until address */
                info(c, "Searching for address\n");
                search_discontinuity = false;
            } else {
                /* advance until unpredictable discontinuity */
                info(c, "Searching for discontinuity\n");
                search_discontinuity = true;
            }

            while (
                !(branch_map.cnt == 0 && (hit_discontinuity || hit_address))) {

                int status = 0;
                int size =
                    disassemble_at_pc(c, pc, &dis_instr, &dunit, &status);
                if (status != 0)
                    goto fail;

                if (!search_discontinuity && branch_map.cnt == 0
                    && pc == packet->address)
                    hit_address = true;

                dis_instr.priv = decomp_ctx.privilege;
                add_trace(c, instr_list, &dis_instr);

                /* advance pc */
                pc += size;

                /* we hit a conditional branch, follow or not and update map */
                switch (dinfo.insn_type) {
                case dis_nonbranch:
                    /* TODO: we need this hack since {m,s,u} ret are not
                     * classified
                     */
                    if (!is_unpred_discontinuity(dis_instr.instr)) {
                        break;
                    }
                    info(c, "Detected mret, uret or sret\n");
                case dis_jsr:    /* There is not real difference ... */
                case dis_branch: /* ... between those two */
                    /* we know that this instruction must have its jump target
                     * encoded in the binary else we would have gotten a
                     * non-predictable discontinuity packet. If
                     * branch_map.cnt == 0 + jump target unknown and we are here
                     * then we know that its actually a branch_map
                     * flush + discontinuity packet.
                     */
                    if (branch_map.cnt > 1 && dinfo.target == 0)
                        err(c,
                            "Can't predict the jump target, never happens\n");

                    if (branch_map.cnt == 1 && dinfo.target == 0) {
                        if (!branch_map.full) {
                            info(
                                c,
                                "We hit the not-full branch_map + address edge case, "
                                "(branch following discontinuity is included in this "
                                "packet)\n");
                            if (!search_discontinuity)
                                err(c,
                                    "Not searching for discontinuity but hit not-full "
                                    "branch + address edge case\n");
                            /* TODO: should I poison addr? */
                            pc = packet->address;
                        } else {
                            info(
                                c,
                                "We hit the full branch_map + address edge case\n");
                            pc = packet->address;
                        }
                        hit_discontinuity = true;
                    } else if (branch_map.cnt > 0 || dinfo.target != 0) {
                        /* we should not hit unpredictable
                         * discontinuities */
                        pc = dinfo.target;
                    } else {
                        /* we finally hit a jump with unknown  destination,
                         * thus the information in this packet  is used up
                         */
                        if (!search_discontinuity)
                            err(c,
                                "We were searching for an address and not a discontinuity\n");
                        pc = packet->address;
                        hit_discontinuity = true;
                        info(c, "Found the discontinuity\n");
                    }
                    break;

                case dis_condbranch:
                    /* this case allows us to exhaust the branch bits */
                    {
                        /* 32 would be undefined */
                        bool branch_taken = branch_map.bits & 1;
                        branch_map.bits >>= 1;
                        branch_map.cnt--;
                        if (dinfo.target == 0)
                            err(c,
                                "Can't predict the jump target, never happens\n");
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
                    err(c, "Invalid insn_type: %d\n", dinfo.insn_type);
                    goto fail;
                }
            }
        } else if (packet->format == F_BRANCH_DIFF) {
            err(c, "F_BRANCH_DIFF decoding: not implemented yet\n");

        } else if (packet->format == F_SYNC) {
            /* Sync pc. As in todo.org described this doesn't work for resync
             * (empty branch map) and context change. For now we dont allow both
             * situations
             */
            decomp_ctx.privilege = packet->privilege;
            pc = packet->address;

            /* since we are abruptly changing the pc we have to check if we
             * leave the section before we can disassemble. This is really ugly
             * TODO: fix
             */
            if (pc >= section->vma + stop_offset || pc < section->vma) {
                free_section_for_debugging(&dinfo);

                section = get_section_for_vma(abfd, pc);
                if (!section) {
                    err(c, "VMA (PC) not pointing to any section\n");
                    goto fail;
                }
                stop_offset =
                    bfd_get_section_size(section) / dinfo.octets_per_byte;

                if (alloc_section_for_debugging(c, abfd, section, &dinfo)) {
                    err(c, "Failed alloc_section_for_debugging\n");
                    goto fail;
                }
                info(c, "Switched to section:%s\n", section->name);
            }

            int status = 0;
            int size = disassemble_at_pc(c, pc, &dis_instr, &dunit, &status);
            if (status != 0)
                goto fail;

            dis_instr.priv = decomp_ctx.privilege;
            add_trace(c, instr_list, &dis_instr);

            pc += size;

            switch (dinfo.insn_type) {
            case dis_nonbranch:
                /* TODO: we need this hack since {m,s,u} ret are not
                 * classified
                 */
                if (!is_unpred_discontinuity(dis_instr.instr)) {
                    break;
                }
                info(c, "Detected mret, uret or sret\n");
            case dis_jsr:    /* There is not real difference ... */
            case dis_branch: /* ... between those two */
                if (dinfo.target == 0)
                    err(c, "Can't predict the jump target, never happens\n");
                pc = dinfo.target;
                break;

            case dis_condbranch:
                if (dinfo.target == 0)
                    err(c, "Can't predict the jump target, never happens\n");
                if (packet->branch == 0) {
                    err(c, "Doing a branch from a F_SYNC packet\n");
                    pc = dinfo.target;
                }
                break;
            case dis_dref: /* TODO: is this useful? */
                break;
            case dis_dref2:
            case dis_condjsr:
            case dis_noninsn:
                err(c, "Invalid insn_type: %d\n", dinfo.insn_type);
                goto fail;
            }
        } else if (packet->format == F_ADDR_ONLY) {
            /* We have to find the instruction where we can apply the address
             * information to. This might either be a discontinuity infromation
             * or a sync up address. So we stop either at the given address or
             * use it on a discontinuity, whichever comes first
             *
             * TODO: we don't allow resync packets for now, that means
             * we won't have issues distinguishing between address
             * sync and unpredictable discontinuities. We can
             * distinguish the case of advance to next address with
             * advance to unpredictable discontinuitiy by inspecting
             * the next packet. The events halt and unqualified happen
             * if we don't have any more packets (next_packet == NULL,
             * TODO: make this better). Exception and privilege change
             * events always happen in packet pairs, which, again, we
             * can figure out by looking at next_packet. If any test
             * so far failed, then we know, since we are disallowing
             * resync packets for now, that we are dealing with a
             * unpredictable discontinuity.
             */
            bool hit_address = false;
            bool hit_discontinuity = false;

            bool search_discontinuity = true;
            if (next_packet == NULL
                || (next_packet->format == F_SYNC
                    && (next_packet->subformat == SF_START
                        || next_packet->subformat == SF_EXCEPTION))) {
                /* advance until address */
                info(c, "Searching for address\n");
                search_discontinuity = false;
            } else {
                /* advance until unpredictable discontinuity */
                info(c, "Searching for discontinuity\n");
                search_discontinuity = true;
            }

            while (!(hit_address || hit_discontinuity)) {
                int status = 0;
                int size =
                    disassemble_at_pc(c, pc, &dis_instr, &dunit, &status);
                if (status != 0)
                    goto fail;

                /* TODO: fix */
                if (!c->config.full_address) {
                    err(c, "full_address false: not implemented yet\n");
                    goto fail;
                }

                if (!search_discontinuity && pc == packet->address)
                    hit_address = true;

                dis_instr.priv = decomp_ctx.privilege;
                add_trace(c, instr_list, &dis_instr);

                /* advance pc */
                pc += size;

                switch (dinfo.insn_type) {
                case dis_nonbranch:
                    /* TODO: we need this hack since {m,s,u} ret are not
                     * classified
                     */
                    if (!is_unpred_discontinuity(dis_instr.instr)) {
                        break;
                    }
                    info(c, "Detected mret, uret or sret\n");
                case dis_jsr:    /* There is not real difference ... */
                case dis_branch: /* ... between those two */
                    if (dinfo.target) {
                        pc = dinfo.target;
                    } else {
                        if (!search_discontinuity)
                            err(c,
                                "We were searching for an address and not a discontinuity\n");
                        info(c, "Found the discontinuity\n");
                        pc = packet->address;
                        hit_discontinuity = true;
                    }
                    break;

                case dis_condbranch:
                    err(c,
                        "We shouldn't hit conditional branches with F_ADDRESS_ONLY\n");
                    break;

                case dis_dref: /* TODO: is this useful? */
                    break;
                case dis_dref2:
                case dis_condjsr:
                case dis_noninsn:
                    err(c, "Invalid insn_type: %d\n", dinfo.insn_type);
                    goto fail;
                }
            }
        }
    }

fail:
    free_section_for_debugging(&dinfo);
    return NULL;
}
