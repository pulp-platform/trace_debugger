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
#include "../util.h"
#include "../util.c"

#define RUN_TEST(fun, ...)                                                     \
    do {                                                                       \
        printf("Running %s: ", #fun);                                          \
        if (fun(__VA_ARGS__))                                                  \
            printf("success\n");                                               \
        else                                                                   \
            printf("fail\n");                                                  \
    } while (false)


static void shiftl_array(uint8_t arr[], size_t len, uint32_t shift)
{
    if (shift >= 8) {
        LOG_ERR("Shift value too large\n");
        return;
    }
    uint32_t carry_in = 0;
    uint32_t carry_out = 0;
    for (size_t i = 0; i < len; i++) {
        /* carry_out = arr[i] & ~((1ull << (8 - shift)) - 1); */
        carry_out = (arr[i] >> (8 - shift)) & MASK_FROM(shift);
        arr[i] <<= shift;
        arr[i] |= carry_in;
        carry_in = carry_out;
    }
    if (carry_out)
        LOG_ERR("Non-zero carry after array shifting\n");
    return;
}


bool test_packet_to_char(uint32_t shift)
{
    bool status = true;
    trdb_init();
    struct tr_packet packet = {0};

    /* Testing F_BRANCH_FULL packet with full branch map */
    packet = (struct tr_packet){.msg_type = 2,
                                .format = F_BRANCH_FULL,
                                .branches = 31,
                                .branch_map = 0x7fffffff,
                                .address = 0xaadeadbe};
    /*                    0xf2,7 + 1, 31-7-8, 31-7-8-8, 31-7-8-8-8*/
    uint8_t expected0[] = {0xf2, 0xff, 0xff, 0xff, 0xff,
                           0xbe, 0xad, 0xde, 0xaa, 0x00};
    shiftl_array(expected0, ARRAY_SIZE(expected0), shift);

    for (size_t i; i < sizeof(expected0); i++) {
        /* printf("test: %" PRIx8 "\n", expected0[i]); */
    }
    /* this is surely enough space */
    uint8_t *bin = malloc(sizeof(struct tr_packet));
    memset(bin, 0, sizeof(struct tr_packet));

    if (!bin) {
        perror("malloc");
        status = false;
        goto fail;
    }

    size_t bitcnt = 0;
    if (packet_to_char(&packet, &bitcnt, shift, bin)) {
        LOG_ERR("Packet conversion failed\n");
        status = false;
    }
    if (bitcnt != (2 + 2 + 5 + branch_map_len(packet.branches) + XLEN)) {
        LOG_ERR("Wrong bit count value: %zu\n", bitcnt);
        status = false;
    }
    if (memcmp(bin, expected0, ARRAY_SIZE(expected0))) {
        LOG_ERR("Packet bits don't match\n");
        status = false;
    }


    /* Testing F_BRANCH_FULL packet with non-full branch map */
    packet = (struct tr_packet){.msg_type = 2,
                                .format = F_BRANCH_FULL,
                                .branches = 25,
                                .branch_map = 0x1ffffff,
                                .address = 0xaadeadbe};

    /*                           7     8     8      2 */
    uint8_t expected1[] = {0x92, 0xff, 0xff, 0xff, 0xfb,
                           0xb6, 0x7a, 0xab, 0x2,  0x00};
    memset(bin, 0, sizeof(struct tr_packet));
    bitcnt = 0;

    if (packet_to_char(&packet, &bitcnt, 0, bin)) {
        LOG_ERR("Packet conversion failed\n");
        status = false;
    }
    if (bitcnt != (2 + 2 + 5 + branch_map_len(packet.branches) + XLEN)) {
        LOG_ERR("Wrong bit count value: %zu\n", bitcnt);
        status = false;
    }
    if (memcmp(bin, expected1, ARRAY_SIZE(expected1))) {
        LOG_ERR("Packet bits don't match\n");
        status = false;
    }

    /* Testing F_ADDR_ONLY packet */
    packet = (struct tr_packet){
        .msg_type = 2, .format = F_ADDR_ONLY, .address = 0xdeadbeef};

    /*                           7     8     8      2 */
    uint8_t expected2[] = {0xfa, 0xee, 0xdb, 0xea, 0x0d, 0x00};
    memset(bin, 0, sizeof(struct tr_packet));
    bitcnt = 0;

    if (packet_to_char(&packet, &bitcnt, 0, bin)) {
        LOG_ERR("Packet conversion failed\n");
        status = false;
    }
    if (bitcnt != (2 + 2 + XLEN)) {
        LOG_ERR("Wrong bit count value: %zu\n", bitcnt);
        status = false;
    }
    if (memcmp(bin, expected2, ARRAY_SIZE(expected2))) {
        LOG_ERR("Packet bits don't match\n");
        status = false;
    }


    /* Testing F_SYNC start packet */
    packet = (struct tr_packet){.msg_type = 2,
                                .format = F_SYNC,
                                .subformat = SF_START,
                                .privilege = 3,
                                .branch = 1,
                                .address = 0xdeadbeef};

    /*                           7     8     8      2 */
    uint8_t expected3[] = {0xce, 0xf8, 0xee, 0xdb, 0xea, 0x0d, 0x00};
    memset(bin, 0, sizeof(struct tr_packet));
    bitcnt = 0;

    if (packet_to_char(&packet, &bitcnt, 0, bin)) {
        LOG_ERR("Packet conversion failed\n");
        status = false;
    }
    if (bitcnt != (6 + PRIVLEN + 1 + XLEN)) {
        LOG_ERR("Wrong bit count value: %zu\n", bitcnt);
        status = false;
    }
    if (memcmp(bin, expected3, ARRAY_SIZE(expected3))) {
        LOG_ERR("Packet bits don't match\n");
        status = false;
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
                           0xb7, 0x7b, 0xaf, 0xfb, 0x3,  0x00};
    memset(bin, 0, sizeof(struct tr_packet));
    bitcnt = 0;

    if (packet_to_char(&packet, &bitcnt, 0, bin)) {
        LOG_ERR("Packet conversion failed\n");
        status = false;
    }
    if (bitcnt != (6 + PRIVLEN + 1 + XLEN + CAUSELEN + 1 + XLEN)) {
        LOG_ERR("Wrong bit count value: %zu\n", bitcnt);
        status = false;
    }
    if (memcmp(bin, expected4, ARRAY_SIZE(expected4))) {
        LOG_ERR("Packet bits don't match\n");
        status = false;
    }

fail:
    free(bin);
    return status;
}

static bool test_trdb_write_trace()
{
    bool status = true;
    trdb_init();

    struct tr_packet packet0 = {0};
    struct tr_packet packet1 = {0};
    struct tr_packet packet2 = {0};

    packet0 = (struct tr_packet){.msg_type = 2,
                                 .format = F_BRANCH_FULL,
                                 .branches = 31,
                                 .branch_map = 0x7fffffff,
                                 .address = 0xaadeadbe};

    packet1 = (struct tr_packet){.msg_type = 2,
                                 .format = F_SYNC,
                                 .subformat = SF_START,
                                 .privilege = 3,
                                 .branch = 1,
                                 .address = 0xdeadbeef};

    packet2 = (struct tr_packet){.msg_type = 2,
                                 .format = F_SYNC,
                                 .subformat = SF_EXCEPTION,
                                 .privilege = 3,
                                 .branch = 1,
                                 .address = 0xdeadbeef,
                                 .ecause = 0x1a,
                                 .interrupt = 1,
                                 .tval = 0xfeebdeed};
    LIST_HEAD(head);
    list_add(&packet2.list, &head);
    list_add(&packet1.list, &head);
    list_add(&packet0.list, &head);

    if (trdb_write_trace("tmp_for_test", &head)) {
        LOG_ERR("trdb_write_trace failed\n");
        status = false;
    }

    /* Read back the file and compare to expected value */
    FILE *fp = fopen("tmp_for_test", "rb");
    if (!fp) {
        perror("fopen");
        status = -1;
        goto fail;
    }

    long len = 0;
    uint8_t *buf = NULL;
    if (!(buf = file_to_char(fp, &len))) {
        LOG_ERR("file_to_char failed\n");
        status = -1;
        goto fail;
    }
    /* compare values */
    uint8_t expected[] = {0xf2, 0xff, 0xff, 0xff, 0xff, 0xbe, 0xad, 0xde, 0xaa,
                          0xce, 0xf8, 0xee, 0xdb, 0xea, 0xed, 0x8d, 0xef, 0xbe,
                          0xad, 0xde, 0x7a, 0xbb, 0xf7, 0xba, 0x3f};
    if (memcmp(buf, expected, len)) {
        LOG_ERR("Binary data mismatch\n");
        status = -1;
    }

fail:
    if (fp)
        fclose(fp);
    free(buf);
    return status;
}

int main(int argc, char *argv[argc + 1])
{
    for (size_t i = 0; i < 8; i++)
        RUN_TEST(test_packet_to_char, i);

    RUN_TEST(test_trdb_write_trace);
}
