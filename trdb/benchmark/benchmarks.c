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

/*
 * Author: Robert Balas (balasr@student.ethz.ch)
 * Description: Run benchmarks for trdb
 */
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/queue.h>

#include "utils.h"
#include "trace_debugger.h"
#include "disassembly.h"
#include "serialize.h"

struct result {
    char *name;
    size_t instrcnt;
    size_t packetcnt;
    size_t payload;
    double bpi;
};

static char *conv_tf(bool b)
{
    return b ? "true" : "false";
}

static double percent(size_t a, size_t total)
{
    return (double)a / total * 100;
}

int compress_cvs_trace(const char *trace_path, struct result *comparison)
{
    int status                     = 0;
    struct trdb_ctx *ctx           = trdb_new();
    struct disassembler_unit dunit = {0};
    struct disassemble_info dinfo  = {0};
    dunit.dinfo                    = &dinfo;

    trdb_init_disassembler_unit_for_pulp(&dunit, NULL);

    struct trdb_packet_head packet_list;
    TAILQ_INIT(&packet_list);
    struct trdb_instr_head instr_list;
    TAILQ_INIT(&instr_list);

    if (!ctx) {
        fprintf(stderr, "Library context allocation failed.\n");
        status = -1;
        goto fail;
    }
    bool full_address  = false;
    bool implicit_ret  = true;
    bool compress_bmap = true;

    trdb_set_dunit(ctx, &dunit);
    trdb_set_full_address(ctx, full_address);
    trdb_set_pulp_extra_packet(ctx, false);
    trdb_set_implicit_ret(ctx, implicit_ret);
    trdb_set_compress_branch_map(ctx, compress_bmap);

    size_t instrcnt = 0;
    status = trdb_cvs_to_trace_list(ctx, trace_path, &instr_list, &instrcnt);
    if (status < 0) {
        fprintf(stderr, "CVS to tr_instr failed\n");
        status = -1;
        goto fail;
    }

    struct tr_instr *instr;
    TAILQ_FOREACH (instr, &instr_list, list) {
        int step = trdb_compress_trace_step_add(ctx, &packet_list, instr);
        if (step == -1) {
            fprintf(stderr, "Compress trace failed\n");
            status = -1;
            goto fail;
        }
    }
    struct trdb_packet_stats stats = {0};
    trdb_get_packet_stats(ctx, &stats);

    size_t packets                = trdb_get_packetcnt(ctx);
    size_t payloadbits            = trdb_get_payloadbits(ctx);
    size_t instrs                 = trdb_get_instrcnt(ctx);
    size_t pulpbits               = trdb_get_pulpbits(ctx);
    size_t exception_packets      = stats.exception_packets;
    size_t addr_only_packets      = stats.addr_only_packets;
    size_t start_packets          = stats.start_packets;
    size_t diff_packets           = stats.diff_packets;
    size_t abs_packets            = stats.abs_packets;
    size_t bmap_full_packets      = stats.bmap_full_packets;
    size_t bmap_full_addr_packets = stats.bmap_full_addr_packets;

    /* compare also against the version which doesn't use address and branchmap
     * compression
     */
    trdb_reset_compression(ctx);

    trdb_set_full_address(ctx, true);
    trdb_set_pulp_extra_packet(ctx, false);
    trdb_set_implicit_ret(ctx, false);
    trdb_set_compress_branch_map(ctx, false);

    TAILQ_FOREACH (instr, &instr_list, list) {
        int step = trdb_compress_trace_step_add(ctx, &packet_list, instr);
        if (step == -1) {
            fprintf(stderr, "Compress trace failed\n");
            status = -1;
            goto fail;
        }
    }

    size_t noc_instrs      = trdb_get_instrcnt(ctx);
    size_t noc_payloadbits = trdb_get_payloadbits(ctx);

    printf("Name of test: %s\n", trace_path);
    if (comparison->instrcnt != instrcnt)
        printf("Number of instruction mismatch! %zu, %zu\n",
               comparison->instrcnt, instrcnt);
    printf("no ret packet count:%zu\n", trdb_get_packetcnt(ctx));
    printf(
        "pulp settings: full address: %s / implicit return: %s / compress full branch map: %s\n",
        conv_tf(full_address), conv_tf(implicit_ret), conv_tf(compress_bmap));
    printf("pulp  instructions: %zu, packets: %zu, payload bytes: %zu\n",
           instrcnt, packets, payloadbits / 8);
    printf("ultra instructions: %zu, packets: %zu, payload bytes: %zu\n",
           comparison->instrcnt, comparison->packetcnt, comparison->payload);
    printf("pulp packet stats: diff: %zu (%2.2lf%%) / abs: %zu (%2.2lf%%) / "
           "addr_only: %zu (%2.2lf%%) \n    bmap_full: %zu (%2.2lf%%) / "
           "bmap_full_with_addr: %zu (%2.2lf%%)\n    start: %zu (%2.2lf%%) / "
           "exception: %zu (%2.2lf%%)\n",
           diff_packets, percent(diff_packets, packets), abs_packets,
           percent(abs_packets, packets), addr_only_packets,
           percent(addr_only_packets, packets), bmap_full_packets,
           percent(bmap_full_packets, packets), bmap_full_addr_packets,
           percent(bmap_full_addr_packets, packets), start_packets,
           percent(start_packets, packets), exception_packets,
           percent(exception_packets, packets));
    double bpi_noc_payload = noc_payloadbits / (double)noc_instrs;
    double bpi_payload     = payloadbits / (double)instrs;
    double bpi_full        = (payloadbits + packets * 6) / (double)instrs;
    double bpi_pulp        = (pulpbits / (double)instrs);
    printf("Bits per instruction (ultra           ): %lf\n", comparison->bpi);
    printf("Bits per instruction (payload no compr): %lf "
           "(%+2.lf%% vs ultra)\n",
           bpi_noc_payload, (bpi_noc_payload / comparison->bpi * 100 - 100));
    printf("Bits per instruction (payload         ): %lf "
           "(%+2.lf%% vs ultra)\n",
           bpi_payload, (bpi_payload / comparison->bpi) * 100 - 100);
    printf("Bits per instruction (payload + header): %lf "
           "(%+2.lf%% vs payload)\n",
           bpi_full, bpi_full / bpi_payload * 100 - 100);
    printf("Bits per instruction (pulp            ): %lf "
           "(%+2.lf%% vs payload + header)\n",
           bpi_pulp, bpi_pulp / bpi_full * 100 - 100);
    printf("\n");

fail:
    trdb_free_packet_list(&packet_list);
    trdb_free_instr_list(&instr_list);
    trdb_free(ctx);
    return status;
}

int main()
{
    int status = EXIT_SUCCESS;

    struct result cmp_results[] = {{"dhyrstone", 215015, 1308, 5628, 0.209},
                                   {"median", 15015, 207, 810, 0.432},
                                   {"mm", 297038, 644, 2011, 0.054},
                                   {"mt-matmul", 41454, 344, 953, 0.184},
                                   {"mt-vvadd", 61072, 759, 2049, 0.268},
                                   {"multiply", 55016, 546, 1837, 0.267},
                                   {"pmp", 425, 7, 39, 0.734},
                                   {"qsort", 235015, 2052, 8951, 0.305},
                                   {"rsort", 375016, 683, 2077, 0.044},
                                   {"spmv", 70015, 254, 1154, 0.132},
                                   {"towers", 15016, 72, 237, 0.126},
                                   {"vvadd", 10016, 111, 316, 0.252}};

    const char *tv_cvs[] = {
        "data/cvs/dhrystone.spike_trace", "data/cvs/median.spike_trace",
        "data/cvs/mm.spike_trace",        "data/cvs/mt-matmul.spike_trace",
        "data/cvs/mt-vvadd.spike_trace",  "data/cvs/multiply.spike_trace",
        "data/cvs/pmp.spike_trace",       "data/cvs/qsort.spike_trace",
        "data/cvs/rsort.spike_trace",     "data/cvs/spmv.spike_trace",
        "data/cvs/towers.spike_trace",    "data/cvs/vvadd.spike_trace"};

    for (unsigned j = 0; j < TRDB_ARRAY_SIZE(tv_cvs); j++) {
        const char *stim = tv_cvs[j];
        if (access(stim, R_OK)) {
            fprintf(stderr, "File not found, skipping test at %s\n", stim);
            continue;
        }
        if (compress_cvs_trace(stim, cmp_results + j))
            status = EXIT_FAILURE;
    }

    const char *mibench_cvs[] = {
        "data/mibench_cvs/cjpeg.trace", "data/mibench_cvs/djpeg.trace",
        "data/fft.trace", "data/mibench_cvs/rijndael.trace",
        "data/mibench_cvs/sha.trace"};

    struct result dummy = {"dhyrstone", 1, 1, 1, 1};
    for (unsigned j = 0; j < TRDB_ARRAY_SIZE(mibench_cvs); j++) {
        const char *stim = mibench_cvs[j];
        if (access(stim, R_OK)) {
            fprintf(stderr, "File not found, skipping test at %s\n", stim);
            continue;
        }
        if (compress_cvs_trace(stim, &dummy))
            status = EXIT_FAILURE;
    }

    return status;
}
