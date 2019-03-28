// Copyright 2018 Robert Balas
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the "License"); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.
//
// Author: Robert Balas (balasr@student.ethz.ch)
// Description: Take packets and funnel them through a 32 bit interface

import trdb_pkg::*;

// TODO: doesn't do alignment yet, just zero fill empty words
// TODO: inserting double WORD header not done yet
module trdb_stream_align8
    #(parameter ID = 1)
    (input logic                         clk_i,
     input logic                         rst_ni,

     input logic [PACKET_LEN-1:0]        packet_bits_i,
     input logic [PACKET_HEADER_LEN-1:0] packet_len_i,
     input logic                         valid_i,
     output logic                        grant_o,

     input logic                         flush_stream_i,
     output logic                        flush_confirm_o,

     output logic [31:0]                 data_o,
     output logic                        valid_o);


    enum logic [1:0] {WORD0, WORD1} cs, ns;

    logic [32+PACKET_TOTAL-1:0]       padded_packet_bits;
    logic [$clog2(32+PACKET_TOTAL):0] low_ptr_d, low_ptr_q, high_ptr;

    // TODO: change hardcoding, use $clog
    // TODO: remove PACKER_HEADER_LEN, move upwards to packet emitter
    // TODO: this is a mess, clean up
    logic [PACKET_BYTE_HEADER_LEN-1:0] packet_bytes_no_header_len;
    logic [PACKET_HEADER_LEN-1:0]      packet_len_no_header;
    logic [PACKET_HEADER_LEN-1+1:0]    effective_packet_len;
    logic [3:0]                        packet_bytes_len;

    logic [3:0]                       offset_q, offset_d;
    logic [3:0]                       offset_inv;
    logic [3+3:0]                     offset_shifted, offset_inv_shifted;
    // TODO: change residual size
    logic [31:0]                      data_q, data_d, residual_q, residual_d;
    logic                             valid_d, valid_q;

    // TODO: this is not used for now, meant for multi core tracing
    logic [4:0]                       instance_id;

    assign instance_id = ID;

    assign valid_o = valid_q;
    assign data_o = data_q;

    assign packet_len_no_header = packet_len_i - 4;
    assign packet_bytes_no_header_len = (packet_len_no_header >> 3)
        + (packet_len_no_header[2:0] != 2'h0);

    assign padded_packet_bits = {32'b0,
                                 packet_bits_i,
                                 packet_bytes_no_header_len};

    // TODO: halfword, byte, nibble, bit alignment
    assign offset_shifted = offset_q << 3;
    assign offset_inv = 4 - offset_q;
    assign offset_inv_shifted = offset_inv << 3;

    // packet length with header TODO: should rename other to payload
    assign effective_packet_len = packet_len_i + PACKET_BYTE_HEADER_LEN;
    // ceiling division
    assign packet_bytes_len = (effective_packet_len >> 3)
        + (effective_packet_len[2:0] != 2'h0);

    always_comb begin
        ns              = cs;
        low_ptr_d       = low_ptr_q;
        valid_d         = '0;
        data_d          = '0;
        grant_o         = '0;

        residual_d      = residual_q;
        offset_d        = offset_q;

        high_ptr        = '0;

        flush_confirm_o = '0;

        if(valid_i) begin
            high_ptr    = low_ptr_q + 32;
            // check if we can still output 32 bit words for the current
            // packet combine "carry" and current data, and save residual
            // for next word
            data_d      = (padded_packet_bits[low_ptr_q +: 32] << offset_shifted) | residual_q;
            residual_d  = (padded_packet_bits[low_ptr_q +: 32] >> offset_inv_shifted);

            valid_d     = '1;
            low_ptr_d   = high_ptr;

            if((high_ptr >> 3) == packet_bytes_len) begin
                // offset_d = offset_d;
                // residual_d = residual_d;
                valid_d   = '1;
                low_ptr_d = '0;
                grant_o   = '1;

            end else if((high_ptr >> 3) > packet_bytes_len) begin
                offset_d = ((packet_bytes_len % 4) + offset_q);

                // figure out if we can still produce a whole word or we should
                // wait for more data by saving it into residual_d
                if(offset_d == 4) begin
                    residual_d = '0;
                    valid_d = '1;
                end else if(offset_d > 4) begin
                    if((offset_d % 4) == 1) begin
                        residual_d = residual_d & 32'hff;
                    end else if ((offset_d % 4) == 2) begin
                        residual_d = residual_d & 32'hffff;
                    end else if ((offset_d % 4) == 3) begin
                        residual_d = residual_d & 32'hffffff;
                    end else
                        residual_d = residual_d;
                    valid_d        = '1;
                end else begin
                    if(offset_d == 1)
                        residual_d = data_d & 32'hff;
                    else if (offset_d == 2)
                        residual_d = data_d & 32'hffff;
                    else if (offset_d == 3)
                        residual_d = data_d & 32'hffffff;
                    else
                        residual_d     = data_d; //TODO: need to mask this
                    valid_d        = '0;
                end

                offset_d   = offset_d % 4;
                //we are done with the current packet
                low_ptr_d  = '0;
                // request the next packet
                grant_o    = '1;
            end

        end else if(flush_stream_i) begin
            // check if we actually have something to flush from residual_d
            flush_confirm_o = '1;
            if(offset_d != 0) begin
                data_d  = residual_d;
                valid_d = '1;
            end
            // TODO: after flush user has to clear
        end
    end


    always_ff @(posedge clk_i, negedge rst_ni) begin
        if(~rst_ni) begin
            cs              <= WORD0;
            offset_q        <= '0;
            low_ptr_q       <= '0;
            residual_q      <= '0;
            valid_q         <= '0;
            data_q          <= '0;
        end else begin
            cs              <= ns;
            offset_q        <= offset_d;
            low_ptr_q       <= low_ptr_d;
            residual_q      <= residual_d;
            valid_q         <= valid_d;
            data_q          <= data_d;
        end
    end

endmodule // trdb_stream_align8
