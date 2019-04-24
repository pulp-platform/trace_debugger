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
module trdb_align
    #(parameter ID = 1)
    (input logic                          clk_i,
     input logic                          rst_ni,

     input logic [PACKET_LEN-1:0]         packet_bits_i,
     input logic [$clog2(PACKET_LEN)-1:0] packet_len_i,
     input logic                          valid_i,
     output logic                         grant_o,

     output logic [31:0]                  data_o,
     output logic                         valid_o);


    enum logic [1:0] {WORD0, WORD1} cs, ns;

    logic [32+PACKET_TOTAL-1:0]       padded_packet_bits;
    logic [$clog2(32+PACKET_TOTAL):0] low_ptr_d, low_ptr_q, high_ptr;

    logic [31:0]                      data_q, data_d, residual_q, residual_d;
    logic [4:0]                       residual_fill_q, residual_fill_d;
    logic                             valid_d, valid_q;

    // TODO: this is not used for now, meant for multi core tracing
    logic [4:0]                       instance_id;

    assign instance_id = ID;

    assign valid_o = valid_q;
    assign data_o = data_q;
    assign padded_packet_bits = {32'b0, packet_bits_i, packet_len_i};

    // TODO: halfword, byte, nibble, bit alignment
    // TODO: flush capabilities
    // TODO: source tag
    // TODO: word tag

    always_comb begin
        ns         = cs;
        low_ptr_d  = low_ptr_q;
        valid_d    = '0;
        data_d     = '0;
        grant_o    = '0;

        if(valid_i) begin
            case(cs)

            WORD0: begin
                ns       = WORD1;
                high_ptr = low_ptr_q + 32-7;
                data_d   = {padded_packet_bits[low_ptr_q +: 32-7], 2'b1,
                           instance_id};
                valid_d = '1;

                low_ptr_d = high_ptr;
                if(high_ptr >= packet_len_i) begin
                    //we are done with the current packet
                    low_ptr_d = '0;
                    // request the next packet
                    grant_o   = '1;
                end
            end

            WORD1: begin
                ns        = WORD0;
                high_ptr  = low_ptr_q + 32;
                // check if we can still output 32 bit words for the current
                // packet
                data_d    = padded_packet_bits[low_ptr_q +: 32];
                valid_d   = '1;

                low_ptr_d = high_ptr;
                // TODO: make this state machine maybe
                if(high_ptr >= packet_len_i) begin
                    //we are done with the current packet
                    low_ptr_d = '0;
                    // request the next packet
                    grant_o   = '1; //TODO: doesn't cause comb loop with fifo?
                end
            end

            endcase
        end
    end


    always_ff @(posedge clk_i, negedge rst_ni) begin
        if(~rst_ni) begin
            cs              <= WORD0;
            low_ptr_q       <= '0;
            residual_q      <= '0;
            residual_fill_q <= '0;
            valid_q         <= '0;
            data_q          <= '0;
        end else begin
            cs              <= ns;
            low_ptr_q       <= low_ptr_d;
            residual_q      <= residual_d;
            residual_fill_q <= residual_fill_d;
            valid_q         <= valid_d;
            data_q          <= data_d;
        end
    end

endmodule // trdb_align
