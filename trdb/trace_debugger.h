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

#ifndef __TRACE_DEBUGGER_H__
#define __TRACE_DEBUGGER_H__

#define PACKAGE "foo" /* quick hack for bfd if not using autotools */
#include <stdbool.h>
#include <inttypes.h>
#include <stdio.h>
#include "bfd.h"
#include "list.h"

#define XLEN 32
#define CAUSELEN 5
#define PRIVLEN 5
#define ILEN 32

#define INSTR_STR_LEN 128
/* Sample captured by the interface to the RISC-V CPU.*/
struct tr_instr {
    /* bool valid; */ /* implicitely true */
    bool exception;
    bool interrupt;
    uint32_t cause : CAUSELEN; /* CAUSELEN */
    uint32_t tval : XLEN;      /* XLEN */
    uint32_t priv : PRIVLEN;   /* PRIVLEN */
    uint32_t iaddr : XLEN;     /* XLEN */
    uint32_t instr : ILEN;     /* ILEN */
    bool compressed;
    /* Disassembled name, only sometimes valid */
    char str[INSTR_STR_LEN];

    struct list_head list;
};

/* TODO: maybe make enums */
#define F_BRANCH_FULL 0
#define F_BRANCH_DIFF 1
#define F_ADDR_ONLY 2
#define F_SYNC 3

#define SF_START 0
#define SF_EXCEPTION 1
#define SF_CONTEXT 2

/* Canonical trace packet representation. There are four possible
 * packet types. See riscv-trace-spec.pdf for details. A list or array
 * of such packets and a bfd struct represent together a compressed
 * list or array of tr_instr.
 */
struct tr_packet {
    uint32_t msg_type : 2; /* UltraSoC specific */
    uint32_t len : 7;
    uint32_t format : 2;

    uint32_t branches : 5;
    uint32_t branch_map;

    uint32_t subformat : 2;
    uint32_t context;
    uint32_t privilege : PRIVLEN;
    /* we need this if the first instruction of an exception is a branch, since
     * that won't be reorcded into the branch map
     */
    bool branch;
    uint32_t address : XLEN;
    uint32_t ecause : CAUSELEN;
    bool interrupt;
    uint32_t tval : XLEN;

    struct list_head list;
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

/* Call this function before any other */
void trdb_init();

/* Call this function to cleanup */
void trdb_close();

/* Given a tr_instr array of length len and a list_head
 * trdb_compress_trace returns the list_head with a number of packets
 * added to it. These packets should allow trdb_decompress_trace to
 * reconstruct the original sequence of tr_instr. Since the
 * function allocates new entries for list_head, the caller has to
 * deallocate them by calling trdb_free_packet_list. Use the functions
 * provided by list.h to handle list_head entries.
 */
struct list_head *trdb_compress_trace(struct list_head *packet_list, size_t len,
                                      struct tr_instr[len]);

/* packet from bitstream where parse single packet */
/* packet from bitstream whole decode function */

/* Generate the original instruction sequence from a list of
 * tr_packets, given the binary from which the instruction sequence
 * was produced.
 */
struct list_head *trdb_decompress_trace(bfd *abfd,
                                        struct list_head *packet_list,
                                        struct list_head *instr_list);

/* Prints a list of tr_packets in a formatted manner to stdout. */
void trdb_dump_packet_list(FILE *stream, struct list_head *packet_list);

/* Prints a single packet in a formatted manner to stdout */
void trdb_print_packet(FILE *stream, struct tr_packet *packet);

/* Free the entries of a list of tr_packets. Used to dealloacte the
 * list returned by trdb_compress_trace.
 */
void trdb_free_packet_list(struct list_head *packet_list);

/* Write a list of tr_packets to a file located at path. Overwrites old file.
 * Return -1 on failure and 0 on success.
 */
int trdb_write_trace(const char *path, struct list_head *packet_list);

size_t trdb_stimuli_to_tr_instr(const char *path, struct tr_instr **samples,
                                int *status);


/* struct packet0 {
 *     uint32_t msg_type : 2;
 *     uint32_t format : 2;   // 00
 *     uint32_t branches : 5;
 *     uint32_t branch_map;
 *     uint32_t address;
 * };
 *
 * struct packet1 {
 *     uint32_t msg_type : 2;
 *     uint32_t format : 2;   // 01
 *     uint32_t branches : 5;
 *     uint32_t branch_map;
 *     uint32_t address;
 * };
 *
 * struct packet2 {
 *     uint32_t msg_type : 2;
 *     uint32_t format : 2;
 *     uint32_t address;
 * };
 *
 * struct packet3 {
 *     uint32_t msg_type : 2;
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
 */

#endif
