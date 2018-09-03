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

/**
 * @file trdb_sv.c
 * @author Robert Balas (balasr@student.ethz.ch)
 * @date 24 Aug 2018
 * @brief Interface for the system verilog testbench
 */

#include <stdlib.h>
#include <stdio.h>
#include <svdpi.h>
#include "trace_debugger.h"

struct trdb_ctx *ctx;

void trdb_sv_alloc()
{
    ctx = trdb_new();
    if (!ctx) {
        fprintf(stderr, "[TRDB-C-SV]: couldn't allocate trdb_ctx\n");
        return;
    }
}


void trdb_sv_free()
{
    free(ctx);
}


void trdb_sv_feed_trace(svLogic ivalid, svLogic iexception, svLogic interrupt,
                        const svLogicVecVal *cause, const svLogicVecVal *tval,
                        const svLogicVecVal *priv, const svLogicVecVal *iaddr,
                        const svLogicVecVal *instr, svLogic compressed)
{
    // Test if we got x or z
    if (ivalid > 1 || iexception > 1 || interrupt > 1 || cause->bval != 0
        || tval->bval != 0 || priv->bval != 0 || iaddr->bval != 0
        || instr->bval != 0 || compressed > 1) {
        fprintf(stderr, "[TRDB-C-SV]: some values or 'x or 'z\n");
    }
    fprintf(stdout, "[TRDB-C-SV]: Received addr %d\n", iaddr->aval);
}
