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
// Description: Peripheral module that pushes tracer data to the udma

module tracer_adapter_if
    #(parameter L2_AWIDTH_NOAL = 12, // TODO: check those values
      parameter TRANS_SIZE     = 16,
      parameter DATA_WIDTH     = 12,
      parameter BUFFER_WIDTH   = 8)
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
     input logic [TRANS_SIZE-1:0]      cfg_rx_bytes_left_i,

     // Data from the tracer that goes to the udma
     output logic [1:0]                data_rx_datasize_o,
     output logic [15:0]               data_rx_data_o,
     output logic                      data_rx_valid_o,
     input logic                       data_rx_ready_i);

    logic [15:0]                       data_rx_data_q;
    logic                              data_rx_valid_q;

    // We are in the same clockdomain as the SoC, so no DC stuff needed
    tracer_adapter_reg_if
        #(.L2_AWIDTH_NOAL(L2_AWIDTH_NOAL),
          .TRANS_SIZE(TRANS_SIZE))
    i_tracer_adapter_reg_if // TODO: maybe do auto port assignment?
        (.clk_i              (clk_i),
         .rst_ni             (rst_ni),

         .cfg_data_i         (cfg_data_i),
         .cfg_addr_i         (cfg_addr_i),
         .cfg_valid_i        (cfg_valid_i),
         .cfg_rw_ni          (cfg_rw_ni),
         .cfg_ready_o        (cfg_ready_o),
         .cfg_data_o         (cfg_data_o),

         .cfg_rx_startaddr_o (cfg_rx_startaddr_o),
         .cfg_rx_size_o      (cfg_rx_size_o),
         .cfg_rx_datasize_o  (data_rx_datasize_o),
         .cfg_rx_continuous_o(cfg_rx_continuous_o),
         .cfg_rx_en_o        (cfg_rx_en_o),
         .cfg_rx_filter_o    (cfg_rx_filter_o),
         .cfg_rx_clr_o       (cfg_rx_clr_o),
         .cfg_rx_en_i        (cfg_rx_en_i),
         .cfg_rx_pending_i   (cfg_rx_pending_i),
         .cfg_rx_curr_addr_i (cfg_rx_curr_addr_i),
         .cfg_rx_bytes_left_i(cfg_rx_bytes_left_i));

    // For now we generate some predictable data, we also just ignore
    // data_rx_ready_i because we don't care if transfers succeed
    assign data_rx_data_o = data_rx_data_q;
    assign data_rx_valid_o = data_rx_data_q;

    always_ff @(posedge clk_i, negedge rst_ni) begin
        if(~rst_ni) begin
            data_rx_data_q  <= 'h0;
            data_rx_valid_q <= 'h0;
        end else begin
            data_rx_data_q  <= '1; //0xffff
            data_rx_valid_q <= 'h1;
        end
    end
endmodule // tracer_adapter_if
