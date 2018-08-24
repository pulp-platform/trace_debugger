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
// Description: Compress instruction traces and filter them


import trdb_pkg::*;

module trace_debugger
    #(parameter ILEN = 32,
      parameter XLEN = 32,
      parameter PRIVLEN = 3, //TODO: check this
      parameter CAUSELEN = 5)
    (input logic                clk_i,
     input logic                rst_ni,

     input logic                ivalid_i,
     input logic                iexception_i,
     input logic                interrupt_i,
     input logic [CAUSELEN-1:0] cause_i,
     input logic [XLEN-1:0]     tval_i,
     input logic [PRIVLEN-1:0]  priv_i,
     input logic [XLEN-1:0]     iaddr_i,
     input logic [ILEN-1:0]     instr_i,
     input logic                compressed_i);

    // TODO: add dependencies for this
    // APB_BUS.Slave              apb_slave);

    // unused variables for a more extensive implementation
    // riscy doesn't have those features
    logic [XLEN-1:0]            lc_tval;
    logic [CONTEXTLEN-1:0]      tc_context;


    // we have three phases, called next cycle (nc), this cycle (tc) and next
    // cycle (nc), based on which we make decision whether we need to emit a
    // packet or not.
    logic [PRIVLEN-1:0]         nc_priv, tc_priv;
    logic                       nc_privchange, tc_privchange;
    logic                       nc_exception, lc_exception;
    logic                       lc_u_discontinuity;
    logic                       tc_first_qualified;
    logic                       tc_is_branch;
    logic                       tc_branch_taken, nc_branch_taken;
    logic                       tc_qualified, lc_qualified;
    logic                       tc_compressed;
    logic [XLEN-1:0]            tc_iaddr, nc_iaddr;

    // pass delayed data
    logic [CAUSELEN-1:0]        lc_cause;
    logic                       lc_interupt;


    // registers to hold onto the input data for a few phases, mostly one
    logic                       interrupt0_q, interrupt0_d;
    logic                       interrupt1_q, interruptj_d;
    logic [CAUSELEN-1:0]        cause0_q, cause0_d;
    logic [CAUSELEN-1:0]        cause1_q, cause1_d;
    logic [PRIVLEN-1:0]         priv0_q, priv0_d;
    logic                       privchange0_q, privchange0_d;
    logic                       exception0_q, exception0_d,
                                exception1_q, exception1_d;
    logic                       u_discontinuity0_q, u_discontinuity0_d,
                                u_discontinuity1_q, u_discontinuity1_d;
    logic                       is_branch0_q, is_branch0_d;
    logic                       compressed0_q, compressed0_d;
    logic [XLEN-1:0]            iaddr0_q, iaddr0_d;
    logic                       qualified0_q, qualified0_d, qualified1_q,
                                qualified1_d;

    // variables for the branch map
    logic                       branch_map_flush;
    logic                       branch_map_full;
    logic                       branch_map_empty;
    logic [30:0]                branch_map;
    logic [4:0]                 branch_map_cnt;

    // variables for the packet generation
    logic                       packet_valid;
    trdb_format_t               packet_format;
    trdb_subformat_t            packet_subformat;

    // generated packet

    logic [PACKET_LEN-1:0]      packet_bits;
    logic [6:0]                 packet_len;
    logic                       packet_gen_valid;


    // manage their input and outputs
    assign interrupt0_d = interrupt_i;
    assign interrupt1_d = interrupt1_q;
    assign cause0_d = cause_i;
    assign cause1_d = cause0_q;
    assign priv0_d = priv_i;
    // assign privchange0_d = ;
    assign exception0_d = iexception_i;
    // assign u_discontinuity0_d (below)
    // assign is_branch0_d (below)
    assign compressed0_d = compressed_i;
    assign iaddr0_d = iaddr_i;
    // assign qualified0_q =

    assign exception1_d = exception0_q;
    assign u_discontinuity1_d = u_discontinuity0_q;
    assign is_branch1_d = is_branch0_q;
    assign qualified1_d = qualified0_q;

    // Hook phase related variables up to proper register
    assign nc_priv = priv0_d;
    assign tc_priv = priv0_q;
    assign lc_interrupt = interrupt1_q;
    assign lc_cause = cause1_q;
    assign nc_privchange = privchange0_d;
    assign tc_privchange = privchange0_q;
    assign nc_exception = exception0_d;
    assign lc_exception = exception1_q;
    assign lc_u_discontinuity = u_discontinuity1_q;
    // assign tc_first_qualified (below)
    assign tc_is_branch = is_branch0_q;
    // assign tc_branch_taken (below)
    assign tc_qualified = qualified0_q;
    assign lc_qualified = qualified1_q;
    assign tc_compressed = compressed0_q;
    assign nc_iaddr = iaddr0_d;
    assign tc_iaddr = iaddr0_q;

    // decide whether a privilege change occured
    always_comb begin
        privchange0_d = (tc_priv != nc_priv);
    end

    // decide whether we have a branch instruction
    // beq, bne, blt, bge, bltu, bgeu, p_bneimm, p_beqimm
    always_comb begin: is_branch
        is_branch0_d
            = ((instr_i & MASK_BEQ)      == MATCH_BEQ) &
              ((instr_i & MASK_BNE)      == MATCH_BNE) &
              ((instr_i & MASK_BLT)      == MATCH_BLT) &
              ((instr_i & MASK_BGE)      == MATCH_BGE) &
              ((instr_i & MASK_BLTU)     == MATCH_BLTU) &
              ((instr_i & MASK_BGEU)     == MATCH_BGEU) &
              ((instr_i & MASK_P_BNEIMM) == MATCH_P_BNEIMM) &
              ((instr_i & MASK_P_BEQIMM) == MATCH_P_BEQIMM);
    end

    // decide whether we have a unpredictable discontinuity
    // jalr, mret, sret, uret
    always_comb begin: is_discontinuity
        u_discontinuity0_d
            = ((instr_i & MASK_JALR) == MATCH_JALR) &
              ((instr_i & MASK_MRET) == MATCH_MRET) &
              ((instr_i & MASK_SRET) == MATCH_SRET) &
              ((instr_i & MASK_URET) == MATCH_URET);
    end

    // figure out whether we are dealing with the first qualified instruction
    always_comb begin: is_qualified
        tc_first_qualified = !lc_qualified && tc_qualified;
    end

    // is this branch taken?
    // TODO: change to tc_compressed? 2 :4
    always_comb begin: is_branch_taken
        tc_branch_taken = tc_compressed ?
                          !(tc_iaddr + 2 == nc_iaddr):
                          !(tc_iaddr + 4 == nc_iaddr);
    end

    // update branch map, careful there is a direct combinatorial path
    trdb_branch_map i_trdb_branch_map
        (.clk_i(clk_i),
         .rst_ni(rst_ni),
         .valid_i(tc_is_branch),
         .branch_taken_i(tc_branch_taken),
         .flush_i(branch_map_flush),
         .map_o(branch_map),
         .branches_o(branch_map_cnt),
         .is_full_o(branch_map_full),
         .is_empty_o(branch_map_empty));


    trdb_priority i_trdb_priority
        (.clk_i(clk_i),
         .rst_ni(rst_ni),

         .lc_exception_i(lc_exception),
         //input logic lc_emitted_exception_sync (hack)

         .tc_first_qualified_i(tc_first_qualified),
         //input logic  tc_unhalted,
         .tc_privchange_i(tc_privchange),
         //input logic resync & branch_map_cnt

         .lc_u_discontinuity_i(lc_u_discontinuity),

         // input logic  resync
         // input logic  branch_map_nonempty,

         //input logic  nc_halt,
         .nc_exception_i(nc_exception),
         .nc_privchange_i(nc_privchange),
         //input logic  nc_qualified,

         .branch_map_full_i(branch_map_full),

         //input logic  tc_context_change,

         .branch_map_empty_i(branch_map_empty),
         .valid_o(packet_valid),
         .packet_format_o(packet_format),
         .packet_subformat_o(packet_subformat));

    trdb_packet_emitter i_trdb_packet_emitter
        (.clk_i(clk_i),
         .rst_ni(rst_ni),
         .packet_format_i(packet_format),
         .packet_subformat_i(packet_subformat),
         .valid_i(packet_valid),
         .interrupt_i(lc_interrupt), // get the value of the trapped instr
         .cause_i(lc_cause),
         .tval_i(lc_tval),
         .priv_i(tc_priv),
         .iaddr_i(tc_iaddr),
         .context_i(tc_context),
         .branch_map_i(branch_map),
         .branch_map_cnt_i(branch_map_cnt),
         .branch_map_empty_i(branch_map_empty),
         .is_branch_i(tc_is_branch),

         .packet_bits_o(packet_bits),
         .packet_len_o(packet_len),
         .valid_o(packet_gen_valid)
         );


    // TODO: assert that we are not dealing with an unsupported instruction
    always_ff @(posedge clk_i, negedge rst_ni) begin
        if(~rst_ni) begin
            interrupt0_q       <= '0;
            interrupt1_q       <= '0;
            cause0_q           <= '0;
            cause1_q           <= '0;
            priv0_q            <= '0;
            exception0_q       <= '0;
            exception1_q       <= '0;
            u_discontinuity0_q <= '0;
            u_discontinuity1_q <= '0;
            is_branch0_q       <= '0;
            compressed0_q      <= '0;
            iaddr0_q           <= '0;
            qualified0_q       <= '0;
            qualified1_q       <= '0;
        end else begin
            interrupt0_q       <= interrupt0_d;
            interrupt1_q       <= interrupt1_d;
            cause0_q           <= cause0_d;
            cause1_q           <= cause1_d;
            priv0_q            <= priv0_d;
            privchange0_q      <= privchange0_d;
            exception0_q       <= exception0_d;
            exception1_q       <= exception1_d;
            u_discontinuity0_q <= u_discontinuity0_d;
            u_discontinuity1_q <= u_discontinuity1_d;
            is_branch0_q       <= is_branch0_d;
            compressed0_q      <= compressed0_d;
            iaddr0_q           <= iaddr0_d;
            qualified0_q       <= qualified0_d;
            qualified1_q       <= qualified1_d;
        end
    end
endmodule // trace_debugger
