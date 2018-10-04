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
    (input logic                      clk_i,
     input logic                      rst_ni,

     // write and reads from the APB
     output logic [31:0]              per_rdata_o,
     output logic                     per_ready_o,
     input logic [31:0]               per_wdata_i,
     input logic [APB_ADDR_WIDTH-1:0] per_addr_i,
     input logic                      per_we_i,
     input logic                      per_valid_i,

     // packet streamer control
     output                           flush_stream_o,
     input                            flush_confirm_i,

     // trace debugger settings
     output logic                     trace_enable_o,
     output logic                     trace_activated_o,
     output logic                     apply_filters_o,
     output logic                     trace_selected_priv_o,
     output logic [1:0]               trace_which_priv_o,
     output logic                     trace_range_o,
     output logic [XLEN-1:0]          trace_lower_addr_o,
     output logic [XLEN-1:0]          trace_higher_addr_o,


     output logic [31:0]              sw_word_o,
     output logic                     sw_valid_o,
     input logic                      sw_grant_i,

     output logic                     tu_req_o
);

    // hold configuration data TODO: reduce size
    logic [31:0] cfg_q, cfg_d;

    // hold control status TODO: reduce size, careful look below too
    logic [31:0] ctrl_q, ctrl_d;

    // allow the user to write to this register to dump data through the trace
    // debugger
    logic [31:0] dump_q, dump_d;
    logic        dump_valid_q, dump_valid_d;
    logic        sw_fifo_not_full;
    logic        clear_sw_fifo;

    // remember address ranges
    logic [XLEN-1:0]       higher_addr_q, higher_addr_d;
    logic [XLEN-1:0]       lower_addr_q, lower_addr_d;



    assign trace_enable_o = cfg_q[TRDB_ENABLE];
    assign trace_activated_o = cfg_q[TRDB_TRACE_ACTIVATED];
    assign apply_filters_o = cfg_q[TRDB_APPLY_FILTERS];
    assign trace_selected_priv_o = cfg_q[TRDB_TRACE_SELECTED_PRIV];
    assign trace_range_o = cfg_q[TRDB_TRACE_RANGE];

    //TODO: map this
    assign trace_which_priv_o = '1;

    assign trace_lower_addr_o = lower_addr_q;
    assign trace_higher_addr_o = higher_addr_q;

    assign flush_stream_o = ctrl_q[TRDB_FLUSH_STREAM];

    // enable all
    // enable privilege tracing
    // enable exception tracing
    // TODO: read write logic fix
    always_comb begin: read_reg
        per_rdata_o = 32'h0;
        if(per_valid_i & ~per_we_i) begin
            case(per_addr_i)
            REG_TRDB_CFG:
                per_rdata_o = cfg_q;
            REG_TRDB_CTRL:
                per_rdata_o = ctrl_q;
            REG_TRDB_DUMP:
                per_rdata_o = 32'h0;
            REG_TRDB_DUMP_WITH_TIME:
                per_rdata_o = 32'h0;
            default:
                per_rdata_o = 32'h0;
            endcase
        end
    end


    // this buffers our software writes, since we are contesting with packets
    // which have a priority over software writes.
    generic_fifo_adv
        #(.DATA_WIDTH(XLEN),
          .DATA_DEPTH(SOFTWARE_BUFFER_STAGES))
    i_sw_fifo
        (.clk(clk_i),
         .rst_n(rst_ni),
         .clear_i(clear_sw_fifo),
         .data_i(dump_q),
         .valid_i(dump_valid_q),
         .grant_o(sw_fifo_not_full), // TODO: make this visible in register
         .data_o(sw_word_o),
         .valid_o(sw_valid_o),
         .grant_i(sw_grant_i),
         .test_mode_i('0)); // TODO: what to do with this

`ifndef SYNTHESIS
    sw_fifo_overflow: assert property
    (@(posedge clk_i) disable iff (~rst_ni) (sw_fifo_not_full == 1'b1))
            else $error("[TRDB]   @%t: Software FIFO is overflowing.", $time);
`endif


    // we can tie this high since according the apb protocol if we react in the
    // next cycle this is allowed
    assign per_ready_o = 1'b1;

    always_comb begin: write_reg
        cfg_d         = cfg_q;
        ctrl_d        = ctrl_q;
        dump_d        = dump_q;
        dump_valid_d  = '0;
        tu_req_o      = '0;
        lower_addr_d  = lower_addr_q;
        higher_addr_d = higher_addr_q;

        if(per_valid_i & per_we_i) begin
            case (per_addr_i)
            REG_TRDB_CFG:
                cfg_d = per_wdata_i;
            REG_TRDB_CTRL:
                ctrl_d = per_wdata_i;
            REG_TRDB_DUMP: begin
                dump_d       = per_wdata_i;
                dump_valid_d = '1;
            end
            REG_TRDB_DUMP_WITH_TIME: begin
                dump_d       = per_wdata_i;
                dump_valid_d = '1;
                tu_req_o     = '1;
            end
            REG_TRDB_LOWER_ADDR:
                lower_addr_d = per_wdata_i;
            REG_TRDB_HIGHER_ADDR:
                higher_addr_d = per_wdata_i;
            endcase
        end else begin
            if(flush_confirm_i) begin
                ctrl_d = {ctrl_q[31:1], 1'b0};
            end
        end
    end

    always_ff @(posedge clk_i, negedge rst_ni) begin
        if(~rst_ni) begin
            cfg_q         <= 'h0;
            ctrl_q        <= 'h0;
            dump_q        <= 'h0;
            dump_valid_q  <= 'h0;
            higher_addr_q <= 'h0;
            lower_addr_q  <= 'h0;

        end else begin
            cfg_q         <= cfg_d;
            ctrl_q        <= ctrl_d;
            dump_q        <= dump_d;
            dump_valid_q  <= dump_valid_d;
            higher_addr_q <= higher_addr_d;
            lower_addr_q  <= lower_addr_d;

        end
    end
endmodule // trdb_reg
