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
 * @file trace_debugger.h
 * @author Robert Balas (balasr@student.ethz.ch)
 * @date 24 Aug 2018
 * @brief Software model for the hardware trace debugger.
 */

#ifndef __TRACE_DEBUGGER_H__
#define __TRACE_DEBUGGER_H__

#define PACKAGE "foo" /* quick hack for bfd if not using autotools */
#include <stdbool.h>
#include <inttypes.h>
#include <stdio.h>
#include <ctype.h>
/* fix nameconflict with basename in libiberty and libgen */
#define HAVE_DECL_BASENAME 1
#include "bfd.h"
#include "disassembly.h"
#include "list.h"

/* custom header */
#define PACKETLEN 7
#define PULPPKTLEN 4
#define MSGTYPELEN 2

/* given header */
#define FORMATLEN 2
#define BRANCHLEN 5
#define XLEN 32
#define CAUSELEN 5
#define PRIVLEN 3
#define ILEN 32
#define CONTEXTLEN 32

/* timer */
#define TIMELEN 40

/* sw dump */
#define SWLEN 32

#define INSTR_STR_LEN 128

/* forward declarations */
struct disassembler_unit;

/**
 * Retired instruction captured by the interface to the RISC-V CPU, with some
 * metadata.
 */
struct tr_instr {
    bool valid;                /**< whether this sample/struct is valid */
    bool exception;            /**< instruction trapped */
    bool interrupt;            /**< exception caused by interrupt */
    uint32_t cause : CAUSELEN; /**< exception cause */
    uint32_t tval : XLEN;      /**< not used in PULP, trap value */
    uint32_t priv : PRIVLEN;   /**< privilege mode */
    uint32_t iaddr : XLEN;     /**< instruction address */
    uint32_t instr : ILEN;     /**< raw instruction value */
    bool compressed;           /**< instruction was originally compressed */
    char str[INSTR_STR_LEN];   /**< disassembled instruction string, only
                                  sometimes valid */
    struct list_head list;     /**< anchor for linked list in list.h */
};

enum tr_packet_format {
    F_BRANCH_FULL = 0,
    F_BRANCH_DIFF = 1,
    F_ADDR_ONLY = 2,
    F_SYNC = 3
};

enum tr_packet_subformat { SF_START = 0, SF_EXCEPTION = 1, SF_CONTEXT = 2 };

enum tr_msg_type { W_EMPTY = 0, W_TIMER = 1, W_TRACE = 2, W_SOFTWARE = 3 };

/**
 * Canonical trace packet representation. This is the high level definition of a
 * packet with some meta information (first part of tr_packet) about the payload
 * (second part). The meta information is specific to the transportation
 * mechanism on the hardware. For the PULP platform there is just a small packet
 * length indicator
 *
 * There are four possible packet types. See <a
 * href="https://github.com/riscv/riscv-trace-spec">riscv-trace-spec</a> for
 * details. A list or array of such packets and a bfd struct represent together
 * a compressed list or array of tr_instr.
 */
struct tr_packet {
    /* transport layer header */
    uint32_t length : PACKETLEN; /**< length of packet in bits */
    uint32_t msg_type : 2;       /**< payload type */
    /* custom data written by user, valid when msg_type = W_SOFTWARE*/
    uint32_t userdata : SWLEN;

    /* timer data */
    uint64_t time;

    /* actual payload of spec, valid  when msg_type = W_TRACE */
    uint32_t format : 2;    /**< header denoting the packet type */
    uint32_t branches : 5;  /**< number of branches saved in branch_map */
    uint32_t branch_map;    /**< bits indicating taken=1 branches */
    uint32_t subformat : 2; /**< further specifies F_SYNC packets */
    uint32_t context : CONTEXTLEN; /**< not used in PULP, context switch */
    uint32_t privilege : PRIVLEN;  /**< current privilege mode */
    /* we need this if the first instruction of an exception is a branch, since
     * that won't be reorcded into the branch map
     */
    bool branch;                /**< special case for F_SYNC packets */
    uint32_t address : XLEN;    /**< address of the instruction */
    uint32_t ecause : CAUSELEN; /**< exception cause */
    bool interrupt;             /**< exception through interrupt */
    uint32_t tval : XLEN;       /**< not used in PULP, trap information */

    struct list_head list; /**< used to make a linked list of tr_packet */
};

/* For type punning.
 * It's pretty annoying to pack our packets tightly, since our packet members
 * are not 8 bit aligned. We assure that each packet is smaller that 128 bits,
 * pack all packet members into a 128 bit integer and read it out to bytes
 * through the union. Since uint8_t doesn't count as a char the compiler assumes
 * a uint8_t pointer can't alias a __uint128_t, thus the union. Or we could use
 * -fno-strict-aliasing.
 */
union trdb_pack {
    __uint128_t bits; /* non-portable gcc stuff. TODO: fix */
    uint8_t bin[16];  /* since uint8_t =/= char strict aliasing might
                       * mess your shit up. Careful this is also
                       * endian dependent
                       */
};


/**
 * Library/trace debugger context, needs to be hold by program and passed to
 * quite a few functions.
 */
struct trdb_ctx;

/**
 * Reset the trace debugger context to a clean state, do this before calling a
 * sequence of trdb_compress_trace_step.
 *
 * @param ctx trace debugger context
 */
void trdb_reset_compression(struct trdb_ctx *ctx);

/**
 * Reset the trace debugger context to a clean state, do this before calling a
 * trdb_decompress_trace.
 *
 * @param ctx trace debugger context
 */
void trdb_reset_decompression(struct trdb_ctx *ctx);

/**
 * Creates trdb library/trace debugger context. Fills in default values. Set
 * $TRDB_LOG for different default logging level.
 *
 * @return a new trdb library context
 */
struct trdb_ctx *trdb_new();

/**
 * Destroys a trdb context.
 *
 * @param ctx a trace debugger context to destroy
 */
void trdb_free(struct trdb_ctx *ctx);

/**
 * Hook a custom logging function for the specified trace debugger context. Per
 * default there is a function that logs to stderr.
 *
 * @param ctx a trace debugger context
 * @param log_fn a custom logging function
 */
void trdb_set_log_fn(struct trdb_ctx *ctx,
                     void (*log_fn)(struct trdb_ctx *ctx, int priority,
                                    const char *file, int line, const char *fn,
                                    const char *format, va_list args));

/**
 * Get the current logging priority. The value controls which messages are
 * logged.
 *
 * @param ctx a trace debugger context
 * @return the current loggin priority
 */
int trdb_get_log_priority(struct trdb_ctx *ctx);

/**
 * Set the current logging priority. The value controls which messages are
 * logged.
 *
 * @param ctx a trace debugger context
 * @param priority the new loggint priority
 */
void trdb_set_log_priority(struct trdb_ctx *ctx, int priority);

/**
 * Compress the given sequence of instruction to a sequence of packets.
 *
 * The packets should allow trdb_decompress_trace() to reconstruct the original
 * sequence of tr_instr. You can pass one instruction per call to it, simulating
 * step by step execution. The state of the compression (keeping the previously
 * fed instructions to make decisions on whether to emit a packet or not and
 * more) is held in @p ctx, which the caller is responsible for. Since the
 * function allocates new entries for list_head, the caller has to deallocate
 * them by calling trdb_free_packet_list(). Use the functions provided by list.h
 * to handle list_head entries.
 *
 * @param ctx trace debugger context/state
 * @param packet_list list to which packets will be appended
 * @param instrs[len]
 * @return the provided @packet_list
 */
int trdb_compress_trace_step(struct trdb_ctx *ctx,
                             struct list_head *packet_list,
                             struct tr_instr *instr);

/**
 * Generate the original instruction sequence from a list of tr_packet, given
 * the binary from which the instruction sequence was produced.
 *
 * @param c the context/state of the trace debugger
 * @param abfd the binary from which the trace was captured
 * @param packet_list the compressed instruction trace
 * @param instr_list list to which the reconstruction instructions will be
 * appended
 * @return 0 on success, -1 on failure
 */
int trdb_decompress_trace(struct trdb_ctx *c, bfd *abfd,
                          struct list_head *packet_list,
                          struct list_head *instr_list);

/**
 * Outputs disassembled trace using fprintf_func in #disassembler_unit.dinfo.
 *
 * @param len length of trace
 * @param trace[len] an array of instructions, only #iaddr and #instr are needed
 * @param dunit the configured disassembler
 */
void trdb_disassemble_trace(size_t len, struct tr_instr trace[len],
                            struct disassembler_unit *dunit);

/**
 * Output disassemble @p instr using fprintf_func in #disassembler_unit.dinfo.
 *
 * @param instr instruction, only #iaddr and #instr are needed
 * @param dunit the configured disassembler
 */
void trdb_disassemble_instr(struct tr_instr *instr,
                            struct disassembler_unit *dunit);

/**
 * Disassemble @p instr using fprint_func in #disassembler_unit.dinfo. This
 * function additionaly uses information from the @p abfd and settings in @dp
 * dunit to produce more useful disassembly, such as intermixing sourcecode,
 * showing file and line numbers, resolving addresses to symbols etc.
 *
 * @param c trace debugger context
 * @param instr instruction to disassemble, only #iaddr is relevant
 * @param abfd the bfd which contains @p instr
 * @param dunit the configured disassembler
 */
void trdb_disassemble_instr_with_bfd(struct trdb_ctx *c, struct tr_instr *instr,
                                     bfd *abfd,
                                     struct disassembler_unit *dunit);

/**
 * Prints a list of tr_packet in a formatted manner to @p stream.
 *
 * @param stream output to write to
 * @param packet_list sequence of packets to print
 */
void trdb_dump_packet_list(FILE *stream, const struct list_head *packet_list);

/**
 * Log a single packet in a formatted manner using the library context @p c.
 *
 * @param c the context of the trace debugger
 * @param packet tr_packet to log
 */
void trdb_log_packet(struct trdb_ctx *c, const struct tr_packet *packet);

/**
 *  Prints a single packet in a formatted manner to @p stream.
 *
 * @param stream output to write to
 * @param packet tr_packet to print
 */
void trdb_print_packet(FILE *stream, const struct tr_packet *packet);

/**
 *  Log a single instruction in a formatted manner using the library context @p
 * c.
 *
 * @param c the context of the trace debugger
 * @param instr tr_instr to log
 */
void trdb_log_instr(struct trdb_ctx *c, const struct tr_instr *packet);

/**
 *  Prints a single instruction in a formatted manner to @p stream.
 *
 * @param stream output to write to
 * @param instr tr_instr to print
 */
void trdb_print_instr(FILE *stream, const struct tr_instr *packet);

/**
 * Compare two instructions for memberwise equality;
 *
 * @param c the context of the trace debugger for logging
 * @param instr0 first instruction of the comparison
 * @param instr1 second instruction of the comparison
 *
 * @return whether both instruction are memberwise equal
 */
bool trdb_compare_instr(struct trdb_ctx *c, const struct tr_instr *instr0,
                        const struct tr_instr *instr1);


/**
 * Free the entries of a list of tr_packet, used to deallocate the list returned
 * by trdb_compress_trace().
 *
 * @param packet_list list to free
 */
void trdb_free_packet_list(struct list_head *packet_list);

/**
 * Free the entries of a list of tr_instr, used to deallocate the list returned
 * by trdb_decompress_trace().
 *
 * @param instr_list list to free
 */
void trdb_free_instr_list(struct list_head *instr_list);

/**
 * Read a stimuli file at @p path into a list of tr_instr. This function
 * allocates the struts in a sequential fashion.
 *
 * @param c the context/state of the trace debugger
 * @param path where the stimuli file is located at
 * @param status will be written with -1 on failure, 0 on success
 * @param instrs list anchor where to append the tr_instr to.
 * @return number of read tr_instr
 */
size_t trdb_stimuli_to_trace_list(struct trdb_ctx *c, const char *path,
                                  int *status, struct list_head *instrs);

/**
 * Read a stimuli file at @p path into an array of tr_instr.
 *
 * @param c the context/state of the trace debugger
 * @param path where the stimuli file is located at
 * @param samples where to write the read array of tr_instr
 * @param status will be written with -1 on failure, 0 on success
 * @return number of read tr_instr
 */
size_t trdb_stimuli_to_trace(struct trdb_ctx *c, const char *path,
                             struct tr_instr **samples, int *status);

/* struct packet0 {
 *     uint32_t format : 2;   // 00
 *     uint32_t branches : 5;
 *     uint32_t branch_map; //TODO: fix to 31 bits
 *     uint32_t address;
 *
 * 2 + 5 + (1 to 31) + (1 to 32) = 9 to 70
 *
 * struct packet1 {
 *     uint32_t format : 2;   // 01
 *     uint32_t branches : 5;
 *     uint32_t branch_map;
 *     uint32_t address;
 * };
 * 2 + 5 + (1 to 31) + (1 to 32) = 9 to 70
 *
 * struct packet2 {
 *     uint32_t format : 2;
 *     uint32_t address;
 * };
 * 2 + (1 to 32) = 3 to 34
 *
 * struct packet3 {
 *     uint32_t format : 2;
 *     uint32_t subformat : 2;
 *     uint32_t context;
 *     uint32_t privilege : PRIVLEN;
 *     bool branch : 1;
 *     uint32_t address : XLEN;
 *     uint32_t ecause : CAUSELEN;
 *     bool interrupt : 1;
 *     uint32_t tval : XLEN;
 * };
 *
 * 2 + 2 + 3 + 1 + 32 + 5 + 1 = 46
 */

#endif
