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

/**
 * @file disassembly.h
 * @author Robert Balas (balasr@student.ethz.ch)
 * @date 25 Aug 2018
 * @brief Collection of disassembly routines using libopcdes and libbfd
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

/**
 * Used to capture all information and functions needed to disassemble.
 */
struct disassembler_unit {
    disassembler_ftype disassemble_fn; /**< does the actual disassembly */
    struct disassemble_info *dinfo;    /**< context for disassembly */
};


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

/**
 * Initialize disassemble_info from libopcodes with hardcoded values for the
 * PULP platform.
 * @param dinfo filled with information of the PULP platform
 */
void init_disassemble_info_for_pulp(struct disassemble_info *dinfo);

/**
 * Initialize disassemble_info from libopcodes by grabbing data out of @p abfd.
 *
 * The options string can be non-@c NULL to pass disassembler specific settings
 * to libopcodes. Currently supported is "no-aliases", which disassembles common
 * abbreviations for certain instructions.
 * @param dinfo filled with information from @p abfd
 * @param abfd the bfd representing the binary
 * @param options disassembly options passed to libopcodes
 */
void init_disassemble_info_from_bfd(struct disassemble_info *dinfo, bfd *abfd,
                                    char *options);

/**
 * Initialize disassembler_unit.
 *
 * A side from calling init_disassembler_info_from_bfd(), it also sets
 * #disassemble_fn to the architecture matching @p abfd.
 * @param dunit filled with information from @p abfd
 * @param abfd the bfd representing the binary
 * @param options disassembly options passed to libopcodes
 */
int init_disassembler_unit(struct disassembler_unit *dunit, bfd *abfd,
                           char *options);

/**
 *  Print the bfd section header to stdout.
 *
 * @param abfd the bfd representing the binary
 * @param section which section to print
 * @param ignored is ignored and only for compatiblity
 */
void dump_section_header(bfd *abfd, asection *section, void *ignored);

/**
 * Print bfd architecuture information to stdout.
 *
 * @param abfd the bfd representing the binary
 */
void dump_bin_info(bfd *abfd);

/**
 * Print all section names of bfd to stdout.
 *
 * @param abfd the bfd representing the binary
 */
void dump_section_names(bfd *abfd);

/**
 * Print all supported targets of the used libopcodes to stdout.
 */
void dump_target_list();

/**
 * Default #print_address_func used in disassemble_info, set by
 * init_disassemble_info_for_pulp() or init_disassemble_info_from_bfd().
 *
 * This is a callback function, if registered, called by libopcodes to custom
 * format addresses. It can also be abused to print other information.
 *
 * @param vma the virtual memory address where this instruction is located at
 * @param dinfo context of disassembly
 */
void riscv32_print_address(bfd_vma vma, struct disassemble_info *dinfo);

/**
 * Return if vma is contained in the given section of abfd.
 *
 * @param abfd the bfd representing the binary
 * @parma section the section to test against
 * @param vma the virtual memory address to test for
 * @return whether @p vma is in contained in @p section
 */
bool vma_in_section(bfd *abfd, asection *section, bfd_vma vma);

/**
 * Return the section of @p abfd that contains the given @p vma or @c NULL.
 *
 * @param abfd the bfd representing the binary
 * @param vma the virtual memory address to
 * @return the section which contains @p vma or @c NULL
 */
asection *get_section_for_vma(bfd *abfd, bfd_vma vma);

/**
 * TODO: not implemented yet
 *
 * @param vma the virtual memory address where the symbol is located at
 * @param dinfo disassembly context, initialized with
 * init_disassemble_info_from_bfd().
 * @return the symbol belonging to the @p vma or @c NULL
 */
asymbol *find_symbol_at_address(bfd_vma vma, struct disassemble_info *dinfo);

/**
 * Disassemble the given asection and print it calling fprintf_func.
 *
 * @p inf must point to a disassembler_unit.
 *
 * @param abfd the bfd representing the binary
 * @param section the section to disassemble
 */
void disassemble_section(bfd *abfd, asection *section, void *inf);

/**
 * Disassemble @p len bytes of data pointed to by @p data with the given
 * @p dunit and print it calling fprintf_func.
 *
 * @param len number of bytes in @p data
 * @param data raw data block to disassemble
 * @param dunit disassembly context
 */
void disassemble_block(size_t len, bfd_byte data[len],
                       struct disassembler_unit *dunit);

/**
 * Disassemble the given instr with the pretended address and disassembler_unit
 * and print it calling fprintf_func.
 *
 * @param instr the raw instruction value up to 32 bits
 * @param addr the address where the instruction is located at
 * @param dunit disassembly context
 */
void disassemble_single_instruction(uint32_t instr, uint32_t addr,
                                    struct disassembler_unit *dunit);

#endif
