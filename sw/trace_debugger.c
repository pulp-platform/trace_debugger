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
#include "trace_debugger.h"
#include "disasm.h"
#include "list.h"

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
                fprintf(stderr, "full_address false: Not implemented yet\n");
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
                fprintf(stderr, "full_address false: Not implemented yet\n");
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
                fprintf(stderr, "full_address false: Not implemented yet\n");
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


void dump_packet_list(struct list_head *packet_list)
{
    struct tr_packet *packet;
    list_for_each_entry(packet, packet_list, list)
    {
        printf("not implemented yet.\n");
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
