// Copyright 2018 ETH Zurich and University of Bologna.
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
// Description: Generate packet on output and buffer


import trdb_pkg::*;


module trdb_packet_emitter
    (input logic                   clk_i,
     input logic                   rst_ni,
     input                         trdb_format_t packet_format_i,
     input                         trdb_subformat_t packet_subformat_i,
     input logic                   valid_i,

     input logic                   interrupt_i,
     input logic [CAUSELEN-1:0]    cause_i,
     input logic [XLEN-1:0]        tval_i,
     input logic [PRIVLEN-1:0]     priv_i,
     input logic [XLEN-1:0]        iaddr_i,
     input logic [CONTEXTLEN-1:0]  context_i,

     input logic [30:0]            branch_map_i,
     input logic [4:0]             branch_map_cnt_i,
     input logic                   branch_map_empty_i,
     input logic                   is_branch_i,

     output logic [PACKET_LEN-1:0] packet_bits_o, //TODO: adjust sizes
     output logic [6:0]            packet_len_o,
     output logic                  valid_o,
     input logic                   grant_i
     );

    logic [PACKET_LEN-1:0]         packet_bits;
    logic [PACKET_HEADER_LEN-1:0]  packet_len;
    logic                          packet_gen_valid;
    logic                          packet_fifo_not_full;

    logic                          clear_fifo;

    logic [4:0]                    branch_packet_off ;


    assign packet_gen_valid = valid_i;

    always_comb begin: branch_map_offset
        assert (branch_map_cnt_i < 31);

        if(branch_map_cnt_i < 1) begin
            branch_packet_off = 1;
        end else if(branch_map_cnt_i < 9) begin
            branch_packet_off = 9;
        end else if(branch_map_cnt_i < 17) begin
            branch_packet_off = 17;
        end else if(branch_map_cnt_i < 25) begin
            branch_packet_off = 25;
        end else begin
            branch_packet_off = 31;
        end
    end

    always_comb begin: set_packet_bits
        packet_bits      = '0;
        packet_len       = '0;

        // TODO: implement this
        assert (packet_format_i != F_BRANCH_DIFF);

        if(valid_i) begin

            // packet format
            packet_bits[1:0] = packet_format_i;

            case(packet_format_i)

            F_BRANCH_FULL: begin
                packet_bits[6:2] = branch_map_cnt_i;

                if(branch_packet_off == 1) begin
                    packet_bits[7+:XLEN] = {iaddr_i, branch_map_i[0]};
                    packet_len = 7 + 1 + XLEN;

                end else if(branch_packet_off == 9) begin
                    packet_bits[7+:9+XLEN] = {iaddr_i, branch_map_i[8:0]};
                    packet_len = 7 + 9 + XLEN;

                end else if(branch_packet_off == 17) begin
                    packet_bits[7+:17+XLEN] = {iaddr_i, branch_map_i[16:0]};
                    packet_len = 7 + 17 + XLEN;

                end else if(branch_packet_off == 25) begin
                    packet_bits[7+:25+XLEN] = {iaddr_i, branch_map_i[24:0]};
                    packet_len = 7 + 25 + XLEN;

                end else begin
                    packet_bits[7+:31+XLEN] = {iaddr_i, branch_map_i[30:0]};
                    packet_len = 7 + 31 + XLEN;
                end
            end

            F_BRANCH_DIFF: begin
                //TODO: variable len addr
            end

            F_ADDR_ONLY: begin
                packet_bits[2+:XLEN] = iaddr_i;
                packet_len           = 2 + XLEN; //TODO: variable len add
            end

            F_SYNC: begin
                // 2: subformat
                // PRIVLEN: privilege level
                // 1: is this instruction a branch
                // XLEN: address
                // CAUSELEN: exception cause
                // 1: is interrupt?
                packet_bits[2+:2+PRIVLEN+1+XLEN+CAUSELEN+1]
                    = {interrupt_i, cause_i, iaddr_i, is_branch_i, priv_i,
                       packet_subformat_i};
                packet_len = 2 + PRIVLEN + 1 + XLEN + CAUSELEN + 1;
            end
            endcase

        end
    end

    // always_comb begin:
    //     assert (packet_fifo_not_full == 1'b1)
    //         else $error("Packet FIFO is overflowing.");
    // end
    //TODO: implement fifo nuking logic
    //TODO: request resync logic on nuked fifo

    assign clear_fifo = 1'b0;

    generic_fifo_adv
        #(.DATA_WIDTH(PACKET_LEN + PACKET_HEADER_LEN),
          .DATA_DEPTH(PACKET_BUFFER_STAGES))
    i_packet_fifo
        (.clk(clk_i),
         .rst_n(rst_ni),
         .clear_i(clear_fifo), //nuke fifo if overflowing
         .data_i({packet_bits, packet_len}),
         .valid_i(packet_gen_valid),
         .grant_o(packet_fifo_not_full),
         .data_o({packet_bits_o, packet_len_o}),
         .valid_o(valid_o),
         .grant_i(grant_i),
         .test_mode_i('0));

endmodule // trdb_packet_emitter
