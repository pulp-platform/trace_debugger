/*
 * t Debugger Software for the PULP platform
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

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "parse.h"
#include "list.h"
#include "trace_debugger.h"
#include "utils.h"


static uint32_t p_branch_map_len(uint32_t branches)
{
    if (branches <= 1) {
        return 1;
    } else if (branches <= 9) {
        return 9;
    } else if (branches <= 17) {
        return 17;
    } else if (branches <= 25) {
        return 25;
    } else if (branches <= 31) {
        return 31;
    }
    return -1;
}

int trdb_pulp_read_single_packet(struct trdb_ctx *c, FILE *fp,
                                 struct tr_packet *packet)
{
    uint8_t header = 0;
    union trdb_pack payload = {0};
    if (!fp) {
        err(c, "fp is null\n");
        return -EINVAL;
    }
    if (fread(&header, 1, 1, fp) != 1) {
        if (feof(fp)) {
            return -1;
        } else if (ferror(fp)) {
            err(c, "ferror: %s\n", strerror(errno));
            return -1;
        }
        return -1;
    }

    /* read packet length it bits (including header) */
    uint8_t len = (header & MASK_FROM(PACKETLEN)) + PACKETLEN;
    payload.bin[0] = header;
    /* compute how many bytes that is */
    unsigned byte_len = len / 8 + (len % 8 != 0 ? 1 : 0);
    /* we have to exclude the header byte */
    if (fread((payload.bin + 1), 1, byte_len - 1, fp) != byte_len - 1) {
        if (feof(fp)) {
            err(c, "incomplete packet read\n");
            return -1;
        } else if (ferror(fp)) {
            err(c, "ferror: %s\n", strerror(errno));
            return -1;
        }
        return -1;
    }
    /* make sure we start from a good state */
    *packet = (struct tr_packet){0};

    packet->length = payload.bits & MASK_FROM(PACKETLEN);
    packet->msg_type = (payload.bits >>= PACKETLEN) & MASK_FROM(MSGTYPELEN);
    packet->format = (payload.bits >>= MSGTYPELEN) & MASK_FROM(FORMATLEN);
    payload.bits >>= FORMATLEN;

    switch (packet->format) {
    case F_BRANCH_FULL:
        packet->branches = payload.bits & MASK_FROM(BRANCHLEN);
        uint32_t len = p_branch_map_len(packet->branches);
        packet->branch_map = (payload.bits >>= BRANCHLEN) & MASK_FROM(len);
        packet->address = (payload.bits >>= len) & MASK_FROM(XLEN);
        return 0;
    case F_BRANCH_DIFF:
        err(c, "not implemented yet\n");
        return -1;
    case F_ADDR_ONLY:
        packet->address = payload.bits & MASK_FROM(XLEN);
        return 0;
    case F_SYNC:
        packet->subformat = payload.bits & MASK_FROM(FORMATLEN);
        packet->privilege = (payload.bits >>= FORMATLEN) & MASK_FROM(PRIVLEN);
        if (packet->subformat == SF_CONTEXT) {
            err(c, "not implemented\n");
            return -1;
        }
        packet->branch = (payload.bits >>= PRIVLEN) & 1;
        packet->address = (payload.bits >>= 1) & MASK_FROM(XLEN);
        if (packet->subformat == SF_START)
            return 0;
        packet->ecause = (payload.bits >>= XLEN) & MASK_FROM(CAUSELEN);
        packet->interrupt = (payload.bits >>= CAUSELEN) & 1;
        if (packet->subformat == SF_EXCEPTION)
            return 0;
    }
    return -1;
}


int trdb_pulp_read_all_packets(struct trdb_ctx *c, const char *path,
                               struct list_head *packet_list)
{
    FILE *fp = fopen(path, "rb");
    if (!fp) {
        err(c, "fopen: %s\n", strerror(errno));
        return -1;
    }

    struct tr_packet *packet = NULL;
    struct tr_packet tmp = {0};

    /* read the file and malloc entries into the given linked list head */
    while (trdb_pulp_read_single_packet(c, fp, &tmp) != -1) {
        packet = malloc(sizeof(*packet));
        if (!packet) {
            err(c, "malloc: %s\n", strerror(errno));
            return -ENOMEM;
        }
        *packet = tmp;

        list_add(&packet->list, packet_list);
    }
    return 0;
}


int trdb_pulp_write_single_packet(struct trdb_ctx *c, struct tr_packet *packet,
                                  FILE *fp)
{
    size_t bitcnt = 0;
    size_t bytecnt = 0;
    uint8_t bin[16] = {0};
    if (!fp) {
        err(c, "bad file pointer\n");
        return -1;
    }
    if (trdb_serialize_packet(c, packet, &bitcnt, 0, bin)) {
        err(c, "failed to serialize packet\n");
        return -1;
    }
    bytecnt = bitcnt / 8 + (bitcnt % 8 != 0);
    if (fwrite(bin, 1, bytecnt, fp) != bytecnt) {
        if (feof(fp)) {
	    /* TODO: uhhh */
        } else if (ferror(fp)) {
            err(c, "ferror: %s\n", strerror(errno));
            return -1;
        }
    }
    return 0;
}
