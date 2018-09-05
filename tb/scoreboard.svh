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
// Description: Scoreboard

import trdb_pkg::*;

class Scoreboard;
    mailbox #(Response) duv_box;
    mailbox #(Response) gm_box;


    class Statistics;

        function void print();
        endfunction;

    endclass // Statistics

    function new(mailbox #(Response) duv_box, mailbox #(Response) gm_box);
        this.duv_box = duv_box;
        this.gm_box  = gm_box;
    endfunction

    function void print;
    endfunction

    task run();
        Response gm_response;
        Response duv_response;
        forever begin
            gm_box.get(gm_response);
            duv_box.get(duv_response);
            $display("got something from both...");
        end
    endtask

endclass // Scoreboard
