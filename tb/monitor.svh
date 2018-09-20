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
    mailbox #(Response) outbox_duv;

    function new(virtual trace_debugger_if duv_if,
                 mailbox #(Response) outbox_duv);
        this.duv_if     = duv_if;
        this.outbox_duv = outbox_duv;
    endfunction

    // old function that assumes that packets are packed in two words frames and
    // are always zero extended to fit into whole words
    task acquire_word2(ref logic tb_eos);
        // helper variables to parse packet
        automatic int pbits;
        automatic int totalbits;
        automatic int max_reads;
        automatic int off;
        automatic int wordmod;
        automatic int packet_ptr;
        automatic trdb_packet packet;
        Response response;

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
                    packet.bits = '0;
                    pbits = (wordmod == 0 ?
                             this.duv_if.packet_word[PACKET_HEADER_LEN-1+7:7]:
                             this.duv_if.packet_word[PACKET_HEADER_LEN-1:0]);

                    // every other words (wordmod == 0) has an additional 7 bits
                    totalbits = pbits + (pbits/(32+32-7)) * 7
                        + ((pbits % (32+32-7)) != 0) * 7
                        - (wordmod == 0 ? 0 : 7);

                    max_reads = (totalbits + 32 - 1) / 32;

                    if(wordmod == 0)  begin
                        packet.bits[packet_ptr+:32-7] = this.duv_if.packet_word[31:7];
                        packet_ptr += 32-7;
                    end else begin
                        packet.bits[packet_ptr+:32] = this.duv_if.packet_word;
                        packet_ptr += 32;
                    end
                    off++;

                end else begin
                    if(wordmod == 0)  begin
                        packet.bits[packet_ptr+:32-7] = this.duv_if.packet_word[31:7];
                        packet_ptr += 32-7;
                    end else begin
                        packet.bits[packet_ptr+:32] = this.duv_if.packet_word;
                        packet_ptr += 32;
                    end
                    off++;
                end

                // next packet
                if(off == max_reads) begin
                    packet_ptr  = 0;
                    off      = 0;
                    response = new(packet);
                    outbox_duv.put(response);
                end

                // emulate word state of streamer
                wordmod ++;
                wordmod %= 2;
            end
        end

    endtask

    task acquire_bytewise(ref logic tb_eos);
        logic [31:0] packet_word;
        logic [7:0]  packet_byte;
        int          packet_byte_len;
        int          effective_bits;
        int          packet_byte_off;
        int          packet_ptr;
        automatic trdb_packet packet;
        Response response;

        forever begin: acquire
            @(posedge this.duv_if.clk_i);
            #STIM_APPLICATION_DEL;

            #(RESP_ACQUISITION_DEL - STIM_APPLICATION_DEL);
            if(tb_eos == 1'b1) begin
                $display("[MONITOR]@%t: Signaled end of simulation.", $time);
                break;
            end

            if(this.duv_if.packet_word_valid == 1'b1) begin
                packet_word = this.duv_if.packet_word;
                for(int i = 0; i < 4; i++) begin
                    packet_byte        = packet_word[i*8+:8];

                    // begin of a new packet
                    if(packet_byte_off == 0) begin
                        // clean slate
                        packet.bits    = '0;
                        // read payload length, assuming its less than 8 bits
                        effective_bits = packet_byte[PACKET_HEADER_LEN-1:0]
                                         + PACKET_HEADER_LEN;
                        packet_byte_len
                            = effective_bits / 8
                              + ((effective_bits % 8) != 0 ? 1 : 0);

                        packet.bits[packet_byte_off*8+:8] = packet_byte;
                        packet_byte_off++;
                    end else begin
                        packet.bits[packet_byte_off*8+:8] = packet_byte;
                        packet_byte_off++;
                    end

                    if(packet_byte_off == packet_byte_len) begin
                        packet_byte_off = 0;
                        response = new(packet);
                        outbox_duv.put(response);
                    end
                end
            end
        end
    endtask

    task run(ref logic tb_eos);
        // acquire_word2(tb_eos);
        acquire_bytewise(tb_eos);

        repeat(10)
            @(posedge this.duv_if.clk_i);

    endtask

endclass // Monitor
