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
// Description: Acquisition of output


class Reader;

    virtual trace_debugger_if duv_if;
    mailbox #(Response) outbox_duv;

    function new(virtual trace_debugger_if duv_if,
                 mailbox #(Response) outbox_duv);
        this.duv_if     = duv_if;
        this.outbox_duv = outbox_duv;
    endfunction

    // old function that assumes that packets are packed in two words frames and
    // are always zero extended to fit into whole words
    task acquire_word2();
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
            @(this.duv_if.cb);

            if(this.duv_if.cb.packet_word_valid && this.duv_if.cb.grant) begin
                if(off == 0) begin
                    //we are dealing with a new packet
                    packet.bits = '0;
                    pbits = (wordmod == 0 ?
                             this.duv_if.cb.packet_word[$clog2(PACKET_LEN)-1+7:7]:
                             this.duv_if.cb.packet_word[$clog2(PACKET_LEN)-1:0]);

                    // every other words (wordmod == 0) has an additional 7 bits
                    totalbits = pbits + (pbits/(32+32-7)) * 7
                        + ((pbits % (32+32-7)) != 0) * 7
                        - (wordmod == 0 ? 0 : 7);

                    max_reads = (totalbits + 32 - 1) / 32;

                    if(wordmod == 0)  begin
                        packet.bits[packet_ptr+:32-7] = this.duv_if.cb.packet_word[31:7];
                        packet_ptr += 32-7;
                    end else begin
                        packet.bits[packet_ptr+:32] = this.duv_if.cb.packet_word;
                        packet_ptr += 32;
                    end
                    off++;

                end else begin
                    if(wordmod == 0)  begin
                        packet.bits[packet_ptr+:32-7] = this.duv_if.cb.packet_word[31:7];
                        packet_ptr += 32-7;
                    end else begin
                        packet.bits[packet_ptr+:32] = this.duv_if.cb.packet_word;
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

    task acquire_bytewise();
        logic [BUS_DATA_WIDTH-1:0] packet_word;
        logic [7:0]                packet_byte;
        int                        packet_byte_len;
        int                        effective_bits;
        int                        packet_byte_off;
        int                        packet_ptr;
        automatic trdb_packet      packet;
        Response                   response;

        int                        tmp = 1;

        forever begin: acquire
            @(this.duv_if.cb);

            if(this.duv_if.cb.packet_word_valid && this.duv_if.cb.grant) begin
                packet_word = this.duv_if.cb.packet_word;
                if($test$plusargs("debug"))
                    $display("[READER]@%t: slurping %h", $time, packet_word);

                for(int i = 0; i < BUS_DATA_WIDTH/8; i++) begin
                    packet_byte        = packet_word[i*8+:8];

                    // begin of a new packet
                    if(packet_byte_off == 0) begin
                        // clean slate
                        packet.bits    = '0;
                        // read payload length, assuming its less than 8 bits
                        effective_bits = packet_byte[PACKET_BYTE_HEADER_LEN-1:0]
                                         * 8
                                         + 8; //for the header

                        // a zero length packet can't be good
                        if(!(effective_bits > PACKET_BYTE_HEADER_LEN))
                           continue;

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

    task run();
        // acquire_word2();
        acquire_bytewise();

        repeat(10)
            @(this.duv_if.cb);

    endtask

endclass // Reader
