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
#include <errno.h>
#include <unistd.h>
#include "../list.h"
#include "../trace_debugger.h"
#include "../trace_debugger.c"
#include "../disassembly.h"
#include "../disassembly.c"
#include "../utils.h"
#include "../utils.c"

#define TRDB_SUCCESS 0
#define TRDB_FAIL -1

#define INIT_TESTS() bool _trdb_test_result = true;

/* TODO: and-ing of test results for main return value */
#define RUN_TEST(fun, ...)                                                     \
    do {                                                                       \
        printf("Running %s: ", #fun);                                          \
        if (!fun(__VA_ARGS__)) {                                               \
            printf("success\n");                                               \
        } else {                                                               \
            printf("fail\n");                                                  \
            _trdb_test_result = false;                                         \
        }                                                                      \
    } while (false)

#define TESTS_SUCCESSFULL() (_trdb_test_result == true)

static void shiftl_array(uint8_t arr[], size_t len, uint32_t shift)
{
    if (shift >= 8) {
        LOG_ERRT("Shift value too large\n");
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
        LOG_ERRT("Non-zero carry after array shifting\n");
    return;
}


static int test_disasm_bfd()
{
    bfd *abfd = NULL;
    disassemble_info dinfo = {0};

    bfd_init();

    abfd = bfd_openr("data/interrupt", NULL);

    if (!(abfd && bfd_check_format(abfd, bfd_object)))
        return TRDB_FAIL;

    /* Override the stream the disassembler outputs to */
    init_disassemble_info(&dinfo, stdout, (fprintf_ftype)fprintf);
    dinfo.fprintf_func = (fprintf_ftype)fprintf;
    dinfo.print_address_func = riscv32_print_address;

    dinfo.flavour = bfd_get_flavour(abfd);
    dinfo.arch = bfd_get_arch(abfd);
    dinfo.mach = bfd_get_mach(abfd);
    dinfo.endian = abfd->xvec->byteorder;
    disassemble_init_for_target(&dinfo);

    /* Tests for disassembly functions */
    if (TRDB_VERBOSE_DEBUG) {
        dump_target_list();
        dump_bin_info(abfd);
    }

    /* set up disassembly context */
    struct disassembler_unit dunit = {0};
    dunit.dinfo = &dinfo;
    dunit.disassemble_fn = disassembler(abfd);
    if (!dunit.disassemble_fn) {
        LOG_ERRT("No suitable disassembler found\n");
        return TRDB_FAIL;
    }
    /* TODO: use this path also in relase mode, but less noisy */
    if (TRDB_VERBOSE_DEBUG) {
        dump_section_names(abfd);

        LOG_INFOT("num_sections: %d\n", bfd_count_sections(abfd));
        disassemble_single_instruction(0x10, 0, &dunit);
        bfd_map_over_sections(abfd, disassemble_section, &dunit);
    }
    bfd_close(abfd);
    return TRDB_SUCCESS;
}


static int test_trdb_serialize_packet(uint32_t shift)
{
    int status = TRDB_SUCCESS;
    struct trdb_ctx *c = trdb_new();
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
    shiftl_array(expected0, TRDB_ARRAY_SIZE(expected0), shift);

    /* this is surely enough space */
    uint8_t *bin = malloc(sizeof(struct tr_packet));
    memset(bin, 0, sizeof(struct tr_packet));

    if (!bin) {
        perror("malloc");
        status = TRDB_FAIL;
        goto fail;
    }

    size_t bitcnt = 0;
    if (trdb_serialize_packet(c, &packet, &bitcnt, shift, bin)) {
        LOG_ERRT("Packet conversion failed\n");
        status = TRDB_FAIL;
    }
    if (bitcnt != (2 + 2 + 5 + branch_map_len(packet.branches) + XLEN)) {
        LOG_ERRT("Wrong bit count value: %zu\n", bitcnt);
        status = TRDB_FAIL;
    }
    if (memcmp(bin, expected0, TRDB_ARRAY_SIZE(expected0))) {
        LOG_ERRT("Packet bits don't match\n");
        status = TRDB_FAIL;
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

    if (trdb_serialize_packet(c, &packet, &bitcnt, 0, bin)) {
        LOG_ERRT("Packet conversion failed\n");
        status = TRDB_FAIL;
    }
    if (bitcnt != (2 + 2 + 5 + branch_map_len(packet.branches) + XLEN)) {
        LOG_ERRT("Wrong bit count value: %zu\n", bitcnt);
        status = TRDB_FAIL;
    }
    if (memcmp(bin, expected1, TRDB_ARRAY_SIZE(expected1))) {
        LOG_ERRT("Packet bits don't match\n");
        status = TRDB_FAIL;
    }

    /* Testing F_ADDR_ONLY packet */
    packet = (struct tr_packet){
        .msg_type = 2, .format = F_ADDR_ONLY, .address = 0xdeadbeef};

    /*                           7     8     8      2 */
    uint8_t expected2[] = {0xfa, 0xee, 0xdb, 0xea, 0x0d, 0x00};
    memset(bin, 0, sizeof(struct tr_packet));
    bitcnt = 0;

    if (trdb_serialize_packet(c, &packet, &bitcnt, 0, bin)) {
        LOG_ERRT("Packet conversion failed\n");
        status = TRDB_FAIL;
    }
    if (bitcnt != (2 + 2 + XLEN)) {
        LOG_ERRT("Wrong bit count value: %zu\n", bitcnt);
        status = TRDB_FAIL;
    }
    if (memcmp(bin, expected2, TRDB_ARRAY_SIZE(expected2))) {
        LOG_ERRT("Packet bits don't match\n");
        status = TRDB_FAIL;
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

    if (trdb_serialize_packet(c, &packet, &bitcnt, 0, bin)) {
        LOG_ERRT("Packet conversion failed\n");
        status = TRDB_FAIL;
    }
    if (bitcnt != (6 + PRIVLEN + 1 + XLEN)) {
        LOG_ERRT("Wrong bit count value: %zu\n", bitcnt);
        status = TRDB_FAIL;
    }
    if (memcmp(bin, expected3, TRDB_ARRAY_SIZE(expected3))) {
        LOG_ERRT("Packet bits don't match\n");
        status = TRDB_FAIL;
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

    if (trdb_serialize_packet(c, &packet, &bitcnt, 0, bin)) {
        LOG_ERRT("Packet conversion failed\n");
        status = TRDB_FAIL;
    }
    if (bitcnt != (6 + PRIVLEN + 1 + XLEN + CAUSELEN + 1 + XLEN)) {
        LOG_ERRT("Wrong bit count value: %zu\n", bitcnt);
        status = TRDB_FAIL;
    }
    if (memcmp(bin, expected4, TRDB_ARRAY_SIZE(expected4))) {
        LOG_ERRT("Packet bits don't match\n");
        status = TRDB_FAIL;
    }

fail:
    trdb_free(c);
    free(bin);
    return status;
}

static int test_trdb_write_packets()
{
    int status = TRDB_SUCCESS;
    struct trdb_ctx *c = trdb_new();

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

    if (trdb_write_packets(c, "tmp0", &head)) {
        LOG_ERRT("trdb_write_packets failed\n");
        status = TRDB_FAIL;
    }

    /* Read back the file and compare to expected value */
    uint8_t *buf = NULL;
    FILE *fp = fopen("tmp0", "rb");
    if (!fp) {
        perror("fopen");
        status = TRDB_FAIL;
        goto fail;
    }

    long len = 0;
    if (!(buf = file_to_char(fp, &len))) {
        LOG_ERRT("file_to_char failed\n");
        status = TRDB_FAIL;
        goto fail;
    }
    /* compare values */
    uint8_t expected[] = {0xf2, 0xff, 0xff, 0xff, 0xff, 0xbe, 0xad, 0xde, 0xaa,
                          0xce, 0xf8, 0xee, 0xdb, 0xea, 0xed, 0x8d, 0xef, 0xbe,
                          0xad, 0xde, 0x7a, 0xbb, 0xf7, 0xba, 0x3f};
    if (memcmp(buf, expected, len)) {
        LOG_ERRT("Binary data mismatch\n");
        status = TRDB_FAIL;
    }
fail:
    if (fp)
        fclose(fp);
    remove("tmp0");
    trdb_free(c);
    free(buf);
    return status;
}


static int test_parse_stimuli_line()
{
    int valid = 0;
    int exception = 0;
    int interrupt = 0;
    uint32_t cause = 0;
    uint32_t tval = 0;
    uint32_t priv = 0;
    uint32_t iaddr = 0;
    uint32_t instr = 0;

    int ret = sscanf(
        "valid=1 exception=0 interrupt=0 cause=00 tval=ff priv=7 addr=1c00809c instr=ffff9317",
        "valid= %d exception= %d interrupt= %d cause= %" SCNx32
        " tval= %" SCNx32 " priv= %" SCNx32 " addr= %" SCNx32
        " instr= %" SCNx32,
        &valid, &exception, &interrupt, &cause, &tval, &priv, &iaddr, &instr);

    if (ret != EOF) {
    } else if (errno != 0) {
        perror("scanf");
        return TRDB_FAIL;
    } else {
        fprintf(stderr, "No matching characters\n");
        return TRDB_FAIL;
    }

    return (valid == 1 && exception == 0 && cause == 0 && tval == 0xff
            && iaddr == 0x1c00809c && instr == 0xffff9317)
               ? TRDB_SUCCESS
               : TRDB_FAIL;
}


static int test_stimuli_to_tr_instr(const char *path)
{
    struct trdb_ctx *c = trdb_new();
    struct tr_instr *tmp;
    struct tr_instr **samples = &tmp;
    int status = 0;
    trdb_stimuli_to_trace(c, path, samples, &status);
    if (status != 0) {
        LOG_ERRT("Stimuli to tr_instr failed\n");
        return TRDB_FAIL;
    }

    trdb_free(c);
    free(*samples);
    return TRDB_SUCCESS;
}


static int test_stimuli_to_packet_dump(const char *path)
{
    trdb_init(); // TODO: remove, need for trdb_compress_trace
    struct trdb_ctx *c = trdb_new();
    struct tr_instr *tmp;
    struct tr_instr **samples = &tmp;
    int status = 0;
    size_t samplecnt = trdb_stimuli_to_trace(c, path, samples, &status);
    if (status != 0) {
        LOG_ERRT("Stimuli to tr_instr failed\n");
        status = TRDB_FAIL;
        goto fail;
    }
    status = TRDB_SUCCESS;

    LIST_HEAD(head);
    struct list_head *ret = trdb_compress_trace(&head, samplecnt, *samples);
    if (!ret) {
        LOG_ERRT("Compress trace failed\n");
        status = TRDB_FAIL;
        goto fail;
    }
    if (TRDB_VERBOSE_DEBUG)
        trdb_dump_packet_list(stdout, &head);

fail:
    trdb_free(c);
    free(*samples);
    trdb_free_packet_list(&head);
    return status;
}


static int test_disassemble_trace(const char *bin_path, const char *trace_path)
{
    trdb_init(); // TODO: remove
    struct trdb_ctx *c = trdb_new();
    struct tr_instr *tmp;
    struct tr_instr **samples = &tmp;
    int status = 0;
    size_t samplecnt = trdb_stimuli_to_trace(c, trace_path, samples, &status);
    if (status != 0) {
        LOG_ERRT("Stimuli to tr_instr failed\n");
        return TRDB_FAIL;
    }

    status = TRDB_SUCCESS;

    bfd_init();
    bfd *abfd = bfd_openr(bin_path, NULL);

    if (!(abfd && bfd_check_format(abfd, bfd_object))) {
        bfd_perror("test_decompress_trace");
        status = TRDB_FAIL;
        goto fail;
    }

    struct disassembler_unit dunit = {0};
    struct disassemble_info dinfo = {0};
    dunit.dinfo = &dinfo;
    init_disassembler_unit(&dunit, abfd, NULL);

    if (TRDB_VERBOSE_DEBUG)
        trdb_disassemble_trace(samplecnt, *samples, &dunit);

fail:
    trdb_free(c);
    free(*samples);
    bfd_close(abfd);
    return status;
}


int test_compress_trace(const char *trace_path, const char *packets_path)
{
    trdb_init();

    struct trdb_ctx *ctx = NULL;

    FILE *expected_packets = NULL;
    FILE *tmp_fp0 = NULL;
    FILE *tmp_fp1 = NULL;

    char *compare = NULL;
    char *expected = NULL;

    struct tr_instr *tmp;
    struct tr_instr **samples = &tmp;
    int status = 0;
    size_t samplecnt = trdb_stimuli_to_trace(ctx, trace_path, samples, &status);
    if (status != 0) {
        LOG_ERRT("Stimuli to tr_instr failed\n");
        return TRDB_FAIL;
    }
    status = TRDB_SUCCESS;

    LIST_HEAD(packet1_head);
    LIST_HEAD(packet0_head);
    LIST_HEAD(instr_head);

    /* legacy do-it-all-at-once compression. Here for regression testing */
    struct list_head *ret =
        trdb_compress_trace(&packet0_head, samplecnt, *samples);
    if (!ret) {
        LOG_ERRT("Compress trace failed\n");
        status = TRDB_FAIL;
        goto fail;
    }

    ctx = trdb_new();
    if (!ctx) {
        LOG_ERRT("Library context allocation failed.\n");
        status = TRDB_FAIL;
        goto fail;
    }

    /* step by step compression */
    for (size_t i = 0; i < samplecnt; i++) {
        int step = trdb_compress_trace_step(ctx, &packet1_head, &(*samples)[i]);
        if (step == -1) {
            LOG_ERRT("Compress trace failed.\n");
            status = TRDB_FAIL;
            goto fail;
        }
    }

    expected_packets = fopen(packets_path, "r");
    if (!expected_packets) {
        perror("fopen");
        status = TRDB_FAIL;
        goto fail;
    }

    tmp_fp0 = fopen("tmp2", "w+");
    if (!tmp_fp0) {
        perror("fopen");
        status = TRDB_FAIL;
        goto fail;
    }
    trdb_dump_packet_list(tmp_fp0, &packet0_head);
    rewind(tmp_fp0);


    tmp_fp1 = fopen("tmp3", "w+");
    if (!tmp_fp1) {
        perror("fopen");
        status = TRDB_FAIL;
        goto fail;
    }
    trdb_dump_packet_list(tmp_fp1, &packet1_head);
    rewind(tmp_fp1);

    size_t linecnt = 0;
    size_t len = 0;
    ssize_t nread_compare;
    ssize_t nread_expected;

    /* test stepwise compression against expected response */
    while ((nread_expected = getline(&expected, &len, expected_packets))
           != -1) {
        linecnt++;
        nread_compare = getline(&compare, &len, tmp_fp1);
        if (nread_compare == -1) {
            LOG_ERRT(
                "Hit EOF too early in expected packets file, new compression\n");
            status = TRDB_FAIL;
            goto fail;
        }
        if (nread_expected != nread_compare
            || strncmp(compare, expected, nread_expected) != 0) {
            LOG_ERRT("Expected packets mismatch on line %zu\n", linecnt);
            LOG_ERRT("Expected: %s", expected);
            LOG_ERRT("Received: %s", compare);
            status = TRDB_FAIL;
            goto fail;
        }
    }

    linecnt = 0;
    /* test legacy against expected response */
    while ((nread_expected = getline(&expected, &len, expected_packets))
           != -1) {
        linecnt++;
        nread_compare = getline(&compare, &len, tmp_fp0);
        if (nread_compare == -1) {
            LOG_ERRT(
                "Hit EOF too early in expected packets file, legacy compression\n");
            status = TRDB_FAIL;
            goto fail;
        }
        if (nread_expected != nread_compare
            || strncmp(compare, expected, nread_expected) != 0) {
            LOG_ERRT("Expected packets mismatch on line %zu\n", linecnt);
            LOG_ERRT("Expected: %s", expected);
            LOG_ERRT("Received: %s", compare);
            status = TRDB_FAIL;
            goto fail;
        }
    }


fail:
    free(compare);
    free(expected);
    trdb_free(ctx);
    if (tmp_fp0)
        fclose(tmp_fp0);
    if (tmp_fp1)
        fclose(tmp_fp1);
    if (expected_packets)
        fclose(expected_packets);
    remove("tmp2");
    remove("tmp3");
    free(*samples);
    trdb_free_packet_list(&packet0_head);
    trdb_free_packet_list(&packet1_head);
    trdb_free_instr_list(&instr_head);
    trdb_close();
    return status;
}


int test_decompress_trace_legacy(const char *bin_path, const char *trace_path)
{
    trdb_init();
    struct trdb_ctx *ctx = NULL;

    bfd_init();
    bfd *abfd = bfd_openr(bin_path, NULL);

    if (!(abfd && bfd_check_format(abfd, bfd_object))) {
        bfd_perror("test_decompress_trace");
        return TRDB_FAIL;
    }

    struct tr_instr *tmp;
    struct tr_instr **samples = &tmp;
    int status = 0;
    size_t samplecnt = trdb_stimuli_to_trace(ctx, trace_path, samples, &status);
    if (status != 0) {
        LOG_ERRT("Stimuli to tr_instr failed\n");
        status = TRDB_FAIL;
        goto fail;
    }
    status = TRDB_SUCCESS;

    LIST_HEAD(packet0_head);
    LIST_HEAD(instr0_head);
    struct list_head *ret =
        trdb_compress_trace(&packet0_head, samplecnt, *samples);
    // trdb_compress_trace_legacy(&packet0_head, samplecnt, *samples);
    if (!ret) {
        LOG_ERRT("Compress trace failed\n");
        status = TRDB_FAIL;
        goto fail;
    }

    ctx = trdb_new();
    if (!ctx) {
        LOG_ERRT("Library context allocation failed.\n");
        status = TRDB_FAIL;
        goto fail;
    }


    if (TRDB_VERBOSE_DEBUG)
        trdb_dump_packet_list(stdout, &packet0_head);

    trdb_decompress_trace(ctx, abfd, &packet0_head, &instr0_head);

    if (TRDB_VERBOSE_DEBUG) {
        LOG_INFOT("Reconstructed trace disassembly:\n");
        struct tr_instr *instr;
        list_for_each_entry_reverse(instr, &instr0_head, list)
        {
            LOG_INFOT("%s", instr->str);
        }
    }

    /* We compare whether the reconstruction matches the original sequence, only
     * the pc for now. Legacy compression.
     */
    struct tr_instr *instr;
    int processedcnt = 0;
    int i = 0;
    list_for_each_entry_reverse(instr, &instr0_head, list)
    {
        /* skip all invalid instructions for the comparison */
        while (!(*samples)[i].valid || (*samples)[i].exception) {
            i++;
        }

        if (instr->iaddr != (*samples)[i].iaddr) {
            LOG_ERRT("%s", instr->str);
            LOG_ERRT("original instr: %" PRIx32 " @%" PRIx32 "\n",
                     (*samples)[i].instr, (*samples)[i].iaddr);
            LOG_ERRT("reconst. instr: %" PRIx32 " @%" PRIx32 "\n", instr->instr,
                     instr->iaddr);
            status = TRDB_FAIL;
            goto fail;
        }
        i++;
        processedcnt++;
    }
    LOG_INFOT("Compared %d instructions\n", processedcnt);

    if (list_empty(&instr0_head)) {
        LOG_ERRT("Empty instruction list.\n");
        return 0;
    }

fail:
    trdb_free(ctx);
    free(*samples);
    trdb_free_packet_list(&packet0_head);
    trdb_free_instr_list(&instr0_head);
    bfd_close(abfd);

    return status;
}


int test_decompress_trace(const char *bin_path, const char *trace_path)
{
    trdb_init();
    struct trdb_ctx *ctx = NULL;

    bfd_init();
    bfd *abfd = bfd_openr(bin_path, NULL);

    if (!(abfd && bfd_check_format(abfd, bfd_object))) {
        bfd_perror("test_decompress_trace");
        return TRDB_FAIL;
    }

    struct tr_instr *tmp;
    struct tr_instr **samples = &tmp;
    int status = 0;
    size_t samplecnt = trdb_stimuli_to_trace(ctx, trace_path, samples, &status);
    if (status != 0) {
        LOG_ERRT("Stimuli to tr_instr failed\n");
        status = TRDB_FAIL;
        goto fail;
    }
    status = TRDB_SUCCESS;

    LIST_HEAD(packet1_head);
    LIST_HEAD(instr1_head);

    ctx = trdb_new();
    if (!ctx) {
        LOG_ERRT("Library context allocation failed.\n");
        status = TRDB_FAIL;
        goto fail;
    }

    /* step by step compression */
    for (size_t i = 0; i < samplecnt; i++) {
        int step = trdb_compress_trace_step(ctx, &packet1_head, &(*samples)[i]);
        if (step == -1) {
            LOG_ERRT("Compress trace failed.\n");
            status = TRDB_FAIL;
            goto fail;
        }
    }

    if (TRDB_VERBOSE_DEBUG)
        trdb_dump_packet_list(stdout, &packet1_head);

    trdb_decompress_trace(ctx, abfd, &packet1_head, &instr1_head);

    if (TRDB_VERBOSE_DEBUG) {
        LOG_INFOT("Reconstructed trace disassembly:\n");
        struct tr_instr *instr;
        list_for_each_entry_reverse(instr, &instr1_head, list)
        {
            LOG_INFOT("%s", instr->str);
        }
    }

    /* We compare whether the reconstruction matches the original sequence, only
     * the pc for now. Legacy compression.
     */
    struct tr_instr *instr;
    int processedcnt = 0;
    int i = 0;
    list_for_each_entry_reverse(instr, &instr1_head, list)
    {
        /* skip all invalid instructions for the comparison */
        while (!(*samples)[i].valid || (*samples)[i].exception) {
            i++;
        }

        if (instr->iaddr != (*samples)[i].iaddr) {
            LOG_ERRT("%s", instr->str);
            LOG_ERRT("original instr: %" PRIx32 "\n", (*samples)[i].iaddr);
            LOG_ERRT("reconst. instr: %" PRIx32 "\n", instr->iaddr);
            status = TRDB_FAIL;
            goto fail;
        }
        i++;
        processedcnt++;
    }
    LOG_INFOT("Compared %d instructions\n", processedcnt);

    if (list_empty(&instr1_head)) {
        LOG_ERRT("Empty instruction list.\n");
        return 0;
    }

fail:
    trdb_free(ctx);
    free(*samples);
    trdb_free_packet_list(&packet1_head);
    trdb_free_instr_list(&instr1_head);
    bfd_close(abfd);

    return status;
}


int main(int argc, char *argv[argc + 1])
{
    const char *tv[] = {
        "data/interrupt",
        "data/trdb_stimuli",
        "data/trdb_stimuli_valid_only_bin",
        "data/trdb_stimuli_valid_only",
        "data/trdb_stimuli_all_bin",
        "data/trdb_stimuli_all",
        "data/hello/build/pulpissimo-riscy/test/test",
        "data/hello/build/pulpissimo-riscy/trdb_stimuli",
        "data/enqueue_delayed/build/pulpissimo-riscy/test/test",
        "data/enqueue_delayed/build/pulpissimo-riscy/trdb_stimuli",
        "data/timer_oneshot/build/pulpissimo-riscy/test/test",
        "data/timer_oneshot/build/pulpissimo-riscy/trdb_stimuli",
        "data/timer_oneshot/build/pulpissimo-riscy/test/test",
        "data/timer_oneshot/build/pulpissimo-riscy/trdb_stimuli",
        "data/uart_send/build/pulpissimo-riscy/test/test",
        "data/uart_send/build/pulpissimo-riscy/trdb_stimuli",
        "data/uart_loopback/build/pulpissimo-riscy/test/test",
        "data/uart_loopback/build/pulpissimo-riscy/trdb_stimuli",
	"data/coremark/build/pulpissimo-riscy/test/test",
        "data/coremark/build/pulpissimo-riscy/trdb_stimuli",

    };


    INIT_TESTS();
    /* for (size_t i = 0; i < 8; i++) */
    /*     RUN_TEST(test_trdb_serialize_packet, i); */

    RUN_TEST(test_disasm_bfd);
    /* RUN_TEST(test_trdb_write_packets); */
    RUN_TEST(test_parse_stimuli_line);
    RUN_TEST(test_stimuli_to_tr_instr, "data/trdb_stimuli");
    RUN_TEST(test_stimuli_to_packet_dump, "data/trdb_stimuli");
    /* NOTE: there is a memory leak ~230 bytes in riscv-dis.c with struct
     * riscv_subset for each instantiation of a disassembler.
     */
    RUN_TEST(test_disassemble_trace, "data/interrupt", "data/trdb_stimuli");
    RUN_TEST(test_compress_trace, "data/trdb_stimuli", "data/trdb_packets");

    if (TRDB_ARRAY_SIZE(tv) % 2 != 0)
        LOG_ERRT("Test vector strings are incomplete.");

    for (unsigned j = 0; j < TRDB_ARRAY_SIZE(tv); j += 2) {
        const char *bin = tv[j];
        const char *stim = tv[j + 1];
        if (access(bin, R_OK) || access(stim, R_OK)) {
            LOG_ERRT("File not found, skipping test at %s\n", bin);
            continue;
        }
        RUN_TEST(test_decompress_trace, bin, stim);
    }

    return TESTS_SUCCESSFULL() ? EXIT_SUCCESS : EXIT_FAILURE;
}
