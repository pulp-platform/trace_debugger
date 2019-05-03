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
// Description: Interface of the trace debugger module

`timescale 1ns/1ns

interface trace_debugger_if (
    input logic clk_i,
    input logic rst_ni,
    input logic test_mode_i
);
    import trdb_pkg::*;
    import trdb_tb_pkg::*;

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

    logic                grant;

    // outputs
    logic [XLEN-1:0]     packet_word;
    logic                packet_word_valid;

    APB_BUS #(.APB_ADDR_WIDTH(32)) apb_bus();

    clocking cb @(posedge clk_i);
        default input #(CLK_PERIOD - RESP_ACQUISITION_DEL) output #STIM_APPLICATION_DEL;

        // trace debugger
        output ivalid;
        output iexception;
        output interrupt;
        output cause;
        output tval;
        output priv;
        output iaddr;
        output instr;
        output compressed;
        inout grant;

        input packet_word;
        input packet_word_valid;

        // apb
        output paddr = apb_bus.paddr;
        output pwdata = apb_bus.pwdata;
        output pwrite = apb_bus.pwrite;
        output psel = apb_bus.psel;
        output penable = apb_bus.penable;
        input  prdata = apb_bus.prdata;
        input  pready = apb_bus.pready;
        input  pslverr = apb_bus.pslverr;

    endclocking

endinterface
