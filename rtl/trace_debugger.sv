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
// Description: Compress instruction traces and filter them

module trace_debugger
    (input logic        clk_i,
     input logic 	rst_ni,

     input logic 	ivalid_i,
     input logic 	iexception_i,
     input logic 	interrupt_i,
     input logic [ 4:0] cause_i,
     input logic [31:0] tval_i,
     input logic [ 2:0] priv_i,
     input logic [31:0] iaddr_i,
     input logic [31:0] instr_i);

    logic 		ivalid_q, ivalid_d;
    logic 		iexception_q, iexception_d;
    logic 		interrupt_q, interrupt_d;
    logic [ 4:0] 	cause_q, cause_d;
    logic [31:0] 	tval_q, tval_d;
    logic [ 2:0] 	priv_q, priv_d;
    logic [31:0] 	iaddr_q, iaddr_d;
    logic [31:0] 	instr_q, instr_d;

    always_ff @(posedge clk_i, negedge rst_ni) begin
        if(~rst_ni) begin
	    ivalid_q 	 <= '0;
	    iexception_q <= '0;
	    interrupt_q  <= '0;
	    cause_q 	 <= '0;
	    tval_q 	 <= '0;
	    priv_q 	 <= '0;
	    iaddr_q 	 <= '0;
	    instr_q 	 <= '0;
        end else begin
	    ivalid_q 	 <= ivalid_i;
	    iexception_q <= iexception_i;
	    interrupt_q  <= interrupt_i;
	    cause_q 	 <= cause_i;
	    tval_q 	 <= tval_i;
	    priv_q 	 <= priv_i;
	    iaddr_q 	 <= iaddr_i;
	    instr_q 	 <= instr_i;
        end
    end
endmodule // trace_debugger
