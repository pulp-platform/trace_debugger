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
// Description: Acquisition of output


class Monitor;

    virtual trace_debugger_if duv_if;
    mailbox #(Stimuli) mail;

    function new(virtual trace_debugger_if duv_if, mailbox #(Stimuli) mail);
        this.duv_if = duv_if;
        this.mail   = mail;
    endfunction

    class Statistics;

        function void print();
        endfunction;

    endclass // Statistics


    task run_golden_model();

    endtask


    task run();
        Statistics stats = new();
        // TODO: run golden model

        // TODO: read output of rtl model
        @(posedge this.duv_if.clk_i);
        #STIM_APPLICATION_DEL;

        repeat(10)
            @(posedge this.duv_if.clk_i);

    endtask

endclass // Monitor
