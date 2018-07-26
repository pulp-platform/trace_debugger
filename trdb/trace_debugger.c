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

/*
 * Author: Robert Balas (balasr@student.ethz.ch)
 * Description: Software model for the hardware trace debugger
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <endian.h> /* Non-standard (posix), use <arpa/inet.h> instead? */
#include <stdbool.h>
#include <inttypes.h>
#include <string.h>
#include "trace_debugger.h"
#include "disasm.h"
#include "list.h"
#include "util.h"

struct trdb_config {
    /* TODO: inspect full-address, iaddress-lsb-p, implicit-except, set-trace */
    uint64_t resync_cnt;
    uint64_t resync_max;
    bool full_address;
    bool iaddress_lsb_p;
    bool implicit_except;
    bool set_trace;
};
static struct trdb_config conf = {0};

static bool is_branch(uint32_t instr)
{
    /* static bool is_##code##_instr(long insn) */
    return is_beq_instr(instr) || is_bne_instr(instr) || is_blt_instr(instr)
           || is_bge_instr(instr) || is_bltu_instr(instr)
           || is_bgeu_instr(instr);
    /* is_jalr_instr(instr) is_jal_instr(instr) */
    /* auipc */
}


static bool branch_taken(struct instr_sample before, struct instr_sample after)
{
    /* TODO: can this cause issues with RVC + degenerate jump (+2) ?*/
    return !(before.iaddr + 4 == after.iaddr
             || before.iaddr + 2 == after.iaddr);
}


static bool is_unpred_discontinuity(uint32_t instr)
{
    return is_jalr_instr(instr);
}


void trdb_init()
{
    conf = (struct trdb_config){.resync_max = UINT64_MAX, .full_address = true};
}


void trdb_close()
{
}


struct list_head *trdb_compress_trace(struct list_head *packet_list,
                                      struct instr_sample instrs[], size_t len)
{
    bool full_address = conf.full_address;

    /* TODO: look at those variables */
    bool resync_pend = false;
    /* uint32_t resync_nh = 0;  */
    bool unhalted = false;       /* TODO: handle halt? */
    bool context_change = false; /* TODO: ?? */

    bool branch_map_full = false;

    bool thisc_qualified = false;
    bool lastc_qualified = false;
    bool firstc_qualified = false;
    bool nextc_qualified = false;
    bool thisc_unqualified = false;
    bool nextc_unqualified = false;

    bool thisc_exception = false;
    bool lastc_exception = false;
    bool nextc_exception = false;

    bool thisc_unpred_disc = false;
    bool lastc_unpred_disc = false; /* last cycle unpredicted discontinuity*/

    uint32_t thisc_privilege = 0;
    uint32_t lastc_privilege = 0;
    uint32_t nextc_privilege = 0;

    bool thisc_privilege_change = false;
    bool lastc_privilege_change = false;
    bool nextc_privilege_change = false;

    bool nextc_halt = false; /* TODO: update this */

    uint32_t branch_map = 0;
    uint32_t branches = 0;

    /* for each cycle */
    for (size_t i = 0; i < len; i++) {
        /* test for qualification by filtering */
        thisc_qualified = true; /* TODO: implement filtering logic */
        nextc_qualified = true;

        /* Update state TODO: maybe just ignore last sample instead?*/
        thisc_exception = instrs[i].exception;
        nextc_exception = i < len ? instrs[i + 1].exception : thisc_exception;

        thisc_unpred_disc = is_unpred_discontinuity(instrs[i].instr);
        thisc_privilege = instrs[i].priv;
        nextc_privilege = i < len ? instrs[i + 1].priv : thisc_privilege;

        thisc_privilege_change = (thisc_privilege != lastc_privilege);
        nextc_privilege_change = (thisc_privilege != nextc_privilege);

        firstc_qualified = !lastc_qualified && thisc_qualified;
        thisc_unqualified = !thisc_qualified;
        nextc_unqualified = !nextc_qualified;


        /* Start of one cycle */
        if (!thisc_qualified) {
            /* check if we even need to record anything */
            lastc_qualified = thisc_qualified;
            lastc_exception = thisc_exception;
            lastc_unpred_disc = thisc_unpred_disc;
            lastc_privilege = thisc_privilege;
            lastc_privilege_change = thisc_privilege_change;
            continue; /* end of cycle */
        }

        if (conf.resync_cnt++ == conf.resync_max) {
            resync_pend = true;
            conf.resync_cnt = 0;
        }

        if (is_branch(instrs[i].instr)) {
            /* update branch map */
            /* in hardware maybe mask and compare is better ? */
            if ((i + 1 < len) && branch_taken(instrs[i], instrs[i + 1]))
                branch_map = branch_map | (1u << branches);
            branches++;
            if (branches == 31) {
                branch_map_full = true;
            }
        }

        /* We trace the packet before the trapped instruction and the
         * first one of the exception handler
         */
        if (lastc_exception) {
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
            tr->branch = 0; /* TODO: figure if taken or not */
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

            resync_pend = false; /* TODO: how to handle this */
            /* end of cycle */

        } else if (firstc_qualified || unhalted || thisc_privilege_change
                   || (resync_pend && branches == 0)) {

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
            tr->branch = 0; /* TODO: figure if taken or not */
            tr->address = instrs[i].iaddr;
            list_add(&tr->list, packet_list);

            resync_pend = false;
            /* end of cycle */

        } else if (lastc_unpred_disc) {
            /* Send te_inst:
             * format 0/1/2
             */
            ALLOC_INIT_PACKET(tr);
            /* TODO: for now only full address */
            if (!full_address) {
                LOG_ERR("full_address false: Not implemented yet\n");
                goto fail;
            }
            if (branches == 0) {
                tr->format = F_ADDR_ONLY;
                tr->address = full_address ? instrs[i].iaddr : 0;

                assert(branch_map == 0);
            } else {
                tr->format = full_address ? F_BRANCH_FULL : F_BRANCH_DIFF;
                tr->branches = branches;
                tr->branch_map = branch_map;
                tr->address = instrs[i].iaddr;

                branches = 0;
                branch_map = 0;
            }
            list_add(&tr->list, packet_list);

            /* end of cycle */

        } else if (resync_pend && branches > 0) {
            /* we treat resync_pend && branches == 0 before */
            /* Send te_inst:
             * format 0/1/2
             */
            ALLOC_INIT_PACKET(tr);
            /* TODO: for now only full address */
            if (!full_address) {
                LOG_ERR("full_address false: Not implemented yet\n");
                goto fail;
            }
            tr->format = full_address ? F_BRANCH_FULL : F_BRANCH_DIFF;
            tr->branches = branches;
            tr->branch_map = branch_map;
            tr->address = full_address ? instrs[i].iaddr : 0;
            list_add(&tr->list, packet_list);

            branches = 0;
            branch_map = 0;
            /* end of cycle */

        } else if (nextc_halt || nextc_exception || nextc_privilege_change
                   || nextc_unqualified) {
            /* Send te_inst:
             * format 0/1/2
             */
            ALLOC_INIT_PACKET(tr);
            /* TODO: for now only full address */
            if (!full_address) {
                LOG_ERR("full_address false: Not implemented yet\n");
                goto fail;
            }
            if (branches == 0) {
                tr->format = F_ADDR_ONLY;
                tr->address = full_address ? instrs[i].iaddr : 0;

                assert(branch_map == 0);
            } else {
                tr->format = full_address ? F_BRANCH_FULL : F_BRANCH_DIFF;
                tr->branches = branches;
                tr->branch_map = branch_map;
                tr->address = full_address ? instrs[i].iaddr : 0;

                branches = 0;
                branch_map = 0;
            }
            list_add(&tr->list, packet_list);

            /* end of cycle */

        } else if (branch_map_full) {
            /* Send te_inst:
             * format 0
             * no address
             */
            assert(branches == 31);
            ALLOC_INIT_PACKET(tr);
            tr->format = F_BRANCH_FULL;
            tr->branches = branches;
            tr->branch_map = branch_map;
            /* tr->address  TODO: no address, study explanation */
            list_add(&tr->list, packet_list);

            branches = 0;
            branch_map = 0;
            /* end of cycle */

        } else if (context_change) {
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
        lastc_qualified = thisc_qualified;
        lastc_exception = thisc_exception;
        lastc_unpred_disc = thisc_unpred_disc;
        lastc_privilege = thisc_privilege;
    }
    return packet_list;
fail:
fail_malloc:
    trdb_free_packet_list(packet_list);
    return NULL;
}


char *trdb_decompress_trace(bfd *abfd, struct list_head *packet_list)
{
    /* We assume our hw block in the pulp generated little endian
     * addresses, thus we should make sure that before we interact
     * with bfd addresses to convert this foreign format to the local
     * host format
     */
    return (char *)0;
}


static uint32_t branch_map_len(uint32_t branches)
{
    if (branches <= 1) {
        return 1;
    } else if (branches <= 9) {
        return 9;
    } else if (branches <= 25) {
        return 25;
    } else if (branches <= 31) {
        return 31;
    }
    return -1;
}


union pack {
    __uint128_t bits; /* non-portable gcc stuff. TODO: fix */
    uint8_t bin[16];  /* since uint8_t =/= char strict aliasing might
                       * mess your shit up. Careful this is also
                       * endian dependent
                       */
} data = {0};


/* bin must be an all zeros array */
static int packet_to_char(struct tr_packet *packet, size_t *bitcnt,
                          uint8_t align, uint8_t bin[])
{
    if (packet->msg_type != 0x2) {
        LOG_ERR("trace packet message type not supported: %d\n",
                packet->msg_type);
        return -1;
    }
    if (!conf.full_address) {
        LOG_ERR("full_address false: not implemented yet\n");
        return -1;
    }
    if (align >= 8) {
        LOG_ERR("bad alignment value: %" PRId8 "\n", align);
        return -1;
    }
    switch (packet->format) {
    case F_BRANCH_FULL: {
        uint32_t len = branch_map_len(packet->branches);

        /* different non-portable way to calculate same packed bits.
         * For some additional assurance. Machine must be little
         * endian (or add the htole128 function)
         */
        assert(__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__);
        /* we need enough space to do the packing it in uint128 */
        assert(128 > XLEN + 9 + 31 + 8);
        /* TODO: assert branch map to overfull */
        /* TODO: branch map full logic handling */
        /* TODO: for now we put the address always */
        data.bits = 0;
        data.bits = (packet->msg_type) | (packet->format << 2)
                    | (packet->branches << 4);
        data.bits |= ((__uint128_t)packet->branch_map & MASK_FROM(len)) << 9;
        data.bits |= ((__uint128_t)packet->address << (9 + len));
        data.bits <<= align;

        *bitcnt = 9 + len + XLEN;
        memcpy(bin, data.bin, (*bitcnt + align) / 8 + 1);

        for (size_t j = 0; j < ((*bitcnt + align) / 8 + 1); j++) {
            /* printf("0x%" PRIx8 "\n", data.bin[j]); */
        }
        return 0;
    }

    case F_BRANCH_DIFF:
        LOG_ERR("F_BRANCH_DIFF packet_to_char not implemented yet\n");
        *bitcnt = 0;
        return -1;

    case F_ADDR_ONLY:
        assert(128 > XLEN + 4 + 8);
        data.bits = packet->msg_type | (packet->format << 2)
                    | ((__uint128_t)packet->address << 4);
        data.bits <<= align;

        *bitcnt = XLEN + 4;
        memcpy(bin, data.bin, (*bitcnt + align) / 8 + 1);
        return 0;

    case F_SYNC:
        assert(PRIVLEN == 5);
        /* check for enough space to the packing */
        assert(128 > 2 + 2 + 2 + PRIVLEN + 1 + XLEN + CAUSELEN + 1 + XLEN);
        /* TODO: for now we ignore the context field since we have
         *  only one hart
         */

        data.bits = 0;
        /* common part to all sub formats */
        data.bits = packet->msg_type | (packet->format << 2)
                    | (packet->subformat << 4) | (packet->privilege << 6);
        *bitcnt = 6 + PRIVLEN;

        switch (packet->subformat) {
        case SF_START:
            data.bits |= (packet->branch << (6 + PRIVLEN))
                         | ((__uint128_t)packet->address << (7 + PRIVLEN));
            *bitcnt += 1 + XLEN;
            break;

        case SF_EXCEPTION:
            data.bits |= (packet->branch << (6 + PRIVLEN))
                         | ((__uint128_t)packet->address << (7 + PRIVLEN))
                         | ((__uint128_t)packet->ecause << (7 + PRIVLEN + XLEN))
                         | ((__uint128_t)packet->interrupt
                            << (7 + PRIVLEN + XLEN + CAUSELEN))
                         | ((__uint128_t)packet->tval
                            << (8 + PRIVLEN + XLEN + CAUSELEN));
            *bitcnt += 1 + XLEN + CAUSELEN + 1 + XLEN;
            break;

        case SF_CONTEXT:
            /* TODO: we still ignore the context field */
            break;
        }

        data.bits <<= align;
        memcpy(bin, data.bin, (*bitcnt + align) / 8 + 1);
        return 0;
    }
    return -1;
}


int trdb_write_trace(char *path, struct list_head *packet_list)
{
    int status = 0;
    FILE *fp = fopen(path, "wb");
    if (!fp) {
        perror("trdb_write_trace");
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
    list_for_each_entry(packet, packet_list, list)
    {
        if (packet_to_char(packet, &bitcnt, alignment, bin)) {
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
            perror("fwrite");
            status = -1;
            goto fail;
        }
        /* we keep that for the next packet */
        carry = bin[good] & MASK_FROM(rest);
        alignment = rest;
    }
    /* done, write remaining carry */
    if (!fwrite(&bin[good], 1, 1, fp)) {
        perror("fwrite");
        status = -1;
    }
fail:
    if (fp)
        fclose(fp);
    return status;
}

void trdb_dump_packet_list(struct list_head *packet_list)
{
    struct tr_packet *packet;
    list_for_each_entry(packet, packet_list, list)
    {
        if (packet->msg_type != 0x2) {
            LOG_ERR("Unsupported message type %" PRIu32 "\n", packet->msg_type);
            return;
        }
        switch (packet->format) {
        case F_BRANCH_FULL:
        case F_BRANCH_DIFF:
            printf("PACKET ");
            packet->format == F_BRANCH_FULL ? printf("0: F_BRANCH_FULL\n")
                                            : printf("1: F_BRANCH_DIFF\n");
            printf("    branches  : %" PRIu32 "\n", packet->branches);
            printf("    branch_map: 0x%" PRIx32 "\n", packet->branch_map);
            printf("    address   : 0x%" PRIx32 "\n", packet->address);
            /* TODO: that special full branch map behaviour */
            break;

        case F_ADDR_ONLY:
            printf("PACKET 2: F_ADDR_ONLY\n");
            printf("    address   : 0x%" PRIx32 "\n", packet->address);
            break;
        case F_SYNC:
            printf("PACKET 3: F_SYNC\n");
            const char *subf[4];
            subf[0] = "SF_START";
            subf[1] = "SF_EXCEPTION";
            subf[2] = "SF_CONTEXT";
            subf[3] = "RESERVED";
            printf("    subformat : %s\n", subf[packet->subformat]);

            switch (packet->subformat) {
            case SF_CONTEXT:
                /* TODO fix this */
                printf("    context   :\n");
                printf("    privilege : 0x%" PRIx32 "\n", packet->privilege);
            case SF_START:
                printf("    branch    : %s\n",
                       packet->branch ? "true" : "false");
                printf("    address   : 0x%" PRIx32 "\n", packet->address);
            case SF_EXCEPTION:
                printf("    ecause    : 0x%" PRIx32 "\n", packet->ecause);
                printf("    interrupt : %s\n",
                       packet->interrupt ? "true" : "false");
                printf("    tval      : 0x%" PRIx32 "\n", packet->tval);
            }
            break;
        }
    }
}


void trdb_free_packet_list(struct list_head *packet_list)
{
    struct tr_packet *packet;
    list_for_each_entry(packet, packet_list, list)
    {
        free(packet);
    }
}
