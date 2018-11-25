/**
 * This file is just a temporary workaround to run the test binary. We are
 * missing some struct definition, which makes pointer dereferencing invalid.
 * Ideally we would use accesor functions in test.c or use an internal header to
 * solve this issue.
 */

/* Configuration state of the trace debugger, used to guide the compression and
 * decompression routine
 */
struct trdb_config {
    /* TODO: Unused, inspect full-address, iaddress-lsb-p, implicit-except,
     * set-trace
     */
    uint64_t resync_max;
    /* bool iaddress_lsb_p; */
    /* bool implicit_except; */
    /* bool set_trace; */

    /* collect as much information as possible, but slower execution */
    bool full_statistics;

    /* set to true to always use absolute addresses in packets, this is used for
     * decompression and compression
     */
    bool full_address;
    /* do the sign extension of addresses like PULP. Only relevant if
     * full_address = false
     */
    bool use_pulp_sext;
    /* Don't regard ret's as unpredictable discontinuity */
    bool implicit_ret;
    /* Use additional packets to jump over vector table */
    bool pulp_vector_table_packet;
    /* whether we compress full branch maps */
    bool compress_full_branch_map;

    /* TODO: move this */
    /* set to true to always diassemble to most general representation */
    bool no_aliases;
};

struct trdb_stats {
    size_t payloadbits;
    size_t packetbits;
    size_t pulpbits;
    size_t instrbits;
    size_t instrs;
    size_t packets;
    size_t zo_addresses;  /**< all zero or all ones addresses */
    size_t zo_branchmaps; /**< all zero or all ones branchmaps */
    size_t addr_only_packets;
    size_t exception_packets;
    size_t start_packets;
    size_t diff_packets;
    size_t abs_packets;
    size_t bmap_full_packets;
    size_t bmap_full_addr_packets;
    uint32_t sext_bits[32];
};

struct trdb_ctx {
    /* specific settings for compression/decompression */
    struct trdb_config config;
    /* state used for compression */
    struct trdb_compress *cmp;
    /* state used for decompression */
    struct trdb_decompress *dec;
    /* state used for disassembling */
    struct tr_instr *dis_instr;
    struct disassembler_unit *dunit;
    /* compression statistics */
    struct trdb_stats stats;
    /* desired logging level and custom logging hook*/
    int log_priority;
    void (*log_fn)(struct trdb_ctx *ctx, int priority, const char *file,
                   int line, const char *fn, const char *format, va_list args);
};
