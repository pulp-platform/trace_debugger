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
// Description: Memory mapped control registers for the trace debugger

import trdb_pkg::*;

module trdb_reg
    #(parameter APB_ADDR_WIDTH = 12)
    (input logic                       clk_i,
     input logic                      rst_ni,

     output logic [31:0]              per_rdata_o,
     output logic                     per_ready_o,
     input logic [31:0]               per_wdata_i,
     input logic [APB_ADDR_WIDTH-1:0] per_addr_i,
     input logic                      per_we_i,
     input logic                      per_valid_i,

     output                           flush_stream_o,
     input                            flush_confirm_i,

     output logic                     trace_enable_o,
     output logic [31:0]              dump_o);

    // hold configuration data TODO: reduce size
    logic [31:0] cfg_q, cfg_d;

    // hold control status TODO: reduce size, careful look below too
    logic [31:0] ctrl_q, ctrl_d;

    // allow the user to write to this register to dump data through the trace
    // debugger
    logic [31:0] dump_q, dump_d;

    assign trace_enable_o = cfg_q[TRDB_ENABLE];
    assign flush_stream_o = ctrl_q[TRDB_FLUSH_STREAM];
    assign dump_o = dump_q;

    // enable all
    // enable privilege tracing
    // enable exception tracing
    //TODO: read write logic fix
    always_comb begin: read_reg
        per_rdata_o = 32'h0;
        if(per_valid_i & ~per_we_i) begin
            case(per_addr_i)
            REG_TRDB_CFG:
                per_rdata_o = cfg_q;
            REG_TRDB_CTRL:
                per_rdata_o = ctrl_q;
            REG_DUMP:
                per_rdata_o = 32'h0;
            default:
                per_rdata_o = 32'h0;
            endcase
        end
    end

    // we can tie this high since according the apb protocol if we react in the
    // next cycle this is allowed
    assign per_ready_o = 1'b1;

    always_ff @(posedge clk_i, negedge rst_ni) begin
        if(~rst_ni) begin
            cfg_q  <= 'h0;
            ctrl_q <= 'h0;
            dump_q <= 'h0;
        end else begin
            if(per_valid_i & per_we_i) begin
                case (per_addr_i)
                REG_TRDB_CFG:
                    cfg_q <= per_wdata_i;
                REG_TRDB_CTRL:
                    ctrl_q <= per_wdata_i;
                REG_DUMP:
                    dump_q <= per_wdata_i;
                endcase
            end else begin
                if(flush_confirm_i) begin
                    ctrl_q <= {ctrl_q[31:1], 1'b0};
                end
            end
        end
    end
endmodule // trdb_reg
