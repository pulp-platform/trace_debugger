/*
 * trdb - Trace Debugger Software for the PULP platform
 *
 * Copyright (C) 2018 Robert Balas
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
 * @file trdb_sv.h
 * @author Robert Balas (balasr@student.ethz.ch)
 * @date 15 Sep 2018
 * @brief Functions of the golden model which are called from the simulator
 */

#ifndef __TRDB_SV_H__
#define __TRDB_SV_H__

#include "svdpi.h"

/**
 * Allocate trdb library context. Ideally the simulator should hold the library
 * context handle but that functionality is still missing and so currently the
 * library does that.
 */
void trdb_sv_alloc();

/**
 * Configure address compression behaviour of trdb. Setting full_address to a
 * non-zero value disables address compression.
 */
void trdb_sv_set_full_address(int full_address);

/**
 * Run the trace compression algorithm step by step. Each call is equivalent to
 * advancing one cycle in the RTL model. Normally we should pass a struct but
 * here we just flattened it, which makes the parameter list a bit unwieldy.
 *
 * @param ivalid if the provided data is valid
 * @param iexception if this instruction is an exception
 * @param interrupt whether this is an interrupt or exception
 * @param cause interrupt/exception cause
 * @param tval trap value
 * @param instr the uncompressed version of the instruction
 * @param compressed whether the instruction was originally compressed
 * @param packet_max_len number of bits in @p packet_bits
 * @param packet_bits the bits representing a serialized packet
 * @param packet_valid whether we generated a packet this call/cycle
 */
void trdb_sv_feed_trace(svLogic ivalid, svLogic iexception, svLogic interrupt,
                        const svLogicVecVal *cause, const svLogicVecVal *tval,
                        const svLogicVecVal *priv, const svLogicVecVal *iaddr,
                        const svLogicVecVal *instr, svLogic compressed,
                        int packet_max_len, svLogicVecVal *packet_bits,
                        svLogic *packet_valid);

/**
 * Free all resources of allocated by trdb_sv_alloc.
 *
 */
void trdb_sv_free();

#endif
