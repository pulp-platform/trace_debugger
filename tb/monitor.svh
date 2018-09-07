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

    task run(ref logic tb_eos);

        // helper variables to parse packet
        automatic int bitcnt;
        automatic int max_reads;
        automatic int off;
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

    endtask

endclass // Monitor
