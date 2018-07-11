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

#define PACKAGE "foo" /* quick hack for bfd if not using autotools */
#include "disasm.h"
#include "bfd.h"
#include "dis-asm.h"
#include "libiberty.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <inttypes.h>

/* TODO: make this global or pass it via args */
disassembler_ftype disassemble_fn;

void override_print_address(bfd_vma addr, struct disassemble_info *dinfo)
{
    (*dinfo->fprintf_func)(dinfo->stream, "0x%08jx", (uintmax_t)addr);
}


asymbol *find_symbol_at_address(bfd_vma addr, struct disassemble_info *dinfo)
{
    return NULL;
}


void dump_section_names(bfd *abfd)
{
    bfd_map_over_sections(abfd, dump_section_header, NULL);
}


void dump_section_header(bfd *abfd, asection *section, void *ignored)
{
    bfd_printf_vma(abfd, bfd_get_section_vma(abfd, section));
    printf("%s\n", bfd_get_section_name(abfd, section));
}


void dump_bin_info(bfd *abfd)
{
    printf("Information about binary:\n");
    printf("flavour: %u\n", bfd_get_flavour(abfd));
    printf("name   : %s\n", bfd_printable_name(abfd));
    printf("size   : %d\n", bfd_get_arch_size(abfd));
    printf("mach   : %s\n", bfd_printable_arch_mach(bfd_get_arch(abfd), 0));
    printf("endian : %d\n", abfd->xvec->byteorder);
}


void dump_target_list()
{
    printf("Available target list:\n");
    const char **list = bfd_target_list();
    for (unsigned int i = 0; list[i]; i++) {
        printf("%s\n", list[i]);
    }
}


void disassemble_section(bfd *abfd, asection *section, void *inf)
{
    /* Do not disassemble sections without machine code*/
    if ((section->flags & (SEC_CODE | SEC_HAS_CONTENTS))
        != (SEC_CODE | SEC_HAS_CONTENTS)) {
        return;
    }

    struct disassemble_info *dinfo = inf;

    bfd_size_type datasize = bfd_get_section_size(section);
    if (datasize == 0)
        return;

    bfd_vma addr_offset = 0;
    bfd_vma stop_offset = datasize / dinfo->octets_per_byte;

    if (addr_offset >= stop_offset)
        return;

    bfd_byte *data = malloc(datasize);
    if (!data) {
        perror("disassemble_section:");
        return;
    }

    if (!bfd_get_section_contents(abfd, section, data, 0, datasize)) {
        bfd_perror("disassemble_section:");
        free(data);
        return;
    }

    dinfo->buffer = data;
    dinfo->buffer_vma = section->vma;
    dinfo->buffer_length = datasize;
    dinfo->section = section;

    printf("Disassembly of section %s:\n", section->name);
    while (addr_offset < stop_offset) {
        printf("0x%016jx  ", (uintmax_t)addr_offset); /* pc */
        int size = (*disassemble_fn)(section->vma + addr_offset, dinfo);
        addr_offset += size;
        printf("\n");
        if (size <= 0) {
            fprintf(stderr, "Encountered instruction with %d bytes, stopping",
                    size);
            break;
        }
    }

    free(data);
}


void disassemble_block(bfd_byte *data, size_t len,
                       struct disassemble_info *dinfo)
{
    size_t pc = 0;
    dinfo->buffer = data;
    dinfo->buffer_vma = pc;
    dinfo->buffer_length = len;

    while (pc < len) {
        int size = (*disassemble_fn)(pc, dinfo);
        pc += size;
        printf("\n");
        if (size <= 0) {
            fprintf(stderr, "Encountered instruction with %d bytes, stopping",
                    size);
            break;
        }
    }
}


void disassemble_single_instruction(uint32_t instr,
                                    struct disassemble_info *dinfo)
{
    size_t len = 8;
    size_t pc = 0;
    bfd_byte *data = malloc(len);
    if (!data) {
        perror("disassemble_single_instruction:");
        return;
    }
    memset(data, 0, len);

    if (dinfo->endian == BFD_ENDIAN_BIG) {
        data[3] = (bfd_byte)(instr & 0xff);
        data[2] = (bfd_byte)((instr >> 8) & 0xff);
        data[1] = (bfd_byte)((instr >> 16) & 0xff);
        data[0] = (bfd_byte)((instr >> 24) & 0xff);
    } else {
        /* Assume unknown is also little endian */
        data[0] = (bfd_byte)(instr & 0xff);
        data[1] = (bfd_byte)((instr >> 8) & 0xff);
        data[2] = (bfd_byte)((instr >> 16) & 0xff);
        data[3] = (bfd_byte)((instr >> 24) & 0xff);
    }
    dinfo->buffer = data;
    dinfo->buffer_vma = pc;
    dinfo->buffer_length = len;

    int size = (*disassemble_fn)(pc, dinfo);
    printf("\n");
    if (size <= 0) {
        fprintf(stderr, "Encountered instruction with %d bytes, stopping",
                size);
    }

    free(data);
}
