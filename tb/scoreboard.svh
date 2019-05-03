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
// Description: Scoreboard

import trdb_pkg::*;

class Scoreboard;
    mailbox #(Response) duv_box;
    mailbox #(Response) gm_box;
    mailbox #(Stimuli) inbox;

    event   tb_eos;

    class Statistics;
        longint total_packets;
        longint packet_bad;

        function void print();
            $display("------------------------------------------------------");
            $display("------------------------------------------------------");
            $display("Simulation Results");
            $display("Test name: %s", testname);
            $display("FULL_ADDRESS: %0d IMPLICIT_RET: %0d",
                     ($test$plusargs("fulladdr") != 0),
                     ($test$plusargs("implicitret") != 0));
            $display("Good packets %0d/%0d", total_packets - packet_bad,
                     total_packets);
            if (packet_bad != 0)
                $display("TEST FAIL");
            else
                $display("TEST PASSED");
            $display("------------------------------------------------------");
            $display("------------------------------------------------------");
        endfunction;

    endclass // Statistics
    Statistics stats;


    function new(mailbox #(Stimuli) inbox, mailbox #(Response) duv_box,
                 event tb_eos);
        this.duv_box = duv_box;
        this.inbox   = inbox;
        this.tb_eos  = tb_eos;
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
        automatic logic [PACKET_TOTAL-1:0] packet_bits;
        automatic logic                  packet_valid;

        automatic trdb_packet packet;
        Response response;
        Stimuli stimuli;

        // reserve memory for golden model
        trdb_sv_alloc();

        // configure golden model
        trdb_sv_set_full_address($test$plusargs("fulladdr") != 0);
        trdb_sv_set_implicit_ret($test$plusargs("implicitret") != 0);

        // acquire stimuli
        inbox.get(stimuli);


        for(int i = stimuli.ivalid.size() - 1; i >= 0; i--) begin
            ivalid         = stimuli.ivalid[i];
            iexception     = stimuli.iexception[i];
            interrupt      = stimuli.interrupt[i];
            cause          = stimuli.cause[i];
            tval           = stimuli.tval[i];
            priv           = stimuli.priv[i];
            priv[2]        = 1'b1; // we force debug mode, stimuli from spike
                                   // traces don't know that bit
            iaddr          = stimuli.iaddr[i];
            instr          = stimuli.instr[i];
            compressed     = stimuli.compressed[i];

            packet_max_len = PACKET_TOTAL;
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

    task add_packet(logic [127:0] bits);
        trdb_packet packet;
        Response response;
        packet.bits = bits;
        response    = new(packet);
        gm_box.put(response);
    endtask

    // generated the response for the sw dump writes TODO: a bit hacky
    task generate_sw_dump();
        logic [3:0] len;
        logic [1:0] msgtype;
        len     = 4; // payload bytes after first byte
        msgtype = W_SOFTWARE;

        add_packet({32'ha0a0a0a0, msgtype,len});
        add_packet({32'hb0b0b0b0, msgtype,len});
        add_packet({32'hc0c0c0c0, msgtype,len});
        add_packet({32'hd0d0d0d0, msgtype,len});
        add_packet({32'hf0f0f0f0, msgtype,len});

    endtask;

    task run();
        automatic int packetcnt;
        automatic int packet_bytes;
        Response gm_response;
        Response duv_response;
        trdb_packet gm_packet;
        trdb_packet duv_packet;
        trdb_marker_e msgtype;
        int tmp = 1;


        gm_box = new();
        // generate responses
        run_gm();
        generate_sw_dump();

        stats     = new();
        packetcnt = 0;

        forever begin
            duv_box.get(duv_response);
            gm_box.get(gm_response);
            gm_packet  = gm_response.packet;
            duv_packet = duv_response.packet;
            stats.total_packets = packetcnt++;

            assert(gm_packet.bits[127:0] === duv_packet.bits[127:0]) begin
                msgtype = trdb_marker_e'{duv_packet.bits[5:4]};
                packet_bytes = duv_packet.bits[3:0] + 1;
                $display("[SCORE]: Packet with msgtype %s, length %0d,",
                         msgtype.name, packet_bytes,
                         "payload %h and number %0d ok",
                         duv_packet.bits, packetcnt);
            end else begin
                $error("[SCORE]: ERROR - Packet mismatch for number %0d\n",
                       packetcnt,
                       "Expected: %h\n", gm_packet.bits,
                       "Received: %h", duv_packet.bits);
                stats.packet_bad++;
            end
        end

    endtask

    task print_stats();
        Response gm_response;
        Response duv_response;
        trdb_packet gm_packet;
        trdb_packet duv_packet;
        trdb_marker_e msgtype;

        if (gm_box.num() > 0)
            $warning("[SCORE]  @%t: GM has %0d pending packets.", $time,
                      gm_box.num());
        if (duv_box.num() > 0) begin
            $warning("[SCORE]  @%t: DUV has %0d pending packets.",
                      $time, duv_box.num());
            while(duv_box.num() > 0) begin
                duv_box.try_get(duv_response);
                duv_packet = duv_response.packet;
                msgtype    = trdb_marker_e'{duv_packet.bits[5:4]};
                $display("[SCORE]  @%t: Remaning: %h", $time, duv_packet.bits);
                $display("[SCORE]  @%t: length: %0d msgtype: %s",
                         $time, duv_packet.bits[6:0], msgtype.name);
            end
        end

        stats.print();

   endtask

endclass // Scoreboard
