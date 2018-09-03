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
 * Description: trdb -- trace debugger for the  PULP platform, compress and
 * decompress instruction traces
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

#define TRDB_NUM_ARGS 1

const char *argp_program_version = "trdb 0.1";
const char *argp_program_bug_address = "<balasr@student.ethz.ch>";

static char doc[] = "trdb -- trace debugger for the PULP platform";
static char args_doc[] = "TRACE-OR-PACKETS";

static struct argp_option options[] = {
    {"verbose", 'v', 0, 0, "Produce verbose output"},
    {"quiet", 'q', 0, 0, "Don't produce any output"},
    {"silent", 's', 0, OPTION_ALIAS},
    {"bfd", 'b', "ELF", 0,
     "ELF binary with which the traces/packets were produced"},
    {"compress", 'c', 0, 0, "Take a stimuli file and compress to packets"},
    {"disassemble", 'd', 0, 0,
     "Take a stimuli file and disassemble. Specify an elf file additionaly for more information"},
    {"output", 'o', "FILE", 0, "Output to FILE instead of stdout"},
    {0}};

struct arguments {
    char *args[TRDB_NUM_ARGS];
    bool silent, verbose, compress, has_elf, disassemble;
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
static int decompress_packets_from_bfd_and_file(struct arguments *);
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
    arguments.output_file = "-";

    argp_parse(&argp, argc, argv, 0, 0, &arguments);

    struct trdb_ctx *ctx = trdb_new();
    if (!ctx) {
        status = EXIT_FAILURE;
        goto fail;
    }
    FILE *output_fp = NULL;
    bfd *abfd = NULL;

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

/* TODO: compress to binary too */
static int compress_trace(struct trdb_ctx *c, FILE *output_fp,
                          struct arguments *arguments)
{
    int status = EXIT_SUCCESS;

    /* init trace debugger structs */
    trdb_init();

    /* read stimuli file and convert to internal data structure */
    struct tr_instr *tmp;
    struct tr_instr **samples = &tmp;
    int success = 0;
    size_t samplecnt =
        trdb_stimuli_to_trace(c, arguments->args[0], samples, &success);
    if (success != 0) {
        LOG_ERRT("trdb_stimuli_to_trace failed\n");
        status = EXIT_FAILURE;
        goto fail;
    }

    /* actual compression step */
    LIST_HEAD(packet_list);
    struct list_head *ret =
        trdb_compress_trace(&packet_list, samplecnt, *samples);
    if (!ret) {
        LOG_ERRT("trdb_compress_trace faile\n");
        status = EXIT_FAILURE;
        goto fail;
    }

    /* write to file or stdout */
    trdb_dump_packet_list(output_fp, &packet_list);

fail:
    trdb_close();
    free(*samples);
    trdb_free_packet_list(&packet_list);

    return status;
}


static int decompress_packets(FILE *output_file, bfd *abfd,
                              struct arguments *arguments)
{
    int status = EXIT_SUCCESS;


    return status;
}

/* TODO: allow no bfd */
static int disassemble_trace(struct trdb_ctx *c, FILE *output_fp, bfd *abfd,
                             struct arguments *arguments)
{
    if (!abfd) {
        LOG_ERRT("Disassembling with no bfd is not supporte yet\n");
        return EXIT_FAILURE;
    }

    int status = EXIT_SUCCESS;

    /* init trace debugger structs */
    trdb_init();

    /* read stimuli file and convert to internal data structure */
    struct tr_instr *tmp;
    struct tr_instr **samples = &tmp;
    int success = 0;
    size_t samplecnt =
        trdb_stimuli_to_trace(c, arguments->args[0], samples, &success);
    if (success != 0) {
        LOG_ERRT("trdb_stimuli_to_trace failed\n");
        status = EXIT_FAILURE;
        goto fail;
    }
    /* initialize disassembler based on bfd */
    struct disassembler_unit dunit = {0};
    struct disassemble_info dinfo = {0};
    dunit.dinfo = &dinfo;
    init_disassembler_unit(&dunit, abfd, NULL);
    dinfo.stream = output_fp; /* redirect output */

    trdb_disassemble_trace(samplecnt, *samples, &dunit);


fail:
    trdb_close();
    free(*samples);
    return status;
}
