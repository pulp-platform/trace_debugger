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
// Description: Captures incoming instruction samples and writes to file

module trace_debugger_stimuli_gen
    (input logic        clk_i,
     input logic        rst_ni,

     input logic        ivalid_i,
     input logic        iexception_i,
     input logic        interrupt_i,
     input logic [ 4:0] cause_i,
     input logic [31:0] tval_i,
     input logic [ 2:0] priv_i,
     input logic [31:0] iaddr_i,
     input logic [31:0] instr_i,
     input logic        compressed_i);

    logic               ivalid_q, ivalid_d;
    logic               iexception_q, iexception_d;
    logic               interrupt_q, interrupt_d;
    logic [ 4:0]        cause_q, cause_d;
    logic [31:0]        tval_q, tval_d;
    logic [ 2:0]        priv_q, priv_d;
    logic [31:0]        iaddr_q, iaddr_d;
    logic [31:0]        instr_q, instr_d;

`ifndef SYNTHESIS
    initial begin

        automatic string name = "trdb_stimuli";
        int    fd;

        fd = $fopen(name, "w");

        if(fd)
            $display("[TRDB] Opening file %s", name);
        else
            $fatal("[TRDB] Failed to open file");

        forever @(negedge clk_i) begin
            if(~rst_ni) begin
                continue;
            end
            // if(!ivalid_i) begin
            //     continue;
            // end
            $fdisplay(fd,"valid=%h exception=%h interrupt=%h cause=%h",
                      ivalid_i, iexception_i, interrupt_i, cause_i,
                      " tval=%h priv=%h compressed=%h addr=%h instr=%h",
                      32'b0, priv_i, compressed_i, iaddr_i, instr_i);
        end
        //tval_i is just 'x for now so ignore it

        //$fclose(fd);
    end
`endif

endmodule // trace_debugger_stimuli_gen
