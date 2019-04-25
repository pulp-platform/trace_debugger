// Copyright 2018 Robert Balas
// Copyright 2019 ETH Zurich and University of Bologna.9 Robert Balas
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the "License"); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.
//
// Author: Robert Balas (balasr@iis.ee.ethz.ch)
// Description: Take packets and funnel them through a DATA_WIDTH bit interface



// TODO: doesn't do alignment yet, just zero fill empty words
// TODO: inserting double WORD header not done yet
module trdb_align8 import trdb_pkg::*; #(
    parameter ID = 1
) (
    input logic                          clk_i,
    input logic                          rst_ni,

    input logic [PACKET_LEN-1:0]         payload_bits_i,
    input logic [$clog2(PACKET_LEN)-1:0] payload_len_i,
    input logic                          valid_i,
    output logic                         grant_o,

    input logic                          flush_stream_i,
    output logic                         flush_confirm_o,


    output logic [BUS_DATA_WIDTH-1:0]    data_o,
    input logic                          grant_i,
    output logic                         valid_o
);

    localparam PACKET_MAX_BYTES = PACKET_TOTAL/8 + (PACKET_TOTAL % 8 != 0);
    localparam DATA_WIDTH       = BUS_DATA_WIDTH;
    localparam DATA_OFFSET_LEN  = BUS_DATA_WIDTH/8 + 1;
    localparam DATA_BYTES       = BUS_DATA_WIDTH/8;

    logic [DATA_WIDTH+PACKET_TOTAL-1:0]       padded_packet_bits;
    logic [$clog2(DATA_WIDTH+PACKET_TOTAL):0] low_ptr_d, low_ptr_q, high_ptr;

    // TODO: change hardcoding, use $clog
    // TODO: remove PACKER_HEADER_LEN, move upwards to packet emitter
    // TODO: this is a mess, clean up
    logic [PACKET_BYTE_HEADER_LEN-1:0]    payload_bytes;
    logic [$clog2(PACKET_LEN)-1:0]        packet_len_no_header;
    logic [$clog2(PACKET_TOTAL)-1:0]      effective_packet_len;
    logic [$clog2(PACKET_MAX_BYTES)-1:0]  packet_bytes;

    // for keeping track of the offset (carry over bytes) of residual
    logic [$clog2(DATA_OFFSET_LEN)-1:0]   offset_q, offset_d, offset_inv;
    // same as above, but in terms of bit position
    logic [$clog2(DATA_OFFSET_LEN)-1+3:0] offset_shifted, offset_inv_shifted;
    // TODO: change residual size
    logic [DATA_WIDTH-1:0]                data_q, data_d, residual_q, residual_d;
    logic                                 valid_d, valid_q;

    logic                                 update;

    enum logic [2:0] {Idle, Waitgrant, Stream, Full, Overfull} state_q, state_d;

    // TODO: this is not used for now, meant for multi core tracing
    logic [4:0]                          instance_id;

    assign instance_id = ID;

   // assign valid_o = valid_q;
    assign data_o = data_q;

    //outgoing handshake
    assign update = valid_o && grant_i;


    // strip off 4 custom bits
    assign packet_len_no_header = payload_len_i - 4;
    assign payload_bytes = (packet_len_no_header >> 3)
        + (packet_len_no_header[2:0] != 2'h0);

    assign padded_packet_bits = {{DATA_WIDTH{1'b0}},
                                 payload_bits_i,
                                 payload_bytes};

    // TODO: halfword, byte, nibble, bit alignment
    assign offset_shifted = offset_q << 3;
    assign offset_inv = DATA_BYTES - offset_q;
    assign offset_inv_shifted = offset_inv << 3;

    // packet length with header TODO: should rename other to payload
    assign effective_packet_len = payload_len_i + PACKET_BYTE_HEADER_LEN;
    // ceiling division
    assign packet_bytes = (effective_packet_len >> 3)
        + (effective_packet_len[2:0] != 2'h0);


    always_comb begin : align_bytes
        logic [15:0] offset;

        low_ptr_d       = low_ptr_q;
        valid_d         = '0;
        valid_o         = '0;
        data_d          = '0;
        grant_o         = '0;

        residual_d      = residual_q;
        offset_d        = offset_q;

        high_ptr        = '0;

        flush_confirm_o = '0;

        state_d         = state_q;

        if(valid_i) begin
            low_ptr_d  = low_ptr_q;
            high_ptr   = low_ptr_q + DATA_BYTES;

            // check if we can still output DATA_WIDTH bit words for the current
            // packet combine "carry" and current data, and save residual
            // for next word
            data_d     = (padded_packet_bits[low_ptr_q*8 +: DATA_WIDTH] << offset_shifted) | residual_q;
            if (update)
                residual_d = (padded_packet_bits[low_ptr_q*8 +: DATA_WIDTH] >> offset_inv_shifted);

            unique case (state_q)
            Idle: begin
                if (high_ptr < packet_bytes) begin
                    state_d = Stream;
                end else if (high_ptr == packet_bytes) begin
                    state_d = Full;
                end else if (high_ptr > packet_bytes) begin
                    state_d = Overfull;
                end
            end
            Stream: begin
                valid_o = '1;
                if (update) begin
                    state_d   = Idle;
                    low_ptr_d = high_ptr;
                end
            end
            Full: begin
                valid_o   = '1;
                if (update) begin
                    state_d   = Idle;
                    low_ptr_d = '0;
                    grant_o   = '1;
                end
            end
            Overfull: begin
                offset = ((packet_bytes % DATA_BYTES) + offset_q);

                if (offset == DATA_BYTES) begin
                    valid_o    = '1;
                    if (update)
                        residual_d = '0;

                end else if (offset > DATA_BYTES) begin
                    valid_o = '1;
                    if (update) begin
                        unique case (offset % DATA_BYTES)
                            0: residual_d = residual_d;
                            1: residual_d = residual_d & {1{8'hff}};
                            2: residual_d = residual_d & {2{8'hff}};
                            3: residual_d = residual_d & {3{8'hff}};
`ifdef TRDB_ARCH64
                            4: residual_d = residual_d & {4{8'hff}};
                            5: residual_d = residual_d & {5{8'hff}};
                            6: residual_d = residual_d & {6{8'hff}};
                            7: residual_d = residual_d & {7{8'hff}};
`endif
                        endcase
                    end

                end else if (offset < DATA_BYTES) begin
                    valid_o = '0;
                    unique case (offset)
                        0: residual_d = data_q; //TODO: need to mask this
                        1: residual_d = data_q & {1{8'hff}};
                        2: residual_d = data_q & {2{8'hff}};
                        3: residual_d = data_q & {3{8'hff}};
`ifdef TRDB_ARCH64
                        4: residual_d = data_q & {4{8'hff}};
                        5: residual_d = data_q & {5{8'hff}};
                        6: residual_d = data_q & {6{8'hff}};
                        7: residual_d = data_q & {7{8'hff}};
`endif
                    endcase
                    state_d   = Idle;
                    offset_d  = offset % DATA_BYTES;
                    //we are done with the current packet
                    low_ptr_d = '0;
                    // request the next packet
                    grant_o   = '1;
                end

                if (update) begin
                    state_d   = Idle;
                    offset_d  = offset % DATA_BYTES;
                    //we are done with the current packet
                    low_ptr_d = '0;
                    // request the next packet
                    grant_o   = '1;
                end

            end
            endcase // unique case (state_q)

        end else if(flush_stream_i) begin
            // check if we actually have something to flush from residual_d
            flush_confirm_o = '1;
            if(offset_d != 0) begin
                data_d  = residual_d;
                valid_d = '1;
            end
            // TODO: after flush user has to clear
        end
    end

    always_ff @(posedge clk_i, negedge rst_ni) begin
        if(~rst_ni) begin
            offset_q   <= '0;
            low_ptr_q  <= '0;
            residual_q <= '0;
            valid_q    <= '0;
            data_q     <= '0;
            state_q    <= Idle;
        end else begin
            offset_q   <= offset_d;
            low_ptr_q  <= low_ptr_d;
            residual_q <= residual_d;
            valid_q    <= valid_d;
            data_q     <= data_d;
            state_q    <= state_d;
        end
    end

endmodule // trdb_align8
