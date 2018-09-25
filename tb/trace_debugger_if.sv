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
// Description: Interface of the trace debugger module

interface trace_debugger_if
    (input logic                clk_i,
     input logic                rst_ni);

    import trdb_pkg::*;

    // inputs
    logic                ivalid;
    logic                iexception;
    logic                interrupt;
    logic [CAUSELEN-1:0] cause;
    logic [XLEN-1:0]     tval;
    logic [PRIVLEN-1:0]  priv;
    logic [XLEN-1:0]     iaddr;
    logic [ILEN-1:0]     instr;
    logic                compressed;

    logic                stall;

    // outputs
    logic [XLEN-1:0]     packet_word;
    logic                packet_word_valid;

    APB_BUS #(.APB_ADDR_WIDTH(32)) apb_bus();


    modport duv
        (input clk_i, rst_ni, ivalid, iexception, interrupt, cause, tval, priv,
         iaddr, instr, compressed, apb_bus, stall, output packet_word,
         packet_word_valid);

    modport tb
        (input clk_i, rst_ni, output ivalid, iexception, interrupt, cause,
         tval, priv, iaddr, instr, compressed, apb_bus, stall,
         input packet_word, packet_word_valid);


endinterface
