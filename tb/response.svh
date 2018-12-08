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
// Description: SV representation of a response (i.e. packets)

import trdb_pkg::*;

class Response;

    trdb_packet packet;

    function new(trdb_packet packet);
        this.packet = packet;
    endfunction

    function void print;
    //     automatic logic [1:0] format = packet[8:7];
    //     case(format) begin
    //         F_BRANCH_DIFF:
    //             $display("F_BRANCH_DIFF not implemented.");
    //         F_BRANCH_FULL:
    //             $write("PACKET ");
    //         format == F_BRANCH_FULL ? $write("0: F_BRANCH_FULL\n")
    //             : $write("1: F_BRANCH_DIFF\n");
    //         $write(stream, "    branches  : %" "\n", packet->branches);
    //         $write(stream, "    branch_map: 0x%" PRIx32 "\n", packet->branch_map);
    //         $write(stream, "    address   : 0x%" PRIx32 "\n", packet->address);
    //     /* TODO: that special full branch map behaviour */
    //     end

    // case F_ADDR_ONLY:
    //     fprintf(stream, "PACKET 2: F_ADDR_ONLY\n");
    //     fprintf(stream, "    address   : 0x%" PRIx32 "\n", packet->address);
    //     break;
    // case F_SYNC:
    //     fprintf(stream, "PACKET 3: F_SYNC\n");
    //     const char *subf[4];
    //     subf[0] = "SF_START";
    //     subf[1] = "SF_EXCEPTION";
    //     subf[2] = "SF_CONTEXT";
    //     subf[3] = "RESERVED";
    //     fprintf(stream, "    subformat : %s\n", subf[packet->subformat]);

    //     /* TODO fix this */
    //     fprintf(stream, "    context   :\n");
    //     fprintf(stream, "    privilege : 0x%" PRIx32 "\n", packet->privilege);
    //     if (packet->subformat == SF_CONTEXT)
    //         return;

    //     fprintf(stream, "    branch    : %s\n",
    //             packet->branch ? "true" : "false");
    //     fprintf(stream, "    address   : 0x%" PRIx32 "\n", packet->address);
    //     if (packet->subformat == SF_START)
    //         return;

    //     fprintf(stream, "    ecause    : 0x%" PRIx32 "\n", packet->ecause);
    //     fprintf(stream, "    interrupt : %s\n",
    //             packet->interrupt ? "true" : "false");
    //     fprintf(stream, "    tval      : 0x%" PRIx32 "\n", packet->tval);
    //     /* SF_EXCEPTION */
    // end

    endfunction

endclass // Response
