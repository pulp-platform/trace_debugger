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
// Description: Testbench

import trdb_tb_pkg::*;

module trdb_tb
    (trace_debugger_if.tb tb_if);

    // run test like instantiate a module
    tb_run i_tb_run();

program automatic tb_run();
    Driver driver;
    Monitor monitor;
    mailbox #(Stimuli) mail;

    initial begin
        driver  = new(tb_if, mail);
        monitor = new(tb_if, mail);

        fork
            driver.run();
            monitor.run();
        join
    end

endprogram

endmodule // trdb_tb
