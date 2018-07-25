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
 * Description: Small tests for trdb
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "../trace_debugger.h"
#include "../trace_debugger.c"

#define RUN_TEST(fun)                                                          \
    do {                                                                       \
        printf("Running %s: ", #fun);                                          \
        if (fun())                                                             \
            printf("success\n");                                               \
        else                                                                   \
            printf("fail\n");                                                  \
    } while (false)

#define LOG_ERR(format, ...)                                                   \
    do {                                                                       \
        fprintf(stderr, "%s:%d:%s(): " format, __FILE__, __LINE__, __func__,   \
                ##__VA_ARGS__);                                                \
    } while (false)

bool test_packet_to_char()
{
    trdb_init();

    /* Testing F_BRANCH_FULL packet with full branch map */
    struct tr_packet packet = {.msg_type = 2,
                               .format = F_BRANCH_FULL,
                               .branches = 31,
                               .branch_map = 0x7fffffff,
                               .address = 0xaadeadbe};
    /*                    0xf2,7 + 1, 31-7-8, 31-7-8-8, 31-7-8-8-8*/
    uint8_t expected0[] = {0xf2, 0xff, 0xff, 0xff, 0xff,
                           0xbe, 0xad, 0xde, 0xaa};
    /* this is surely enough space */
    uint8_t *bin = malloc(sizeof(struct tr_packet));
    memset(bin, 0, sizeof(struct tr_packet));

    if (!bin) {
        perror("malloc");
        goto fail;
    }

    size_t bitcnt = 0;
    if (packet_to_char(&packet, &bitcnt, bin))
        goto fail;
    if (bitcnt != (2 + 2 + 5 + branch_map_len(packet.branches) + XLEN)) {
        LOG_ERR("Wrong bit count value: %zu\n", bitcnt);
        goto fail;
    }
    if (memcmp(bin, expected0, sizeof(expected0) / sizeof(expected0[0]))) {
        LOG_ERR("Packet bits don't match\n");
        goto fail;
    }

    /* Testing F_BRANCH_FULL packet with non-full branch map */
    packet = (struct tr_packet){.msg_type = 2,
                                .format = F_BRANCH_FULL,
                                .branches = 25,
                                .branch_map = 0x1ffffff,
                                .address = 0xaadeadbe};

    /*                           7     8     8      2 */
    uint8_t expected1[] = {0x92, 0xff, 0xff, 0xff, 0xfb, 0xb6, 0x7a, 0xab, 0x2};
    memset(bin, 0, sizeof(struct tr_packet));
    bitcnt = 0;

    if (packet_to_char(&packet, &bitcnt, bin))
        goto fail;
    if (bitcnt != (2 + 2 + 5 + branch_map_len(packet.branches) + XLEN)) {
        LOG_ERR("Wrong bit count value: %zu\n", bitcnt);
        goto fail;
    }
    if (memcmp(bin, expected1, sizeof(expected1) / sizeof(expected1[0]))) {
        LOG_ERR("Packet bits don't match\n");
        goto fail;
    }

    /* Testing F_ADDR_ONLY packet */
    packet = (struct tr_packet){
        .msg_type = 2, .format = F_ADDR_ONLY, .address = 0xdeadbeef};

    /*                           7     8     8      2 */
    uint8_t expected2[] = {0xfa, 0xee, 0xdb, 0xea, 0x0d};
    memset(bin, 0, sizeof(struct tr_packet));
    bitcnt = 0;

    if (packet_to_char(&packet, &bitcnt, bin))
        goto fail;
    if (bitcnt != (2 + 2 + XLEN)) {
        LOG_ERR("Wrong bit count value: %zu\n", bitcnt);
        goto fail;
    }
    if (memcmp(bin, expected2, sizeof(expected2) / sizeof(expected2[0]))) {
        LOG_ERR("Packet bits don't match\n");
        goto fail;
    }


    /* Testing F_SYNC start packet */
    packet = (struct tr_packet){.msg_type = 2,
                                .format = F_SYNC,
                                .subformat = SF_START,
                                .privilege = 3,
                                .branch = 1,
                                .address = 0xdeadbeef};

    /*                           7     8     8      2 */
    uint8_t expected3[] = {0xce, 0xf8, 0xee, 0xdb, 0xea, 0x0d};
    memset(bin, 0, sizeof(struct tr_packet));
    bitcnt = 0;

    if (packet_to_char(&packet, &bitcnt, bin))
        goto fail;
    if (bitcnt != (6 + PRIVLEN + 1 + XLEN)) {
        LOG_ERR("Wrong bit count value: %zu\n", bitcnt);
        goto fail;
    }
    if (memcmp(bin, expected3, sizeof(expected3) / sizeof(expected3[0]))) {
        LOG_ERR("Packet bits don't match\n");
        goto fail;
    }


    /* Testing F_SYNC exception packet */
    packet = (struct tr_packet){.msg_type = 2,
                                .format = F_SYNC,
                                .subformat = SF_EXCEPTION,
                                .privilege = 3,
                                .branch = 1,
                                .address = 0xdeadbeef,
                                .ecause = 0x1a,
                                .interrupt = 1,
                                .tval = 0xfeebdeed};

    /* 0x3fbaf7bb7adeadbeef8de */
    uint8_t expected4[] = {0xde, 0xf8, 0xee, 0xdb, 0xea, 0xad,
                           0xb7, 0x7b, 0xaf, 0xfb, 0x3};
    memset(bin, 0, sizeof(struct tr_packet));
    bitcnt = 0;

    if (packet_to_char(&packet, &bitcnt, bin))
        goto fail;
    if (bitcnt != (6 + PRIVLEN + 1 + XLEN + CAUSELEN + 1 + XLEN)) {
        LOG_ERR("Wrong bit count value: %zu\n", bitcnt);
        goto fail;
    }
    if (memcmp(bin, expected4, sizeof(expected4) / sizeof(expected4[0]))) {
        LOG_ERR("Packet bits don't match\n");
        goto fail;
    }

    free(bin);
    return true;

fail:
    free(bin);
    return false;
}

int main(int argc, char *argv[argc + 1])
{
    RUN_TEST(test_packet_to_char);
}
