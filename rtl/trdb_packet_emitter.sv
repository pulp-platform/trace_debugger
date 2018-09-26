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

     input logic                   lc_u_discontinuity_i,

     input logic [30:0]            branch_map_i,
     input logic [4:0]             branch_map_cnt_i,
     input logic                   branch_map_empty_i,
     input logic                   branch_map_full_i,
     input logic                   is_branch_i,

     output logic                  branch_map_flush_o,
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
    logic                          branch_map_flush_q, branch_map_flush_d;

    logic [4:0]                    branch_packet_off ;

    logic                          branch_map_edge_case;

    assign branch_map_flush_o = branch_map_flush_q;
    assign packet_gen_valid = valid_i;

    always_comb begin: branch_map_offset

        if(branch_map_cnt_i <= 1) begin
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


    branch_map_size_check: assert property
    (@(posedge clk_i) disable iff (~rst_ni) (branch_map_cnt_i < 32))
        else $error("[TRDB]    @%t: branch_map_cnt_i=%d is too large",
                    $time, branch_map_cnt_i);


    unimplemented_packet : assert property
    (@(posedge clk_i) disable iff (~rst_ni) (packet_format_i != F_BRANCH_DIFF))
     else $error("[TRDB]   @%t: Invalid packet format signaled.", $time);


    packet_fifo_overflow: assert property
    (@(posedge clk_i) disable iff (~rst_ni) (packet_fifo_not_full == 1'b1))
            else $error("[TRDB]   @%t: Packet FIFO is overflowing.", $time);


    always_comb begin: set_packet_bits
        packet_bits        = '0;
        packet_len         = '0;
        branch_map_flush_d = '0;

        // TODO: actually this might not be necessary
        branch_map_edge_case = lc_u_discontinuity_i;

        if(valid_i) begin

            // TODO: adapt msg type
            packet_bits[1:0]   = 2'h2;

            // packet format
            packet_bits[3:2]   = packet_format_i;

            // always flush branchmap
            branch_map_flush_d = '1;

            case(packet_format_i)

            F_BRANCH_FULL: begin
                // We shouldn't have an empty branch map if we want to generate
                // this packet TODO: there is an issue since branch_map_flush_d
                // immediately changes branch_map_cnt_i
                //  assert(branch_map_cnt_i != 0);

                packet_bits[8:4]   = branch_map_cnt_i;
                branch_map_flush_d = '1;

                if(branch_packet_off == 1) begin
                    packet_bits[9+:1+XLEN] = {iaddr_i, branch_map_i[0]};
                    packet_len = 2 + FORMATLEN + BRANCHLEN + 1 + XLEN;

                end else if(branch_packet_off == 9) begin
                    packet_bits[9+:9+XLEN] = {iaddr_i, branch_map_i[8:0]};
                    packet_len = 2 + FORMATLEN + BRANCHLEN + 9 + XLEN;

                end else if(branch_packet_off == 17) begin
                    packet_bits[9+:17+XLEN] = {iaddr_i, branch_map_i[16:0]};
                    packet_len = 2 + FORMATLEN + BRANCHLEN + 17 + XLEN;

                end else if(branch_packet_off == 25) begin
                    packet_bits[9+:25+XLEN] = {iaddr_i, branch_map_i[24:0]};
                    packet_len = 2 + FORMATLEN + BRANCHLEN + 25 + XLEN;

                end else if (branch_packet_off == 31 && !branch_map_full_i) begin
                    packet_bits[9+:31+XLEN] = {iaddr_i, branch_map_i[30:0]};
                    packet_len = 2 + FORMATLEN + BRANCHLEN + 31 + XLEN;

                end else begin
                    packet_bits[9+:31+XLEN] = {branch_map_edge_case ? iaddr_i :
                                               32'b0, branch_map_i[30:0]};
                    packet_len = 2 + FORMATLEN + BRANCHLEN + 31 +
                                 (branch_map_edge_case ? XLEN : 0);
                end
            end

            F_BRANCH_DIFF: begin
                //TODO: variable len addr
            end

            F_ADDR_ONLY: begin
                packet_bits[4+:XLEN] = iaddr_i;
                packet_len           = 2 + FORMATLEN + XLEN; //TODO: variable len add
            end

            F_SYNC: begin
                // 2: subformat
                // PRIVLEN: privilege level
                // 1: is this instruction a branch
                // XLEN: address
                // CAUSELEN: exception cause
                // 1: is interrupt?
                assert(packet_subformat_i != SF_CONTEXT);
                assert(packet_subformat_i != SF_UNDEF);

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
        end
    end

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


    always_ff @(posedge clk_i, negedge rst_ni) begin
        if(~rst_ni) begin
            branch_map_flush_q <= '0;
        end else begin
            branch_map_flush_q <= branch_map_flush_d;
        end
    end

endmodule // trdb_packet_emitter
