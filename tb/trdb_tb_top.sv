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
// Description: Top level testbench

// TODO: inspect this value
`timescale 1ns/1ns

module trdb_tb_top #(
    parameter NumRegs = 3
);

    import trdb_tb_pkg::*;

    // testbench signals to dut
    logic      clk = 'b1;
    logic      rst_n = 'b0;
    logic      eos_s = 'b0; //end of simulation
    logic      test_mode;


    assign test_mode = '0;

    trace_debugger_if duv_if(clk, rst_n, test_mode);

    //instantiate duv
    trace_debugger_wrapper i_trace_debugger_wrapper(.duv_if(duv_if));

    // randomized handshake
    always_ff @(duv_if.cb) begin
        //duv_if.grant = repeat (NumRegs) @(posedge clk) duv_if.packet_word_valid;
        duv_if.cb.grant <= $urandom_range(0, 1);
    end

    //instantiate testbench
    trdb_tb i_trdb_tb(.tb_if(duv_if));

    // clock generation
    initial begin: clock_gen
        while (!eos_s) begin
            #CLK_PHASE_HI clk = 1'b0;
            #CLK_PHASE_LO clk = 1'b1;
        end
    end: clock_gen

    // reset generation
    initial begin: reset_gen
        rst_n = 1'b0;
        // wait a few cycles
        repeat (RESET_WAIT_CYCLES) begin
            @(posedge clk); //TODO: was posedge, see below
        end
        //TODO: think about when the reset sould happen
        #RESET_DEL rst_n = 1'b1;
        if($test$plusargs("debug"))
            $display("[RESET]  @%t: Reset deasserted.", $time);

    end: reset_gen

    // set timing format
    initial begin: timing_format
        $timeformat(-9, 0, "ns", 9);
    end: timing_format


endmodule // trdb_tb
