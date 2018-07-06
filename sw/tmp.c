/* #include <string.h> */
/* #include <stdbool.h> */
/* #include <stdio.h> */
/* #include "bfd.h" */
/* #include "dis-asm.h" */


/* void override_print_address(bfd_vma addr, struct disassemble_info *info) */
/* { */
/*     printf("0x%x", (unsigned int)addr); */
/* } */


/* int main(int argc, char **argv) */
/* { */
/*     bfd *bfdFile; */
/*     asection *section; */
/*     disassembler_ftype disassemble; */
/*     disassemble_info info; */
/*     unsigned long count, pc; */


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


/*     /\* Retrieve the .text code section *\/ */
/*     section = bfd_get_section_by_name(bfdFile, ".text"); */
/*     if (section == NULL) { */
/*     	printf("Error accessing .text section\n"); */
/*     	return -1; */
/*     } */


/*     /\* Construct disassembler_ftype class *\/ */
/*     disassemble = disassembler(bfdFile); */
/*     if (disassemble == NULL) { */
/*     	printf("Error creating disassembler\n"); */
/*     	return -1; */
/*     } */


/*     /\* Construct and configure the disassembler_info class using stdout *\/ */
/*     init_disassemble_info(&info, stdout, (fprintf_ftype)fprintf); */
/*     info.print_address_func = override_print_address; */
/*     info.arch = bfd_get_arch(bfdFile); */
/*     info.mach = bfd_get_mach(bfdFile); */
/*     info.buffer_vma = section->vma; */
/*     info.buffer_length = section->size; */
/*     info.section = section; */
/*     bfd_malloc_and_get_section(bfdFile, section, &info.buffer); */
/*     disassemble_init_for_target(&info); */


/*     /\* Start diassembling at the file start_address *\/ */
/*     pc = bfd_get_start_address(bfdFile); */
/*     do { */
/*     	printf("0x%x  ", (unsigned int)pc); */
/*     	count = disassemble(pc, &info); */
/*     	pc += count; */
/*     	printf("\n"); */
/*     } while (count > 0 && pc < bfd_get_start_address(bfdFile) + section->size); */
/*     return 0; */
/* } */
