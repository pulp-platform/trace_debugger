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
// Description: Testbench

import trdb_tb_pkg::*;

module trdb_tb
    (trace_debugger_if tb_if);

    // run test like instantiate a module
    tb_run i_tb_run();

program automatic tb_run;
    Driver driver;
    Reader reader;
    Scoreboard scoreboard;

    event tb_eos;

    mailbox #(Stimuli) stimuli;
    mailbox #(Response) gm_scoreboard;
    mailbox #(Response) duv_scoreboard;

    initial begin
        stimuli        = new();
        gm_scoreboard  = new();
        duv_scoreboard = new();

        driver         = new(tb_if, stimuli, tb_eos);
        reader         = new(tb_if, duv_scoreboard);
        scoreboard     = new(stimuli, duv_scoreboard, tb_eos);

        fork
            driver.run();
            reader.run();
            scoreboard.run();
        join_any

        wait (tb_eos.triggered) begin
            $display("[TB]  @%t: Signaled end of stimulation.", $time);
            scoreboard.print_stats();
        end
    end

endprogram

endmodule // trdb_tb
