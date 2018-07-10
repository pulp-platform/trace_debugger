/*
 * Copyright (C) 2018 ETH Zurich and University of Bologna
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Author: Robert Balas (balasr@student.ethz.ch)
 * Description: Collection of disassembler routines using libopcodes and libbfd
 */

#define PACKAGE "foo" /* quick hack for bfd if not using autotools */
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


bfd *abfd = NULL;
disassemble_info dinfo = {0};
disassembler_ftype disassemble_fn;

void dump_section_header(bfd *, asection *, void *);


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
    /* Do not disassemble sections withouth machine code*/
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
	    fprintf(stderr, "Encountered zero in size, stopping...");
	    break;
	}
    }

    free(data);
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
	fprintf(stderr, "Encountered zero in size");
    }

    free(data);
}


int main(int argc, char *argv[])
{
    bfd_init();

    abfd = bfd_openr("interrupt", NULL);

    if (!(abfd && bfd_check_format(abfd, bfd_object)))
	return EXIT_FAILURE;

    /* Override the stream the disassembler outputs to */
    init_disassemble_info(&dinfo, NULL, (fprintf_ftype)fprintf);
    dinfo.fprintf_func = (fprintf_ftype)fprintf;
    dinfo.stream = stdout;

    dinfo.flavour = bfd_get_flavour(abfd);
    dinfo.arch = bfd_get_arch(abfd);
    dinfo.mach = bfd_get_mach(abfd);
    dinfo.endian = abfd->xvec->byteorder;
    disassemble_init_for_target(&dinfo);

    dump_target_list();
    dump_bin_info(abfd);

    disassemble_fn = disassembler(abfd);
    if (!disassemble_fn) {
	fprintf(stderr, "No suitable disassembler found\n");
	return EXIT_FAILURE;
    }
    /* TODO: bfd_count_sections */
    dump_section_names(abfd);

    bfd_map_over_sections(abfd, disassemble_section, &dinfo);

    bfd_close(abfd);

    return EXIT_SUCCESS;
}
