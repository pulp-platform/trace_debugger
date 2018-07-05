onerror {resume}
quietly WaveActivateNextPane {} 0
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/clk_i
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/rst_ni
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/clock_en_i
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/test_en_i
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/fregfile_disable_i
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/boot_addr_i
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/core_id_i
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/cluster_id_i
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/instr_req_o
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/instr_gnt_i
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/instr_rvalid_i
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/instr_addr_o
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/instr_rdata_i
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/data_req_o
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/data_gnt_i
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/data_rvalid_i
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/data_we_o
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/data_be_o
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/data_addr_o
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/data_wdata_o
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/data_rdata_i
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/data_err_i
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/apu_master_req_o
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/apu_master_ready_o
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/apu_master_gnt_i
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/apu_master_op_o
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/apu_master_type_o
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/apu_master_flags_o
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/apu_master_valid_i
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/apu_master_result_i
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/apu_master_flags_i
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/irq_i
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/irq_id_i
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/irq_ack_o
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/irq_id_o
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/irq_sec_i
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/sec_lvl_o
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/debug_req_i
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/debug_gnt_o
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/debug_rvalid_o
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/debug_addr_i
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/debug_we_i
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/debug_wdata_i
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/debug_rdata_o
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/debug_halted_o
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/debug_halt_i
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/debug_resume_i
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/clk_i
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/controller_i/pipe_flush_i
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/is_decoding
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/iexception_o
add wave -noupdate -expand -group Core -color Magenta /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/interrupt_o
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_valid
add wave -noupdate -expand -group Core -color White /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ivalid_o
add wave -noupdate -expand -group Core -color Orchid /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/instr_o
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/cause_o
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/tval_o
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/priv_o
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/iaddr_o
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/fetch_enable_i
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/core_busy_o
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ext_perf_counters_i
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/is_hwlp_id
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/hwlp_dec_cnt_id
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/instr_valid_id
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/instr_rdata_id
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/is_compressed_id
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/illegal_c_insn_id
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/pc_if
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/pc_id
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/clear_instr_valid
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/pc_set
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/pc_mux_id
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/exc_pc_mux_id
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/exc_cause
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/trap_addr_mux
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/lsu_load_err
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/lsu_store_err
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/useincr_addr_ex
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/data_misaligned
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/mult_multicycle
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/jump_target_id
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/jump_target_ex
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/branch_in_ex
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/branch_decision
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ctrl_busy
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_busy
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/lsu_busy
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/apu_busy
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/pc_ex
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/alu_en_ex
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/alu_operator_ex
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/alu_operand_a_ex
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/alu_operand_b_ex
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/alu_operand_c_ex
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/bmask_a_ex
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/bmask_b_ex
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/imm_vec_ext_ex
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/alu_vec_mode_ex
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/mult_operator_ex
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/mult_operand_a_ex
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/mult_operand_b_ex
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/mult_operand_c_ex
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/mult_en_ex
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/mult_sel_subword_ex
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/mult_signed_mode_ex
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/mult_imm_ex
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/mult_dot_op_a_ex
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/mult_dot_op_b_ex
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/mult_dot_op_c_ex
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/mult_dot_signed_ex
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/fpu_op_ex
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/fprec_csr
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/frm_csr
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/fflags
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/fflags_csr
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/fflags_we
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/apu_en_ex
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/apu_type_ex
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/apu_flags_ex
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/apu_op_ex
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/apu_lat_ex
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/apu_waddr_ex
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/apu_read_regs
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/apu_read_regs_valid
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/apu_read_dep
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/apu_write_regs
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/apu_write_regs_valid
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/apu_write_dep
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/perf_apu_type
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/perf_apu_cont
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/perf_apu_dep
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/perf_apu_wb
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/regfile_waddr_ex
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/regfile_we_ex
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/regfile_waddr_fw_wb_o
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/regfile_we_wb
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/regfile_wdata
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/regfile_alu_waddr_ex
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/regfile_alu_we_ex
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/regfile_alu_waddr_fw
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/regfile_alu_we_fw
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/regfile_alu_wdata_fw
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/csr_access_ex
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/csr_op_ex
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/mtvec
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/utvec
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/csr_access
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/csr_op
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/csr_addr
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/csr_addr_int
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/csr_rdata
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/csr_wdata
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/current_priv_lvl
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/data_we_ex
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/data_type_ex
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/data_sign_ext_ex
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/data_reg_offset_ex
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/data_req_ex
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/data_load_event_ex
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/data_misaligned_ex
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/lsu_rdata
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/halt_if
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_ready
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_ready
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_valid
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_valid
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/wb_valid
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/lsu_ready_ex
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/lsu_ready_wb
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/apu_ready_wb
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/instr_req_int
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/m_irq_enable
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/u_irq_enable
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/csr_irq_sec
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/epc
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/csr_save_cause
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/csr_save_if
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/csr_save_id
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/csr_cause
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/csr_restore_mret_id
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/csr_restore_uret_id
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/hwlp_start
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/hwlp_end
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/hwlp_cnt
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/csr_hwlp_regid
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/csr_hwlp_we
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/csr_hwlp_data
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/dbg_settings
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/dbg_req
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/dbg_ack
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/dbg_stall
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/dbg_trap
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/dbg_reg_rreq
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/dbg_reg_raddr
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/dbg_reg_rdata
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/dbg_reg_wreq
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/dbg_reg_waddr
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/dbg_reg_wdata
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/dbg_csr_req
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/dbg_csr_addr
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/dbg_csr_we
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/dbg_csr_wdata
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/dbg_jump_addr
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/dbg_jump_req
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/perf_imiss
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/perf_jump
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/perf_jr_stall
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/perf_ld_stall
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/core_ctrl_firstfetch
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/core_busy_int
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/core_busy_q
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/is_interrupt
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/clk
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/clock_en
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/dbg_busy
add wave -noupdate -expand -group Core /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/sleeping
add wave -noupdate -expand -group {IF Stage} -expand -group {Hwlp Ctrl} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/hwloop_controller_i/current_pc_i
add wave -noupdate -expand -group {IF Stage} -expand -group {Hwlp Ctrl} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/hwloop_controller_i/hwlp_start_addr_i
add wave -noupdate -expand -group {IF Stage} -expand -group {Hwlp Ctrl} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/hwloop_controller_i/hwlp_end_addr_i
add wave -noupdate -expand -group {IF Stage} -expand -group {Hwlp Ctrl} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/hwloop_controller_i/hwlp_counter_i
add wave -noupdate -expand -group {IF Stage} -expand -group {Hwlp Ctrl} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/hwloop_controller_i/hwlp_dec_cnt_o
add wave -noupdate -expand -group {IF Stage} -expand -group {Hwlp Ctrl} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/hwloop_controller_i/hwlp_dec_cnt_id_i
add wave -noupdate -expand -group {IF Stage} -expand -group {Hwlp Ctrl} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/hwloop_controller_i/hwlp_jump_o
add wave -noupdate -expand -group {IF Stage} -expand -group {Hwlp Ctrl} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/hwloop_controller_i/hwlp_targ_addr_o
add wave -noupdate -expand -group {IF Stage} -expand -group {Hwlp Ctrl} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/hwloop_controller_i/pc_is_end_addr
add wave -noupdate -expand -group {IF Stage} -expand -group {Hwlp Ctrl} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/hwloop_controller_i/j
add wave -noupdate -expand -group {IF Stage} -expand -group Prefetch -group FIFO /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/prefetch_32/prefetch_buffer_i/fifo_i/clk
add wave -noupdate -expand -group {IF Stage} -expand -group Prefetch -group FIFO /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/prefetch_32/prefetch_buffer_i/fifo_i/rst_n
add wave -noupdate -expand -group {IF Stage} -expand -group Prefetch -group FIFO /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/prefetch_32/prefetch_buffer_i/fifo_i/clear_i
add wave -noupdate -expand -group {IF Stage} -expand -group Prefetch -group FIFO /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/prefetch_32/prefetch_buffer_i/fifo_i/in_addr_i
add wave -noupdate -expand -group {IF Stage} -expand -group Prefetch -group FIFO /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/prefetch_32/prefetch_buffer_i/fifo_i/in_rdata_i
add wave -noupdate -expand -group {IF Stage} -expand -group Prefetch -group FIFO /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/prefetch_32/prefetch_buffer_i/fifo_i/in_valid_i
add wave -noupdate -expand -group {IF Stage} -expand -group Prefetch -group FIFO /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/prefetch_32/prefetch_buffer_i/fifo_i/in_ready_o
add wave -noupdate -expand -group {IF Stage} -expand -group Prefetch -group FIFO /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/prefetch_32/prefetch_buffer_i/fifo_i/in_replace2_i
add wave -noupdate -expand -group {IF Stage} -expand -group Prefetch -group FIFO /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/prefetch_32/prefetch_buffer_i/fifo_i/in_is_hwlp_i
add wave -noupdate -expand -group {IF Stage} -expand -group Prefetch -group FIFO /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/prefetch_32/prefetch_buffer_i/fifo_i/out_valid_o
add wave -noupdate -expand -group {IF Stage} -expand -group Prefetch -group FIFO /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/prefetch_32/prefetch_buffer_i/fifo_i/out_ready_i
add wave -noupdate -expand -group {IF Stage} -expand -group Prefetch -group FIFO /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/prefetch_32/prefetch_buffer_i/fifo_i/out_rdata_o
add wave -noupdate -expand -group {IF Stage} -expand -group Prefetch -group FIFO /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/prefetch_32/prefetch_buffer_i/fifo_i/out_addr_o
add wave -noupdate -expand -group {IF Stage} -expand -group Prefetch -group FIFO /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/prefetch_32/prefetch_buffer_i/fifo_i/unaligned_is_compressed_o
add wave -noupdate -expand -group {IF Stage} -expand -group Prefetch -group FIFO /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/prefetch_32/prefetch_buffer_i/fifo_i/out_valid_stored_o
add wave -noupdate -expand -group {IF Stage} -expand -group Prefetch -group FIFO /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/prefetch_32/prefetch_buffer_i/fifo_i/out_is_hwlp_o
add wave -noupdate -expand -group {IF Stage} -expand -group Prefetch -group FIFO /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/prefetch_32/prefetch_buffer_i/fifo_i/addr_n
add wave -noupdate -expand -group {IF Stage} -expand -group Prefetch -group FIFO /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/prefetch_32/prefetch_buffer_i/fifo_i/addr_int
add wave -noupdate -expand -group {IF Stage} -expand -group Prefetch -group FIFO /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/prefetch_32/prefetch_buffer_i/fifo_i/addr_Q
add wave -noupdate -expand -group {IF Stage} -expand -group Prefetch -group FIFO /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/prefetch_32/prefetch_buffer_i/fifo_i/rdata_n
add wave -noupdate -expand -group {IF Stage} -expand -group Prefetch -group FIFO /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/prefetch_32/prefetch_buffer_i/fifo_i/rdata_int
add wave -noupdate -expand -group {IF Stage} -expand -group Prefetch -group FIFO /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/prefetch_32/prefetch_buffer_i/fifo_i/rdata_Q
add wave -noupdate -expand -group {IF Stage} -expand -group Prefetch -group FIFO /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/prefetch_32/prefetch_buffer_i/fifo_i/valid_n
add wave -noupdate -expand -group {IF Stage} -expand -group Prefetch -group FIFO /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/prefetch_32/prefetch_buffer_i/fifo_i/valid_int
add wave -noupdate -expand -group {IF Stage} -expand -group Prefetch -group FIFO /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/prefetch_32/prefetch_buffer_i/fifo_i/valid_Q
add wave -noupdate -expand -group {IF Stage} -expand -group Prefetch -group FIFO /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/prefetch_32/prefetch_buffer_i/fifo_i/is_hwlp_n
add wave -noupdate -expand -group {IF Stage} -expand -group Prefetch -group FIFO /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/prefetch_32/prefetch_buffer_i/fifo_i/is_hwlp_int
add wave -noupdate -expand -group {IF Stage} -expand -group Prefetch -group FIFO /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/prefetch_32/prefetch_buffer_i/fifo_i/is_hwlp_Q
add wave -noupdate -expand -group {IF Stage} -expand -group Prefetch -group FIFO /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/prefetch_32/prefetch_buffer_i/fifo_i/addr_next
add wave -noupdate -expand -group {IF Stage} -expand -group Prefetch -group FIFO /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/prefetch_32/prefetch_buffer_i/fifo_i/rdata
add wave -noupdate -expand -group {IF Stage} -expand -group Prefetch -group FIFO /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/prefetch_32/prefetch_buffer_i/fifo_i/rdata_unaligned
add wave -noupdate -expand -group {IF Stage} -expand -group Prefetch -group FIFO /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/prefetch_32/prefetch_buffer_i/fifo_i/valid
add wave -noupdate -expand -group {IF Stage} -expand -group Prefetch -group FIFO /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/prefetch_32/prefetch_buffer_i/fifo_i/valid_unaligned
add wave -noupdate -expand -group {IF Stage} -expand -group Prefetch -group FIFO /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/prefetch_32/prefetch_buffer_i/fifo_i/aligned_is_compressed
add wave -noupdate -expand -group {IF Stage} -expand -group Prefetch -group FIFO /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/prefetch_32/prefetch_buffer_i/fifo_i/unaligned_is_compressed
add wave -noupdate -expand -group {IF Stage} -expand -group Prefetch -group FIFO /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/prefetch_32/prefetch_buffer_i/fifo_i/aligned_is_compressed_st
add wave -noupdate -expand -group {IF Stage} -expand -group Prefetch -group FIFO /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/prefetch_32/prefetch_buffer_i/fifo_i/unaligned_is_compressed_st
add wave -noupdate -expand -group {IF Stage} -expand -group Prefetch /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/prefetch_32/prefetch_buffer_i/clk
add wave -noupdate -expand -group {IF Stage} -expand -group Prefetch /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/prefetch_32/prefetch_buffer_i/rst_n
add wave -noupdate -expand -group {IF Stage} -expand -group Prefetch /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/prefetch_32/prefetch_buffer_i/req_i
add wave -noupdate -expand -group {IF Stage} -expand -group Prefetch /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/prefetch_32/prefetch_buffer_i/branch_i
add wave -noupdate -expand -group {IF Stage} -expand -group Prefetch /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/prefetch_32/prefetch_buffer_i/addr_i
add wave -noupdate -expand -group {IF Stage} -expand -group Prefetch /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/prefetch_32/prefetch_buffer_i/hwloop_i
add wave -noupdate -expand -group {IF Stage} -expand -group Prefetch /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/prefetch_32/prefetch_buffer_i/hwloop_target_i
add wave -noupdate -expand -group {IF Stage} -expand -group Prefetch /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/prefetch_32/prefetch_buffer_i/hwlp_branch_o
add wave -noupdate -expand -group {IF Stage} -expand -group Prefetch /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/prefetch_32/prefetch_buffer_i/ready_i
add wave -noupdate -expand -group {IF Stage} -expand -group Prefetch /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/prefetch_32/prefetch_buffer_i/valid_o
add wave -noupdate -expand -group {IF Stage} -expand -group Prefetch /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/prefetch_32/prefetch_buffer_i/rdata_o
add wave -noupdate -expand -group {IF Stage} -expand -group Prefetch /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/prefetch_32/prefetch_buffer_i/addr_o
add wave -noupdate -expand -group {IF Stage} -expand -group Prefetch /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/prefetch_32/prefetch_buffer_i/is_hwlp_o
add wave -noupdate -expand -group {IF Stage} -expand -group Prefetch /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/prefetch_32/prefetch_buffer_i/instr_req_o
add wave -noupdate -expand -group {IF Stage} -expand -group Prefetch /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/prefetch_32/prefetch_buffer_i/instr_gnt_i
add wave -noupdate -expand -group {IF Stage} -expand -group Prefetch /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/prefetch_32/prefetch_buffer_i/instr_addr_o
add wave -noupdate -expand -group {IF Stage} -expand -group Prefetch /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/prefetch_32/prefetch_buffer_i/instr_rdata_i
add wave -noupdate -expand -group {IF Stage} -expand -group Prefetch /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/prefetch_32/prefetch_buffer_i/instr_rvalid_i
add wave -noupdate -expand -group {IF Stage} -expand -group Prefetch /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/prefetch_32/prefetch_buffer_i/busy_o
add wave -noupdate -expand -group {IF Stage} -expand -group Prefetch /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/prefetch_32/prefetch_buffer_i/CS
add wave -noupdate -expand -group {IF Stage} -expand -group Prefetch /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/prefetch_32/prefetch_buffer_i/NS
add wave -noupdate -expand -group {IF Stage} -expand -group Prefetch /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/prefetch_32/prefetch_buffer_i/hwlp_CS
add wave -noupdate -expand -group {IF Stage} -expand -group Prefetch /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/prefetch_32/prefetch_buffer_i/hwlp_NS
add wave -noupdate -expand -group {IF Stage} -expand -group Prefetch /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/prefetch_32/prefetch_buffer_i/instr_addr_q
add wave -noupdate -expand -group {IF Stage} -expand -group Prefetch /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/prefetch_32/prefetch_buffer_i/fetch_addr
add wave -noupdate -expand -group {IF Stage} -expand -group Prefetch /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/prefetch_32/prefetch_buffer_i/fetch_is_hwlp
add wave -noupdate -expand -group {IF Stage} -expand -group Prefetch /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/prefetch_32/prefetch_buffer_i/addr_valid
add wave -noupdate -expand -group {IF Stage} -expand -group Prefetch /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/prefetch_32/prefetch_buffer_i/fifo_valid
add wave -noupdate -expand -group {IF Stage} -expand -group Prefetch /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/prefetch_32/prefetch_buffer_i/fifo_ready
add wave -noupdate -expand -group {IF Stage} -expand -group Prefetch /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/prefetch_32/prefetch_buffer_i/fifo_clear
add wave -noupdate -expand -group {IF Stage} -expand -group Prefetch /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/prefetch_32/prefetch_buffer_i/fifo_hwlp
add wave -noupdate -expand -group {IF Stage} -expand -group Prefetch /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/prefetch_32/prefetch_buffer_i/valid_stored
add wave -noupdate -expand -group {IF Stage} -expand -group Prefetch /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/prefetch_32/prefetch_buffer_i/hwlp_masked
add wave -noupdate -expand -group {IF Stage} -expand -group Prefetch /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/prefetch_32/prefetch_buffer_i/hwlp_branch
add wave -noupdate -expand -group {IF Stage} -expand -group Prefetch /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/prefetch_32/prefetch_buffer_i/hwloop_speculative
add wave -noupdate -expand -group {IF Stage} -expand -group Prefetch /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/prefetch_32/prefetch_buffer_i/unaligned_is_compressed
add wave -noupdate -expand -group {IF Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/clk
add wave -noupdate -expand -group {IF Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/rst_n
add wave -noupdate -expand -group {IF Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/m_trap_base_addr_i
add wave -noupdate -expand -group {IF Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/u_trap_base_addr_i
add wave -noupdate -expand -group {IF Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/trap_addr_mux_i
add wave -noupdate -expand -group {IF Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/boot_addr_i
add wave -noupdate -expand -group {IF Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/req_i
add wave -noupdate -expand -group {IF Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/instr_req_o
add wave -noupdate -expand -group {IF Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/instr_addr_o
add wave -noupdate -expand -group {IF Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/instr_gnt_i
add wave -noupdate -expand -group {IF Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/instr_rvalid_i
add wave -noupdate -expand -group {IF Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/instr_rdata_i
add wave -noupdate -expand -group {IF Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/hwlp_dec_cnt_id_o
add wave -noupdate -expand -group {IF Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/is_hwlp_id_o
add wave -noupdate -expand -group {IF Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/instr_valid_id_o
add wave -noupdate -expand -group {IF Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/instr_rdata_id_o
add wave -noupdate -expand -group {IF Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/is_compressed_id_o
add wave -noupdate -expand -group {IF Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/illegal_c_insn_id_o
add wave -noupdate -expand -group {IF Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/pc_if_o
add wave -noupdate -expand -group {IF Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/pc_id_o
add wave -noupdate -expand -group {IF Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/clear_instr_valid_i
add wave -noupdate -expand -group {IF Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/pc_set_i
add wave -noupdate -expand -group {IF Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/exception_pc_reg_i
add wave -noupdate -expand -group {IF Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/pc_mux_i
add wave -noupdate -expand -group {IF Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/exc_pc_mux_i
add wave -noupdate -expand -group {IF Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/exc_vec_pc_mux_i
add wave -noupdate -expand -group {IF Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/jump_target_id_i
add wave -noupdate -expand -group {IF Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/jump_target_ex_i
add wave -noupdate -expand -group {IF Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/hwlp_start_i
add wave -noupdate -expand -group {IF Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/hwlp_end_i
add wave -noupdate -expand -group {IF Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/hwlp_cnt_i
add wave -noupdate -expand -group {IF Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/dbg_jump_addr_i
add wave -noupdate -expand -group {IF Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/dbg_jump_req_i
add wave -noupdate -expand -group {IF Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/halt_if_i
add wave -noupdate -expand -group {IF Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/id_ready_i
add wave -noupdate -expand -group {IF Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/if_busy_o
add wave -noupdate -expand -group {IF Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/perf_imiss_o
add wave -noupdate -expand -group {IF Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/offset_fsm_cs
add wave -noupdate -expand -group {IF Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/offset_fsm_ns
add wave -noupdate -expand -group {IF Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/if_valid
add wave -noupdate -expand -group {IF Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/if_ready
add wave -noupdate -expand -group {IF Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/valid
add wave -noupdate -expand -group {IF Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/prefetch_busy
add wave -noupdate -expand -group {IF Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/branch_req
add wave -noupdate -expand -group {IF Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/fetch_addr_n
add wave -noupdate -expand -group {IF Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/fetch_valid
add wave -noupdate -expand -group {IF Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/fetch_ready
add wave -noupdate -expand -group {IF Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/fetch_rdata
add wave -noupdate -expand -group {IF Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/fetch_addr
add wave -noupdate -expand -group {IF Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/is_hwlp_id_q
add wave -noupdate -expand -group {IF Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/fetch_is_hwlp
add wave -noupdate -expand -group {IF Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/exc_pc
add wave -noupdate -expand -group {IF Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/hwlp_jump
add wave -noupdate -expand -group {IF Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/hwlp_branch
add wave -noupdate -expand -group {IF Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/hwlp_target
add wave -noupdate -expand -group {IF Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/hwlp_dec_cnt
add wave -noupdate -expand -group {IF Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/hwlp_dec_cnt_if
add wave -noupdate -expand -group {IF Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/trap_base_addr
add wave -noupdate -expand -group {IF Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/instr_decompressed
add wave -noupdate -expand -group {IF Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/illegal_c_insn
add wave -noupdate -expand -group {IF Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/if_stage_i/instr_compressed_int
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/clk
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/rst_n
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/test_en_i
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/fregfile_disable_i
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/fetch_enable_i
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/ctrl_busy_o
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/core_ctrl_firstfetch_o
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/is_decoding_o
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/hwlp_dec_cnt_i
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/is_hwlp_i
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/instr_valid_i
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/instr_rdata_i
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/instr_req_o
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/branch_in_ex_o
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/branch_decision_i
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/jump_target_o
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/clear_instr_valid_o
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/pc_set_o
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/pc_mux_o
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/exc_pc_mux_o
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/trap_addr_mux_o
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/illegal_c_insn_i
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/is_compressed_i
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/pc_if_i
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/pc_id_i
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/halt_if_o
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/id_ready_o
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/ex_ready_i
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/wb_ready_i
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/id_valid_o
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/ex_valid_i
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/pc_ex_o
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/alu_operand_a_ex_o
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/alu_operand_b_ex_o
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/alu_operand_c_ex_o
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/bmask_a_ex_o
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/bmask_b_ex_o
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/imm_vec_ext_ex_o
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/alu_vec_mode_ex_o
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/regfile_waddr_ex_o
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/regfile_we_ex_o
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/regfile_alu_waddr_ex_o
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/regfile_alu_we_ex_o
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/alu_en_ex_o
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/alu_operator_ex_o
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/mult_operator_ex_o
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/mult_operand_a_ex_o
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/mult_operand_b_ex_o
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/mult_operand_c_ex_o
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/mult_en_ex_o
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/mult_sel_subword_ex_o
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/mult_signed_mode_ex_o
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/mult_imm_ex_o
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/mult_dot_op_a_ex_o
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/mult_dot_op_b_ex_o
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/mult_dot_op_c_ex_o
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/mult_dot_signed_ex_o
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/fpu_op_ex_o
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/apu_en_ex_o
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/apu_type_ex_o
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/apu_op_ex_o
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/apu_lat_ex_o
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/apu_flags_ex_o
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/apu_waddr_ex_o
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/apu_read_regs_o
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/apu_read_regs_valid_o
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/apu_read_dep_i
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/apu_write_regs_o
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/apu_write_regs_valid_o
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/apu_write_dep_i
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/apu_perf_dep_o
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/apu_busy_i
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/frm_i
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/csr_access_ex_o
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/csr_op_ex_o
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/current_priv_lvl_i
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/csr_irq_sec_o
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/csr_cause_o
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/csr_save_if_o
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/csr_save_id_o
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/csr_restore_mret_id_o
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/csr_restore_uret_id_o
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/csr_save_cause_o
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/hwlp_start_o
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/hwlp_end_o
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/hwlp_cnt_o
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/csr_hwlp_regid_i
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/csr_hwlp_we_i
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/csr_hwlp_data_i
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/data_req_ex_o
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/data_we_ex_o
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/data_type_ex_o
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/data_sign_ext_ex_o
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/data_reg_offset_ex_o
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/data_load_event_ex_o
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/data_misaligned_ex_o
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/prepost_useincr_ex_o
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/data_misaligned_i
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/irq_i
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/irq_sec_i
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/irq_id_i
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/m_irq_enable_i
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/u_irq_enable_i
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/irq_ack_o
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/irq_id_o
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/exc_cause_o
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/lsu_load_err_i
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/lsu_store_err_i
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/dbg_settings_i
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/dbg_req_i
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/dbg_ack_o
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/dbg_stall_i
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/dbg_trap_o
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/dbg_reg_rreq_i
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/dbg_reg_raddr_i
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/dbg_reg_rdata_o
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/dbg_reg_wreq_i
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/dbg_reg_waddr_i
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/dbg_reg_wdata_i
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/dbg_jump_req_i
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/regfile_waddr_wb_i
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/regfile_we_wb_i
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/regfile_wdata_wb_i
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/regfile_alu_waddr_fw_i
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/regfile_alu_we_fw_i
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/regfile_alu_wdata_fw_i
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/mult_multicycle_i
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/perf_jump_o
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/perf_jr_stall_o
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/perf_ld_stall_o
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/instr
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/deassert_we
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/illegal_insn_dec
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/ebrk_insn
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/mret_insn_dec
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/uret_insn_dec
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/ecall_insn_dec
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/pipe_flush_dec
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/rega_used_dec
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/regb_used_dec
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/regc_used_dec
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/branch_taken_ex
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/jump_in_id
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/jump_in_dec
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/misaligned_stall
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/jr_stall
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/load_stall
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/csr_apu_stall
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/instr_multicycle
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/halt_id
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/imm_i_type
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/imm_iz_type
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/imm_s_type
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/imm_sb_type
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/imm_u_type
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/imm_uj_type
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/imm_z_type
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/imm_s2_type
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/imm_bi_type
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/imm_s3_type
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/imm_vs_type
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/imm_vu_type
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/imm_shuffleb_type
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/imm_shuffleh_type
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/imm_shuffle_type
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/imm_clip_type
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/imm_a
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/imm_b
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/jump_target
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/irq_req_ctrl
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/irq_sec_ctrl
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/irq_id_ctrl
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/exc_ack
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/exc_kill
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/regfile_addr_ra_id
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/regfile_addr_rb_id
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/regfile_addr_rc_id
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/regfile_fp_a
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/regfile_fp_b
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/regfile_fp_c
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/regfile_fp_d
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/regfile_waddr_id
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/regfile_alu_waddr_id
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/regfile_alu_we_id
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/regfile_data_ra_id
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/regfile_data_rb_id
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/regfile_data_rc_id
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/alu_en
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/alu_operator
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/alu_op_a_mux_sel
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/alu_op_b_mux_sel
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/alu_op_c_mux_sel
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/regc_mux
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/imm_a_mux_sel
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/imm_b_mux_sel
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/jump_target_mux_sel
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/mult_operator
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/mult_en
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/mult_int_en
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/mult_sel_subword
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/mult_signed_mode
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/mult_dot_en
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/mult_dot_signed
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/fpu_op
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/apu_en
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/apu_type
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/apu_op
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/apu_lat
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/apu_flags
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/apu_waddr
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/apu_read_regs
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/apu_read_regs_valid
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/apu_write_regs
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/apu_write_regs_valid
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/apu_flags_src
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/apu_stall
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/fp_rnd_mode
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/regfile_we_id
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/regfile_alu_waddr_mux_sel
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/data_we_id
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/data_type_id
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/data_sign_ext_id
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/data_reg_offset_id
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/data_req_id
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/data_load_event_id
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/hwloop_regid
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/hwloop_regid_int
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/hwloop_we
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/hwloop_we_int
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/hwloop_target_mux_sel
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/hwloop_start_mux_sel
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/hwloop_cnt_mux_sel
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/hwloop_target
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/hwloop_start
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/hwloop_start_int
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/hwloop_end
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/hwloop_cnt
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/hwloop_cnt_int
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/hwloop_valid
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/csr_access
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/csr_op
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/csr_status
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/prepost_useincr
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/operand_a_fw_mux_sel
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/operand_b_fw_mux_sel
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/operand_c_fw_mux_sel
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/operand_a_fw_id
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/operand_b_fw_id
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/operand_c_fw_id
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/operand_b
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/operand_b_vec
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/alu_operand_a
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/alu_operand_b
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/alu_operand_c
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/bmask_a_mux
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/bmask_b_mux
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/alu_bmask_a_mux_sel
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/alu_bmask_b_mux_sel
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/mult_imm_mux
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/bmask_a_id_imm
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/bmask_b_id_imm
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/bmask_a_id
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/bmask_b_id
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/imm_vec_ext_id
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/mult_imm_id
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/alu_vec_mode
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/scalar_replication
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/reg_d_ex_is_reg_a_id
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/reg_d_ex_is_reg_b_id
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/reg_d_ex_is_reg_c_id
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/reg_d_wb_is_reg_a_id
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/reg_d_wb_is_reg_b_id
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/reg_d_wb_is_reg_c_id
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/reg_d_alu_is_reg_a_id
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/reg_d_alu_is_reg_b_id
add wave -noupdate -expand -group {ID Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/reg_d_alu_is_reg_c_id
add wave -noupdate -expand -group RF /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/registers_i/mem
add wave -noupdate -expand -group RF_FP /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/registers_i/mem_fp
add wave -noupdate -expand -group Decoder /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/decoder_i/deassert_we_i
add wave -noupdate -expand -group Decoder /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/decoder_i/data_misaligned_i
add wave -noupdate -expand -group Decoder /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/decoder_i/mult_multicycle_i
add wave -noupdate -expand -group Decoder /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/decoder_i/instr_multicycle_o
add wave -noupdate -expand -group Decoder /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/decoder_i/illegal_insn_o
add wave -noupdate -expand -group Decoder /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/decoder_i/ebrk_insn_o
add wave -noupdate -expand -group Decoder /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/decoder_i/mret_insn_o
add wave -noupdate -expand -group Decoder /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/decoder_i/uret_insn_o
add wave -noupdate -expand -group Decoder /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/decoder_i/ecall_insn_o
add wave -noupdate -expand -group Decoder /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/decoder_i/pipe_flush_o
add wave -noupdate -expand -group Decoder /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/decoder_i/rega_used_o
add wave -noupdate -expand -group Decoder /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/decoder_i/regb_used_o
add wave -noupdate -expand -group Decoder /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/decoder_i/regc_used_o
add wave -noupdate -expand -group Decoder /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/decoder_i/reg_fp_a_o
add wave -noupdate -expand -group Decoder /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/decoder_i/reg_fp_b_o
add wave -noupdate -expand -group Decoder /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/decoder_i/reg_fp_c_o
add wave -noupdate -expand -group Decoder /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/decoder_i/reg_fp_d_o
add wave -noupdate -expand -group Decoder /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/decoder_i/bmask_a_mux_o
add wave -noupdate -expand -group Decoder /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/decoder_i/bmask_b_mux_o
add wave -noupdate -expand -group Decoder /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/decoder_i/alu_bmask_a_mux_sel_o
add wave -noupdate -expand -group Decoder /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/decoder_i/alu_bmask_b_mux_sel_o
add wave -noupdate -expand -group Decoder /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/decoder_i/instr_rdata_i
add wave -noupdate -expand -group Decoder /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/decoder_i/illegal_c_insn_i
add wave -noupdate -expand -group Decoder /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/decoder_i/alu_en_o
add wave -noupdate -expand -group Decoder /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/decoder_i/alu_operator_o
add wave -noupdate -expand -group Decoder /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/decoder_i/alu_op_a_mux_sel_o
add wave -noupdate -expand -group Decoder /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/decoder_i/alu_op_b_mux_sel_o
add wave -noupdate -expand -group Decoder /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/decoder_i/alu_op_c_mux_sel_o
add wave -noupdate -expand -group Decoder /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/decoder_i/alu_vec_mode_o
add wave -noupdate -expand -group Decoder /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/decoder_i/scalar_replication_o
add wave -noupdate -expand -group Decoder /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/decoder_i/imm_a_mux_sel_o
add wave -noupdate -expand -group Decoder /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/decoder_i/imm_b_mux_sel_o
add wave -noupdate -expand -group Decoder /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/decoder_i/regc_mux_o
add wave -noupdate -expand -group Decoder /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/decoder_i/mult_operator_o
add wave -noupdate -expand -group Decoder /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/decoder_i/mult_int_en_o
add wave -noupdate -expand -group Decoder /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/decoder_i/mult_dot_en_o
add wave -noupdate -expand -group Decoder /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/decoder_i/mult_imm_mux_o
add wave -noupdate -expand -group Decoder /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/decoder_i/mult_sel_subword_o
add wave -noupdate -expand -group Decoder /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/decoder_i/mult_signed_mode_o
add wave -noupdate -expand -group Decoder /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/decoder_i/mult_dot_signed_o
add wave -noupdate -expand -group Decoder /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/decoder_i/fpu_op_o
add wave -noupdate -expand -group Decoder /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/decoder_i/apu_en_o
add wave -noupdate -expand -group Decoder /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/decoder_i/apu_type_o
add wave -noupdate -expand -group Decoder /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/decoder_i/apu_op_o
add wave -noupdate -expand -group Decoder /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/decoder_i/apu_lat_o
add wave -noupdate -expand -group Decoder /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/decoder_i/apu_flags_src_o
add wave -noupdate -expand -group Decoder /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/decoder_i/fp_rnd_mode_o
add wave -noupdate -expand -group Decoder /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/decoder_i/regfile_mem_we_o
add wave -noupdate -expand -group Decoder /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/decoder_i/regfile_alu_we_o
add wave -noupdate -expand -group Decoder /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/decoder_i/regfile_alu_waddr_sel_o
add wave -noupdate -expand -group Decoder /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/decoder_i/csr_access_o
add wave -noupdate -expand -group Decoder /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/decoder_i/csr_status_o
add wave -noupdate -expand -group Decoder /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/decoder_i/csr_op_o
add wave -noupdate -expand -group Decoder /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/decoder_i/current_priv_lvl_i
add wave -noupdate -expand -group Decoder /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/decoder_i/data_req_o
add wave -noupdate -expand -group Decoder /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/decoder_i/data_we_o
add wave -noupdate -expand -group Decoder /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/decoder_i/prepost_useincr_o
add wave -noupdate -expand -group Decoder /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/decoder_i/data_type_o
add wave -noupdate -expand -group Decoder /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/decoder_i/data_sign_extension_o
add wave -noupdate -expand -group Decoder /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/decoder_i/data_reg_offset_o
add wave -noupdate -expand -group Decoder /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/decoder_i/data_load_event_o
add wave -noupdate -expand -group Decoder /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/decoder_i/hwloop_we_o
add wave -noupdate -expand -group Decoder /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/decoder_i/hwloop_target_mux_sel_o
add wave -noupdate -expand -group Decoder /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/decoder_i/hwloop_start_mux_sel_o
add wave -noupdate -expand -group Decoder /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/decoder_i/hwloop_cnt_mux_sel_o
add wave -noupdate -expand -group Decoder /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/decoder_i/jump_in_dec_o
add wave -noupdate -expand -group Decoder /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/decoder_i/jump_in_id_o
add wave -noupdate -expand -group Decoder /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/decoder_i/jump_target_mux_sel_o
add wave -noupdate -expand -group Decoder /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/decoder_i/regfile_mem_we
add wave -noupdate -expand -group Decoder /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/decoder_i/regfile_alu_we
add wave -noupdate -expand -group Decoder /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/decoder_i/data_req
add wave -noupdate -expand -group Decoder /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/decoder_i/hwloop_we
add wave -noupdate -expand -group Decoder /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/decoder_i/csr_illegal
add wave -noupdate -expand -group Decoder /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/decoder_i/jump_in_id
add wave -noupdate -expand -group Decoder /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/decoder_i/csr_op
add wave -noupdate -expand -group Decoder /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/decoder_i/apu_en
add wave -noupdate -expand -group Controller /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/controller_i/clk
add wave -noupdate -expand -group Controller /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/controller_i/rst_n
add wave -noupdate -expand -group Controller /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/controller_i/fetch_enable_i
add wave -noupdate -expand -group Controller /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/controller_i/ctrl_busy_o
add wave -noupdate -expand -group Controller /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/controller_i/first_fetch_o
add wave -noupdate -expand -group Controller /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/controller_i/is_decoding_o
add wave -noupdate -expand -group Controller /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/controller_i/deassert_we_o
add wave -noupdate -expand -group Controller /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/controller_i/illegal_insn_i
add wave -noupdate -expand -group Controller /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/controller_i/ecall_insn_i
add wave -noupdate -expand -group Controller /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/controller_i/mret_insn_i
add wave -noupdate -expand -group Controller /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/controller_i/uret_insn_i
add wave -noupdate -expand -group Controller /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/controller_i/pipe_flush_i
add wave -noupdate -expand -group Controller /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/controller_i/ebrk_insn_i
add wave -noupdate -expand -group Controller /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/controller_i/csr_status_i
add wave -noupdate -expand -group Controller /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/controller_i/instr_multicycle_i
add wave -noupdate -expand -group Controller /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/controller_i/instr_valid_i
add wave -noupdate -expand -group Controller /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/controller_i/instr_req_o
add wave -noupdate -expand -group Controller /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/controller_i/pc_set_o
add wave -noupdate -expand -group Controller /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/controller_i/pc_mux_o
add wave -noupdate -expand -group Controller /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/controller_i/exc_pc_mux_o
add wave -noupdate -expand -group Controller /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/controller_i/trap_addr_mux_o
add wave -noupdate -expand -group Controller /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/controller_i/data_req_ex_i
add wave -noupdate -expand -group Controller /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/controller_i/data_misaligned_i
add wave -noupdate -expand -group Controller /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/controller_i/data_load_event_i
add wave -noupdate -expand -group Controller /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/controller_i/mult_multicycle_i
add wave -noupdate -expand -group Controller /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/controller_i/apu_en_i
add wave -noupdate -expand -group Controller /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/controller_i/apu_read_dep_i
add wave -noupdate -expand -group Controller /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/controller_i/apu_write_dep_i
add wave -noupdate -expand -group Controller /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/controller_i/apu_stall_o
add wave -noupdate -expand -group Controller /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/controller_i/branch_taken_ex_i
add wave -noupdate -expand -group Controller /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/controller_i/jump_in_id_i
add wave -noupdate -expand -group Controller /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/controller_i/jump_in_dec_i
add wave -noupdate -expand -group Controller /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/controller_i/irq_i
add wave -noupdate -expand -group Controller /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/controller_i/irq_req_ctrl_i
add wave -noupdate -expand -group Controller /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/controller_i/irq_sec_ctrl_i
add wave -noupdate -expand -group Controller /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/controller_i/irq_id_ctrl_i
add wave -noupdate -expand -group Controller /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/controller_i/m_IE_i
add wave -noupdate -expand -group Controller /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/controller_i/u_IE_i
add wave -noupdate -expand -group Controller /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/controller_i/current_priv_lvl_i
add wave -noupdate -expand -group Controller /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/controller_i/irq_ack_o
add wave -noupdate -expand -group Controller /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/controller_i/irq_id_o
add wave -noupdate -expand -group Controller /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/controller_i/exc_cause_o
add wave -noupdate -expand -group Controller /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/controller_i/exc_ack_o
add wave -noupdate -expand -group Controller /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/controller_i/exc_kill_o
add wave -noupdate -expand -group Controller /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/controller_i/csr_save_if_o
add wave -noupdate -expand -group Controller /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/controller_i/csr_save_id_o
add wave -noupdate -expand -group Controller /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/controller_i/csr_cause_o
add wave -noupdate -expand -group Controller /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/controller_i/csr_irq_sec_o
add wave -noupdate -expand -group Controller /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/controller_i/csr_restore_mret_id_o
add wave -noupdate -expand -group Controller /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/controller_i/csr_restore_uret_id_o
add wave -noupdate -expand -group Controller /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/controller_i/csr_save_cause_o
add wave -noupdate -expand -group Controller /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/controller_i/dbg_req_i
add wave -noupdate -expand -group Controller /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/controller_i/dbg_ack_o
add wave -noupdate -expand -group Controller /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/controller_i/dbg_stall_i
add wave -noupdate -expand -group Controller /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/controller_i/dbg_jump_req_i
add wave -noupdate -expand -group Controller /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/controller_i/dbg_settings_i
add wave -noupdate -expand -group Controller /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/controller_i/dbg_trap_o
add wave -noupdate -expand -group Controller /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/controller_i/regfile_alu_waddr_id_i
add wave -noupdate -expand -group Controller /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/controller_i/regfile_we_ex_i
add wave -noupdate -expand -group Controller /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/controller_i/regfile_waddr_ex_i
add wave -noupdate -expand -group Controller /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/controller_i/regfile_we_wb_i
add wave -noupdate -expand -group Controller /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/controller_i/regfile_alu_we_fw_i
add wave -noupdate -expand -group Controller /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/controller_i/operand_a_fw_mux_sel_o
add wave -noupdate -expand -group Controller /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/controller_i/operand_b_fw_mux_sel_o
add wave -noupdate -expand -group Controller /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/controller_i/operand_c_fw_mux_sel_o
add wave -noupdate -expand -group Controller /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/controller_i/reg_d_ex_is_reg_a_i
add wave -noupdate -expand -group Controller /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/controller_i/reg_d_ex_is_reg_b_i
add wave -noupdate -expand -group Controller /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/controller_i/reg_d_ex_is_reg_c_i
add wave -noupdate -expand -group Controller /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/controller_i/reg_d_wb_is_reg_a_i
add wave -noupdate -expand -group Controller /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/controller_i/reg_d_wb_is_reg_b_i
add wave -noupdate -expand -group Controller /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/controller_i/reg_d_wb_is_reg_c_i
add wave -noupdate -expand -group Controller /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/controller_i/reg_d_alu_is_reg_a_i
add wave -noupdate -expand -group Controller /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/controller_i/reg_d_alu_is_reg_b_i
add wave -noupdate -expand -group Controller /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/controller_i/reg_d_alu_is_reg_c_i
add wave -noupdate -expand -group Controller /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/controller_i/halt_if_o
add wave -noupdate -expand -group Controller /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/controller_i/halt_id_o
add wave -noupdate -expand -group Controller /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/controller_i/misaligned_stall_o
add wave -noupdate -expand -group Controller /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/controller_i/jr_stall_o
add wave -noupdate -expand -group Controller /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/controller_i/load_stall_o
add wave -noupdate -expand -group Controller /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/controller_i/id_ready_i
add wave -noupdate -expand -group Controller /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/controller_i/ex_valid_i
add wave -noupdate -expand -group Controller /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/controller_i/wb_ready_i
add wave -noupdate -expand -group Controller /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/controller_i/perf_jump_o
add wave -noupdate -expand -group Controller /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/controller_i/perf_jr_stall_o
add wave -noupdate -expand -group Controller /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/controller_i/perf_ld_stall_o
add wave -noupdate -expand -group Controller /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/controller_i/ctrl_fsm_cs
add wave -noupdate -expand -group Controller /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/controller_i/ctrl_fsm_ns
add wave -noupdate -expand -group Controller /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/controller_i/jump_done
add wave -noupdate -expand -group Controller /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/controller_i/jump_done_q
add wave -noupdate -expand -group Controller /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/controller_i/jump_in_dec
add wave -noupdate -expand -group Controller /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/controller_i/branch_in_id
add wave -noupdate -expand -group Controller /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/controller_i/boot_done
add wave -noupdate -expand -group Controller /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/controller_i/boot_done_q
add wave -noupdate -expand -group Controller /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/controller_i/irq_enable_int
add wave -noupdate -expand -group {Int Ctrl} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/int_controller_i/clk
add wave -noupdate -expand -group {Int Ctrl} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/int_controller_i/rst_n
add wave -noupdate -expand -group {Int Ctrl} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/int_controller_i/irq_req_ctrl_o
add wave -noupdate -expand -group {Int Ctrl} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/int_controller_i/irq_sec_ctrl_o
add wave -noupdate -expand -group {Int Ctrl} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/int_controller_i/irq_id_ctrl_o
add wave -noupdate -expand -group {Int Ctrl} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/int_controller_i/ctrl_ack_i
add wave -noupdate -expand -group {Int Ctrl} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/int_controller_i/ctrl_kill_i
add wave -noupdate -expand -group {Int Ctrl} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/int_controller_i/irq_i
add wave -noupdate -expand -group {Int Ctrl} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/int_controller_i/irq_sec_i
add wave -noupdate -expand -group {Int Ctrl} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/int_controller_i/irq_id_i
add wave -noupdate -expand -group {Int Ctrl} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/int_controller_i/m_IE_i
add wave -noupdate -expand -group {Int Ctrl} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/int_controller_i/u_IE_i
add wave -noupdate -expand -group {Int Ctrl} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/int_controller_i/current_priv_lvl_i
add wave -noupdate -expand -group {Int Ctrl} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/int_controller_i/exc_ctrl_cs
add wave -noupdate -expand -group {Int Ctrl} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/int_controller_i/exc_ctrl_ns
add wave -noupdate -expand -group {Int Ctrl} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/int_controller_i/irq_enable_ext
add wave -noupdate -expand -group {Int Ctrl} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/int_controller_i/irq_id_q
add wave -noupdate -expand -group {Int Ctrl} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/int_controller_i/irq_sec_q
add wave -noupdate -expand -group {Hwloop Regs} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/hwloop_regs_i/clk
add wave -noupdate -expand -group {Hwloop Regs} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/hwloop_regs_i/rst_n
add wave -noupdate -expand -group {Hwloop Regs} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/hwloop_regs_i/hwlp_start_data_i
add wave -noupdate -expand -group {Hwloop Regs} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/hwloop_regs_i/hwlp_end_data_i
add wave -noupdate -expand -group {Hwloop Regs} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/hwloop_regs_i/hwlp_cnt_data_i
add wave -noupdate -expand -group {Hwloop Regs} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/hwloop_regs_i/hwlp_we_i
add wave -noupdate -expand -group {Hwloop Regs} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/hwloop_regs_i/hwlp_regid_i
add wave -noupdate -expand -group {Hwloop Regs} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/hwloop_regs_i/valid_i
add wave -noupdate -expand -group {Hwloop Regs} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/hwloop_regs_i/hwlp_dec_cnt_i
add wave -noupdate -expand -group {Hwloop Regs} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/hwloop_regs_i/hwlp_start_addr_o
add wave -noupdate -expand -group {Hwloop Regs} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/hwloop_regs_i/hwlp_end_addr_o
add wave -noupdate -expand -group {Hwloop Regs} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/hwloop_regs_i/hwlp_counter_o
add wave -noupdate -expand -group {Hwloop Regs} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/hwloop_regs_i/hwlp_start_q
add wave -noupdate -expand -group {Hwloop Regs} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/hwloop_regs_i/hwlp_end_q
add wave -noupdate -expand -group {Hwloop Regs} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/hwloop_regs_i/hwlp_counter_q
add wave -noupdate -expand -group {Hwloop Regs} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/hwloop_regs_i/hwlp_counter_n
add wave -noupdate -expand -group {Hwloop Regs} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/id_stage_i/hwloop_regs_i/i
add wave -noupdate -expand -group {EX Stage} -group ALU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/clk
add wave -noupdate -expand -group {EX Stage} -group ALU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/rst_n
add wave -noupdate -expand -group {EX Stage} -group ALU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/enable_i
add wave -noupdate -expand -group {EX Stage} -group ALU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/operator_i
add wave -noupdate -expand -group {EX Stage} -group ALU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/operand_a_i
add wave -noupdate -expand -group {EX Stage} -group ALU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/operand_b_i
add wave -noupdate -expand -group {EX Stage} -group ALU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/operand_c_i
add wave -noupdate -expand -group {EX Stage} -group ALU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/vector_mode_i
add wave -noupdate -expand -group {EX Stage} -group ALU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/bmask_a_i
add wave -noupdate -expand -group {EX Stage} -group ALU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/bmask_b_i
add wave -noupdate -expand -group {EX Stage} -group ALU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/imm_vec_ext_i
add wave -noupdate -expand -group {EX Stage} -group ALU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/result_o
add wave -noupdate -expand -group {EX Stage} -group ALU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/comparison_result_o
add wave -noupdate -expand -group {EX Stage} -group ALU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/ready_o
add wave -noupdate -expand -group {EX Stage} -group ALU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/ex_ready_i
add wave -noupdate -expand -group {EX Stage} -group ALU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/operand_a_rev
add wave -noupdate -expand -group {EX Stage} -group ALU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/operand_a_neg
add wave -noupdate -expand -group {EX Stage} -group ALU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/operand_a_neg_rev
add wave -noupdate -expand -group {EX Stage} -group ALU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/operand_b_neg
add wave -noupdate -expand -group {EX Stage} -group ALU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/div_shift
add wave -noupdate -expand -group {EX Stage} -group ALU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/div_valid
add wave -noupdate -expand -group {EX Stage} -group ALU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/bmask
add wave -noupdate -expand -group {EX Stage} -group ALU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/adder_op_b_negate
add wave -noupdate -expand -group {EX Stage} -group ALU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/adder_op_a
add wave -noupdate -expand -group {EX Stage} -group ALU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/adder_op_b
add wave -noupdate -expand -group {EX Stage} -group ALU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/adder_in_a
add wave -noupdate -expand -group {EX Stage} -group ALU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/adder_in_b
add wave -noupdate -expand -group {EX Stage} -group ALU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/adder_result
add wave -noupdate -expand -group {EX Stage} -group ALU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/adder_result_expanded
add wave -noupdate -expand -group {EX Stage} -group ALU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/adder_round_value
add wave -noupdate -expand -group {EX Stage} -group ALU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/adder_round_result
add wave -noupdate -expand -group {EX Stage} -group ALU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/shift_left
add wave -noupdate -expand -group {EX Stage} -group ALU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/shift_use_round
add wave -noupdate -expand -group {EX Stage} -group ALU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/shift_arithmetic
add wave -noupdate -expand -group {EX Stage} -group ALU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/shift_amt_left
add wave -noupdate -expand -group {EX Stage} -group ALU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/shift_amt
add wave -noupdate -expand -group {EX Stage} -group ALU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/shift_amt_int
add wave -noupdate -expand -group {EX Stage} -group ALU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/shift_amt_norm
add wave -noupdate -expand -group {EX Stage} -group ALU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/shift_op_a
add wave -noupdate -expand -group {EX Stage} -group ALU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/shift_result
add wave -noupdate -expand -group {EX Stage} -group ALU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/shift_right_result
add wave -noupdate -expand -group {EX Stage} -group ALU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/shift_left_result
add wave -noupdate -expand -group {EX Stage} -group ALU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/shift_op_a_32
add wave -noupdate -expand -group {EX Stage} -group ALU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/is_equal
add wave -noupdate -expand -group {EX Stage} -group ALU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/is_greater
add wave -noupdate -expand -group {EX Stage} -group ALU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/f_is_greater
add wave -noupdate -expand -group {EX Stage} -group ALU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/cmp_signed
add wave -noupdate -expand -group {EX Stage} -group ALU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/is_equal_vec
add wave -noupdate -expand -group {EX Stage} -group ALU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/is_greater_vec
add wave -noupdate -expand -group {EX Stage} -group ALU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/cmp_result
add wave -noupdate -expand -group {EX Stage} -group ALU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/f_is_qnan
add wave -noupdate -expand -group {EX Stage} -group ALU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/f_is_snan
add wave -noupdate -expand -group {EX Stage} -group ALU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/f_is_nan
add wave -noupdate -expand -group {EX Stage} -group ALU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/result_minmax
add wave -noupdate -expand -group {EX Stage} -group ALU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/fp_canonical_nan
add wave -noupdate -expand -group {EX Stage} -group ALU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/sel_minmax
add wave -noupdate -expand -group {EX Stage} -group ALU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/do_min
add wave -noupdate -expand -group {EX Stage} -group ALU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/minmax_is_fp_special
add wave -noupdate -expand -group {EX Stage} -group ALU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/minmax_b
add wave -noupdate -expand -group {EX Stage} -group ALU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/fclass_result
add wave -noupdate -expand -group {EX Stage} -group ALU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/f_sign_inject_result
add wave -noupdate -expand -group {EX Stage} -group ALU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/clip_result
add wave -noupdate -expand -group {EX Stage} -group ALU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/clip_is_lower_neg
add wave -noupdate -expand -group {EX Stage} -group ALU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/clip_is_lower_u
add wave -noupdate -expand -group {EX Stage} -group ALU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/shuffle_byte_sel
add wave -noupdate -expand -group {EX Stage} -group ALU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/shuffle_reg_sel
add wave -noupdate -expand -group {EX Stage} -group ALU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/shuffle_reg1_sel
add wave -noupdate -expand -group {EX Stage} -group ALU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/shuffle_reg0_sel
add wave -noupdate -expand -group {EX Stage} -group ALU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/shuffle_through
add wave -noupdate -expand -group {EX Stage} -group ALU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/shuffle_r1
add wave -noupdate -expand -group {EX Stage} -group ALU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/shuffle_r0
add wave -noupdate -expand -group {EX Stage} -group ALU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/shuffle_r1_in
add wave -noupdate -expand -group {EX Stage} -group ALU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/shuffle_r0_in
add wave -noupdate -expand -group {EX Stage} -group ALU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/shuffle_result
add wave -noupdate -expand -group {EX Stage} -group ALU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/pack_result
add wave -noupdate -expand -group {EX Stage} -group ALU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/ff_input
add wave -noupdate -expand -group {EX Stage} -group ALU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/cnt_result
add wave -noupdate -expand -group {EX Stage} -group ALU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/clb_result
add wave -noupdate -expand -group {EX Stage} -group ALU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/ff1_result
add wave -noupdate -expand -group {EX Stage} -group ALU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/ff_no_one
add wave -noupdate -expand -group {EX Stage} -group ALU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/fl1_result
add wave -noupdate -expand -group {EX Stage} -group ALU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/bitop_result
add wave -noupdate -expand -group {EX Stage} -group ALU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/extract_is_signed
add wave -noupdate -expand -group {EX Stage} -group ALU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/extract_sign
add wave -noupdate -expand -group {EX Stage} -group ALU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/bmask_first
add wave -noupdate -expand -group {EX Stage} -group ALU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/bmask_inv
add wave -noupdate -expand -group {EX Stage} -group ALU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/bextins_and
add wave -noupdate -expand -group {EX Stage} -group ALU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/bextins_result
add wave -noupdate -expand -group {EX Stage} -group ALU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/bclr_result
add wave -noupdate -expand -group {EX Stage} -group ALU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/bset_result
add wave -noupdate -expand -group {EX Stage} -group ALU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/result_div
add wave -noupdate -expand -group {EX Stage} -group ALU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/div_ready
add wave -noupdate -expand -group {EX Stage} -group ALU_DIV /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/int_div/div_i/Clk_CI
add wave -noupdate -expand -group {EX Stage} -group ALU_DIV /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/int_div/div_i/Rst_RBI
add wave -noupdate -expand -group {EX Stage} -group ALU_DIV /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/int_div/div_i/OpA_DI
add wave -noupdate -expand -group {EX Stage} -group ALU_DIV /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/int_div/div_i/OpB_DI
add wave -noupdate -expand -group {EX Stage} -group ALU_DIV /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/int_div/div_i/OpBShift_DI
add wave -noupdate -expand -group {EX Stage} -group ALU_DIV /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/int_div/div_i/OpBIsZero_SI
add wave -noupdate -expand -group {EX Stage} -group ALU_DIV /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/int_div/div_i/OpBSign_SI
add wave -noupdate -expand -group {EX Stage} -group ALU_DIV /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/int_div/div_i/OpCode_SI
add wave -noupdate -expand -group {EX Stage} -group ALU_DIV /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/int_div/div_i/InVld_SI
add wave -noupdate -expand -group {EX Stage} -group ALU_DIV /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/int_div/div_i/OutRdy_SI
add wave -noupdate -expand -group {EX Stage} -group ALU_DIV /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/int_div/div_i/OutVld_SO
add wave -noupdate -expand -group {EX Stage} -group ALU_DIV /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/int_div/div_i/Res_DO
add wave -noupdate -expand -group {EX Stage} -group ALU_DIV /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/int_div/div_i/ResReg_DP
add wave -noupdate -expand -group {EX Stage} -group ALU_DIV /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/int_div/div_i/ResReg_DN
add wave -noupdate -expand -group {EX Stage} -group ALU_DIV /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/int_div/div_i/ResReg_DP_rev
add wave -noupdate -expand -group {EX Stage} -group ALU_DIV /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/int_div/div_i/AReg_DP
add wave -noupdate -expand -group {EX Stage} -group ALU_DIV /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/int_div/div_i/AReg_DN
add wave -noupdate -expand -group {EX Stage} -group ALU_DIV /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/int_div/div_i/BReg_DP
add wave -noupdate -expand -group {EX Stage} -group ALU_DIV /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/int_div/div_i/BReg_DN
add wave -noupdate -expand -group {EX Stage} -group ALU_DIV /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/int_div/div_i/RemSel_SN
add wave -noupdate -expand -group {EX Stage} -group ALU_DIV /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/int_div/div_i/RemSel_SP
add wave -noupdate -expand -group {EX Stage} -group ALU_DIV /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/int_div/div_i/CompInv_SN
add wave -noupdate -expand -group {EX Stage} -group ALU_DIV /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/int_div/div_i/CompInv_SP
add wave -noupdate -expand -group {EX Stage} -group ALU_DIV /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/int_div/div_i/ResInv_SN
add wave -noupdate -expand -group {EX Stage} -group ALU_DIV /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/int_div/div_i/ResInv_SP
add wave -noupdate -expand -group {EX Stage} -group ALU_DIV /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/int_div/div_i/AddMux_D
add wave -noupdate -expand -group {EX Stage} -group ALU_DIV /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/int_div/div_i/AddOut_D
add wave -noupdate -expand -group {EX Stage} -group ALU_DIV /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/int_div/div_i/AddTmp_D
add wave -noupdate -expand -group {EX Stage} -group ALU_DIV /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/int_div/div_i/BMux_D
add wave -noupdate -expand -group {EX Stage} -group ALU_DIV /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/int_div/div_i/OutMux_D
add wave -noupdate -expand -group {EX Stage} -group ALU_DIV /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/int_div/div_i/Cnt_DP
add wave -noupdate -expand -group {EX Stage} -group ALU_DIV /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/int_div/div_i/Cnt_DN
add wave -noupdate -expand -group {EX Stage} -group ALU_DIV /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/int_div/div_i/CntZero_S
add wave -noupdate -expand -group {EX Stage} -group ALU_DIV /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/int_div/div_i/ARegEn_S
add wave -noupdate -expand -group {EX Stage} -group ALU_DIV /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/int_div/div_i/BRegEn_S
add wave -noupdate -expand -group {EX Stage} -group ALU_DIV /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/int_div/div_i/ResRegEn_S
add wave -noupdate -expand -group {EX Stage} -group ALU_DIV /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/int_div/div_i/ABComp_S
add wave -noupdate -expand -group {EX Stage} -group ALU_DIV /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/int_div/div_i/PmSel_S
add wave -noupdate -expand -group {EX Stage} -group ALU_DIV /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/int_div/div_i/LoadEn_S
add wave -noupdate -expand -group {EX Stage} -group ALU_DIV /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/int_div/div_i/State_SN
add wave -noupdate -expand -group {EX Stage} -group ALU_DIV /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_i/int_div/div_i/State_SP
add wave -noupdate -expand -group {EX Stage} -group MUL /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/mult_i/clk
add wave -noupdate -expand -group {EX Stage} -group MUL /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/mult_i/rst_n
add wave -noupdate -expand -group {EX Stage} -group MUL /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/mult_i/enable_i
add wave -noupdate -expand -group {EX Stage} -group MUL /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/mult_i/operator_i
add wave -noupdate -expand -group {EX Stage} -group MUL /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/mult_i/short_subword_i
add wave -noupdate -expand -group {EX Stage} -group MUL /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/mult_i/short_signed_i
add wave -noupdate -expand -group {EX Stage} -group MUL /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/mult_i/op_a_i
add wave -noupdate -expand -group {EX Stage} -group MUL /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/mult_i/op_b_i
add wave -noupdate -expand -group {EX Stage} -group MUL /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/mult_i/op_c_i
add wave -noupdate -expand -group {EX Stage} -group MUL /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/mult_i/imm_i
add wave -noupdate -expand -group {EX Stage} -group MUL /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/mult_i/dot_signed_i
add wave -noupdate -expand -group {EX Stage} -group MUL /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/mult_i/dot_op_a_i
add wave -noupdate -expand -group {EX Stage} -group MUL /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/mult_i/dot_op_b_i
add wave -noupdate -expand -group {EX Stage} -group MUL /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/mult_i/dot_op_c_i
add wave -noupdate -expand -group {EX Stage} -group MUL /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/mult_i/result_o
add wave -noupdate -expand -group {EX Stage} -group MUL /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/mult_i/multicycle_o
add wave -noupdate -expand -group {EX Stage} -group MUL /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/mult_i/ready_o
add wave -noupdate -expand -group {EX Stage} -group MUL /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/mult_i/ex_ready_i
add wave -noupdate -expand -group {EX Stage} -group MUL /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/mult_i/short_op_a
add wave -noupdate -expand -group {EX Stage} -group MUL /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/mult_i/short_op_b
add wave -noupdate -expand -group {EX Stage} -group MUL /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/mult_i/short_op_c
add wave -noupdate -expand -group {EX Stage} -group MUL /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/mult_i/short_mul
add wave -noupdate -expand -group {EX Stage} -group MUL /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/mult_i/short_mac
add wave -noupdate -expand -group {EX Stage} -group MUL /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/mult_i/short_round
add wave -noupdate -expand -group {EX Stage} -group MUL /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/mult_i/short_round_tmp
add wave -noupdate -expand -group {EX Stage} -group MUL /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/mult_i/short_result
add wave -noupdate -expand -group {EX Stage} -group MUL /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/mult_i/short_mac_msb1
add wave -noupdate -expand -group {EX Stage} -group MUL /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/mult_i/short_mac_msb0
add wave -noupdate -expand -group {EX Stage} -group MUL /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/mult_i/short_imm
add wave -noupdate -expand -group {EX Stage} -group MUL /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/mult_i/short_subword
add wave -noupdate -expand -group {EX Stage} -group MUL /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/mult_i/short_signed
add wave -noupdate -expand -group {EX Stage} -group MUL /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/mult_i/short_shift_arith
add wave -noupdate -expand -group {EX Stage} -group MUL /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/mult_i/mulh_imm
add wave -noupdate -expand -group {EX Stage} -group MUL /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/mult_i/mulh_subword
add wave -noupdate -expand -group {EX Stage} -group MUL /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/mult_i/mulh_signed
add wave -noupdate -expand -group {EX Stage} -group MUL /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/mult_i/mulh_shift_arith
add wave -noupdate -expand -group {EX Stage} -group MUL /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/mult_i/mulh_carry_q
add wave -noupdate -expand -group {EX Stage} -group MUL /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/mult_i/mulh_active
add wave -noupdate -expand -group {EX Stage} -group MUL /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/mult_i/mulh_save
add wave -noupdate -expand -group {EX Stage} -group MUL /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/mult_i/mulh_clearcarry
add wave -noupdate -expand -group {EX Stage} -group MUL /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/mult_i/mulh_ready
add wave -noupdate -expand -group {EX Stage} -group MUL /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/mult_i/mulh_CS
add wave -noupdate -expand -group {EX Stage} -group MUL /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/mult_i/mulh_NS
add wave -noupdate -expand -group {EX Stage} -group MUL /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/mult_i/int_op_a_msu
add wave -noupdate -expand -group {EX Stage} -group MUL /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/mult_i/int_op_b_msu
add wave -noupdate -expand -group {EX Stage} -group MUL /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/mult_i/int_result
add wave -noupdate -expand -group {EX Stage} -group MUL /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/mult_i/int_is_msu
add wave -noupdate -expand -group {EX Stage} -group MUL /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/mult_i/dot_char_result
add wave -noupdate -expand -group {EX Stage} -group MUL /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/mult_i/dot_short_result
add wave -noupdate -expand -group {EX Stage} -group APU_DISP /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/genblk1/apu_disp_i/clk_i
add wave -noupdate -expand -group {EX Stage} -group APU_DISP /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/genblk1/apu_disp_i/rst_ni
add wave -noupdate -expand -group {EX Stage} -group APU_DISP /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/genblk1/apu_disp_i/enable_i
add wave -noupdate -expand -group {EX Stage} -group APU_DISP /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/genblk1/apu_disp_i/apu_lat_i
add wave -noupdate -expand -group {EX Stage} -group APU_DISP /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/genblk1/apu_disp_i/apu_waddr_i
add wave -noupdate -expand -group {EX Stage} -group APU_DISP /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/genblk1/apu_disp_i/apu_waddr_o
add wave -noupdate -expand -group {EX Stage} -group APU_DISP /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/genblk1/apu_disp_i/apu_multicycle_o
add wave -noupdate -expand -group {EX Stage} -group APU_DISP /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/genblk1/apu_disp_i/apu_singlecycle_o
add wave -noupdate -expand -group {EX Stage} -group APU_DISP /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/genblk1/apu_disp_i/active_o
add wave -noupdate -expand -group {EX Stage} -group APU_DISP /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/genblk1/apu_disp_i/stall_o
add wave -noupdate -expand -group {EX Stage} -group APU_DISP /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/genblk1/apu_disp_i/read_regs_i
add wave -noupdate -expand -group {EX Stage} -group APU_DISP /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/genblk1/apu_disp_i/read_regs_valid_i
add wave -noupdate -expand -group {EX Stage} -group APU_DISP /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/genblk1/apu_disp_i/read_dep_o
add wave -noupdate -expand -group {EX Stage} -group APU_DISP /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/genblk1/apu_disp_i/write_regs_i
add wave -noupdate -expand -group {EX Stage} -group APU_DISP /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/genblk1/apu_disp_i/write_regs_valid_i
add wave -noupdate -expand -group {EX Stage} -group APU_DISP /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/genblk1/apu_disp_i/write_dep_o
add wave -noupdate -expand -group {EX Stage} -group APU_DISP /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/genblk1/apu_disp_i/perf_type_o
add wave -noupdate -expand -group {EX Stage} -group APU_DISP /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/genblk1/apu_disp_i/perf_cont_o
add wave -noupdate -expand -group {EX Stage} -group APU_DISP /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/genblk1/apu_disp_i/apu_master_req_o
add wave -noupdate -expand -group {EX Stage} -group APU_DISP /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/genblk1/apu_disp_i/apu_master_ready_o
add wave -noupdate -expand -group {EX Stage} -group APU_DISP /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/genblk1/apu_disp_i/apu_master_gnt_i
add wave -noupdate -expand -group {EX Stage} -group APU_DISP /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/genblk1/apu_disp_i/apu_master_valid_i
add wave -noupdate -expand -group {EX Stage} -group APU_DISP /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/genblk1/apu_disp_i/addr_req
add wave -noupdate -expand -group {EX Stage} -group APU_DISP /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/genblk1/apu_disp_i/addr_inflight
add wave -noupdate -expand -group {EX Stage} -group APU_DISP /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/genblk1/apu_disp_i/addr_waiting
add wave -noupdate -expand -group {EX Stage} -group APU_DISP /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/genblk1/apu_disp_i/addr_inflight_dn
add wave -noupdate -expand -group {EX Stage} -group APU_DISP /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/genblk1/apu_disp_i/addr_waiting_dn
add wave -noupdate -expand -group {EX Stage} -group APU_DISP /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/genblk1/apu_disp_i/valid_req
add wave -noupdate -expand -group {EX Stage} -group APU_DISP /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/genblk1/apu_disp_i/valid_inflight
add wave -noupdate -expand -group {EX Stage} -group APU_DISP /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/genblk1/apu_disp_i/valid_waiting
add wave -noupdate -expand -group {EX Stage} -group APU_DISP /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/genblk1/apu_disp_i/valid_inflight_dn
add wave -noupdate -expand -group {EX Stage} -group APU_DISP /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/genblk1/apu_disp_i/valid_waiting_dn
add wave -noupdate -expand -group {EX Stage} -group APU_DISP /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/genblk1/apu_disp_i/returned_req
add wave -noupdate -expand -group {EX Stage} -group APU_DISP /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/genblk1/apu_disp_i/returned_inflight
add wave -noupdate -expand -group {EX Stage} -group APU_DISP /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/genblk1/apu_disp_i/returned_waiting
add wave -noupdate -expand -group {EX Stage} -group APU_DISP /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/genblk1/apu_disp_i/req_accepted
add wave -noupdate -expand -group {EX Stage} -group APU_DISP /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/genblk1/apu_disp_i/active
add wave -noupdate -expand -group {EX Stage} -group APU_DISP /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/genblk1/apu_disp_i/apu_lat
add wave -noupdate -expand -group {EX Stage} -group APU_DISP /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/genblk1/apu_disp_i/read_deps_req
add wave -noupdate -expand -group {EX Stage} -group APU_DISP /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/genblk1/apu_disp_i/read_deps_inflight
add wave -noupdate -expand -group {EX Stage} -group APU_DISP /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/genblk1/apu_disp_i/read_deps_waiting
add wave -noupdate -expand -group {EX Stage} -group APU_DISP /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/genblk1/apu_disp_i/write_deps_req
add wave -noupdate -expand -group {EX Stage} -group APU_DISP /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/genblk1/apu_disp_i/write_deps_inflight
add wave -noupdate -expand -group {EX Stage} -group APU_DISP /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/genblk1/apu_disp_i/write_deps_waiting
add wave -noupdate -expand -group {EX Stage} -group APU_DISP /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/genblk1/apu_disp_i/read_dep_req
add wave -noupdate -expand -group {EX Stage} -group APU_DISP /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/genblk1/apu_disp_i/read_dep_inflight
add wave -noupdate -expand -group {EX Stage} -group APU_DISP /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/genblk1/apu_disp_i/read_dep_waiting
add wave -noupdate -expand -group {EX Stage} -group APU_DISP /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/genblk1/apu_disp_i/write_dep_req
add wave -noupdate -expand -group {EX Stage} -group APU_DISP /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/genblk1/apu_disp_i/write_dep_inflight
add wave -noupdate -expand -group {EX Stage} -group APU_DISP /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/genblk1/apu_disp_i/write_dep_waiting
add wave -noupdate -expand -group {EX Stage} -group APU_DISP /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/genblk1/apu_disp_i/stall_full
add wave -noupdate -expand -group {EX Stage} -group APU_DISP /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/genblk1/apu_disp_i/stall_type
add wave -noupdate -expand -group {EX Stage} -group APU_DISP /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/genblk1/apu_disp_i/stall_nack
add wave -noupdate -expand -group {EX Stage} -group FPU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/genblk1/genblk1/fpu_i/clk_i
add wave -noupdate -expand -group {EX Stage} -group FPU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/genblk1/genblk1/fpu_i/rst_ni
add wave -noupdate -expand -group {EX Stage} -group FPU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/genblk1/genblk1/fpu_i/fpu_en_i
add wave -noupdate -expand -group {EX Stage} -group FPU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/genblk1/genblk1/fpu_i/operand_a_i
add wave -noupdate -expand -group {EX Stage} -group FPU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/genblk1/genblk1/fpu_i/operand_b_i
add wave -noupdate -expand -group {EX Stage} -group FPU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/genblk1/genblk1/fpu_i/operand_c_i
add wave -noupdate -expand -group {EX Stage} -group FPU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/genblk1/genblk1/fpu_i/rm_i
add wave -noupdate -expand -group {EX Stage} -group FPU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/genblk1/genblk1/fpu_i/fpu_op_i
add wave -noupdate -expand -group {EX Stage} -group FPU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/genblk1/genblk1/fpu_i/prec_i
add wave -noupdate -expand -group {EX Stage} -group FPU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/genblk1/genblk1/fpu_i/result_o
add wave -noupdate -expand -group {EX Stage} -group FPU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/genblk1/genblk1/fpu_i/valid_o
add wave -noupdate -expand -group {EX Stage} -group FPU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/genblk1/genblk1/fpu_i/flags_o
add wave -noupdate -expand -group {EX Stage} -group FPU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/genblk1/genblk1/fpu_i/divsqrt_busy_o
add wave -noupdate -expand -group {EX Stage} -group FPU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/genblk1/genblk1/fpu_i/divsqrt_enable
add wave -noupdate -expand -group {EX Stage} -group FPU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/genblk1/genblk1/fpu_i/fpu_enable
add wave -noupdate -expand -group {EX Stage} -group FPU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/genblk1/genblk1/fpu_i/fma_enable
add wave -noupdate -expand -group {EX Stage} -group FPU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/genblk1/genblk1/fpu_i/fpu_operand_a
add wave -noupdate -expand -group {EX Stage} -group FPU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/genblk1/genblk1/fpu_i/fpu_operand_b
add wave -noupdate -expand -group {EX Stage} -group FPU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/genblk1/genblk1/fpu_i/fpu_result
add wave -noupdate -expand -group {EX Stage} -group FPU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/genblk1/genblk1/fpu_i/fpu_flags
add wave -noupdate -expand -group {EX Stage} -group FPU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/genblk1/genblk1/fpu_i/fpu_of
add wave -noupdate -expand -group {EX Stage} -group FPU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/genblk1/genblk1/fpu_i/fpu_uf
add wave -noupdate -expand -group {EX Stage} -group FPU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/genblk1/genblk1/fpu_i/fpu_zero
add wave -noupdate -expand -group {EX Stage} -group FPU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/genblk1/genblk1/fpu_i/fpu_ix
add wave -noupdate -expand -group {EX Stage} -group FPU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/genblk1/genblk1/fpu_i/fpu_iv
add wave -noupdate -expand -group {EX Stage} -group FPU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/genblk1/genblk1/fpu_i/fpu_inf
add wave -noupdate -expand -group {EX Stage} -group FPU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/genblk1/genblk1/fpu_i/fpu_valid
add wave -noupdate -expand -group {EX Stage} -group FPU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/genblk1/genblk1/fpu_i/div_start
add wave -noupdate -expand -group {EX Stage} -group FPU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/genblk1/genblk1/fpu_i/sqrt_start
add wave -noupdate -expand -group {EX Stage} -group FPU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/genblk1/genblk1/fpu_i/divsqrt_operand_a
add wave -noupdate -expand -group {EX Stage} -group FPU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/genblk1/genblk1/fpu_i/divsqrt_operand_b
add wave -noupdate -expand -group {EX Stage} -group FPU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/genblk1/genblk1/fpu_i/divsqrt_result
add wave -noupdate -expand -group {EX Stage} -group FPU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/genblk1/genblk1/fpu_i/divsqrt_flags
add wave -noupdate -expand -group {EX Stage} -group FPU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/genblk1/genblk1/fpu_i/divsqrt_nv
add wave -noupdate -expand -group {EX Stage} -group FPU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/genblk1/genblk1/fpu_i/divsqrt_ix
add wave -noupdate -expand -group {EX Stage} -group FPU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/genblk1/genblk1/fpu_i/divsqrt_of
add wave -noupdate -expand -group {EX Stage} -group FPU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/genblk1/genblk1/fpu_i/divsqrt_uf
add wave -noupdate -expand -group {EX Stage} -group FPU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/genblk1/genblk1/fpu_i/divsqrt_zero
add wave -noupdate -expand -group {EX Stage} -group FPU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/genblk1/genblk1/fpu_i/divsqrt_valid
add wave -noupdate -expand -group {EX Stage} -group FPU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/genblk1/genblk1/fpu_i/fma_operand_a
add wave -noupdate -expand -group {EX Stage} -group FPU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/genblk1/genblk1/fpu_i/fma_operand_b
add wave -noupdate -expand -group {EX Stage} -group FPU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/genblk1/genblk1/fpu_i/fma_operand_c
add wave -noupdate -expand -group {EX Stage} -group FPU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/genblk1/genblk1/fpu_i/fma_result
add wave -noupdate -expand -group {EX Stage} -group FPU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/genblk1/genblk1/fpu_i/fma_op
add wave -noupdate -expand -group {EX Stage} -group FPU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/genblk1/genblk1/fpu_i/fma_valid
add wave -noupdate -expand -group {EX Stage} -group FPU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/genblk1/genblk1/fpu_i/fma_flags
add wave -noupdate -expand -group {EX Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/clk
add wave -noupdate -expand -group {EX Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/rst_n
add wave -noupdate -expand -group {EX Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_operator_i
add wave -noupdate -expand -group {EX Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_operand_a_i
add wave -noupdate -expand -group {EX Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_operand_b_i
add wave -noupdate -expand -group {EX Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_operand_c_i
add wave -noupdate -expand -group {EX Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_en_i
add wave -noupdate -expand -group {EX Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/bmask_a_i
add wave -noupdate -expand -group {EX Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/bmask_b_i
add wave -noupdate -expand -group {EX Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/imm_vec_ext_i
add wave -noupdate -expand -group {EX Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_vec_mode_i
add wave -noupdate -expand -group {EX Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/mult_operator_i
add wave -noupdate -expand -group {EX Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/mult_operand_a_i
add wave -noupdate -expand -group {EX Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/mult_operand_b_i
add wave -noupdate -expand -group {EX Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/mult_operand_c_i
add wave -noupdate -expand -group {EX Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/mult_en_i
add wave -noupdate -expand -group {EX Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/mult_sel_subword_i
add wave -noupdate -expand -group {EX Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/mult_signed_mode_i
add wave -noupdate -expand -group {EX Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/mult_imm_i
add wave -noupdate -expand -group {EX Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/mult_dot_op_a_i
add wave -noupdate -expand -group {EX Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/mult_dot_op_b_i
add wave -noupdate -expand -group {EX Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/mult_dot_op_c_i
add wave -noupdate -expand -group {EX Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/mult_dot_signed_i
add wave -noupdate -expand -group {EX Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/mult_multicycle_o
add wave -noupdate -expand -group {EX Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/fpu_op_i
add wave -noupdate -expand -group {EX Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/fpu_prec_i
add wave -noupdate -expand -group {EX Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/fpu_fflags_o
add wave -noupdate -expand -group {EX Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/fpu_fflags_we_o
add wave -noupdate -expand -group {EX Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/apu_en_i
add wave -noupdate -expand -group {EX Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/apu_op_i
add wave -noupdate -expand -group {EX Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/apu_lat_i
add wave -noupdate -expand -group {EX Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/apu_waddr_i
add wave -noupdate -expand -group {EX Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/apu_flags_i
add wave -noupdate -expand -group {EX Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/apu_read_regs_i
add wave -noupdate -expand -group {EX Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/apu_read_regs_valid_i
add wave -noupdate -expand -group {EX Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/apu_read_dep_o
add wave -noupdate -expand -group {EX Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/apu_write_regs_i
add wave -noupdate -expand -group {EX Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/apu_write_regs_valid_i
add wave -noupdate -expand -group {EX Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/apu_write_dep_o
add wave -noupdate -expand -group {EX Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/apu_perf_type_o
add wave -noupdate -expand -group {EX Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/apu_perf_cont_o
add wave -noupdate -expand -group {EX Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/apu_perf_wb_o
add wave -noupdate -expand -group {EX Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/apu_busy_o
add wave -noupdate -expand -group {EX Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/apu_ready_wb_o
add wave -noupdate -expand -group {EX Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/apu_master_req_o
add wave -noupdate -expand -group {EX Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/apu_master_ready_o
add wave -noupdate -expand -group {EX Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/apu_master_gnt_i
add wave -noupdate -expand -group {EX Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/apu_master_op_o
add wave -noupdate -expand -group {EX Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/apu_master_valid_i
add wave -noupdate -expand -group {EX Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/apu_master_result_i
add wave -noupdate -expand -group {EX Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/lsu_en_i
add wave -noupdate -expand -group {EX Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/lsu_rdata_i
add wave -noupdate -expand -group {EX Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/branch_in_ex_i
add wave -noupdate -expand -group {EX Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/regfile_alu_waddr_i
add wave -noupdate -expand -group {EX Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/regfile_alu_we_i
add wave -noupdate -expand -group {EX Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/regfile_we_i
add wave -noupdate -expand -group {EX Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/regfile_waddr_i
add wave -noupdate -expand -group {EX Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/csr_access_i
add wave -noupdate -expand -group {EX Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/csr_rdata_i
add wave -noupdate -expand -group {EX Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/regfile_waddr_wb_o
add wave -noupdate -expand -group {EX Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/regfile_we_wb_o
add wave -noupdate -expand -group {EX Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/regfile_wdata_wb_o
add wave -noupdate -expand -group {EX Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/regfile_alu_waddr_fw_o
add wave -noupdate -expand -group {EX Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/regfile_alu_we_fw_o
add wave -noupdate -expand -group {EX Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/regfile_alu_wdata_fw_o
add wave -noupdate -expand -group {EX Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/jump_target_o
add wave -noupdate -expand -group {EX Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/branch_decision_o
add wave -noupdate -expand -group {EX Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/lsu_ready_ex_i
add wave -noupdate -expand -group {EX Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/ex_ready_o
add wave -noupdate -expand -group {EX Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/ex_valid_o
add wave -noupdate -expand -group {EX Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/wb_ready_i
add wave -noupdate -expand -group {EX Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_result
add wave -noupdate -expand -group {EX Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/mult_result
add wave -noupdate -expand -group {EX Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_cmp_result
add wave -noupdate -expand -group {EX Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/regfile_we_lsu
add wave -noupdate -expand -group {EX Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/regfile_waddr_lsu
add wave -noupdate -expand -group {EX Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/wb_contention
add wave -noupdate -expand -group {EX Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/wb_contention_lsu
add wave -noupdate -expand -group {EX Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/alu_ready
add wave -noupdate -expand -group {EX Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/mult_ready
add wave -noupdate -expand -group {EX Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/fpu_busy
add wave -noupdate -expand -group {EX Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/apu_valid
add wave -noupdate -expand -group {EX Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/apu_waddr
add wave -noupdate -expand -group {EX Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/apu_result
add wave -noupdate -expand -group {EX Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/apu_stall
add wave -noupdate -expand -group {EX Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/apu_active
add wave -noupdate -expand -group {EX Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/apu_singlecycle
add wave -noupdate -expand -group {EX Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/apu_multicycle
add wave -noupdate -expand -group {EX Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/apu_req
add wave -noupdate -expand -group {EX Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/apu_ready
add wave -noupdate -expand -group {EX Stage} /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/ex_stage_i/apu_gnt
add wave -noupdate -expand -group LSU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/load_store_unit_i/clk
add wave -noupdate -expand -group LSU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/load_store_unit_i/rst_n
add wave -noupdate -expand -group LSU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/load_store_unit_i/data_req_o
add wave -noupdate -expand -group LSU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/load_store_unit_i/data_gnt_i
add wave -noupdate -expand -group LSU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/load_store_unit_i/data_rvalid_i
add wave -noupdate -expand -group LSU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/load_store_unit_i/data_err_i
add wave -noupdate -expand -group LSU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/load_store_unit_i/data_addr_o
add wave -noupdate -expand -group LSU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/load_store_unit_i/data_we_o
add wave -noupdate -expand -group LSU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/load_store_unit_i/data_be_o
add wave -noupdate -expand -group LSU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/load_store_unit_i/data_wdata_o
add wave -noupdate -expand -group LSU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/load_store_unit_i/data_rdata_i
add wave -noupdate -expand -group LSU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/load_store_unit_i/data_we_ex_i
add wave -noupdate -expand -group LSU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/load_store_unit_i/data_type_ex_i
add wave -noupdate -expand -group LSU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/load_store_unit_i/data_wdata_ex_i
add wave -noupdate -expand -group LSU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/load_store_unit_i/data_reg_offset_ex_i
add wave -noupdate -expand -group LSU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/load_store_unit_i/data_sign_ext_ex_i
add wave -noupdate -expand -group LSU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/load_store_unit_i/data_rdata_ex_o
add wave -noupdate -expand -group LSU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/load_store_unit_i/data_req_ex_i
add wave -noupdate -expand -group LSU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/load_store_unit_i/operand_a_ex_i
add wave -noupdate -expand -group LSU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/load_store_unit_i/operand_b_ex_i
add wave -noupdate -expand -group LSU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/load_store_unit_i/addr_useincr_ex_i
add wave -noupdate -expand -group LSU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/load_store_unit_i/data_misaligned_ex_i
add wave -noupdate -expand -group LSU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/load_store_unit_i/data_misaligned_o
add wave -noupdate -expand -group LSU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/load_store_unit_i/load_err_o
add wave -noupdate -expand -group LSU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/load_store_unit_i/store_err_o
add wave -noupdate -expand -group LSU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/load_store_unit_i/lsu_ready_ex_o
add wave -noupdate -expand -group LSU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/load_store_unit_i/lsu_ready_wb_o
add wave -noupdate -expand -group LSU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/load_store_unit_i/ex_valid_i
add wave -noupdate -expand -group LSU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/load_store_unit_i/busy_o
add wave -noupdate -expand -group LSU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/load_store_unit_i/data_addr_int
add wave -noupdate -expand -group LSU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/load_store_unit_i/data_type_q
add wave -noupdate -expand -group LSU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/load_store_unit_i/rdata_offset_q
add wave -noupdate -expand -group LSU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/load_store_unit_i/data_sign_ext_q
add wave -noupdate -expand -group LSU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/load_store_unit_i/data_we_q
add wave -noupdate -expand -group LSU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/load_store_unit_i/wdata_offset
add wave -noupdate -expand -group LSU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/load_store_unit_i/data_be
add wave -noupdate -expand -group LSU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/load_store_unit_i/data_wdata
add wave -noupdate -expand -group LSU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/load_store_unit_i/misaligned_st
add wave -noupdate -expand -group LSU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/load_store_unit_i/CS
add wave -noupdate -expand -group LSU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/load_store_unit_i/NS
add wave -noupdate -expand -group LSU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/load_store_unit_i/rdata_q
add wave -noupdate -expand -group LSU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/load_store_unit_i/data_rdata_ext
add wave -noupdate -expand -group LSU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/load_store_unit_i/rdata_w_ext
add wave -noupdate -expand -group LSU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/load_store_unit_i/rdata_h_ext
add wave -noupdate -expand -group LSU /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/load_store_unit_i/rdata_b_ext
add wave -noupdate -expand -group CSR /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/cs_registers_i/clk
add wave -noupdate -expand -group CSR /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/cs_registers_i/rst_n
add wave -noupdate -expand -group CSR /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/cs_registers_i/core_id_i
add wave -noupdate -expand -group CSR /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/cs_registers_i/cluster_id_i
add wave -noupdate -expand -group CSR /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/cs_registers_i/mtvec_o
add wave -noupdate -expand -group CSR /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/cs_registers_i/utvec_o
add wave -noupdate -expand -group CSR /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/cs_registers_i/boot_addr_i
add wave -noupdate -expand -group CSR /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/cs_registers_i/csr_access_i
add wave -noupdate -expand -group CSR /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/cs_registers_i/csr_addr_i
add wave -noupdate -expand -group CSR /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/cs_registers_i/csr_wdata_i
add wave -noupdate -expand -group CSR /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/cs_registers_i/csr_op_i
add wave -noupdate -expand -group CSR /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/cs_registers_i/csr_rdata_o
add wave -noupdate -expand -group CSR /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/cs_registers_i/frm_o
add wave -noupdate -expand -group CSR /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/cs_registers_i/fprec_o
add wave -noupdate -expand -group CSR /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/cs_registers_i/fflags_i
add wave -noupdate -expand -group CSR /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/cs_registers_i/fflags_we_i
add wave -noupdate -expand -group CSR /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/cs_registers_i/m_irq_enable_o
add wave -noupdate -expand -group CSR /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/cs_registers_i/u_irq_enable_o
add wave -noupdate -expand -group CSR /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/cs_registers_i/csr_irq_sec_i
add wave -noupdate -expand -group CSR /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/cs_registers_i/sec_lvl_o
add wave -noupdate -expand -group CSR /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/cs_registers_i/epc_o
add wave -noupdate -expand -group CSR /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/cs_registers_i/priv_lvl_o
add wave -noupdate -expand -group CSR /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/cs_registers_i/pc_if_i
add wave -noupdate -expand -group CSR /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/cs_registers_i/pc_id_i
add wave -noupdate -expand -group CSR /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/cs_registers_i/csr_save_if_i
add wave -noupdate -expand -group CSR /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/cs_registers_i/csr_save_id_i
add wave -noupdate -expand -group CSR /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/cs_registers_i/csr_restore_mret_i
add wave -noupdate -expand -group CSR /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/cs_registers_i/csr_restore_uret_i
add wave -noupdate -expand -group CSR /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/cs_registers_i/csr_cause_i
add wave -noupdate -expand -group CSR /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/cs_registers_i/csr_save_cause_i
add wave -noupdate -expand -group CSR /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/cs_registers_i/hwlp_start_i
add wave -noupdate -expand -group CSR /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/cs_registers_i/hwlp_end_i
add wave -noupdate -expand -group CSR /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/cs_registers_i/hwlp_cnt_i
add wave -noupdate -expand -group CSR /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/cs_registers_i/hwlp_data_o
add wave -noupdate -expand -group CSR /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/cs_registers_i/hwlp_regid_o
add wave -noupdate -expand -group CSR /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/cs_registers_i/hwlp_we_o
add wave -noupdate -expand -group CSR /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/cs_registers_i/id_valid_i
add wave -noupdate -expand -group CSR /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/cs_registers_i/is_compressed_i
add wave -noupdate -expand -group CSR /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/cs_registers_i/is_decoding_i
add wave -noupdate -expand -group CSR /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/cs_registers_i/imiss_i
add wave -noupdate -expand -group CSR /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/cs_registers_i/pc_set_i
add wave -noupdate -expand -group CSR /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/cs_registers_i/jump_i
add wave -noupdate -expand -group CSR /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/cs_registers_i/branch_i
add wave -noupdate -expand -group CSR /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/cs_registers_i/branch_taken_i
add wave -noupdate -expand -group CSR /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/cs_registers_i/ld_stall_i
add wave -noupdate -expand -group CSR /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/cs_registers_i/jr_stall_i
add wave -noupdate -expand -group CSR /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/cs_registers_i/apu_typeconflict_i
add wave -noupdate -expand -group CSR /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/cs_registers_i/apu_contention_i
add wave -noupdate -expand -group CSR /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/cs_registers_i/apu_dep_i
add wave -noupdate -expand -group CSR /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/cs_registers_i/apu_wb_i
add wave -noupdate -expand -group CSR /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/cs_registers_i/mem_load_i
add wave -noupdate -expand -group CSR /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/cs_registers_i/mem_store_i
add wave -noupdate -expand -group CSR /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/cs_registers_i/ext_counters_i
add wave -noupdate -expand -group CSR /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/cs_registers_i/csr_wdata_int
add wave -noupdate -expand -group CSR /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/cs_registers_i/csr_rdata_int
add wave -noupdate -expand -group CSR /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/cs_registers_i/csr_we_int
add wave -noupdate -expand -group CSR /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/cs_registers_i/frm_q
add wave -noupdate -expand -group CSR /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/cs_registers_i/frm_n
add wave -noupdate -expand -group CSR /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/cs_registers_i/fflags_q
add wave -noupdate -expand -group CSR /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/cs_registers_i/fflags_n
add wave -noupdate -expand -group CSR /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/cs_registers_i/fprec_q
add wave -noupdate -expand -group CSR /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/cs_registers_i/fprec_n
add wave -noupdate -expand -group CSR /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/cs_registers_i/mepc_q
add wave -noupdate -expand -group CSR /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/cs_registers_i/mepc_n
add wave -noupdate -expand -group CSR /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/cs_registers_i/uepc_q
add wave -noupdate -expand -group CSR /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/cs_registers_i/uepc_n
add wave -noupdate -expand -group CSR /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/cs_registers_i/exception_pc
add wave -noupdate -expand -group CSR /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/cs_registers_i/mstatus_q
add wave -noupdate -expand -group CSR /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/cs_registers_i/mstatus_n
add wave -noupdate -expand -group CSR /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/cs_registers_i/mcause_q
add wave -noupdate -expand -group CSR /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/cs_registers_i/mcause_n
add wave -noupdate -expand -group CSR /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/cs_registers_i/ucause_q
add wave -noupdate -expand -group CSR /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/cs_registers_i/ucause_n
add wave -noupdate -expand -group CSR /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/cs_registers_i/mtvec_n
add wave -noupdate -expand -group CSR /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/cs_registers_i/mtvec_q
add wave -noupdate -expand -group CSR /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/cs_registers_i/mtvec_reg_q
add wave -noupdate -expand -group CSR /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/cs_registers_i/utvec_n
add wave -noupdate -expand -group CSR /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/cs_registers_i/utvec_q
add wave -noupdate -expand -group CSR /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/cs_registers_i/is_irq
add wave -noupdate -expand -group CSR /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/cs_registers_i/priv_lvl_n
add wave -noupdate -expand -group CSR /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/cs_registers_i/priv_lvl_q
add wave -noupdate -expand -group CSR /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/cs_registers_i/priv_lvl_reg_q
add wave -noupdate -expand -group CSR /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/cs_registers_i/id_valid_q
add wave -noupdate -expand -group CSR /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/cs_registers_i/PCCR_in
add wave -noupdate -expand -group CSR /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/cs_registers_i/PCCR_inc
add wave -noupdate -expand -group CSR /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/cs_registers_i/PCCR_inc_q
add wave -noupdate -expand -group CSR /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/cs_registers_i/PCCR_q
add wave -noupdate -expand -group CSR /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/cs_registers_i/PCCR_n
add wave -noupdate -expand -group CSR /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/cs_registers_i/PCMR_n
add wave -noupdate -expand -group CSR /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/cs_registers_i/PCMR_q
add wave -noupdate -expand -group CSR /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/cs_registers_i/PCER_n
add wave -noupdate -expand -group CSR /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/cs_registers_i/PCER_q
add wave -noupdate -expand -group CSR /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/cs_registers_i/perf_rdata
add wave -noupdate -expand -group CSR /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/cs_registers_i/pccr_index
add wave -noupdate -expand -group CSR /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/cs_registers_i/pccr_all_sel
add wave -noupdate -expand -group CSR /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/cs_registers_i/is_pccr
add wave -noupdate -expand -group CSR /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/cs_registers_i/is_pcer
add wave -noupdate -expand -group CSR /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/cs_registers_i/is_pcmr
add wave -noupdate -expand -group Debug /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/debug_unit_i/clk
add wave -noupdate -expand -group Debug /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/debug_unit_i/rst_n
add wave -noupdate -expand -group Debug /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/debug_unit_i/debug_req_i
add wave -noupdate -expand -group Debug /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/debug_unit_i/debug_gnt_o
add wave -noupdate -expand -group Debug /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/debug_unit_i/debug_rvalid_o
add wave -noupdate -expand -group Debug /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/debug_unit_i/debug_addr_i
add wave -noupdate -expand -group Debug /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/debug_unit_i/debug_we_i
add wave -noupdate -expand -group Debug /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/debug_unit_i/debug_wdata_i
add wave -noupdate -expand -group Debug /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/debug_unit_i/debug_rdata_o
add wave -noupdate -expand -group Debug /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/debug_unit_i/debug_halted_o
add wave -noupdate -expand -group Debug /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/debug_unit_i/debug_halt_i
add wave -noupdate -expand -group Debug /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/debug_unit_i/debug_resume_i
add wave -noupdate -expand -group Debug /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/debug_unit_i/settings_o
add wave -noupdate -expand -group Debug /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/debug_unit_i/trap_i
add wave -noupdate -expand -group Debug /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/debug_unit_i/exc_cause_i
add wave -noupdate -expand -group Debug /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/debug_unit_i/stall_o
add wave -noupdate -expand -group Debug /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/debug_unit_i/dbg_req_o
add wave -noupdate -expand -group Debug /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/debug_unit_i/dbg_ack_i
add wave -noupdate -expand -group Debug /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/debug_unit_i/regfile_rreq_o
add wave -noupdate -expand -group Debug /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/debug_unit_i/regfile_raddr_o
add wave -noupdate -expand -group Debug /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/debug_unit_i/regfile_rdata_i
add wave -noupdate -expand -group Debug /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/debug_unit_i/regfile_wreq_o
add wave -noupdate -expand -group Debug /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/debug_unit_i/regfile_waddr_o
add wave -noupdate -expand -group Debug /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/debug_unit_i/regfile_wdata_o
add wave -noupdate -expand -group Debug /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/debug_unit_i/csr_req_o
add wave -noupdate -expand -group Debug /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/debug_unit_i/csr_addr_o
add wave -noupdate -expand -group Debug /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/debug_unit_i/csr_we_o
add wave -noupdate -expand -group Debug /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/debug_unit_i/csr_wdata_o
add wave -noupdate -expand -group Debug /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/debug_unit_i/csr_rdata_i
add wave -noupdate -expand -group Debug /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/debug_unit_i/pc_if_i
add wave -noupdate -expand -group Debug /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/debug_unit_i/pc_id_i
add wave -noupdate -expand -group Debug /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/debug_unit_i/pc_ex_i
add wave -noupdate -expand -group Debug /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/debug_unit_i/data_load_event_i
add wave -noupdate -expand -group Debug /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/debug_unit_i/instr_valid_id_i
add wave -noupdate -expand -group Debug /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/debug_unit_i/sleeping_i
add wave -noupdate -expand -group Debug /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/debug_unit_i/branch_in_ex_i
add wave -noupdate -expand -group Debug /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/debug_unit_i/branch_taken_i
add wave -noupdate -expand -group Debug /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/debug_unit_i/jump_req_o
add wave -noupdate -expand -group Debug /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/debug_unit_i/jump_addr_o
add wave -noupdate -expand -group Debug /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/debug_unit_i/rdata_sel_q
add wave -noupdate -expand -group Debug /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/debug_unit_i/rdata_sel_n
add wave -noupdate -expand -group Debug /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/debug_unit_i/state_q
add wave -noupdate -expand -group Debug /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/debug_unit_i/state_n
add wave -noupdate -expand -group Debug /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/debug_unit_i/settings_q
add wave -noupdate -expand -group Debug /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/debug_unit_i/settings_n
add wave -noupdate -expand -group Debug /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/debug_unit_i/addr_q
add wave -noupdate -expand -group Debug /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/debug_unit_i/wdata_q
add wave -noupdate -expand -group Debug /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/debug_unit_i/regfile_rreq_q
add wave -noupdate -expand -group Debug /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/debug_unit_i/regfile_rreq_n
add wave -noupdate -expand -group Debug /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/debug_unit_i/regfile_fp_sel_q
add wave -noupdate -expand -group Debug /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/debug_unit_i/regfile_fp_sel_n
add wave -noupdate -expand -group Debug /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/debug_unit_i/jump_req_q
add wave -noupdate -expand -group Debug /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/debug_unit_i/jump_req_n
add wave -noupdate -expand -group Debug /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/debug_unit_i/csr_req_q
add wave -noupdate -expand -group Debug /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/debug_unit_i/csr_req_n
add wave -noupdate -expand -group Debug /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/debug_unit_i/regfile_wreq
add wave -noupdate -expand -group Debug /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/debug_unit_i/regfile_fp_wr
add wave -noupdate -expand -group Debug /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/debug_unit_i/stall_cs
add wave -noupdate -expand -group Debug /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/debug_unit_i/stall_ns
add wave -noupdate -expand -group Debug /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/debug_unit_i/dbg_rdata
add wave -noupdate -expand -group Debug /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/debug_unit_i/dbg_resume
add wave -noupdate -expand -group Debug /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/debug_unit_i/dbg_halt
add wave -noupdate -expand -group Debug /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/debug_unit_i/dbg_cause_q
add wave -noupdate -expand -group Debug /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/debug_unit_i/dbg_cause_n
add wave -noupdate -expand -group Debug /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/debug_unit_i/dbg_ssth_q
add wave -noupdate -expand -group Debug /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/debug_unit_i/dbg_ssth_n
add wave -noupdate -expand -group Debug /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/debug_unit_i/ssth_clear
add wave -noupdate -expand -group Debug /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/debug_unit_i/pc_tracking_fsm_cs
add wave -noupdate -expand -group Debug /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/debug_unit_i/pc_tracking_fsm_ns
add wave -noupdate -expand -group Debug /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/debug_unit_i/ppc_int
add wave -noupdate -expand -group Debug /tb_pulp/i_dut/soc_domain_i/pulp_soc_i/fc_subsystem_i/FC_CORE/lFC_CORE/debug_unit_i/npc_int
TreeUpdate [SetDefaultTree]
WaveRestoreCursors {{Cursor 1} {14822584749 ps} 0}
quietly wave cursor active 1
configure wave -namecolwidth 250
configure wave -valuecolwidth 100
configure wave -justifyvalue left
configure wave -signalnamewidth 1
configure wave -snapdistance 10
configure wave -datasetprefix 0
configure wave -rowmargin 4
configure wave -childrowmargin 2
configure wave -gridoffset 0
configure wave -gridperiod 1
configure wave -griddelta 40
configure wave -timeline 0
configure wave -timelineunits ns
update
WaveRestoreZoom {14822537377 ps} {14822875001 ps}
