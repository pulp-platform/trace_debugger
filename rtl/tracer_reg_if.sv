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
// Description: Manages peripheral state, i.e. the memory mapped registers

`define REG_RX_SADDR  5'h0 //BASEADDR+0x00
`define REG_RX_SIZE   5'h1 //BASEADDR+0x04
`define REG_RX_CFG    5'h2 //BASEADDR+0x08
`define REG_RX_INTCFG 5'h3 //BASEADDR+0x0C

module tracer_reg_if
  #(parameter L2_AWIDTH_NOAL = 12,
    parameter TRANS_SIZE = 16)
   (input logic                       clk_i,
    input logic                       rst_ni,

    // Configuration r/w from APB
    input logic [31:0]                cfg_data_i,
    input logic [4:0]                 cfg_addr_i,
    input logic                       cfg_valid_i,
    input logic                       cfg_rw_ni,
    output logic [31:0]               cfg_data_o,
    output logic                      cfg_ready_o,

    // Configuration state for the udma
    output logic [L2_AWIDTH_NOAL-1:0] cfg_rx_startaddr_o,
    output logic [TRANS_SIZE-1:0]     cfg_rx_size_o,
    output logic [1:0]                cfg_rx_datasize_o,
    output logic                      cfg_rx_continuous_o,
    output logic                      cfg_rx_en_o,
    output logic                      cfg_rx_filter_o,
    output logic                      cfg_rx_clr_o,
    input logic                       cfg_rx_en_i,
    input logic                       cfg_rx_pending_i,
    input logic [L2_AWIDTH_NOAL-1:0]  cfg_rx_curr_addr_i,
    input logic [TRANS_SIZE-1:0]      cfg_rx_bytes_left_i);

    // This peripheral only outputs data, keep necessary state
    logic [L2_AWIDTH_NOAL-1:0] rx_startaddr_q;
    logic [TRANS_SIZE-1 : 0]   rx_size_q;
    // logic [1 : 0]              rx_datasize_q;
    logic                      rx_continuous_q;
    logic                      rx_en_q;
    logic                      rx_filter_q;
    logic                      rx_clr_q;

    // Internal read and write addresses for the registers
    logic [4:0]                        wr_addr;
    logic [4:0]                        rd_addr;

    assign cfg_rx_startaddr_o  = rx_startaddr_q;
    assign cfg_rx_size_o       = rx_size_q;
    assign cfg_rx_datasize_o   = 2'h2; //rx_datasize_q;
    assign cfg_rx_continuous_o = rx_continuous_q;
    assign cfg_rx_en_o         = rx_en_q;
    assign cfg_rx_filter_o     = rx_filter_q;
    assign cfg_rx_clr_o        = rx_clr_q;

    assign wr_addr = (cfg_valid_i & ~cfg_rw_ni) ? cfg_addr_i : 5'h0;
    assign rd_addr = (cfg_valid_i &  cfg_rw_ni) ? cfg_addr_i : 5'h0;

    assign cfg_ready_o = 1'b1;


    always_comb begin
        cfg_data_o = 32'h0;
        case (rd_addr)
        `REG_RX_SADDR:
            cfg_data_o = cfg_rx_curr_addr_i;
        `REG_RX_SIZE:
            cfg_data_o[TRANS_SIZE-1:0] = cfg_rx_bytes_left_i;
        `REG_RX_CFG:
            cfg_data_o = {26'h0, cfg_rx_pending_i, cfg_rx_en_i,
                          rx_filter_q, 2'h2, rx_continuous_q};
        default:
            cfg_data_o = 'h0;
        endcase
    end


    always_ff @(posedge clk_i, negedge rst_ni) begin
        if(~rst_ni) begin
            rx_startaddr_q  <= 'h0;
            rx_size_q       <= 'h0;
            rx_continuous_q <= 'h0;
            rx_filter_q     <= 'h0;
            rx_en_q          = 'h0; // TODO: figure this out
            rx_clr_q         = 'h0;
            // rx_datasize_q   <= 'h2; //b -> h
        end else begin
            rx_en_q  = 'h0;
            rx_clr_q = 'h0;

            if (cfg_valid_i & ~cfg_rw_ni) begin
                case (wr_addr)
                `REG_RX_SADDR:
                    rx_startaddr_q <= cfg_data_i[L2_AWIDTH_NOAL-1:0];
                `REG_RX_SIZE:
                    rx_size_q <= cfg_data_i[TRANS_SIZE-1:0];
                `REG_RX_CFG:
                begin
                    rx_clr_q         = cfg_data_i[5];
                    rx_en_q          = cfg_data_i[4];
                    rx_filter_q     <= cfg_data_i[3];
                    // rx_datasize_q   <= cfg_data_i[2:1];
                    rx_continuous_q <= cfg_data_i[0];
                end
                //TODO: default case
                endcase
            end
        end
    end
endmodule // tracer_reg_if
