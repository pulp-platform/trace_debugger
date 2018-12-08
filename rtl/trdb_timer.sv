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
// Description: Keep track of cpu clock cycles for time packets

import trdb_pkg::*;

module trdb_timer
    #(parameter TIMER_WIDTH = 40)
    (input logic                    clk_i,
     input logic                    rst_ni,
     input logic                    manual_rst_i, // TODO: make this handshake

     input logic                    tu_req_i,
     output logic                   tu_valid_o,
     input logic                    tu_grant_i,

     output logic [TIMER_WIDTH-1:0] trdb_time_o);

    logic [TIMER_WIDTH-1:0]         trdb_time_q, trdb_time_d;

    enum logic [1:0] {IDLE, REQ} cs, ns;

    assign trdb_time_o = trdb_time_q;

    // the request logic has a one cycle delay, but when we generated a timer
    // packet by a write to REG_TRDB_DUMP_WITH_TIME, we have to wait for the sw
    // packet anyway
    always_comb begin: timer_req_packet
        ns         = cs;
        tu_valid_o = '0;

        case(cs)
        IDLE: begin
            tu_valid_o = '0;
            if(tu_req_i)
                ns = REQ;
        end

        REQ: begin
            tu_valid_o = '1;
            if(tu_grant_i)
                ns = IDLE;
        end
        endcase // case (cs)
    end

    always_comb begin: inc_timer
        if(!manual_rst_i)
            trdb_time_d <= trdb_time_q + 1;
        else
            trdb_time_d <= '0;
    end

    always_ff @(posedge clk_i, negedge rst_ni) begin
        if(~rst_ni) begin
            trdb_time_q <= '0;
            cs          <= IDLE;
        end else begin
            trdb_time_q <= trdb_time_d;
            cs          <= ns;
        end
    end

endmodule
