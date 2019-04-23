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
// Description: Generate packet on output and buffer


import trdb_pkg::*;


module trdb_packet_emitter
    (input logic                   clk_i,
     input logic                    rst_ni,

     // trace packet generation request with associated conditions
     input trdb_format_e            packet_format_i,
     input trdb_subformat_e         packet_subformat_i,

     input logic [$clog2(XLEN):0]   keep_bits_i,
     input logic                    valid_i,

     input logic                    use_full_addr_i,

     input logic                    interrupt_i,
     input logic [CAUSELEN-1:0]     cause_i,
     input logic [XLEN-1:0]         tval_i,
     input logic [PRIVLEN-1:0]      priv_i,
     input logic [XLEN-1:0]         iaddr_i,
     input logic [CONTEXTLEN-1:0]   context_i,

     input logic                    lc_u_discontinuity_i,

     input logic [30:0]             branch_map_i,
     input logic [4:0]              branch_map_cnt_i,
     input logic                    branch_map_empty_i,
     input logic                    branch_map_full_i,
     input logic                    is_branch_i,

     // command the branch map to flush when we generated a packet
     output logic                   branch_map_flush_o,

     // information for trdb_priority to decide what packet type we need
     output logic [XLEN-1:0]        diff_addr_o,

     // software packet generation request (dumping a write to stream)
     input logic                    sw_valid_i,
     input logic [XLEN-1:0]         sw_word_i,
     output logic                   sw_grant_o,

     // timer packet generation request
     input logic                    tu_valid_i,
     input logic [TIMER_WIDTH-1:0]  tu_time_i,
     input logic                    tu_fulltime_i,
     output logic                   tu_grant_o,

     // packet bits which were generated from request
     output logic [PACKET_LEN-1:0]  packet_bits_o, //TODO: adjust sizes
     output logic [6:0]             packet_len_o,
     output logic                   packet_valid_o);

    logic [PACKET_LEN-1:0]         packet_bits;
    logic [$clog2(PACKET_LEN)-1:0] packet_len;
    logic                          packet_gen_valid;

    // request to flush the branch map so that it is empty for the next packet
    logic                          branch_map_flush_q, branch_map_flush_d;

    // needed for the calculation of the length of the branch map len field
    logic [4:0]                    branch_packet_off ;

    // to handle the edge case where we need to store an address in a full
    // branch map
    logic                          branch_map_edge_case;

    // compute and handle differential addresses
    logic [XLEN-1:0]               caddr;
    logic [XLEN-1:0]               diff_addr;
    logic [XLEN-1:0]               last_addr_q, last_addr_d;

    assign branch_map_flush_o = branch_map_flush_q;

    // quantize the length of the branch map to the fixed values below
    always_comb begin: branch_map_offset
        if(branch_map_cnt_i == 0) begin
            branch_packet_off = 31;
        end else if(branch_map_cnt_i <= 1) begin
            branch_packet_off = 1;
        end else if(branch_map_cnt_i <= 9) begin
            branch_packet_off = 9;
        end else if(branch_map_cnt_i <= 17) begin
            branch_packet_off = 17;
        end else if(branch_map_cnt_i <= 25) begin
            branch_packet_off = 25;
        end else begin
            branch_packet_off = 31;
        end
    end

`ifndef SYNTHESIS
    branch_map_size_check: assert property
    (@(posedge clk_i) disable iff (~rst_ni) (branch_map_cnt_i < 32))
        else $error("[TRDB]    @%t: branch_map_cnt_i=%d is too large",
                    $time, branch_map_cnt_i);
`endif

    assign diff_addr = last_addr_q - iaddr_i;

    assign diff_addr_o = diff_addr;

    always_comb begin: compress_address
       caddr = (use_full_addr_i || packet_format_i == F_BRANCH_FULL) ?
               iaddr_i : diff_addr;
    end


    always_comb begin: set_packet_bits
        packet_bits          = '0;
        packet_len           = '0;
        branch_map_flush_d   = '0;

        branch_map_edge_case = lc_u_discontinuity_i;
        packet_gen_valid     = '0;

        sw_grant_o           = '0;
        tu_grant_o           = '0;
        last_addr_d          = last_addr_q;

        if(valid_i) begin
            // remember full address in this packet for next packet
            last_addr_d          = iaddr_i;

            packet_gen_valid   = '1;
            packet_bits[1:0]   = W_TRACE;

            // packet format
            packet_bits[3:2]   = packet_format_i;

            // always flush branchmap
            branch_map_flush_d = '1;

            case(packet_format_i)

            F_BRANCH_FULL, F_BRANCH_DIFF: begin
                // We shouldn't have an empty branch map if we want to generate
                // this packet TODO: there is an issue since branch_map_flush_d
                // immediately changes branch_map_cnt_i
                //  assert(branch_map_cnt_i != 0);

                packet_bits[8:4]   = branch_map_cnt_i;
                branch_map_flush_d = '1;

                if(branch_packet_off == 1) begin
                    packet_bits[9+:1+XLEN] = {caddr, branch_map_i[0]};
                    packet_len = 2 + FORMATLEN + BRANCHLEN + 1 + keep_bits_i;

                end else if(branch_packet_off == 9) begin
                    packet_bits[9+:9+XLEN] = {caddr, branch_map_i[8:0]};
                    packet_len = 2 + FORMATLEN + BRANCHLEN + 9 + keep_bits_i;

                end else if(branch_packet_off == 17) begin
                    packet_bits[9+:17+XLEN] = {caddr, branch_map_i[16:0]};
                    packet_len = 2 + FORMATLEN + BRANCHLEN + 17 + keep_bits_i;

                end else if(branch_packet_off == 25) begin
                    packet_bits[9+:25+XLEN] = {caddr, branch_map_i[24:0]};
                    packet_len = 2 + FORMATLEN + BRANCHLEN + 25 + keep_bits_i;

                end else if (branch_packet_off == 31 && !branch_map_full_i) begin
                    packet_bits[9+:31+XLEN] = {caddr, branch_map_i[30:0]};
                    packet_len = 2 + FORMATLEN + BRANCHLEN + 31 + keep_bits_i;

                end else begin
                    packet_bits[9+:31+XLEN] = {branch_map_edge_case ? caddr :
                                               32'b0, branch_map_i[30:0]};
                    packet_len = 2 + FORMATLEN + BRANCHLEN + 31 +
                                 (branch_map_edge_case ? keep_bits_i : 0);
                    // if we dont have to put an address we indicate that by
                    // settings branchmap to 0
                    packet_bits[8:4] = branch_map_edge_case ?
                                       packet_bits[8:4] : 5'b0;
                    // don't remember any address since there is none

                    // TODO: the spec says something about extending branch-map
                    // to fill the address field when branches = 0. Does this
                    // come into play here?
                    last_addr_d = branch_map_edge_case ? iaddr_i : last_addr_q;
                end
            end

            F_ADDR_ONLY: begin
                packet_bits[4+:XLEN] = caddr;
                packet_len           = 2 + FORMATLEN + keep_bits_i;
            end

            F_SYNC: begin
                // 2: subformat
                // PRIVLEN: privilege level
                // 1: is this instruction a branch
                // XLEN: address
                // CAUSELEN: exception cause
                // 1: is interrupt?
`ifndef SYNTHESIS
                assert(packet_subformat_i != SF_CONTEXT);
                assert(packet_subformat_i != SF_UNDEF);
`endif

                case(packet_subformat_i)

                SF_START: begin
                    packet_bits[4+:2+PRIVLEN+1+XLEN]
                        = {iaddr_i, is_branch_i, priv_i, packet_subformat_i};
                    packet_len = 2 + FORMATLEN + FORMATLEN + PRIVLEN + 1 + XLEN;
                end

                SF_EXCEPTION: begin
                    packet_bits[4+:2+PRIVLEN+1+XLEN+CAUSELEN+1]
                        = {interrupt_i, cause_i, iaddr_i, is_branch_i, priv_i,
                           packet_subformat_i};
                    packet_len = 2 + FORMATLEN + FORMATLEN + PRIVLEN + 1 + XLEN
                                 + CAUSELEN + 1;
                end

                SF_CONTEXT: begin
                end

                endcase

            end
            endcase

        end else if(sw_valid_i) begin
            packet_gen_valid     = '1;
            sw_grant_o           = '1;
            packet_bits[1:0]     = W_SOFTWARE;
            packet_bits[2+:32]   = sw_word_i;
            packet_len           = 2 + 32;

        end else if(tu_valid_i) begin
            packet_gen_valid            = '1;
            tu_grant_o                  = '1;
            packet_bits[1:0]            = W_TIME;
            // TODO: handle fulltime flag
            packet_bits[2+:TIMER_WIDTH] = tu_time_i;
            packet_len                  = 2+TIMER_WIDTH;

        end

    end

    assign packet_bits_o = packet_bits;
    assign packet_len_o = packet_len;
    assign packet_valid_o = packet_gen_valid;


    always_ff @(posedge clk_i, negedge rst_ni) begin
        if(~rst_ni) begin
            branch_map_flush_q <= '0;
            last_addr_q        <= '0;
        end else begin
            branch_map_flush_q <= branch_map_flush_d;
            last_addr_q        <= last_addr_d;
        end
    end

endmodule // trdb_packet_emitter
