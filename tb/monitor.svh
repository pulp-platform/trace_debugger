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
    mailbox #(Stimuli) inbox;
    mailbox #(Response) outbox_gm;
    mailbox #(Response) outbox_duv;

    function new(virtual trace_debugger_if duv_if, mailbox #(Stimuli) inbox,
                 mailbox #(Response) outbox_gm,
                 mailbox #(Response) outbox_duv);
        this.duv_if     = duv_if;
        this.inbox      = inbox;
        this.outbox_duv = outbox_duv;
        this.outbox_gm  = outbox_gm;
    endfunction


    task run(ref logic tb_eos);
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

        // helper variables to parse packet
        automatic int bitcnt;
        automatic int max_reads;
        automatic int off;
        automatic trdb_packet packet;
        Response response;

        // get stimuli
        Stimuli stimuli;
        inbox.get(stimuli);

        // reserve memory for golden model
        trdb_sv_alloc();

        // run golden model TODO: put in seperate task
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
                               iaddr, instr, compressed, packet_max_len, packet_bits,
                               packet_valid);

            if(packet_valid) begin
                packet.bits = packet_bits;
                response    = new(packet);
                outbox_gm.put(response);
            end
        end
        $display("[MONITOR]@%t: Finished golden model computation, queued %d \
                packets.", $time, outbox_gm.num());

        forever begin: acquire
            @(posedge this.duv_if.clk_i);
            #STIM_APPLICATION_DEL;

            #(RESP_ACQUISITION_DEL - STIM_APPLICATION_DEL);
            if(tb_eos == 1'b1) begin
                $display("[MONITOR]@%t: Signaled end of simulation.", $time);
                break;
            end

            if(this.duv_if.packet_word_valid == 1'b1) begin
                if(off == 0) begin
                    //we are dealing with a new packet
                    packet.bits            = '0;
                    bitcnt                 = this.duv_if.packet_word[PACKET_HEADER_LEN-1:0];
                    max_reads              = (bitcnt + 32 - 1) / 32;
                    packet.slices[3 - off] = this.duv_if.packet_word;
                    off++;
                end else begin
                    packet.slices[3 - off] = this.duv_if.packet_word;
                    off++;
                end

                // next packet
                if(off == max_reads) begin
                    off      = 0;
                    response = new(packet);
                    outbox_duv.put(response);
                end
            end
        end

        repeat(10)
            @(posedge this.duv_if.clk_i);


        //cleanup
        trdb_sv_free();

    endtask

endclass // Monitor
