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
 * Description: trdb -- trace debugger tools for the  PULP platform, compress
 * and decompress instruction traces
 */

#define PACKAGE "foo" /* quick hack for bfd if not using autotools */
#include <stdio.h>
#include <stdlib.h>
#include <argp.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <sys/queue.h>
#include "bfd.h"
#include "utils.h"
#include "trace_debugger.h"
#include "disassembly.h"
#include "serialize.h"

#define TRDB_NUM_ARGS 1

const char *argp_program_version     = "trdb 0.1";
const char *argp_program_bug_address = "<balasr@student.ethz.ch>";

static char doc[]      = "trdb -- trace debugger tools for the PULP platform";
static char args_doc[] = "TRACE-OR-PACKETS";

#define TRDB_OPT_DEMANGLE 1
#define TRDB_OPT_NO_ALIASES 2
#define TRDB_OPT_PREFIX_ADDRESSES 3
#define TRDB_OPT_INLINES 4
#define TRDB_OPT_FULL_ADDR 5
#define TRDB_OPT_CVS 6

static struct argp_option options[] = {
    {"verbose", 'v', 0, 0, "Produce verbose output"},
    {"quiet", 'q', 0, 0, "Don't produce any output"},
    /* {"silent", 's', 0, OPTION_ALIAS}, */
    {"bfd", 'b', "ELF", 0,
     "ELF binary with which the traces/packets were produced"},
    {"compress", 'c', 0, 0, "Take a stimuli file and compress to packets"},
    {"cvs", TRDB_OPT_CVS, 0, 0, "Set TRACE-OR-PACKETS file type to cvs"},
    {"binary-format", 'p', "FORMAT", 0,
     "Specify binary input/output format, by default like the pulp trace debugger"},
    {"trace-file", 't', 0, 0, "Input file is a trace file"},
    {"dump", 'h', 0, 0, "Dump trace or packets in human readable format"},
    {"extract", 'x', 0, 0, "Take a packet file and produce a stimuli file"},
    {"full-address", TRDB_OPT_FULL_ADDR, 0, 0,
     "Assume no sign-extension compression on addresses"},
    {"disassemble", 'd', 0, 0, "disassemble the output"},
    {"no-aliases", TRDB_OPT_NO_ALIASES, 0, 0,
     "Do not use aliases for disassembled instructions"},
    {"demangle", TRDB_OPT_DEMANGLE, 0, 0,
     "Demangle symbols, sometimes helps readability"},
    {"prefix-addresses", TRDB_OPT_PREFIX_ADDRESSES, 0, 0,
     "Print complete address alongside disassembly"},
    {"file-offsets", 'F', 0, 0,
     "Include file offsets when displaying information"},
    {"source", 'S', 0, 0, "Intermix source code with disassembly"},
    {"function-context", 's', 0, 0,
     "Show when address coincides with function name entrypoint"},
    {"line-numbers", 'l', 0, 0, "Include line numbers and filenames in output"},
    {"inlines", TRDB_OPT_INLINES, 0, 0,
     "Print all inlines for source line (with -l)"},
    {"output", 'o', "FILE", 0, "Write to FILE instead of stdout"},
    {0}};

struct arguments {
    char *args[TRDB_NUM_ARGS];
    bool silent, verbose, compress, has_elf, disassemble, decompress,
        trace_file, binary_output, human, full_address, cvs;
    uint32_t settings_disasm;
    char *binary_format;
    char *output_file;
    char *elf_file;
};

static error_t parse_opt(int key, char *arg, struct argp_state *state)
{
    struct arguments *arguments = state->input;
    switch (key) {
    case 'q':
        arguments->silent = true;
        break;
    case 'o':
        /* TODO: can I hold pointer to arg? */
        arguments->output_file = arg;
        break;
    case 'b':
        arguments->has_elf  = true;
        arguments->elf_file = arg;
        break;
    case 'c':
        arguments->compress = true;
        break;
    case TRDB_OPT_CVS:
        arguments->cvs = true;
        break;
    case 'p':
        arguments->binary_output = true;
        arguments->binary_format = arg;
        break;
    case 't':
        arguments->trace_file = true;
        break;
    case 'h':
        arguments->human = true;
        break;
    case 'd':
        arguments->disassemble = true;
        break;
    case 'x':
        arguments->decompress = true;
        break;
    case TRDB_OPT_FULL_ADDR:
        arguments->full_address = true;
        break;
    case TRDB_OPT_NO_ALIASES:
        arguments->settings_disasm |= TRDB_NO_ALIASES;
        break;
    case TRDB_OPT_DEMANGLE:
        arguments->settings_disasm |= TRDB_DO_DEMANGLE;
        break;
    case TRDB_OPT_PREFIX_ADDRESSES:
        arguments->settings_disasm |= TRDB_PREFIX_ADDRESSES;
        break;
    case 'F':
        arguments->settings_disasm |= TRDB_DISPLAY_FILE_OFFSETS;
        break;
    case 'S':
        arguments->settings_disasm |= TRDB_SOURCE_CODE;
        break;
    case 's':
        arguments->settings_disasm |= TRDB_FUNCTION_CONTEXT;
        break;
    case 'l':
        arguments->settings_disasm |= TRDB_LINE_NUMBERS;
        break;
    case TRDB_OPT_INLINES:
        arguments->settings_disasm |= TRDB_INLINES;
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

static int dump_packets(struct trdb_ctx *c, FILE *output_fp,
                        struct arguments *arguments);

static int dump_trace(struct trdb_ctx *c, FILE *output_fp,
                      struct arguments *arguments);

int main(int argc, char *argv[argc + 1])
{
    int status = EXIT_SUCCESS;
    struct arguments arguments;
    /* set default */
    arguments                 = (struct arguments){0};
    arguments.silent          = false;
    arguments.verbose         = false;
    arguments.compress        = false;
    arguments.cvs             = false;
    arguments.binary_output   = false;
    arguments.human           = false;
    arguments.full_address    = false;
    arguments.has_elf         = false;
    arguments.disassemble     = false;
    arguments.decompress      = false;
    arguments.settings_disasm = 0;
    arguments.output_file     = "-";
    arguments.binary_format   = "";

    argp_parse(&argp, argc, argv, 0, 0, &arguments);

    FILE *output_fp = NULL;
    bfd *abfd       = NULL;

    struct trdb_ctx *ctx = trdb_new();

    if (!ctx) {
        fprintf(stderr, "creating trdb library context failed\n");
        status = EXIT_FAILURE;
        goto fail;
    }

    /* general settings */
    trdb_set_full_address(ctx, arguments.full_address);
    trdb_set_compress_branch_map(ctx, false);
    /* trdb_set_implicit_ret(ctx, true); */
    /* trdb_set_pulp_extra_packet(ctx, true); */

    /* prepare output */
    if (arguments.output_file[0] != '-') {
        output_fp = fopen(arguments.output_file, "w");
        if (!output_fp) {
            fprintf(stderr, "fopen: %s", strerror(errno));
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

    if (arguments.decompress) {
        status = decompress_packets(ctx, output_fp, abfd, &arguments);
    } else if (arguments.compress) {
        status = compress_trace(ctx, output_fp, &arguments);
    } else if (arguments.human) {
        if (arguments.trace_file)
            status = dump_trace(ctx, output_fp, &arguments);
        else
            status = dump_packets(ctx, output_fp, &arguments);
    } else if (arguments.trace_file) {
        status = disassemble_trace(ctx, output_fp, abfd, &arguments);
    } else {
        /* by default we decompress */
        status = decompress_packets(ctx, output_fp, abfd, &arguments);
    }

fail:
    trdb_free(ctx);
    if (output_fp)
        fclose(output_fp);
    if (abfd)
        bfd_close(abfd);
    return status;
}

static int compress_trace(struct trdb_ctx *c, FILE *output_fp,
                          struct arguments *arguments)
{
    int status = EXIT_SUCCESS;

    struct trdb_packet_head packet_list;
    TAILQ_INIT(&packet_list);
    /* read stimuli file and convert to internal data structure */
    struct tr_instr *tmp      = NULL;
    struct tr_instr **samples = &tmp;
    struct trdb_instr_head instr_list;
    TAILQ_INIT(&instr_list);

    int success      = 0;
    size_t samplecnt = 0;
    if (arguments->cvs) {
        success = trdb_cvs_to_trace_list(c, arguments->args[0], &instr_list,
                                         &samplecnt);
    } else {
        success =
            trdb_stimuli_to_trace(c, arguments->args[0], samples, &samplecnt);
    }

    if (success < 0) {
        fprintf(stderr, "reading stimuli file failed: %s\n",
                trdb_errstr(trdb_errcode(success)));
        status = EXIT_FAILURE;
        goto fail;
    }

    struct tr_instr *instr;

    /* step by step compression */
    if (arguments->cvs) {
        TAILQ_FOREACH (instr, &instr_list, list) {
            int step = trdb_compress_trace_step_add(c, &packet_list, instr);
            if (step < 0) {
                fprintf(stderr, "compress trace failed: %s\n",
                        trdb_errstr(trdb_errcode(status)));
                status = EXIT_FAILURE;
                goto fail;
            }
        }
    } else {
        for (size_t i = 0; i < samplecnt; i++) {
            int step =
                trdb_compress_trace_step_add(c, &packet_list, &(*samples)[i]);
            if (step < 0) {
                fprintf(stderr, "compress trace failed: %s\n",
                        trdb_errstr(trdb_errcode(status)));
                status = EXIT_FAILURE;
                goto fail;
            }
        }
    }
    /* we either produce binary or human readable output */
    if (arguments->binary_output) {
        struct tr_packet *packet;
        TAILQ_FOREACH (packet, &packet_list, list) {
            status = trdb_pulp_write_single_packet(c, packet, output_fp);
            if (status < 0) {
                fprintf(stderr, "failed to serialize packets: %s\n",
                        trdb_errstr(trdb_errcode(status)));
                status = EXIT_FAILURE;
                goto fail;
            }
        }
    } else {
        /* write to file or stdout */
        trdb_dump_packet_list(output_fp, &packet_list);
    }
fail:
    free(*samples);
    trdb_free_packet_list(&packet_list);
    trdb_free_instr_list(&instr_list);

    return status;
}

static int decompress_packets(struct trdb_ctx *c, FILE *output_fp, bfd *abfd,
                              struct arguments *arguments)
{
    int status       = EXIT_SUCCESS;
    const char *path = arguments->args[0];
    struct disassemble_info dinfo;
    struct disassembler_unit dunit;
    struct trdb_packet_head packet_list;
    TAILQ_INIT(&packet_list);
    struct trdb_instr_head instr_list;
    TAILQ_INIT(&instr_list);

    if (!abfd) {
        fprintf(stderr, "need to provide a binary (--bfd) for decompression\n");
        status = EXIT_FAILURE;
        goto fail;
    }

    /* by default we parse assuming data is generated from PULP */
    if ((status = trdb_pulp_read_all_packets(c, path, &packet_list)) < 0) {
        fprintf(stderr, "failed to parse packets: %s\n",
                trdb_errstr(trdb_errcode(status)));
        status = EXIT_FAILURE;
        goto fail;
    }

    /* reconstruct the original instruction sequence */
    status = trdb_decompress_trace(c, abfd, &packet_list, &instr_list);
    if (status < 0) {
        fprintf(stderr, "decompressing trace failed: %s\n",
                trdb_errstr(trdb_errcode(status)));
        fprintf(stderr, "possible causes: corrupt packets or wrong bfd\n");
        fprintf(stderr, "continuing anyway...\n");
    }

    /* setup the disassembler to consider data from the bfd */
    dinfo = (struct disassemble_info){0};
    dunit = (struct disassembler_unit){0};

    dunit.dinfo = &dinfo;
    status      = trdb_alloc_dinfo_with_bfd(c, abfd, &dunit);
    if (status < 0) {
        fprintf(stderr, "failed to configure bfd: %s\n",
                trdb_errstr(trdb_errcode(status)));
        status = EXIT_FAILURE;
        goto fail;
    }

    /* configure disassemble output */
    trdb_set_disassembly_conf(&dunit, arguments->settings_disasm);
    dinfo.fprintf_func = (fprintf_ftype)fprintf;
    dinfo.stream       = output_fp;

    struct tr_instr *instr;
    TAILQ_FOREACH (instr, &instr_list, list) {
        if (arguments->disassemble)
            trdb_disassemble_instr_with_bfd(c, instr, abfd, &dunit);
        /* trdb_disassemble_instr(instr, &dunit); */
        else
            trdb_print_instr(output_fp, instr);
    }

fail:
    /* trdb_free_dinfo_with_bfd(c, abfd, &dunit); */
    trdb_free_packet_list(&packet_list);
    trdb_free_instr_list(&instr_list);
    return status;
}

static int disassemble_trace(struct trdb_ctx *c, FILE *output_fp, bfd *abfd,
                             struct arguments *arguments)
{
    int status = EXIT_SUCCESS;

    struct disassemble_info dinfo;
    struct disassembler_unit dunit;

    /* read stimuli file and convert to internal data structure */
    struct tr_instr *tmp;
    struct tr_instr **samples = &tmp;
    size_t samplecnt          = 0;
    int success               = 0;

    success = trdb_stimuli_to_trace(c, arguments->args[0], samples, &samplecnt);
    if (success < 0) {
        fprintf(stderr, "reading stimuli file failed: %s\n",
                trdb_errstr(trdb_errcode(status)));
        status = EXIT_FAILURE;
        goto fail;
    }

    dinfo       = (struct disassemble_info){0};
    dunit       = (struct disassembler_unit){0};
    dunit.dinfo = &dinfo;

    /* setup the disassembler to consider data from the bfd */
    if (abfd) {
        if (trdb_alloc_dinfo_with_bfd(c, abfd, &dunit)) {
            fprintf(stderr, "failed to configure bfd: %s\n",
                    trdb_errstr(trdb_errcode(status)));
            status = EXIT_FAILURE;
            goto fail;
        }
        /* configure disassemble output */
        dinfo.fprintf_func = (fprintf_ftype)fprintf;
        dinfo.stream       = output_fp;

    } else {
        /* if we can't use a bfd assume its pulp riscv */
        init_disassemble_info(&dinfo, stdout, (fprintf_ftype)fprintf);
        trdb_init_disassemble_info_for_pulp(&dinfo);
        dunit.disassemble_fn = print_insn_riscv;
    }

    trdb_disassemble_trace(samplecnt, *samples, &dunit);

fail:
    if (abfd)
        trdb_free_dinfo_with_bfd(c, abfd, &dunit);
    free(*samples);
    return status;
}

static int dump_packets(struct trdb_ctx *c, FILE *output_fp,
                        struct arguments *arguments)
{
    int status = EXIT_SUCCESS;

    if (!strcmp(arguments->binary_format, "")) {
        fprintf(
            stderr,
            "provide a --binary-format (-p) argument to interpret this file\n");
        return EXIT_FAILURE;
    }

    const char *path = arguments->args[0];
    struct trdb_packet_head packet_list;
    TAILQ_INIT(&packet_list);

    if (!strcmp(arguments->binary_format, "pulp")) {
        if ((status = trdb_pulp_read_all_packets(c, path, &packet_list)) < 0) {
            fprintf(stderr, "failed to parse packets: %s\n",
                    trdb_errstr(trdb_errcode(status)));
            status = EXIT_FAILURE;
            goto fail;
        }
        trdb_dump_packet_list(output_fp, &packet_list);

    } else {
        fprintf(stderr, "binary format %s not supported/recognized\n",
                arguments->binary_format);
        status = EXIT_FAILURE;
        goto fail;
    }

fail:
    trdb_free_packet_list(&packet_list);
    return status;
}

static int dump_trace(struct trdb_ctx *c, FILE *output_fp,
                      struct arguments *arguments)
{
    int status = EXIT_SUCCESS;

    const char *path = arguments->args[0];
    struct trdb_instr_head instr_list;
    size_t instrcnt = 0;
    TAILQ_INIT(&instr_list);

    if (arguments->cvs) {
        status = trdb_cvs_to_trace_list(c, path, &instr_list, &instrcnt);
        if (status < 0) {
            fprintf(stderr, "failed to parse cvs file: %s\n",
                    trdb_errstr(trdb_errcode(status)));
            status = EXIT_FAILURE;
            goto fail;
        }
        trdb_dump_instr_list(output_fp, &instr_list);
    } else {
        fprintf(stderr, "dump trace for non cvs file not implemented yet");
        status = EXIT_FAILURE;
        goto fail;
    }

fail:
    trdb_free_instr_list(&instr_list);
    return status;
}
