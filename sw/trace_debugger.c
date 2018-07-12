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
#include "trace_debugger.h"
#include "disasm.h"

bool is_branch(uint32_t instr)
{
    /* static bool is_##code##_instr(long insn) */
    return is_beq_instr(instr) || is_bne_instr(instr) || is_blt_instr(instr)
           || is_bge_instr(instr) || is_bltu_instr(instr)
           || is_bgeu_instr(instr);
    /* is_jalr_instr(instr) is_jal_instr(instr) */
}

char *trdb_compress_trace(struct instr_sample instrs[], size_t len)
{
    struct instr_sample cinstr = instrs[0];
    //    /* Start of one cycle */
    //    if (!qualified) {
    //  return; /* end of cycle */
    //    }
    //
    if (is_branch(cinstr.instr)) {
        /* update branch map */
        return 0;
    }
    //
    //    if (last_cycle_was_exception) {
    //  /*
    //   * Send te_inst:
    //   * format 3
    //   * subformat 1
    //   * resync_pend = 0
    //   */
    //  return; /* end of cycle */
    //    }
    //
    //    if (1st_cycle_qualified || unhalted || privilege_change || resync_nh)
    //    {
    //  /*
    //   * Send te_inst:
    //   * format 3
    //   * subformat 0
    //   * resync_pend = 0
    //   */
    //  return; /* end of cycle */
    //    }
    //
    //    if (last_cycle_unpred_discon) {
    //  /*
    //   * Send te_inst:
    //   * format 0/1/2
    //   */
    //    }
    //
    //    if (resync_pend && branches > 0) {
    //  /*
    //   * Send te_inst:
    //   * format 0/1/2
    //   */
    //    }
    //
    //    if (next_cycle_is_halt || exception || privilege_change ||
    //    unqualified) {
    //  /*
    //   * Send te_inst:
    //   * format 0/1/2
    //   */
    //    }
    //
    //    if (branch_map_full) {
    //  /*
    //   * Send te_inst:
    //   * no address
    //   */
    //    }
    //
    //    if (context_change) {
    //  /*
    //   * Send te_inst:
    //   * format 3
    //   * subformat 2
    //   */
    //    }
    //
    return (char *)1;
}


char *trdb_decompress_trace(char *compressed_trace)
{
    return (char *)1;
}
