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
// Description: SV representation of a stimuli file

import trdb_pkg::*;

class Stimuli;
    // TODO: make struct instead
    logic                ivalid[$];
    logic                iexception[$];
    logic                interrupt[$];
    logic [CAUSELEN-1:0] cause[$];
    logic [XLEN-1:0]     tval[$];
    logic [PRIVLEN-1:0]  priv[$];
    logic [XLEN-1:0]     iaddr[$];
    logic [ILEN-1:0]     instr[$];
    logic                compressed[$];

    function new();
    endfunction

    function void print;

    endfunction

endclass // Stimuli
