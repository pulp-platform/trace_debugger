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
// Description: Connect interface to DUV

module trace_debugger_wrapper
    (trace_debugger_if duv_if);

    trace_debugger i_trace_debugger
        (.clk_i(duv_if.clk_i),
         .rst_ni(duv_if.rst_ni),
         .test_mode_i(duv_if.test_mode_i),
         .ivalid_i(duv_if.ivalid),
         .iexception_i(duv_if.iexception),
         .interrupt_i(duv_if.interrupt),
         .cause_i(duv_if.cause),
         .tval_i(duv_if.tval),
         .priv_i(duv_if.priv),
         .iaddr_i(duv_if.iaddr),
         .instr_i(duv_if.instr),
         .compressed_i(duv_if.compressed),
         .apb_slave(duv_if.apb_bus),
         .packet_word_o(duv_if.packet_word),
         .packet_word_valid_o(duv_if.packet_word_valid),
         .grant_i(duv_if.grant));

endmodule // trace_debugger_wrapper
