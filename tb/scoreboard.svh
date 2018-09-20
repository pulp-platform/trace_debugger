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
    mailbox #(Stimuli) inbox;


    class Statistics;
        longint total_packets;
        longint packet_bad;

        function void print();
            $display("------------------------------------------------------");
            $display("------------------------------------------------------");
            $display("Simulation Results");
            $display("Good packets %0d/%0d", total_packets - packet_bad,
                     total_packets);
            $display("------------------------------------------------------");
            $display("------------------------------------------------------");
        endfunction;

    endclass // Statistics

    function new(mailbox #(Stimuli) inbox, mailbox #(Response) duv_box);
        this.duv_box = duv_box;
        this.inbox = inbox;
    endfunction


    function void print;
    endfunction


    task run_gm();
        automatic logic                ivalid;
        automatic logic                iexception;
        automatic logic                interrupt;
        automatic logic [CAUSELEN-1:0] cause;
        automatic logic [XLEN-1:0]     tval;
        automatic logic [PRIVLEN-1:0]  priv;
        automatic logic [XLEN-1:0]     iaddr;
        automatic logic [ILEN-1:0]     instr;
        automatic logic                compressed;

        automatic int packet_max_len;
        automatic logic [PACKET_LEN-1:0] packet_bits;
        automatic logic                  packet_valid;

        automatic trdb_packet packet;
        Response response;
        Stimuli stimuli;

        // reserve memory for golden model
        trdb_sv_alloc();
        gm_box = new();

        // acquire stimuli
        inbox.get(stimuli);


        for(int i = stimuli.ivalid.size() - 1; i >= 0; i--) begin
            ivalid         = stimuli.ivalid[i];
            iexception     = stimuli.iexception[i];
            interrupt      = stimuli.interrupt[i];
            cause          = stimuli.cause[i];
            tval           = stimuli.tval[i];
            priv           = stimuli.priv[i];
            iaddr          = stimuli.iaddr[i];
            instr          = stimuli.instr[i];
            compressed     = stimuli.compressed[i];

            packet_max_len = PACKET_LEN;
            trdb_sv_feed_trace(ivalid, iexception, interrupt, cause, tval, priv,
                               iaddr, instr, compressed, packet_max_len,
                               packet_bits, packet_valid);

            if(packet_valid) begin
                packet.bits = packet_bits;
                response    = new(packet);
                gm_box.put(response);
            end
        end
        $display("[SCORE]  @%t: Finished golden model computation, queued %d \
                packets.", $time, gm_box.num());

        //cleanup
        trdb_sv_free();

    endtask


    task run(ref logic tb_eos);
        automatic int packetcnt;
        Statistics stats;
        Response gm_response;
        Response duv_response;
        trdb_packet gm_packet;
        trdb_packet duv_packet;

        // generate responses
        run_gm();

        stats     = new();
        packetcnt = 0;

        // since we pre-generated all responses we can just iterate on it to
        // finish
        while (gm_box.num() > 0) begin
            if(tb_eos == 1'b1) begin
                $display("[SCORE]  @%t: Signaled end of stimulation.", $time);
                break;
            end

            gm_box.get(gm_response);
            duv_box.get(duv_response);
            gm_packet  = gm_response.packet;
            duv_packet = duv_response.packet;
            stats.total_packets = packetcnt++;
            //TODO: remove this range when packet length is fixed
            if(gm_packet.bits[127:7] != duv_packet.bits[127:7]) begin
                $display("[SCORE]  @%t: ERROR - Packet mismatch for number %0d",
                         $time, packetcnt);
                $display("[SCORE]  @%t: Expected: %h", $time, gm_packet.bits);
                $display("[SCORE]  @%t: Received: %h", $time, duv_packet.bits);
                stats.packet_bad++;
            end else begin
                $display("[SCORE] @%t: Packet number %0d ok", $time, packetcnt);
            end
        end

        if(gm_box.num() > 0)
            $display ("[SCORE]  @%t: GM has %0d pending packets.", $time,
                      gm_box.num());
        if(duv_box.num() > 0)
            $display ("[SCORE]  @%t: DUV has %0d pending packets.",
                      $time, duv_box.num());

        stats.print();

    endtask

endclass // Scoreboard
