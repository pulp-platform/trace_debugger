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
 * @file serialize.h
 * @author Robert Balas (balasr@student.ethz.ch)
 * @date 25 Sep 2018
 * @brief Read binary data into packets
 */

#include "trace_debugger.h"

/**
 * Packs the @p packet into an array @p bin, aligned by @p align and writes the
 * packet length in bits into @p bitcnt. This function is specific to the PULP
 * platform since it omits certain fields such as "context", which are always
 * set to zero.
 *
 * @param c a trace debugger context
 * @param packet the data to serialize
 * @param bitcnt written with the number of bits in @p packet
 * @param align alignment of packet bits in @p bin, larger or equal to zero and
 * smaller than eight.
 * @param bin the array where the packet bits will be written into
 * @return 0 on success, a negative error code otherwise.
 * @return -trdb_invalid if @p align >= 8
 * @return -trdb_bad_packet if packet is longer than 16 bytes or unknown
 */
int trdb_pulp_serialize_packet(struct trdb_ctx *c, struct tr_packet *packet,
                               size_t *bitcnt, uint8_t align, uint8_t bin[]);

/**
 * Read a packet from @p fp stored in the PULP binary format.
 *
 * @param c trace debugger context
 * @param fp file to read from
 * @param packet filled out with read packet data
 * @param bytes the number of bytes of the packet (with header)
 * @return 0 on success, a negative error code otherwise
 * @return -trdb_invalid if @p c, @p fp or @p packet is NULL
 * @return -trdb_bad_packet if only an incomplete packet could be read from @p
 * fp or none at all
 * @return -trdb_file_read if the contents of @fp could not be fully read
 * @return -trdb_bad_config if the decoding assumptions don't hold because of
 * contradictionary data in @p packet
 */
int trdb_pulp_read_single_packet(struct trdb_ctx *c, FILE *fp,
                                 struct tr_packet *packet, uint32_t *bytes);

/**
 * Repeatedly calls trdb_pulp_read_single_packet until encountering EOF or an
 * incomplete packet. The incomplete packet is just ignored and all the previous
 * good packets are kept.
 *
 * @param c trace debugger context
 * @param path file from which to read packet data
 * @param packet_list appened with read packets
 * @return 0 on success, a negative error code otherwise
 * @return -trdb_invalid if @p c, @p path or @p packet_list is NULL
 * @return -trdb_file_open if file at @p path could not be found
 * @return -trdb_nomem if out of memory
 */
int trdb_pulp_read_all_packets(struct trdb_ctx *c, const char *path,
                               struct list_head *packet_list);

/**
 * Serialize a single packet, like the PULP trace debugger.
 * @param c trace debugger context
 * @param packet data to serialize
 * @param fp file to write to
 * @return 0 on success, a negative error code otherwise
 * @return -trdb_invalid if @p c, @p fp or @p packet is NULL
 * @return -trdb_file_write if @p fp could not be fully written
 * @return -trdb_bad_packet if @p packet is malformed
 */
int trdb_pulp_write_single_packet(struct trdb_ctx *c, struct tr_packet *packet,
                                  FILE *fp);

/**
 * Write a list of tr_packets to a file located at @p path.
 *
 * This function calls trdb_serialize_packet() to tightly align the packets,
 * which can have a non-power-of-two size.
 *
 * @param c the context/state of the trace debugger
 * @param path where the file is located at
 * @param packet_list list of packets to write
 * @return 0 on success, a negative error code otherwise
 * @return -trdb_invalid if @p c, @p path or @p packet_list is NULL
 * @return -trdb_file_open if file at @p path could not be full read
 * @return -trdb_file_write if file at @p path could not be fully written
 */
int trdb_write_packets(struct trdb_ctx *c, const char *path,
                       struct list_head *packet_list);

/**
 * Read a stimuli file at @p path into a list of tr_instr. This function
 * allocates the struts in a sequential fashion.
 *
 * @param c the context/state of the trace debugger
 * @param path where the stimuli file is located at
 * @param instrs list anchor where to append the tr_instr to
 * @param count written with the number of produced tr_instr.
 * @return 0 on success, a negative error code otherwise
 * @return -trdb_invalid if @p c, @p path or @p instrs is NULL
 * @return -trdb_file_open if file at @p path could not be opened
 * @return -trdb_nomem if out of memory
 * @return -trdb_scan_file if parsing the file at @p failed
 */
int trdb_stimuli_to_trace_list(struct trdb_ctx *c, const char *path,
                               struct list_head *instrs, size_t *count);

/**
 * Read a stimuli file at @p path into an array of tr_instr.
 *
 * @param c the context/state of the trace debugger
 * @param path where the stimuli file is located at
 * @param samples where to write the read array of tr_instr
 * @param count written with the number of produced tr_instr
 * @return 0 on success, a negative error code otherwise
 * @return -trdb_invalid if @p c, @p path or @p instrs is NULL
 * @return -trdb_file_open if file at @p path could not be opened
 * @return -trdb_nomem if out of memory
 * @return -trdb_scan_file if parsing the file at @p failed
 */
int trdb_stimuli_to_trace(struct trdb_ctx *c, const char *path,
                          struct tr_instr **samples, size_t *count);

/**
 * Load cvs file, where each line represents an input vector to the trace
 * debugger, into a list of tr_instr.
 *
 * @param c the context/state of the trace debugger
 * @param path where the cvs file is located at
 * @param instrs list anchor where to append the tr_instr to
 * @param count written with the number of produced tr_instr
 * @return 0 on success, a negative error code otherwise
 * @return -trdb_error_code.trdb_invalid if @p path or @p instrs is NULL
 * @return -trdb_file_open if file at @p path could not be opened
 * @return -trdb_bad_cvs_header if first line of file at @p path does not match
 * expected value
 * @return -trdb_nomem if out of memory
 * @return -trdb_scan_state_invalid if scanning comma seperated values fails
 * @return -trdb_scan_file if parsing the file at @p failed
 */
int trdb_cvs_to_trace_list(struct trdb_ctx *c, const char *path,
                           struct list_head *instrs, size_t *count);
