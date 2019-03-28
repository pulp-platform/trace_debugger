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
// Description: Order packet generation request by priority


import trdb_pkg::*;

// TODO: add commented out signals
module trdb_priority
    (input logic                   valid_i,

     input logic [XLEN-1:0]        full_addr_i,
     input logic [XLEN-1:0]        diff_addr_i,

     input logic                   lc_exception_i,
     input logic                   lc_exception_sync_i,

     input logic                   tc_first_qualified_i,
     input logic                   nc_unqualified_i,
     //input logic  tc_unhalted,
     input logic                   tc_privchange_i,
     //input logic resync & branch_map_cnt

     input logic                   lc_u_discontinuity_i,

     // input logic  resync
     // input logic  branch_map_nonempty,

     //input logic  nc_halt,
     input logic                   nc_exception_i,
     input logic                   nc_privchange_i,
     //input logic  nc_qualified,

     input logic                   branch_map_full_i,

     //input logic  tc_context_change,

     input logic                   branch_map_empty_i,

     input logic                   use_full_addr_i,

     // how many bits of the address are useful
     output logic [$clog2(XLEN):0] keep_bits_o,
     output logic                  valid_o,
     output trdb_format_e          packet_format_o,
     output trdb_subformat_e       packet_subformat_o);

    // to compress addresses we can, instead of always giving out the full
    // address, only give out the difference to the last emitted packet address
    logic                          prefer_differential;

    logic [$clog2(XLEN)-1:0]       diff_addr_zeros, diff_addr_ones;
    logic [$clog2(XLEN)-1:0]       full_addr_zeros, full_addr_ones;

    logic [$clog2(XLEN)-1:0]       abs_sign_extendable, diff_sign_extendable,
                                   sign_extendable;


    // TODO: temporarily put here to make it compile
    logic         tc_unhalted_i;
    logic         nc_halt_i;
    logic         tc_context_change;

    assign tc_unhalted_i     = '0;
    assign nc_halt_i         = '0;
    assign tc_context_change = '0;


    // TODO: same as in C-code, differential address generation when
    // TODO: assert for X's
    always_comb begin: select_packet
        // TODO: default value for packet_*format_o
        valid_o            = '0;
        packet_format_o    = F_ADDR_ONLY;
        packet_subformat_o = SF_UNDEF;

        if (valid_i) begin
            if (lc_exception_i) begin
                packet_format_o    = F_SYNC;
                packet_subformat_o = SF_EXCEPTION;
                valid_o            = '1;
                // TODO: lc_emitted_exception_sync
                // TODO: missing some conditions
            end else if (lc_exception_sync_i) begin
                packet_format_o    = F_SYNC;
                packet_subformat_o = SF_START;
                valid_o            = '1;

            end else if (tc_first_qualified_i || tc_unhalted_i ||
                         tc_privchange_i) begin
                packet_format_o    = F_SYNC;
                packet_subformat_o = SF_START;
                valid_o            = '1;

            end else if (lc_u_discontinuity_i) begin
                if (branch_map_empty_i)
                    packet_format_o = F_ADDR_ONLY;
                else begin
                    if (use_full_addr_i) begin
                        packet_format_o = F_BRANCH_FULL;
                    end else begin
                        packet_format_o = prefer_differential ?
                                          F_BRANCH_DIFF : F_BRANCH_FULL;
                    end
                end
                valid_o = '1;

                //TODO: resync and branch_map_nonempty clause
            end else if (nc_halt_i || nc_exception_i || nc_privchange_i ||
                         nc_unqualified_i) begin
                if(branch_map_empty_i)
                    packet_format_o = F_ADDR_ONLY;
                else begin
                    if (use_full_addr_i) begin
                        packet_format_o = F_BRANCH_FULL;
                    end else begin
                        packet_format_o = prefer_differential ?
                                          F_BRANCH_DIFF : F_BRANCH_FULL;
                    end
                end
                valid_o = '1;

            end else if (branch_map_full_i) begin
                // prefer_differential doesn't matter since we don't have an
                // address anyway
                packet_format_o    = F_BRANCH_FULL;
                valid_o            = '1;

            end else if (tc_context_change) begin
                packet_format_o    = F_SYNC;
                packet_subformat_o = SF_CONTEXT;
                valid_o            = '1;
            end
        end
    end

    // we want to take the address which needs the least bits to represent by
    // omitting the bits which can be inferred through sign-extension
    // TODO: we don't need to know the exact number of leading zeros/ones
    always_comb begin: absolute_or_differential
        abs_sign_extendable  = full_addr_zeros > full_addr_ones ?
                               full_addr_zeros : full_addr_ones;
        diff_sign_extendable = diff_addr_zeros > diff_addr_ones ?
                               diff_addr_zeros : diff_addr_ones;
        prefer_differential  = diff_sign_extendable > abs_sign_extendable;
        sign_extendable      = prefer_differential ?
                               diff_sign_extendable : abs_sign_extendable;

        // if we always want the full address then we have to signal that we
        // want to keep all bits
        keep_bits_o = use_full_addr_i ? XLEN : XLEN - sign_extendable + 1;
    end

    trdb_lzc
        #(.WIDTH(XLEN),
          .MODE(1))
    i_trdb_lzc_full
        (.in_i(full_addr_i),
         .cnt_o(full_addr_zeros),
         .empty_o());

    trdb_lzc
        #(.WIDTH(XLEN),
          .MODE(1))
    i_trdb_loc_full
        (.in_i(~full_addr_i),
         .cnt_o(full_addr_ones),
         .empty_o());

    trdb_lzc
        #(.WIDTH(XLEN),
          .MODE(1))
    i_trdb_lzc_diff
        (.in_i(diff_addr_i),
         .cnt_o(diff_addr_zeros),
         .empty_o());

    trdb_lzc
        #(.WIDTH(XLEN),
          .MODE(1))
    i_trdb_loc_diff
        (.in_i(~diff_addr_i),
         .cnt_o(diff_addr_ones),
         .empty_o());


endmodule // trdb_priority
