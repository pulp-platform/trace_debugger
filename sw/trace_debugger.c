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
#include "trace_debugger.h"
#include "disasm.h"
#include "list.h"

bool is_branch(uint32_t instr)
{
    /* static bool is_##code##_instr(long insn) */
    return is_beq_instr(instr) || is_bne_instr(instr) || is_blt_instr(instr)
	   || is_bge_instr(instr) || is_bltu_instr(instr)
	   || is_bgeu_instr(instr);
    /* is_jalr_instr(instr) is_jal_instr(instr) */
    /* auipc */
}

struct list_head *trdb_compress_trace(struct list_head *packet_list,
				      struct instr_sample instrs[], size_t len)
{
    /* TODO: inspect full-address, iaddress-lsb-p, implicit-except, set-trace */
    bool full_address = false;
    /* bool iaddress_lsb_p = false; */
    /* bool implicit_except = false; */
    /* bool set_trace = false; */

    /* TODO: look at those variables */
    uint32_t resync_pend = 0;
    uint32_t resync_nh = 0; /* resync pending and branch map holds no history
			     * from previous instr
			     */
    bool unhalted = false; /* TODO: handle halt? */
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
    bool lastc_unpred_disc = false; /* last cylce unpredicted discontinuity*/

    uint32_t thisc_privilege = 0;
    uint32_t lastc_privilege = 0;
    uint32_t nextc_privilege = 0;
    bool thisc_privilege_change = false;
    bool nextc_privilege_change = false;

    bool nextc_halt = false; /* TODO: update this */

    uint32_t branch_map = 0;
    uint32_t branches = 0;

    /* for each cycle */
    for (size_t i = 0; i < len; i++) {
	/* test for qualification by filtering */
	thisc_qualified = true; /* TODO: implement filtering logic */
	nextc_qualified = true;

	/* TODO: resync counter */

	/* Update state TODO: maybe just ignore last sample instead?*/
	thisc_exception = instrs[i].exception;
	nextc_exception = i < len ? instrs[i + 1].exception : thisc_exception;
	thisc_unpred_disc = false; /* TODO: implement this logic */
	thisc_privilege = instrs[i].priv;
	nextc_privilege = i < len ? instrs[i + 1].priv : thisc_privilege;

	firstc_qualified = !lastc_qualified && thisc_qualified;
	thisc_unqualified = !thisc_qualified;
	nextc_unqualified = !nextc_qualified;
	thisc_privilege_change = (thisc_privilege != lastc_privilege);
	nextc_privilege_change = (thisc_privilege != nextc_privilege);

	/* Start of one cycle */
	if (!thisc_qualified) {
	    /* update last cycle state */
	    lastc_qualified = thisc_qualified;
	    lastc_exception = thisc_exception;
	    lastc_unpred_disc = thisc_unpred_disc;
	    lastc_privilege = thisc_privilege;
	    continue; /* end of cycle */
	}

	if (is_branch(instrs[i].instr)) {
	    /* update branch map */
	    branch_map = branch_map | (1u << branches);
	    branches++;
	    if (branches == 31) {
		branch_map_full = true;
	    }
	}

	if (thisc_exception) {
	    /*
	     * Send te_inst:
	     * format 3
	     * subformat 1 (exception -> all fields present)
	     * resync_pend = 0
	     */
	    struct tr_packet *tr = malloc(sizeof(*tr));
	    INIT_PACKET(tr);
	    tr->format = 3;    /* sync */
	    tr->subformat = 1; /* exception */
	    tr->context = 0;   /* TODO: what comes here? */
	    tr->privilege = instrs[i].priv;
	    tr->branch = 0; /* TODO: figure if taken or not */
	    tr->address = instrs[i].iaddr;
	    tr->ecause = instrs[i].cause;
	    tr->interrupt = instrs[i].interrupt;
	    tr->tval = instrs[i].tval;
	    resync_pend = 0; /* TODO: how to handle this */
	    list_add(&tr->list, packet_list);
	    /* end of cycle */

	} else if (firstc_qualified || unhalted || thisc_privilege_change
		   || resync_nh) {

	    /* General synchronization packet */
	    /*
	     * Send te_inst:
	     * format 3
	     * subformat 0 (start, no ecause, interrupt and tval)
	     * resync_pend = 0
	     */
	    struct tr_packet *tr = malloc(sizeof(*tr));
	    INIT_PACKET(tr);
	    tr->format = 3;    /* sync */
	    tr->subformat = 0; /* start */
	    tr->context = 0;   /* TODO: what comes here? */
	    tr->privilege = instrs[i].priv;
	    tr->branch = 0; /* TODO: figure if taken or not */
	    tr->address = instrs[i].iaddr;
	    resync_pend = 0;
	    list_add(&tr->list, packet_list);
	    /* end of cycle */

	} else if (lastc_unpred_disc) {
	    /*
	     * Send te_inst:
	     * format 0/1/2
	     */
	    struct tr_packet *tr = malloc(sizeof(*tr));
	    INIT_PACKET(tr);
	    tr->format = 0;
	    tr->branches = branches;
	    tr->branch_map = branch_map;
	    tr->address = instrs[i].iaddr;
	    list_add(&tr->list, packet_list);
	    branches = 0;
	    branch_map = 0;
	    /* end of cycle */

	} else if (resync_pend && branches > 0) {
	    /*
	     * Send te_inst:
	     * format 0/1/2
	     */
	    struct tr_packet *tr = malloc(sizeof(*tr));
	    INIT_PACKET(tr);
	    tr->format = 1;
	    tr->branches = branches;
	    tr->branch_map = branch_map;
	    tr->address =
		instrs[i].iaddr; /* TODO: this should be differential address */
	    list_add(&tr->list, packet_list);
	    branches = 0;
	    branch_map = 0;
	    /* end of cycle */

	} else if (nextc_halt || nextc_exception || nextc_privilege_change
		   || nextc_unqualified) {
	    /*
	     * Send te_inst:
	     * format 0/1/2
	     */
	    struct tr_packet *tr = malloc(sizeof(*tr));
	    INIT_PACKET(tr);
	    tr->format = 2;
	    if (!full_address) {
		tr->address = 0; /* TODO: set address */
	    } else {
		tr->address = instrs[i].iaddr;
	    }
	    list_add(&tr->list, packet_list);
	    /* end of cycle */

	} else if (branch_map_full) {
	    /*
	     * Send te_inst:
	     * format 0
	     * no address
	     */
	    assert(branches == 31);
	    struct tr_packet *tr = malloc(sizeof(*tr));
	    INIT_PACKET(tr);
	    tr->format = 0;
	    tr->branches =
		branches; /*branch_map_full indiciates full addr map*/
	    tr->branch_map = branch_map;
	    /* tr->address  TODO: no address, study explanation */
	    list_add(&tr->list, packet_list);
	    branches = 0;
	    branch_map = 0;
	    /* end of cycle */

	} else if (context_change) {
	    /*
	     * Send te_inst:
	     * format 3
	     * subformat 2
	     */
	    struct tr_packet *tr = malloc(sizeof(*tr));
	    INIT_PACKET(tr);

	    tr->format = 3;
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
}


char *trdb_decompress_trace(struct list_head *packet_list)
{
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


void free_packet_list(struct list_head *packet_list)
{
    struct tr_packet *packet;
    list_for_each_entry(packet, packet_list, list)
    {
	free(packet);
    }
}
