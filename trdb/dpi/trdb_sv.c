/*
 * trdb - Trace Debugger Software for the PULP platform
 *
 * Copyright (C) 2018 Robert Balas
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
 * @date 4 Sep 2018
 * @brief Interface for the system verilog testbench
 */

#include <stdlib.h>
#include <stdio.h>
#include <svdpi.h>
#include <stdint.h>
/* #include "autogen_trdb_sv.h" */
#include "trdb_sv.h"
#include "utils.h"
#include "trace_debugger.h"
#include "serialize.h"

struct trdb_packet_head packets = TAILQ_HEAD_INITIALIZER(packets);

/* TODO: send context to simulator per userdata methods */

struct trdb_ctx *ctx;
int packetcnt = 0;

/* We print even errors to stdout since the simulator doesn't interleave stderr
 * and stdout
 */
static void log_stdout_dpi(struct trdb_ctx *ctx, int priority, const char *file,
                           int line, const char *fn, const char *format,
                           va_list args)
{
    (void)ctx;
    (void)priority;
    fprintf(stdout, "trdb-dpi: %s:%d:0: %s(): ", file, line, fn);
    vfprintf(stdout, format, args);
}

void trdb_sv_alloc()
{
    ctx = trdb_new();
    if (!ctx) {
        fprintf(stdout, "trdb-dpi: couldn't allocate trdb_ctx\n");
        return;
    }
    trdb_set_log_fn(ctx, log_stdout_dpi);
}

void trdb_sv_free()
{
    trdb_free_packet_list(&packets);
    free(ctx);
}

void trdb_sv_set_full_address(int full_address)
{
    trdb_set_full_address(ctx, full_address);
}

void trdb_sv_set_implicit_ret(int implicit_ret)
{
    trdb_set_implicit_ret(ctx, implicit_ret);
}

void trdb_sv_feed_trace(svLogic ivalid, svLogic iexception, svLogic interrupt,
                        const svLogicVecVal *cause, const svLogicVecVal *tval,
                        const svLogicVecVal *priv, const svLogicVecVal *iaddr,
                        const svLogicVecVal *instr, svLogic compressed,
                        int packet_max_len, svLogicVecVal *packet_bits,
                        svLogic *packet_valid)
{
    // Test if we got x or z
    if (ivalid > 1 || iexception > 1 || interrupt > 1 || cause->bval != 0 ||
        tval->bval != 0 || priv->bval != 0 || iaddr->bval != 0 ||
        instr->bval != 0 || compressed > 1) {
        err(ctx,
            "some values are 'x or 'z, implicitly converting to binary and continuing... \n");
    }

    *packet_valid = 0;

    /* Note: aval and bval are uint32_t per standard */
    struct tr_instr tr_instr = {.valid      = ivalid,
                                .exception  = iexception,
                                .interrupt  = interrupt,
                                .cause      = cause->aval,
                                .tval       = tval->aval,
                                .priv       = priv->aval,
                                .iaddr      = iaddr->aval,
                                .instr      = instr->aval,
                                .compressed = compressed};

    /* upper bounded local buffer for serialization */
    size_t bitcnt                          = 0;
    uint8_t buff[sizeof(struct tr_packet)] = {0};
    svLogicVecVal tmp                      = {0};
    struct tr_packet *latest_packet        = NULL;

    int p = trdb_compress_trace_step_add(ctx, &packets, &tr_instr);

    if (!TAILQ_EMPTY(&packets)) {
        latest_packet = TAILQ_LAST(&packets, trdb_packet_head);
    }

    if (p == -1)
        err(ctx, "compression step failed\n");
    if (p == 1) {
        if (trdb_pulp_serialize_packet(ctx, latest_packet, &bitcnt, 0, buff)) {
            err(ctx, "failed to serialize packet, continuing...\n");
        }
        packetcnt++;
        // TODO: change that to debug
        info(ctx, "ID: %d\n", packetcnt);
        trdb_print_packet(stdout, latest_packet);

        /* SV_PACKED_DATA_NELEMS returns the number ov svLogicVecVal's, which
         * each have 4 bytes
         */
        size_t total_bytes = SV_PACKED_DATA_NELEMS(packet_max_len) * 4;

        /* this is just a integer divions (ceiling) of bitcount/8 */
        size_t packet_bytes = (bitcnt / 8 + (bitcnt % 8 != 0));

        if ((size_t)packet_max_len / 8 < packet_bytes) {
            err(ctx, "packet size on the sv side is too small\n");
            return;
        }

        /* fill in vector with packet and zero pad */
        for (size_t i = 0; i < total_bytes; i++) {
            if (i < packet_bytes)
                tmp.aval = buff[i];
            else
                tmp.aval = 0;
            tmp.bval = 0;

            svPutPartselLogic(packet_bits, tmp, i * 8, 8);
        }
        *packet_valid = 1;
    }
}
