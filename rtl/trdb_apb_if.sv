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
// Description: Translate APB to tracer configuration interface


module trdb_apb_if
    #(parameter APB_ADDR_WIDTH = 12)
    (input logic [APB_ADDR_WIDTH-1:0]  paddr,
     input logic [31:0]                pwdata,
     input logic                       pwrite,
     input logic                       psel,
     input logic                       penable,
     output logic [31:0]               prdata,
     output logic                      pready,
     output logic                      pslverr,

     input logic [31:0]                per_rdata_i,
     input logic                       per_ready_i,
     output logic [31:0]               per_wdata_o,
     output logic [APB_ADDR_WIDTH-1:0] per_addr_o,
     output logic                      per_we_o,
     output logic                      per_valid_o);


    // we always get the whole 32 bit address, but if we get an access,
    // indicated through psel we don't really need to check the upper bits.
    assign per_addr_o = paddr[11:0];
    assign per_we_o = pwrite;
    assign per_wdata_o = pwdata;
    // psel indicates if your device is selected
    // penable indicates the access phase
    assign per_valid_o = psel & penable;

    assign prdata = per_rdata_i;
    assign pready = per_ready_i;

    // we never cause an error
    assign pslverr = 1'b0;


endmodule // trdb_apb_if
