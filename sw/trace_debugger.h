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

#include <stdbool.h>
#include <inttypes.h>
#include "list.h"

#define XLEN 32
#define CAUSELEN 5
#define PRIVLEN 5
#define ILEN 32
#define MASK_FROM(len) ((1ull << len) - 1)

struct instr_sample {
    /* bool valid; */
    bool exception;
    bool interrupt;
    uint32_t cause : CAUSELEN; /* CAUSELEN */
    uint32_t tval : XLEN;      /* XLEN */
    uint32_t priv : PRIVLEN;   /* PRIVLEN */
    uint32_t iaddr : XLEN;     /* XLEN */
    uint32_t instr : ILEN;     /* ILEN */
};


struct tr_packet {
    uint32_t msg_type : 2;
    uint32_t format : 2;

    uint32_t branches : 5;
    uint32_t branch_map;

    uint32_t subformat : 2;
    uint32_t context;
    uint32_t privilege : PRIVLEN;
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
    INIT_PACKET(name);


/* Turns out zero initializing a dynamically allocated struct is not that easy
 */
#define INIT_PACKET(p)                                                         \
    do {                                                                       \
        p->msg_type = 0x2; /* TODO: better set this 0 to prevent confutions */ \
        p->format = 0;                                                         \
        p->branches = 0;                                                       \
        p->branch_map = 0;                                                     \
        p->subformat = 0;                                                      \
        p->context = 0;                                                        \
        p->privilege = 0;                                                      \
        p->branch = false;                                                     \
        p->address = 0;                                                        \
        p->ecause = 0;                                                         \
        p->interrupt = false;                                                  \
        p->tval = 0;                                                           \
    } while (false)

struct list_head *trdb_compress_trace(struct list_head *packet_list,
                                      struct instr_sample[1], size_t len);

char *trdb_decompress_trace(struct list_head *packet_list);

void dump_packet_list(struct list_head *packet_list);

void free_packet_list(struct list_head *packet_list);

/* inline uint32_t cause(struct instr_sample *instr) */
/* { */
/*     return instr->cause & MASK_FROM(CAUSELEN); */
/* } */

/* inline uint32_t tval(struct instr_sample *instr) */
/* { */
/*     return instr->tval & MASK_FROM(XLEN); */
/* } */

/* inline uint32_t priv(struct instr_sample *instr) */
/* { */
/*     return instr->priv & MASK_FROM(PRIVLEN); */
/* } */

/* inline uint32_t iaddr(struct instr_sample *instr) */
/* { */
/*     return instr->iaddr & 0xffffffff; */
/* } */

/* inline uint32_t instr(struct instr_sample *instr) */
/* { */
/*     return instr->instr & 0xffffffff; */
/* } */

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
