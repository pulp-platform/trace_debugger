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
#include "bfd.h"
#include "disassembly.h"
#include "list.h"

#define XLEN 32
#define CAUSELEN 5
#define PRIVLEN 3 //TODO: fix this to 3 or 2
#define ILEN 32
#define CONTEXTLEN 32

#define INSTR_STR_LEN 128
/**
 * Library context, needs to be hold by program and passed to quite a few functions.
 */
struct trdb_ctx;

/**
 * Retired instruction captured by the interface to the RISC-V CPU, with some
 * metadata.
 */
struct tr_instr {
    /* bool valid; */          /* implicitely true */
    bool exception;            /**< instruction trapped */
    bool interrupt;            /**< exception caused by interrupt */
    uint32_t cause : CAUSELEN; /**< exception cause */
    uint32_t tval : XLEN;      /**< not used in PULP, trap value */
    uint32_t priv : PRIVLEN;   /**< privilege mode */
    uint32_t iaddr : XLEN;     /**< instruction address */
    uint32_t instr : ILEN;     /**< raw instruction value */
    bool compressed;           /**< instruction was originally compressed */
    /* Disassembled name, only sometimes valid */
    char str[INSTR_STR_LEN]; /**< disassembled instruction string */

    // TODO: put this into trdb_packet instead
    struct list_head list;
};

/* TODO: make enums */
#define F_BRANCH_FULL 0
#define F_BRANCH_DIFF 1
#define F_ADDR_ONLY 2
#define F_SYNC 3

#define SF_START 0
#define SF_EXCEPTION 1
#define SF_CONTEXT 2


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
    uint32_t length: 7;
    /* actual payload of spec */
    uint32_t msg_type : 2; /**< UltraSoC specific TODO: remove */
    uint32_t format : 2;   /**< header denoting the packet type */

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

#define ALLOC_INIT_PACKET(name)                                                \
    struct tr_packet *name = malloc(sizeof(*name));                            \
    if (!name) {                                                               \
        perror("malloc");                                                      \
        goto fail_malloc;                                                      \
    }                                                                          \
    *name = (struct tr_packet){.msg_type = 2};

#define INIT_PACKET(p)                                                         \
    do {                                                                       \
        *p = (struct tr_packet){0};                                            \
    } while (false)

/**
 * Initialize the trace debugger, call before any other functions.
 */
void trdb_init();

/**
 * Release resources of the trace debugger, currently a NOP.
 */
void trdb_close();

/**
 * Compress the given sequence of instruction to a sequence of packets.
 *
 * The packets should allow trdb_decompress_trace() to reconstruct the original
 * sequence of tr_instr. Since the function allocates new entries for list_head,
 * the caller has to deallocate them by calling trdb_free_packet_list(). Use the
 * functions provided by list.h to handle list_head entries.
 *
 * @param packet_list list to which packets will be appended
 * @param len number of tr_instr
 * @param instrs[len]
 * @return the provided @packet_list
 */
struct list_head *trdb_compress_trace(struct list_head *packet_list, size_t len,
                                      struct tr_instr instrs[len]);

/* packet from bitstream where parse single packet */
/* packet from bitstream whole decode function */

/**
 * Generate the original instruction sequence from a list of tr_packet, given
 * the binary from which the instruction sequence was produced.
 *
 * @param abfd the binary from which the trace was captured
 * @param packet_list the compressed instruction trace
 * @param instr_list list to which the reconstruction instructions will be
 * appended
 * @return the provided @packet_list
 */
struct list_head *trdb_decompress_trace(bfd *abfd,
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
 * Prints a list of tr_packet in a formatted manner to @p stream.
 *
 * @param stream output to write to
 * @param packet_list sequence of packets to print
 */
void trdb_dump_packet_list(FILE *stream, struct list_head *packet_list);

/**
 *  Prints a single packet in a formatted manner to @p stream.
 *
 * @param stream output to write to
 * @param packet tr_packet to print
 */
void trdb_print_packet(FILE *stream, struct tr_packet *packet);

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
 * Write a list of tr_packets to a file located at @p path.
 *
 * This function calls serialize_packet() to tightly align the packets,
 *which
 * can have a non-power-of-two size.
 *
 * @param path where the file is located at
 * @param packet_list list of packets to write
 * @return -1 on failure and 0 on success
 */
int trdb_write_packets(const char *path, struct list_head *packet_list);

/**
 * Read a stimuli file at @p path into an array of tr_instr.
 *
 * @param path where the stimuli file is located at
 * @param samples where to write the read array of tr_instr
 * @param status will be written with -1 on failure, 0 on success
 * @return number of read tr_instr
 */
size_t trdb_stimuli_to_trace(const char *path, struct tr_instr **samples,
                             int *status);

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
 * 2 + 2 + 2 + 1 + 32 + 5 + 1 = 45
 */

#endif
