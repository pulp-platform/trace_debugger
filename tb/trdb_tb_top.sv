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
// Description: Top level testbench

// TODO: inspect this value
//`timescale 1ns/1ns

module trdb_tb_top
    ();

    // stimuli file
    const string stimuli_path = "tb/stimuli/test";

    // clock and acquisition related settings
    const time CLK_PHASE_HI         = 5ns;
    const time CLK_PHASE_LO         = 5ns;
    const time CLK_PERIOD           = CLK_PHASE_HI + CLK_PHASE_LO;
    const time STIM_APPLICATION_DEL = CLK_PERIOD * 0.1;
    const time RESP_ACQUISITION_DEL = CLK_PERIOD * 0.9;
    const time RESET_DEL            = STIM_APPLICATION_DEL;

    const int  RESET_WAIT_CYCLES    = 4;

    // testbench signals to dut
    logic      clk = 'b1;
    logic      rst_n = 'b0;
    logic      eos_s = 'b0; //end of simulation


    trace_debugger_if duv_if(clk, rst_n);

    //instantiate duv
    trace_debugger_wrapper i_trace_debugger_wrapper(.duv_if(duv_if.duv));

    //instantiate testbench
    trdb_tb i_trdb_tb(.tb_if(duv_if.tb));

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
            @(posedge clk);
        end
        #RESET_DEL rst_n = 1'b1;
    end: reset_gen

    // set timing format
    initial begin: timing_format
        $timeformat(-9, 0, "ns", 9);
    end: timing_format

    // parse stimuli file
    initial begin
        string      line;
        int         fp, c;
        string      err_str;

        $display("[Stimuli]: opening file %s", stimuli_path);
        fp = $fopen(stimuli_path, "r");

        if($ferror(fp, err_str)) begin
            $display(err_str);
            $finish;
        end

        while ($fscanf(fp, "%s", line) != -1) begin
            $display("%s", line);
            // case (cmd)
            // "": continue;
            // "done":break;
            // endcase // case (cmd)
        end

        if ($ferror(fp, err_str)) begin
            $display("%s", err_str);
        end else if ($feof(fp)) begin
            $display("[Stimuli]: end of file %s", stimuli_path);
        end

        if(!fp)
            $fclose(fp);
    end

endmodule // trdb_tb
