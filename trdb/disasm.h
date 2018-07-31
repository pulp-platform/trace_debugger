/*
 * trdb - Trace Debugger Software for the PULP platform
 *
 * Copyright (C) 2018 ETH Zurich and University of Bologna
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

/*
 * Author: Robert Balas (balasr@student.ethz.ch)
 * Description: Collection of disassembler routines using libopcodes and libbfd
 */


#ifndef __DISASM_H__
#define __DISASM_H__

#define PACKAGE "foo" /* quick hack for bfd if not using autotools */
#include <stdbool.h>
#include <inttypes.h>
#include "bfd.h"
#include "dis-asm.h"

/* TODO: berkley */
#define DECLARE_INSN(code, match, mask)                                        \
    static const uint32_t match_##code = match;                                \
    static const uint32_t mask_##code = mask;                                  \
    static bool is_##code##_instr(long instr)                                  \
    {                                                                          \
        return (instr & mask) == match;                                        \
    }

#include "riscv_encoding.h"
#undef DECLARE_INSN


void dump_section_header(bfd *, asection *, void *);

void dump_bin_info(bfd *);

void riscv32_print_address(bfd_vma, struct disassemble_info *);

asymbol *find_symbol_at_address(bfd_vma, struct disassemble_info *);

void dump_section_names(bfd *);

void dump_target_list();

void disassemble_section(bfd *, asection *, void *);

void disassemble_block(bfd_byte *, size_t, struct disassemble_info *);

void disassemble_single_instruction(uint32_t, struct disassemble_info *);

#endif
