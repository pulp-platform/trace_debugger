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
// Description: Peripheral module that pushes tracer data to the udma

module tracer_if
    #(parameter L2_AWIDTH_NOAL = 12, // L2 address bus
      parameter TRANS_SIZE     = 16) // buffer size in L2 / transfer length
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
     output logic [31:0]               data_rx_data_o,
     output logic                      data_rx_valid_o,
     input logic                       data_rx_ready_i,

     // Data from the trace debugger unit to this interface
     input logic [31:0]                trdb_packet_i,
     input logic                       trdb_word_valid_i,
     output logic                      trdb_stall_o);

    // buffer data from trace debugger
    logic [31:0]                       data_rx_data_q;
    logic                              data_rx_valid_q;

    // statemachine for stall signal
    enum logic [1:0] {IDLE, TRANSFER, STALL} cs, ns;

    // signal that the udma fifo is overflowing
    logic stall;

    // We are in the same clockdomain as the SoC, so no DC stuff needed. This
    // block just buffers all the udma configuration data, it is the same
    // boilerplate stuff as in the other interfaces.
    tracer_reg_if
        #(.L2_AWIDTH_NOAL(L2_AWIDTH_NOAL),
          .TRANS_SIZE(TRANS_SIZE))
    i_tracer_reg_if
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

    // Signal the trace debugger if we have to stall because of FIFO overflow in
    // UDMA
    always_comb begin
        ns    = cs;
        stall = '0;

        case(cs)

        IDLE: begin
            stall = '0;
            if(data_rx_valid_q && data_rx_ready_i) begin
                ns = TRANSFER;
            end else if(data_rx_valid_q && !data_rx_ready_i) begin
                ns = STALL;
            end else begin
                ns = IDLE;
            end
        end

        TRANSFER: begin
            stall = '0;
            if(data_rx_valid_q && data_rx_ready_i) begin
                ns    = TRANSFER;
            end else if(data_rx_valid_q && !data_rx_ready_i) begin
                ns    = STALL;
            end else if(!data_rx_valid_q) begin
                ns    = IDLE;
            end
        end

        STALL: begin
            stall = '1;
            if(!data_rx_ready_i) begin
                ns = STALL;
            end else begin
                ns = TRANSFER;
            end
        end

        endcase // case (cs)
    end

    assign trdb_stall_o = stall;
    assign data_rx_data_o = data_rx_data_q;
    assign data_rx_valid_o = data_rx_valid_q;

    always_ff @(posedge clk_i, negedge rst_ni) begin
        if(~rst_ni) begin
            cs              <= IDLE;
            data_rx_data_q  <= 'h0;
            data_rx_valid_q <= 'h0;
        end else begin
            cs              <= ns;
            data_rx_data_q  <= trdb_packet_i;
            data_rx_valid_q <= trdb_word_valid_i;
        end
    end
endmodule // tracer_if
