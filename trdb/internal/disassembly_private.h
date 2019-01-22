/*
 * trdb - Trace Debugger Software for the PULP platform
 *
 * Copyright (C) 2018 Robert Balas
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @file disassembly_private.h
 * @author Robert Balas (balasr@student.ethz.ch)
 * @date 10 Nov 2018
 * @brief Disassembly helper function used internally
 */
#include <stdint.h>
#include <stdbool.h>

/* define functions which help figure out what function we are dealing with */
#define DECLARE_INSN(code, match, mask)                                        \
    static const uint32_t match_##code = match;                                \
    static const uint32_t mask_##code  = mask;                                 \
    static bool is_##code##_instr(long instr)                                  \
    {                                                                          \
        return (instr & mask) == match;                                        \
    }

#include "riscv_encoding.h"
#undef DECLARE_INSN

/* ABI names for x-registers */
#define X_RA 1
#define X_SP 2
#define X_GP 3
#define X_TP 4
#define X_T0 5
#define X_T1 6
#define X_T2 7
#define X_T3 28

#define OP_MASK_RD 0x1f
#define OP_SH_RD 7
#define MASK_RD (OP_MASK_RD << OP_SH_RD)

#define OP_MASK_RS1 0x1f
#define OP_SH_RS1 15
#define MASK_RS1 (OP_MASK_RS1 << OP_SH_RS1)

#define RV_X(x, s, n) (((x) >> (s)) & ((1 << (n)) - 1))
#define ENCODE_ITYPE_IMM(x) (RV_X(x, 0, 12) << 20)
#define MASK_IMM ENCODE_ITYPE_IMM(-1U)

static bool is_really_c_jalr_instr(long instr)
{
    /* we demand that rd is nonzero */
    return is_c_jalr_instr(instr) && ((instr & MASK_RD) != 0);
}

static bool is_really_c_jr_instr(long instr)
{
    /* we demand that rd is nonzero */
    return is_c_jr_instr(instr) && ((instr & MASK_RD) != 0);
}

static bool is_c_ret_instr(long instr)
{
    return (instr & (MASK_C_JR | MASK_RD)) == (MATCH_C_JR | (X_RA << OP_SH_RD));
}

static bool is_ret_instr(long instr)
{
    return (instr & (MASK_JALR | MASK_RD | MASK_RS1 | MASK_IMM)) ==
           (MATCH_JALR | (X_RA << OP_SH_RS1));
}

enum trdb_ras { none = 0, ret = 1, call = 2, coret = 3 };

static enum trdb_ras is_jalr_funcall(uint32_t instr)
{
    /* According the the calling convention outlined in the risc-v spec we are
     * dealing with a function call if instr == jalr and rd=x1/x5=link.
     * Furthermore the following hints for the RAS (return address stack) are
     * encoded using rs and rd:
     *
     *(rd=!link && rs=link             pop aka ret)
     * rd=link && rs!=link             push
     * rd=link && rs=link && rd!=rs    push and pop, used for coroutines
     * rd=link && rs=link && rd==rs    push
     *
     * This is table 2.1 in riscv-spec-v2.2
     */
    bool is_jalr    = is_jalr_instr(instr);
    uint32_t rd     = (instr & MASK_RD) >> OP_SH_RD;
    uint32_t rs     = (instr & MASK_RS1) >> OP_SH_RS1;
    bool is_rd_link = (rd == X_RA) || (rd == X_T0);
    bool is_rs_link = (rs == X_RA) || (rs == X_T0);
    bool is_eq_link = is_rd_link && is_rs_link && rd == rs;

    if (is_jalr && !is_rd_link && is_rs_link)
        return ret;
    else if (is_jalr && is_rd_link && !is_rs_link)
        return call;
    else if (is_jalr && is_rd_link && is_rs_link && !is_eq_link)
        return coret;
    else if (is_jalr && is_rd_link && is_rs_link && is_eq_link)
        return ret;
    else
        return none;
}

static enum trdb_ras is_c_jalr_funcall(uint32_t instr)
{
    /* C.JALR expands to jalr x1=X_RA, rs1, 0 */
    bool is_c_jalr  = is_really_c_jalr_instr(instr);
    uint32_t rs     = (instr & MASK_RD) >> OP_SH_RD;
    bool is_rs_link = (rs == X_RA) || (rs == X_T0);
    if (!is_c_jalr)
        return none;

    /* this is again derived from table 2.1 in riscv-spec-v2.2 */
    if (is_rs_link && (X_RA != rs))
        return coret;
    else
        return call;
}

static enum trdb_ras is_jal_funcall(uint32_t instr)
{
    /* if jal with rd=x1/x5 then we know it's a function call */
    bool is_jal     = is_jal_instr(instr);
    bool is_rd_link = (instr & MASK_RD) == (X_RA << OP_SH_RD) ||
                      (instr & MASK_RD) == (X_T0 << OP_SH_RD);
    if (is_jal && is_rd_link)
        return call;
    else
        return none;
}

static enum trdb_ras is_c_jal_funcall(uint32_t instr)
{
    /* C.JAL expands to jal x1, offset[11:1] */
    bool is_c_jal = is_c_jal_instr(instr);
    if (is_c_jal)
        return call;
    else
        return none;
}

static enum trdb_ras get_instr_ras_type(uint32_t instr)
{
    enum trdb_ras type = none;
    if (is_jalr_instr(instr)) {
        type = is_jalr_funcall(instr);
    } else if (is_jal_instr(instr)) {
        type = is_jal_funcall(instr);
    } else if (is_really_c_jalr_instr(instr)) {
        type = is_c_jalr_funcall(instr);
    } else if (is_c_jal_instr(instr)) {
        type = call; /* C.JAL expands to jal x1, offset [11:1] */
    } else if (is_c_ret_instr(instr)) {
        type = ret;
    }
    return type;
}

/**
 * Computes the length of a the given RISC-V instruction instr.
 *
 * So far only up to 64 bit instructions are supported.
 *
 * @param instr raw instruction value up to 64 bit long
 * @return number of bytes which make up the instruction
 */
static inline unsigned int riscv_instr_len(uint64_t instr)
{
    if ((instr & 0x3) != 0x3) /* RVC.  */
        return 2;
    if ((instr & 0x1f) != 0x1f) /* Base ISA and extensions in 32-bit space.  */
        return 4;
    if ((instr & 0x3f) == 0x1f) /* 48-bit extensions.  */
        return 6;
    if ((instr & 0x7f) == 0x3f) /* 64-bit extensions.  */
        return 8;
    /* Longer instructions not supported at the moment.  */
    return 2;
}
