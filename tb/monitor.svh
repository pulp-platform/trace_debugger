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


    task run();
        automatic logic                ivalid;
        automatic logic                iexception;
        automatic logic                interrupt;
        automatic logic [CAUSELEN-1:0] cause;
        automatic logic [XLEN-1:0]     tval;
        automatic logic [PRIVLEN-1:0]  priv;
        automatic logic [XLEN-1:0]     iaddr;
        automatic logic [ILEN-1:0]     instr;
        automatic logic                compressed;

        Statistics stats = new();

        // get stimuli
        Stimuli stimuli;
        mail.get(stimuli);



        // // run golden model
        // while(stimuli.ivalid.size() > 0) begin
        //     ivalid     = stimuli.ivalid.pop_back();
        //     iexception = stimuli.iexception.pop_back();
        //     interrupt  = stimuli.interrupt.pop_back();
        //     cause      = stimuli.cause.pop_back();
        //     tval       = stimuli.tval.pop_back();
        //     priv       = stimuli.priv.pop_back();
        //     iaddr      = stimuli.iaddr.pop_back();
        //     instr      = stimuli.instr.pop_back();
        //     compressed = stimuli.compressed.pop_back();

        // end

        // TODO: read output of rtl model
        @(posedge this.duv_if.clk_i);
        #STIM_APPLICATION_DEL;

        #(RESP_ACQUISITION_DEL - STIM_APPLICATION_DEL);

        repeat(10)
            @(posedge this.duv_if.clk_i);

    endtask

endclass // Monitor
