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

    task run(ref logic tb_eos);
        automatic int packetcnt;
        Statistics stats;
        Response gm_response;
        Response duv_response;
        trdb_packet gm_packet;
        trdb_packet duv_packet;

        stats     = new();
        packetcnt = 0;

        forever begin
            if(tb_eos == 1'b1) begin
                //TODO: do statistics report
                $display("[SCORE]  @%t: Signaled end of stimulation.", $time);
                if(gm_box.num() > 0)
                    $display ("[SCORE]  @%t: GM has %d pending packets.", $time,
                              gm_box.num());
                if(duv_box.num() > 0)
                    $display ("[SCORE]  @%t: DUV has %d pending packets.",
                              $time, duv_box.num());

                break;
            end

            gm_box.get(gm_response);
            duv_box.get(duv_response);
            gm_packet  = gm_response.packet;
            duv_packet = duv_response.packet;
            packetcnt++;
            //TODO: remove this range when packet length is fixed
            if(gm_packet.bits[127:7] != duv_packet.bits[127:7]) begin
                $display("[SCORE]  @%t: ERROR - Packet mismatch for number %d",
                         $time, packetcnt);
                $display("[SCORE]  @%t: Expected: %h", $time, gm_packet.bits);
                $display("[SCORE]  @%t: Received: %h", $time, duv_packet.bits);
            end

        end
    endtask

endclass // Scoreboard
