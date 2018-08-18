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
// Description: Order packet generation request by priority


import trace_debugger_defines::*;

// TODO: add commented out signals
module trdb_priority
    (input logic  clk_i,
     input logic  rst_ni,

     input logic  lc_exception_i,
     //input logic lc_emitted_exception_sync (hack)

     input logic  tc_first_qualified_i,
     //input logic  tc_unhalted,
     input logic  tc_privchange_i,
     //input logic resync & branch_map_cnt

     input logic  lc_u_discontinuity_i,

     // input logic  resync
     // input logic  branch_map_nonempty,

     //input logic  nc_halt,
     input logic  nc_exception_i,
     input logic  nc_privchange_i,
     //input logic  nc_qualified,

     input logic  branch_map_full_i,

     //input logic  tc_context_change,

     input logic  branch_map_empty_i,

     output logic valid_o,
     output       trdb_format_t packet_format_o,
     output       trdb_subformat_t packet_subformat_o);

    // TODO: same as in C-code, differential address generation when
    // TODO: assert for X's
    always_comb begin
        // TODO: default value for packet_*format_o
        valid_o            = '1;
        packet_format_o    = F_ADDR_ONLY;
        packet_subformat_o = SF_UNDEF;

        if (lc_exception_i) begin
            packet_format_o    = F_SYNC;
            packet_subformat_o = SF_START;
            // TODO: lc_emitted_exception_sync
            // TODO: missing some conditions
        end else if (tc_first_qualified_i || tc_unhalted_i ||
                     tc_privchange_i) begin
            packet_format_o  = F_SYNC;
            packet_subformat = SF_START;
        end else if (lc_u_discontinuity_i) begin
            if(branch_map_empty_i)
                packet_format_o = F_ADDR_ONLY;
            else
                packet_format_o = F_BRANCH_FULL;
            //TODO: resync and branch_map_nonempty clause
        end else if (nc_halt_i || nc_exception_i || nc_privchange_i ||
                     nc_unqualified_i) begin
            if(branch_map_empty_i)
                packet_format_o = F_ADDR_ONLY;
            else
                packet_format_o = F_BRANCH_FULL;
        end else if (branch_map_full_i) begin
            packet_format = F_BRANCH_FULL;
        end else if (tc_context_change) begin
            packet_format_o    = F_SYNC;
            packet_subformat_o = SF_CONTEXT;
        end else if begin
            // no cases are matching so we don't want to emit a packet
            valid_o = '0;
        end
    end

endmodule // trdb_priority
