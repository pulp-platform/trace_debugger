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
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <inttypes.h>
#include "disassembly.h"
#include "utils.h"
#include "bfd.h"
#include "dis-asm.h"
#include "libiberty.h"


void init_disassemble_info_for_pulp(struct disassemble_info *dinfo)
{
    const bfd_arch_info_type *riscv32_arch = bfd_scan_arch("riscv:rv32");
    dinfo->fprintf_func = (fprintf_ftype)fprintf;
    dinfo->print_address_func = riscv32_print_address;
    dinfo->flavour = bfd_target_elf_flavour; /* TODO: const */
    dinfo->arch = riscv32_arch->arch;
    dinfo->mach = riscv32_arch->mach;
    dinfo->endian = BFD_ENDIAN_LITTLE;
}


void init_disassemble_info_from_bfd(struct disassemble_info *dinfo, bfd *abfd,
                                    char *options)
{
    init_disassemble_info(dinfo, stdout, (fprintf_ftype)fprintf);
    dinfo->fprintf_func = (fprintf_ftype)fprintf;
    dinfo->print_address_func = riscv32_print_address;

    dinfo->flavour = bfd_get_flavour(abfd);
    dinfo->arch = bfd_get_arch(abfd);
    dinfo->mach = bfd_get_mach(abfd);
    dinfo->endian = abfd->xvec->byteorder;
    dinfo->disassembler_options = options;
    disassemble_init_for_target(dinfo);
}


int init_disassembler_unit(struct disassembler_unit *dunit, bfd *abfd,
                           char *options)
{
    /* initialize libopcodes disassembler */
    struct disassemble_info *dinfo = dunit->dinfo;
    if (!dinfo) {
        LOG_ERRT("disassemble_info is null\n");
        return -1;
    }
    init_disassemble_info_from_bfd(dinfo, abfd, options);
    dunit->disassemble_fn = disassembler(abfd);
    if (!dunit->disassemble_fn) {
        LOG_ERRT("No suitable disassembler found\n");
        return -1;
    }
    return 0;
}


void riscv32_print_address(bfd_vma addr, struct disassemble_info *dinfo)
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


bool vma_in_section(bfd *abfd, asection *section, bfd_vma vma)
{
    return (vma >= section->vma
            && vma < (section->vma + bfd_section_size(abfd, section)));
}


asection *get_section_for_vma(bfd *abfd, bfd_vma vma)
{
    asection *p;
    for (p = abfd->sections; p != NULL; p = p->next) {
        if (vma_in_section(abfd, p, vma)) {
            return p;
        }
    }
    return NULL;
}


void disassemble_section(bfd *abfd, asection *section, void *inf)
{
    /* Do not disassemble sections without machine code*/
    if ((section->flags & (SEC_CODE | SEC_HAS_CONTENTS))
        != (SEC_CODE | SEC_HAS_CONTENTS)) {
        return;
    }
    struct disassembler_unit *dunit = inf;
    if (!dunit) {
        LOG_ERRT("disassembler_unit is NULL\n");
        return;
    }

    struct disassemble_info *dinfo = dunit->dinfo;
    disassembler_ftype disassemble_fn = dunit->disassemble_fn;
    if (!dinfo || !disassemble_fn) {
        LOG_ERRT("Unitialized member of disassembler_unit");
        return;
    }

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
        /* print instr address */
        (*dinfo->fprintf_func)(dinfo->stream, "0x%016jx  ",
                               (uintmax_t)(section->vma + addr_offset));
        int size = (*disassemble_fn)(section->vma + addr_offset, dinfo);
        addr_offset += size;
        (*dinfo->fprintf_func)(dinfo->stream, "\n");
        if (size <= 0) {
            LOG_ERRT("Encountered instruction with %d bytes, stopping", size);
            break;
        }
    }

    free(data);
}


void disassemble_block(size_t len, bfd_byte data[len],
                       struct disassembler_unit *dunit)
{
    if (!dunit) {
        LOG_ERRT("disassembler_unit is NULL\n");
        return;
    }

    struct disassemble_info *dinfo = dunit->dinfo;
    disassembler_ftype disassemble_fn = dunit->disassemble_fn;
    if (!dinfo || !disassemble_fn) {
        LOG_ERRT("Unitialized member of disassembler_unit");
        return;
    }

    size_t pc = 0;
    dinfo->buffer = data;
    dinfo->buffer_vma = pc;
    dinfo->buffer_length = len;

    while (pc < len) {
        int size = (*disassemble_fn)(pc, dinfo);
        pc += size;
        (*dinfo->fprintf_func)(dinfo->stream, "\n");
        if (size <= 0) {
            LOG_ERRT("Encountered instruction with %d bytes, stopping", size);
            break;
        }
    }
}


void disassemble_single_instruction(uint32_t instr, uint32_t addr,
                                    struct disassembler_unit *dunit)
{
    if (!dunit) {
        LOG_ERRT("disassembler_unit is NULL\n");
        return;
    }

    struct disassemble_info *dinfo = dunit->dinfo;
    disassembler_ftype disassemble_fn = dunit->disassemble_fn;
    if (!dinfo || !disassemble_fn) {
        LOG_ERRT("Unitialized member of disassembler_unit\n");
        return;
    }

    size_t len = 8;
    bfd_byte *data = malloc(len * sizeof(*data));
    if (!data) {
        perror("disassemble_single_instruction:");
        return;
    }
    memset(data, 0, len * sizeof(*data));

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
    dinfo->buffer_vma = addr;
    dinfo->buffer_length = len;

    int size = (*disassemble_fn)(addr, dinfo);
    (*dinfo->fprintf_func)(dinfo->stream, "\n");
    if (size <= 0) {
        LOG_ERRT("Encountered instruction with %d bytes, stopping", size);
    }

    free(data);
}
