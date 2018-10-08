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
 * @return 0 on success, -1 on failure
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
 * @return 0 on successful read, -1 on failure
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
 * @param 0 on succesfull read, -1 on failure
 */
int trdb_pulp_read_all_packets(struct trdb_ctx *c, const char *path,
                               struct list_head *packet_list);

/**
 * Serialize a single packet, like the PULP trace debugger.
 * @param c trace debugger context
 * @param packet data to serialize
 * @param fp file to write to
 * @return 0 on successfull write, -1 on failure
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
 * @return -1 on failure and 0 on success
 */
int trdb_write_packets(struct trdb_ctx *c, const char *path,
                       struct list_head *packet_list);
