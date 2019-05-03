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
// Description: Testbench settings
package trdb_tb_pkg;

    import trdb_pkg::*;

    // stimuli file
    // const string default_stimuli_path       = "trdb/data/trdb_stimuli";
    // const string  stimuli_path      = "trdb/data/enqueue_delayed/build/pulpissimo-riscy/trdb_stimuli";
    // const string  stimuli_path      = "trdb/data/timer_oneshot/build/pulpissimo-riscy/trdb_stimuli";
    // const string  stimuli_path      = "trdb/data/uart_loopback/build/pulpissimo-riscy/trdb_stimuli";

    // test that is being run by default
    string default_stimuli_path = "trdb/data/coremark/build/pulpissimo-riscy/trdb_stimuli";
    // name of current test
    string testname;
                                         // clock and acquisition related settings
    const time CLK_PHASE_HI         = 5ns;
    const time CLK_PHASE_LO         = 5ns;
    const time CLK_PERIOD           = CLK_PHASE_HI + CLK_PHASE_LO;
    const time STIM_APPLICATION_DEL = CLK_PERIOD * 0.1;
    const time RESP_ACQUISITION_DEL = CLK_PERIOD * 0.9;
    const time RESET_DEL            = STIM_APPLICATION_DEL;

    const int  RESET_WAIT_CYCLES    = 4;

    typedef struct {
        logic      ivalid;
        logic      iexception;
        logic      interrupt;
        logic [CAUSELEN-1:0] cause;
        logic [XLEN-1:0]     tval;
        logic [PRIVLEN-1:0]  priv;
        logic [XLEN-1:0]     iaddr;
        logic [ILEN-1:0] instr;
        logic            compressed;
    } trdb_instr;

    typedef union packed {
        // TODO: we would like to have this parametrizable
        logic [127:0] bits;
        logic [4][31:0] slices;
    } trdb_packet;

    import "DPI-C" function void trdb_sv_alloc();
    import "DPI-C" function void trdb_sv_free();
    import "DPI-C" function void trdb_sv_set_full_address(input int full_address);
    import "DPI-C" function void trdb_sv_set_implicit_ret(input int implicit_ret);
    import "DPI-C" function void trdb_sv_feed_trace
        (input logic ivalid, iexception, interrupt,
         input logic [CAUSELEN-1:0]    cause, input logic [XLEN-1:0] tval,
         input logic [PRIVLEN-1:0]     priv, input logic [XLEN-1:0] iaddr,
         input logic [ILEN-1:0]        instr, input logic compressed,
         input int                     packet_max_len,
         output logic [PACKET_TOTAL-1:0] packet_bits, output logic packet_valid);

`include "../tb/stimuli.svh"
`include "../tb/response.svh"
`include "../tb/driver.svh"
`include "../tb/reader.svh"
`include "../tb/scoreboard.svh"

endpackage // trdb_tb_pkg
