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
 * Description: Running a few tests
 */

#define PACKAGE "foo" /* quick hack for bfd if not using autotools */
#include "bfd.h"
#include "../disasm.h"
#include "../trace_debugger.h"
#include <stdio.h>
#include <stdlib.h>

bfd *abfd = NULL;
disassemble_info dinfo = {0};
disassembler_ftype disassemble_fn;

int main(int argc, char *argv[argc + 1])
{
    bfd_init();

    abfd = bfd_openr("interrupt", NULL);

    if (!(abfd && bfd_check_format(abfd, bfd_object)))
        return EXIT_FAILURE;

    /* Override the stream the disassembler outputs to */
    init_disassemble_info(&dinfo, stdout, (fprintf_ftype)fprintf);
    dinfo.fprintf_func = (fprintf_ftype)fprintf;
    dinfo.print_address_func = riscv32_print_address;

    dinfo.flavour = bfd_get_flavour(abfd);
    dinfo.arch = bfd_get_arch(abfd);
    dinfo.mach = bfd_get_mach(abfd);
    dinfo.endian = abfd->xvec->byteorder;
    disassemble_init_for_target(&dinfo);

    /* Tests for trace_debugger.h */
    struct tr_instr sample = {0};
    struct tr_instr sample_arr[1];
    sample_arr[0] = sample;
    LIST_HEAD(packet_list);
    trdb_compress_trace(&packet_list, 1, sample_arr);

    /* Tests for disassembly functions */
    dump_target_list();
    dump_bin_info(abfd);

    disassemble_fn = disassembler(abfd);
    if (!disassemble_fn) {
        fprintf(stderr, "No suitable disassembler found\n");
        return EXIT_FAILURE;
    }
    /* TODO: bfd_count_sections */
    dump_section_names(abfd);
    printf("num_sections: %d\n", bfd_count_sections(abfd));
    disassemble_single_instruction(0x10, &dinfo);
    bfd_map_over_sections(abfd, disassemble_section, &dinfo);

    bfd_close(abfd);

    return EXIT_SUCCESS;
}
