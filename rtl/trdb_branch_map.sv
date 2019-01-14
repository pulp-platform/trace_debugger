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
// Description: Records the sequence of taken and not taken branches

module trdb_branch_map
    (input logic         clk_i,
     input logic         rst_ni,

     input logic         valid_i,
     input logic         branch_taken_i,
     input logic         flush_i,

     output logic [30:0] map_o,
     output logic [4:0]  branches_o,
     output logic        is_full_o,
     output logic        is_empty_o);

    logic [30:0]         map_d, map_q;
    logic [4:0]          branchcnt_d, branchcnt_q;

    // when valid_i then store the new branch value branch_taken_i into map_d.
    // branchcnt_q contains the current number of stored branches.
    always_comb begin
        map_d       = map_q;
        branchcnt_d = branchcnt_q;

        // TODO: do a case here?
        if(flush_i) begin
            map_d       = '0;
            branchcnt_d = '0;
        end

        if(valid_i) begin
            if(flush_i) begin
                map_d[0]    = ~branch_taken_i;
                branchcnt_d = 5'b1;
            end else begin
                map_d[branchcnt_q] = ~branch_taken_i;
                branchcnt_d        = branchcnt_q + 1;
            end
        end
    end

    // we have a direct input output path because we need the result this cycle
    // maybe should be changed later if this path is too long
    assign map_o = map_d;
    assign branches_o = branchcnt_d;
    assign is_full_o = (branchcnt_d == 31);
    assign is_empty_o = (branchcnt_d == 0);

    always_ff @(posedge clk_i, negedge rst_ni) begin
        if(~rst_ni) begin
            map_q <= '0;
            branchcnt_q <= '0;
        end else begin
            map_q <= map_d;
            branchcnt_q <= branchcnt_d;
        end
    end
endmodule // trdb_branch_map
