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


#ifndef __DISASSEMBLY_H__
#define __DISASSEMBLY_H__

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

/* Struct used to capture all information and functions needed to disassemble */
struct disassembler_unit {
    disassembler_ftype disassemble_fn;
    struct disassemble_info *dinfo;
};


/* Returns the length of a the given RISC-V instruction instr.*/
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

/* Initialize disassemble_info from libopcodes with hardcoded values for
 * the PULP platform.
 */
void init_disassemble_info_for_pulp(struct disassemble_info *dinfo);

/* Initialize disassemble_info from libopcodes by grabbing data out of the given
 * bfd. The options string can be non-NULL to pass disassembler specific
 * settings to libopcodes.
 */
void init_disassemble_info_from_bfd(struct disassemble_info *dinfo, bfd *abfd,
                                    char *options);

/* Initialize disassembler_unit. Aside from calling
 * init_disassembler_info_from_bfd, it also sets disassemble_fn to the
 * architecture matching abfd.
 */
int init_disassembler_unit(struct disassembler_unit *dunit, bfd *abfd,
                           char *options);

/* Print the bfd section header to stdout.
 */
void dump_section_header(bfd *abfd, asection *section, void *ignored);

/* Print bfd architecuture information to stdout.
 */
void dump_bin_info(bfd *abfd);

/* Print all section names of bfd to stdout.*/
void dump_section_names(bfd *abfd);

/* Print all supported targets of the used  libopcodes to stdout. */
void dump_target_list();

/* Default fprintf_func used in disassemble_info, set by
 * init_disassemble_info_*.
 */
void riscv32_print_address(bfd_vma vma, struct disassemble_info *dinfo);

/* Returns true if vma is contained in the given section of abfd */
bool vma_in_section(bfd *abfd, asection *section, bfd_vma vma);

/* Return the section of abfd that contains the given vma or NULL*/
asection *get_section_for_vma(bfd *abfd, bfd_vma vma);

/* TODO: not implemented yet*/
asymbol *find_symbol_at_address(bfd_vma vma, struct disassemble_info *dinfo);

/* Print the section belonging to bfd to stdout. The void pointer inf must point
 * to a disassembler_unit struct.
 */
void disassemble_section(bfd *abfd, asection *section, void *inf);

/* Disassemble len bytes of data pointed to by bfd_byte with the given
 * disassembler_unit.
 */
void disassemble_block(size_t len, bfd_byte data[len],
                       struct disassembler_unit *dunit);

/* Disassembler the given instr with the pretended address addr and
 * disassembler_unit dunit.
 */
void disassemble_single_instruction(uint32_t instr, uint32_t addr,
                                    struct disassembler_unit *dunit);

#endif
