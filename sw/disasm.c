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
 * Description:
 */

#define PACKAGE "foo" /* quick hack for bfd if not using autotools */
/* #include <stdio.h> */
/* #include <stdlib.h> */
/* #include <string.h> */
/* #include <assert.h> */
/* #include "bfd.h" */
/* #include "dis-asm.h" */
/* #include "disasm.h" */

/* int main(int argc, char **argv) */
/* { */
/*     bfd *abfd = NULL; */
/*     disassemble_info dinfo = {0}; */

/*     bfd_init(); */
/*     bfdFile = bfd_openr("/bin/ls", NULL); */
/*     if (bfdFile == NULL) { */
/* 	printf("Error [%x]: %s\n", bfd_get_error(), */
/* 	       bfd_errmsg(bfd_get_error())); */
/* 	return -1; */
/*     } */
/*     if (!bfd_check_format(bfdFile, bfd_object)) { */
/* 	printf("Error [%x]: %s\n", bfd_get_error(), */
/* 	       bfd_errmsg(bfd_get_error())); */
/* 	return -1; */
/*     } */

/*     dis->arch = bfd_arch_i386; */
/*     dis->read_memory_func = buffer_read_memory; */
/*     dis->buffer_length = 64; */
/*     dis->buffer = malloc(dis->buffer_length); */
/*     if (!dis->buffer) */
/* 	return EXIT_FAILURE; */
/*     memset(dis->buffer, 0x90, dis->buffer_length); */
/*     disassemble_init_for_target(dis); */

/*     int instr_size = print_insn_i386(0, dis); */

/*     printf("instruction size is %d\n", instr_size); */

/*     return EXIT_SUCCESS; */
/* } */


#include <bfd.h>
#include <dis-asm.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <libiberty.h>

/*
 * Holds state for BFD and libopcodes.
 */
bfd *abfd = NULL;
disassemble_info dinfo = {0};
#define PATH_MAX 256

/*
 * Temporary hack to signal when disassembling should stop.
 */
static bool stop_disassembling = FALSE;

/*
 * Gets path to currently running executable.
 */
bool get_target_path(char *target_path, size_t size)
{
    char *path;
    ssize_t len;

    pid_t pid = getpid();
    sprintf(target_path, "/proc/%d/exe", (int)pid);

    path = strdup(target_path);
    len = readlink(path, target_path, size);

    target_path[len] = '\0';
    free(path);
    return TRUE;
}

/*
 * libopcodes appends spaces on the end of some instructions so for
 * comparisons, we want to strip those first.
 */
void strip_tail(char *str, unsigned int size)
{
    int i;
    for (i = 0; i < size; i++) {
	if (!isgraph(str[i])) {
	    str[i] = '\0';
	    break;
	}
    }
}


/*
 * Used as a callback for libopcodes so we can do something useful with the
 * disassembly. Currently this just outputs to stdout.
 */
int custom_fprintf(void *stream, const char *format, ...)
{
    /* silly amount */
    char str[128] = {0};
    int rv;
    va_list args;

    va_start(args, format);
    rv = vsnprintf(str, ARRAY_SIZE(str) - 1, format, args);
    va_end(args);

    puts(str);
    strip_tail(str, ARRAY_SIZE(str));

    /* if (dinfo.insn_info_valid) { */
    /* 	switch (dinfo.insn_type) { */
    /* 	case dis_noninsn: */
    /* 	    printf("not an instruction\n"); */
    /* 	    break; */
    /* 	case dis_nonbranch: */
    /* 	    printf("not a branch\n"); */
    /* 	    break; */
    /* 	case dis_branch: */
    /* 	    printf("is a branch\n"); */
    /* 	    break; */
    /* 	case dis_condbranch: */
    /* 	    printf("is a conditional branch\n"); */
    /* 	    break; */
    /* 	case dis_jsr: */
    /* 	    printf("jump to subroutine\n"); */
    /* 	    break; */
    /* 	case dis_condjsr: */
    /* 	    printf("conditional jump to subroutine\n"); */
    /* 	    break; */
    /* 	case dis_dref: */
    /* 	    printf("data reference in instruction\n"); */
    /* 	    break; */
    /* 	case dis_dref2: */
    /* 	    printf("two data references in instruction\n"); */
    /* 	    break; */
    /* 	default: */
    /* 	    printf("not enumerated\n"); */
    /* 	    break; */
    /* 	} */
    /* } else { */
    /* 	printf("insn_info not valid\n"); */
    /* } */

    return rv;
}

/*
 * Initialises libopcodes disassembler and returns an instance of it.
 */
disassembler_ftype init_disasm(bfd *abfd, disassemble_info *dinfo)
{
    /* Override the stream the disassembler outputs to */
    init_disassemble_info(dinfo, NULL, custom_fprintf);
    dinfo->flavour = bfd_get_flavour(abfd);
    dinfo->arch = bfd_get_arch(abfd);
    dinfo->mach = bfd_get_mach(abfd);
    dinfo->endian = abfd->xvec->byteorder;
    disassemble_init_for_target(dinfo);

    return disassembler(abfd);
}

/*
 * Method of locating section from VMA taken from opdis.
 */
typedef struct {
    bfd_vma vma;
    asection *sec;
} BFD_VMA_SECTION;

/*
 * Loads section and fills in dinfo accordingly. Since this function
allocates
 * memory in dinfo->buffer, callers need to call free once they are finished.
 */
bool load_section(bfd *abfd, disassemble_info *dinfo, asection *s)
{
    int size = bfd_section_size(s->owner, s);
    unsigned char *buf = xmalloc(size);

    if (!bfd_get_section_contents(s->owner, s, buf, 0, size)) {
	free(buf);
	return FALSE;
    }

    dinfo->section = s;
    dinfo->buffer = buf;
    dinfo->buffer_length = size;
    dinfo->buffer_vma = bfd_section_vma(s->owner, s);

    printf("Allocated %d bytes for %s section\n: 0x%lX", size, s->name,
	   dinfo->buffer_vma);
    return TRUE;
}

/*
 * Used to locate section for a vma.
 */
void vma_in_section(bfd *abfd, asection *s, void *data)
{
    BFD_VMA_SECTION *req = data;

    if (req && req->vma >= s->vma
	&& req->vma < (s->vma + bfd_section_size(abfd, s))) {
	req->sec = s;
    }
}

/*
 * Locate and load section containing vma.
 */
bool load_section_for_vma(bfd *abfd, disassemble_info *dinfo, bfd_vma vma)
{
    BFD_VMA_SECTION req = {vma, NULL};
    bfd_map_over_sections(abfd, vma_in_section, &req);

    if (!req.sec) {
	return FALSE;
    } else {
	return load_section(abfd, dinfo, req.sec);
    }
}

/*
 * Start disassembling from entry point.
 */
bool disassemble_entry(bfd *abfd, disassemble_info *dinfo,
		       disassembler_ftype disassembler)
{
    bfd_vma vma = bfd_get_start_address(abfd);

    /* First locate and load the section containing the vma */
    if (load_section_for_vma(abfd, dinfo, vma)) {
	int size;

	/* Keep disassembling until signalled otherwise or error */
	while (true) {
	    dinfo->insn_info_valid = 0;
	    size = disassembler(vma, dinfo);
	    /* printf("Disassembled %d bytes at 0x%lX\n", size, vma); */

	    if (size == 0 || size == -1 || stop_disassembling) {
		break;
	    }

	    vma += size;
	}

	free(dinfo->buffer);
	return TRUE;
    }

    return FALSE;
}

int main(void)
{
    char target_path[PATH_MAX] = {0};

    bfd_init();

    /* Get path for the running instance of this program */
    get_target_path(target_path, ARRAY_SIZE(target_path));

    abfd = bfd_openr(target_path, NULL);

    if (abfd != NULL && bfd_check_format(abfd, bfd_object)) {
	disassembler_ftype disassembler = init_disasm(abfd, &dinfo);

	disassemble_entry(abfd, &dinfo, disassembler);

	bfd_close(abfd);
    }

    return EXIT_SUCCESS;
}
