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
 * Description: trdb -- trace debugger tools for the  PULP platform, compress
 * and decompress instruction traces
 */

#define PACKAGE "foo" /* quick hack for bfd if not using autotools */
#include <stdio.h>
#include <stdlib.h>
#include <argp.h>
#include <stdbool.h>
#include <string.h>
#include "bfd.h"
#include "../utils.h"
#include "../list.h"
#include "../trace_debugger.h"
#include "../disassembly.h"
#include "../parse.h"

#define TRDB_NUM_ARGS 1

const char *argp_program_version = "trdb 0.1";
const char *argp_program_bug_address = "<balasr@student.ethz.ch>";

static char doc[] = "trdb -- trace debugger tools for the PULP platform";
static char args_doc[] = "TRACE-OR-PACKETS";

static struct argp_option options[] = {
    {"verbose", 'v', 0, 0, "Produce verbose output"},
    {"quiet", 'q', 0, 0, "Don't produce any output"},
    {"silent", 's', 0, OPTION_ALIAS},
    {"bfd", 'b', "ELF", 0,
     "ELF binary with which the traces/packets were produced"},
    {"compress", 'c', 0, 0, "Take a stimuli file and compress to packets"},
    {"extract", 'x', 0, 0, "Take a packet file and produce a stimuli file"},
    {"disassemble", 'd', 0, 0, "disassemble the output"},
    {"output", 'o', "FILE", 0, "Output to FILE instead of stdout"},
    {0}};

struct arguments {
    char *args[TRDB_NUM_ARGS];
    bool silent, verbose, compress, has_elf, disassemble, decompress;
    char *output_file;
    char *elf_file;
};

static error_t parse_opt(int key, char *arg, struct argp_state *state)
{
    struct arguments *arguments = state->input;
    switch (key) {
    case 'q':
    case 's':
        arguments->silent = true;
        break;
    case 'o':
        /* TODO: can I hold pointer to arg? */
        arguments->output_file = arg;
        break;
    case 'b':
        arguments->has_elf = true;
        arguments->elf_file = arg;
        break;
    case 'c':
        arguments->compress = true;
        break;
    case 'd':
        arguments->disassemble = true;
        break;
    case 'x':
        arguments->decompress = true;
        break;
    case ARGP_KEY_ARG:
        if (state->arg_num >= TRDB_NUM_ARGS)
            argp_usage(state);
        arguments->args[state->arg_num] = arg;
        break;
    case ARGP_KEY_END:
        if (state->arg_num < TRDB_NUM_ARGS)
            argp_usage(state);
        break;
    default:
        return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

static struct argp argp = {options, parse_opt, args_doc, doc};

static int compress_trace(struct trdb_ctx *c, FILE *output_fp,
                          struct arguments *);

static int decompress_packets(struct trdb_ctx *c, FILE *output_fp, bfd *abfd,
                              struct arguments *arguments);


static int disassemble_trace(struct trdb_ctx *c, FILE *output_fp, bfd *abfd,
                             struct arguments *);

int main(int argc, char *argv[argc + 1])
{
    int status = EXIT_SUCCESS;
    struct arguments arguments;
    /* set default */
    /* arguments = (struct arguments){0}; */
    arguments.silent = false;
    arguments.verbose = false;
    arguments.compress = false;
    arguments.has_elf = false;
    arguments.disassemble = false;
    arguments.decompress = false;
    arguments.output_file = "-";

    argp_parse(&argp, argc, argv, 0, 0, &arguments);

    FILE *output_fp = NULL;
    bfd *abfd = NULL;

    struct trdb_ctx *ctx = trdb_new();

    if (!ctx) {
        status = EXIT_FAILURE;
        goto fail;
    }

    /* prepare output */
    if (arguments.output_file[0] != '-') {
        output_fp = fopen(arguments.output_file, "w");
        if (!output_fp) {
            perror("fopen");
            status = EXIT_FAILURE;
            goto fail;
        }
    } else {
        output_fp = stdout;
    }

    /* commands that can use a bfd */
    bfd_init();
    if (arguments.has_elf) {
        abfd = bfd_openr(arguments.elf_file, NULL);
        if (!(abfd && bfd_check_format(abfd, bfd_object))) {
            bfd_perror("bfd_openr or bfd_check_format");
            status = EXIT_FAILURE;
            goto fail;
        }
    }

    if (arguments.compress) {
        status = compress_trace(ctx, output_fp, &arguments);
    } else if (arguments.decompress) {
        status = decompress_packets(ctx, output_fp, abfd, &arguments);
    } else if (arguments.disassemble) {
        status = disassemble_trace(ctx, output_fp, abfd, &arguments);
    }


fail:
    trdb_free(ctx);
    if (output_fp)
        fclose(output_fp);
    if (abfd)
        bfd_close(abfd);
    return status;
}


/* TODO: compress to pulp binary output too */
static int compress_trace(struct trdb_ctx *c, FILE *output_fp,
                          struct arguments *arguments)
{
    int status = EXIT_SUCCESS;

    LIST_HEAD(packet_list);
    /* read stimuli file and convert to internal data structure */
    struct tr_instr *tmp;
    struct tr_instr **samples = &tmp;
    int success = 0;
    size_t samplecnt =
        trdb_stimuli_to_trace(c, arguments->args[0], samples, &success);
    if (success != 0) {
        fprintf(stderr, "trdb_stimuli_to_trace failed\n");
        status = EXIT_FAILURE;
        goto fail;
    }


    /* step by step compression */
    for (size_t i = 0; i < samplecnt; i++) {
        int step = trdb_compress_trace_step(c, &packet_list, &(*samples)[i]);
        if (step == -1) {
            fprintf(stderr, "Compress trace failed.\n");
            status = EXIT_FAILURE;
            goto fail;
        }
    }

    /* write to file or stdout */
    trdb_dump_packet_list(output_fp, &packet_list);

fail:
    free(*samples);
    trdb_free_packet_list(&packet_list);

    return status;
}


static int decompress_packets(struct trdb_ctx *c, FILE *output_fp, bfd *abfd,
                              struct arguments *arguments)
{
    int status = EXIT_SUCCESS;
    const char *path = arguments->args[0];
    struct disassemble_info dinfo;
    struct disassembler_unit dunit;

    LIST_HEAD(packet_list);
    LIST_HEAD(instr_list);

    if (!abfd) {
        fprintf(stderr, "need to provide a binary for decompression\n");
        status = EXIT_FAILURE;
        goto fail;
    }

    /* by default we parse assuming data is generated from PULP */
    if (trdb_pulp_read_all_packets(c, path, &packet_list)) {
        fprintf(stderr, "failed to parse packets\n");
        status = EXIT_FAILURE;
        goto fail;
    }

    /* reconstruct the original instruction sequence */
    if (trdb_decompress_trace(c, abfd, &packet_list, &instr_list)) {
        fprintf(stderr,
                "failed to decompress packets due to either corrupt"
                "packets or wrong bfd, continuing anyway...\n");
        status = EXIT_FAILURE;
    }

    /* setup the disassembler to consider data from the bfd */
    dinfo = (struct disassemble_info){0};
    dunit = (struct disassembler_unit){0};

    dunit.dinfo = &dinfo;
    if (trdb_alloc_dinfo_with_bfd(c, abfd, &dunit)) {
        fprintf(stderr, "failed to configure bfd\n");
        status = EXIT_FAILURE;
        goto fail;
    }

    /* configure disassemble output */
    dinfo.fprintf_func = (fprintf_ftype)fprintf;
    dinfo.stream = output_fp;


    struct tr_instr *instr;
    list_for_each_entry_reverse(instr, &instr_list, list)
    {
        if (arguments->disassemble)
            trdb_disassemble_instr(instr, &dunit);
        else
            trdb_print_instr(output_fp, instr);
    }

fail:
    trdb_free_dinfo_with_bfd(c, abfd, &dunit);
    trdb_free_packet_list(&packet_list);
    trdb_free_instr_list(&instr_list);
    return status;
}

/* TODO: allow no bfd */
static int disassemble_trace(struct trdb_ctx *c, FILE *output_fp, bfd *abfd,
                             struct arguments *arguments)
{
    int status = EXIT_SUCCESS;

    struct disassemble_info dinfo;
    struct disassembler_unit dunit;

    /* read stimuli file and convert to internal data structure */
    struct tr_instr *tmp;
    struct tr_instr **samples = &tmp;
    int success = 0;
    size_t samplecnt =
        trdb_stimuli_to_trace(c, arguments->args[0], samples, &success);
    if (success != 0) {
        fprintf(stderr, "trdb_stimuli_to_trace failed\n");
        status = EXIT_FAILURE;
        goto fail;
    }

    dinfo = (struct disassemble_info){0};
    dunit = (struct disassembler_unit){0};
    dunit.dinfo = &dinfo;

    /* setup the disassembler to consider data from the bfd */
    if (abfd) {
        if (trdb_alloc_dinfo_with_bfd(c, abfd, &dunit)) {
            fprintf(stderr, "failed to configure bfd\n");
            status = EXIT_FAILURE;
            goto fail;
        }
        /* configure disassemble output */
        dinfo.fprintf_func = (fprintf_ftype)fprintf;
        dinfo.stream = output_fp;

    } else {
        /* if we can't use a bfd assume its pulp riscv */
        init_disassemble_info(&dinfo, stdout, (fprintf_ftype)fprintf);
        init_disassemble_info_for_pulp(&dinfo);
        dunit.disassemble_fn = print_insn_riscv;
    }


    trdb_disassemble_trace(samplecnt, *samples, &dunit);

fail:
    if (abfd)
        trdb_free_dinfo_with_bfd(c, abfd, &dunit);
    free(*samples);
    return status;
}
