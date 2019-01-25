/*
 * trdb - Trace Debugger Software for the PULP platform
 *
 * Copyright (C) 2018 Robert Balas
 * Copyright (C) 1990-2017 Free Software Foundation, Inc.
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
 * Description: Collection of disassembler routines using libopcodes and
 * libbfd. Reuses and changes alot of code from GNU objdump.
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
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "filenames.h"
#include "disassembly.h"
#include "disassembly_private.h"
#include "trace_debugger.h"
#include "utils.h"
#include "bfd.h"
#include "elf-bfd.h"
#include "dis-asm.h"
/* fix nameconflict with basename in libiberty and libgen */
#define HAVE_DECL_BASENAME 1
#include "libiberty.h"
#include "demangle.h"

/* windows only flag that is null on linux*/
#define O_BINARY 0

/* TODO: move that stuff below into context object */
static const char *prefix;
static int prefix_strip = 0;
static size_t prefix_length;
static int file_start_context = 0;

/* Variables for handling include file path table.  */
static const char **include_paths;
static int include_path_count;

/* Hold the last function name and the last line number we displayed
   in a disassembly.  */
static char *prev_functionname;
static unsigned int prev_line;
static unsigned int prev_discriminator;

/* We keep a list of all files that we have seen when doing a
   disassembly with source, so that we know how much of the file to
   display.  This can be important for inlined functions.  */
struct print_file_list {
    struct print_file_list *next;
    const char *filename;
    const char *modname;
    const char *map;
    size_t mapsize;
    const char **linemap;
    unsigned maxline;
    unsigned last_line;
    unsigned max_printed;
    int first;
};

/* Pseudo FILE object for strings.  */
typedef struct {
    char *buffer;
    size_t pos;
    size_t alloc;
} SFILE;

static struct print_file_list *print_files;

static int insn_width;
static int show_raw_insn;

static int dump_dynamic_reloc_info = 0;
static int dump_reloc_info         = 0;

static bool disassemble_all;
static int wide_output;

void trdb_init_disassemble_info_for_pulp(struct disassemble_info *dinfo)
{
    init_disassemble_info(dinfo, stdout, (fprintf_ftype)fprintf);
    dinfo->fprintf_func       = (fprintf_ftype)fprintf;
    dinfo->print_address_func = trdb_riscv32_print_address;

    dinfo->flavour = bfd_target_elf_flavour;
    dinfo->arch    = bfd_arch_riscv;
    dinfo->mach    = bfd_mach_riscv32;
    dinfo->endian  = BFD_ENDIAN_LITTLE;
    disassemble_init_for_target(dinfo);
}

int trdb_init_disassembler_unit_for_pulp(struct disassembler_unit *dunit,
                                         char *options)
{
    struct disassemble_info *dinfo = dunit->dinfo;
    if (!dinfo)
        return -trdb_invalid;
    trdb_init_disassemble_info_for_pulp(dinfo);
    dinfo->disassembler_options = options;
    dunit->disassemble_fn       = print_insn_riscv;
    return 0;
}

void trdb_init_disassemble_info_from_bfd(struct disassemble_info *dinfo,
                                         bfd *abfd, char *options)
{
    init_disassemble_info(dinfo, stdout, (fprintf_ftype)fprintf);
    dinfo->fprintf_func       = (fprintf_ftype)fprintf;
    dinfo->print_address_func = trdb_riscv32_print_address;

    dinfo->flavour              = bfd_get_flavour(abfd);
    dinfo->arch                 = bfd_get_arch(abfd);
    dinfo->mach                 = bfd_get_mach(abfd);
    dinfo->endian               = abfd->xvec->byteorder;
    dinfo->disassembler_options = options;
    disassemble_init_for_target(dinfo);
}

int trdb_init_disassembler_unit(struct disassembler_unit *dunit, bfd *abfd,
                                char *options)
{
    /* initialize libopcodes disassembler */
    if (!dunit)
        return -trdb_invalid;

    struct disassemble_info *dinfo = dunit->dinfo;
    if (!dinfo)
        return -trdb_invalid;

    trdb_init_disassemble_info_from_bfd(dinfo, abfd, options);
    dunit->disassemble_fn = disassembler(abfd);
    if (!dunit->disassemble_fn)
        return -trdb_arch_support;

    return 0;
}

void trdb_riscv32_print_address(bfd_vma addr, struct disassemble_info *dinfo)
{
    (*dinfo->fprintf_func)(dinfo->stream, "0x%08jx", (uintmax_t)addr);
}

/* Sort symbols into value order.  */
static int compare_symbols(const void *ap, const void *bp)
{
    const asymbol *a = *(const asymbol **)ap;
    const asymbol *b = *(const asymbol **)bp;
    const char *an;
    const char *bn;
    size_t anl;
    size_t bnl;
    bfd_boolean af;
    bfd_boolean bf;
    flagword aflags;
    flagword bflags;

    if (bfd_asymbol_value(a) > bfd_asymbol_value(b))
        return 1;
    else if (bfd_asymbol_value(a) < bfd_asymbol_value(b))
        return -1;

    if (a->section > b->section)
        return 1;
    else if (a->section < b->section)
        return -1;

    an  = bfd_asymbol_name(a);
    bn  = bfd_asymbol_name(b);
    anl = strlen(an);
    bnl = strlen(bn);

    /* The symbols gnu_compiled and gcc2_compiled convey no real
       information, so put them after other symbols with the same value.  */
    af = (strstr(an, "gnu_compiled") != NULL ||
          strstr(an, "gcc2_compiled") != NULL);
    bf = (strstr(bn, "gnu_compiled") != NULL ||
          strstr(bn, "gcc2_compiled") != NULL);

    if (af && !bf)
        return 1;
    if (!af && bf)
        return -1;

        /* We use a heuristic for the file name, to try to sort it after
           more useful symbols.  It may not work on non Unix systems, but it
           doesn't really matter; the only difference is precisely which
           symbol names get printed.  */

#define file_symbol(s, sn, snl)                                                \
    (((s)->flags & BSF_FILE) != 0 ||                                           \
     ((sn)[(snl)-2] == '.' && ((sn)[(snl)-1] == 'o' || (sn)[(snl)-1] == 'a')))

    af = file_symbol(a, an, anl);
    bf = file_symbol(b, bn, bnl);

    if (af && !bf)
        return 1;
    if (!af && bf)
        return -1;

    /* Try to sort global symbols before local symbols before function
       symbols before debugging symbols.  */

    aflags = a->flags;
    bflags = b->flags;

    if ((aflags & BSF_DEBUGGING) != (bflags & BSF_DEBUGGING)) {
        if ((aflags & BSF_DEBUGGING) != 0)
            return 1;
        else
            return -1;
    }
    if ((aflags & BSF_FUNCTION) != (bflags & BSF_FUNCTION)) {
        if ((aflags & BSF_FUNCTION) != 0)
            return -1;
        else
            return 1;
    }
    if ((aflags & BSF_LOCAL) != (bflags & BSF_LOCAL)) {
        if ((aflags & BSF_LOCAL) != 0)
            return 1;
        else
            return -1;
    }
    if ((aflags & BSF_GLOBAL) != (bflags & BSF_GLOBAL)) {
        if ((aflags & BSF_GLOBAL) != 0)
            return -1;
        else
            return 1;
    }

    if (bfd_get_flavour(bfd_asymbol_bfd(a)) == bfd_target_elf_flavour &&
        bfd_get_flavour(bfd_asymbol_bfd(b)) == bfd_target_elf_flavour) {
        bfd_vma asz, bsz;

        asz = 0;
        if ((a->flags & BSF_SYNTHETIC) == 0)
            asz = ((elf_symbol_type *)a)->internal_elf_sym.st_size;
        bsz = 0;
        if ((b->flags & BSF_SYNTHETIC) == 0)
            bsz = ((elf_symbol_type *)b)->internal_elf_sym.st_size;
        if (asz != bsz)
            return asz > bsz ? -1 : 1;
    }

    /* Symbols that start with '.' might be section names, so sort them
       after symbols that don't start with '.'.  */
    if (an[0] == '.' && bn[0] != '.')
        return 1;
    if (an[0] != '.' && bn[0] == '.')
        return -1;

    /* Finally, if we can't distinguish them in any other way, try to
       get consistent results by sorting the symbols by name.  */
    return strcmp(an, bn);
}

/* Sort relocs into address order.  */
static int compare_relocs(const void *ap, const void *bp)
{
    const arelent *a = *(const arelent **)ap;
    const arelent *b = *(const arelent **)bp;

    if (a->address > b->address)
        return 1;
    else if (a->address < b->address)
        return -1;

    /* So that associated relocations tied to the same address show up
     * in the correct order, we don't do any further sorting.
     */
    if (a > b)
        return 1;
    else if (a < b)
        return -1;
    else
        return 0;
}

/* Some symbol names are significant and should be kept in the table of sorted
 * symbol names, even if they are marked as debugging/section symbols.
 */
static bfd_boolean is_significant_symbol_name(const char *name)
{
    return strcmp(name, ".plt") == 0 || strcmp(name, ".got") == 0 ||
           strcmp(name, ".plt.got") == 0;
}

/* Filter out (in place) symbols that are useless for disassembly.
 * COUNT is the number of elements in SYMBOLS.
 * Return the number of useful symbols.
 */
static long remove_useless_symbols(asymbol **symbols, long count)
{
    asymbol **in_ptr = symbols, **out_ptr = symbols;

    while (--count >= 0) {
        asymbol *sym = *in_ptr++;

        if (sym->name == NULL || sym->name[0] == '\0')
            continue;
        if ((sym->flags & (BSF_DEBUGGING | BSF_SECTION_SYM)) &&
            !is_significant_symbol_name(sym->name))
            continue;
        if (bfd_is_und_section(sym->section) ||
            bfd_is_com_section(sym->section))
            continue;

        *out_ptr++ = sym;
    }
    return out_ptr - symbols;
}

/* Print an address (VMA) to the output stream in INFO.
 * If SKIP_ZEROES is TRUE, omit leading zeroes.
 */
static void trdb_print_value(bfd_vma vma, struct disassemble_info *inf,
                             bfd_boolean skip_zeroes)
{
    char buf[30];
    char *p;
    struct trdb_disasm_aux *aux;

    aux = inf->application_data;
    bfd_sprintf_vma(aux->abfd, buf, vma);
    if (!skip_zeroes)
        p = buf;
    else {
        for (p = buf; *p == '0'; ++p)
            ;
        if (*p == '\0')
            --p;
    }
    (*inf->fprintf_func)(inf->stream, "%s", p);
}

/* Print the name of a symbol.  */
static void trdb_print_symname(bfd *abfd, struct disassemble_info *inf,
                               asymbol *sym, bool do_demangle)
{
    char *alloc;
    const char *name, *version_string = NULL;
    bfd_boolean hidden = FALSE;

    alloc = NULL;
    name  = bfd_asymbol_name(sym);
    if (do_demangle && name[0] != '\0') {
        /* Demangle the name.  */
        alloc = bfd_demangle(abfd, name, DMGL_ANSI | DMGL_PARAMS);
        if (alloc != NULL)
            name = alloc;
    }

    if ((sym->flags & BSF_SYNTHETIC) == 0)
        version_string = bfd_get_symbol_version_string(abfd, sym, &hidden);

    if (bfd_is_und_section(bfd_get_section(sym)))
        hidden = TRUE;

    if (inf != NULL) {
        (*inf->fprintf_func)(inf->stream, "%s", name);
        if (version_string && *version_string != '\0')
            (*inf->fprintf_func)(inf->stream, hidden ? "@%s" : "@@%s",
                                 version_string);
    } else {
        printf("%s", name);
        if (version_string && *version_string != '\0')
            printf(hidden ? "@%s" : "@@%s", version_string);
    }

    if (alloc != NULL)
        free(alloc);
}

/* Read in symbol table */
static asymbol **slurp_symtab(struct trdb_ctx *c, bfd *abfd, long *symcount)
{
    asymbol **sy = NULL;
    long storage = 0;

    if (!(bfd_get_file_flags(abfd) & HAS_SYMS)) {
        *symcount = 0;
        return NULL;
    }

    storage = bfd_get_symtab_upper_bound(abfd);
    if (storage < 0) {
        err(c, "failed to read symbol table from: %s\n",
            bfd_get_filename(abfd));
        return NULL;
    }
    if (storage) {
        sy = malloc(storage);
        if (!sy) {
            err(c, "malloc: %s", strerror(errno));
            return NULL;
        }
    }

    *symcount = bfd_canonicalize_symtab(abfd, sy);
    if (*symcount < 0) {
        err(c, "%s\n", bfd_get_filename(abfd));
        return NULL;
    }
    return sy;
}

/* Read in dynamic symbol table */
static asymbol **slurp_dynamic_symtab(struct trdb_ctx *c, bfd *abfd,
                                      long *dynsymcount)
{
    asymbol **sy = NULL;
    long storage = 0;

    storage = bfd_get_dynamic_symtab_upper_bound(abfd);
    if (storage < 0) {
        if (!(bfd_get_file_flags(abfd) & DYNAMIC)) {
            err(c, "%s: not a dynamic object\n", bfd_get_filename(abfd));
            *dynsymcount = 0;
            return NULL;
        }

        err(c, "%s\n", bfd_get_filename(abfd));
        return NULL;
    }
    if (storage) {
        sy = malloc(storage);
        if (!sy) {
            err(c, "malloc: %s\n", strerror(errno));
            return NULL;
        }
    }

    *dynsymcount = bfd_canonicalize_dynamic_symtab(abfd, sy);
    if (*dynsymcount < 0) {
        err(c, "%s\n", bfd_get_filename(abfd));
        return NULL;
    }
    return sy;
}

/* Locate a symbol given a bfd and a section and a VMA. If dunit->require_sec is
 * TRUE, then always require the symbol to be in the section. Returns NULL if
 * there is no suitable symbol. If PLACE is not NULL, then *PLACE is set to the
 * index of the symbol in sorted_syms.
 */
static asymbol *
find_symbol_for_address(bfd_vma vma, struct disassemble_info *inf, long *place)
{

    struct trdb_disasm_aux *aux = inf->application_data;
    /* Indices in sorted_syms.  */
    long min       = 0;
    long max_count = aux->sorted_symcount;
    long thisplace;
    bfd *abfd;
    asection *sec;
    unsigned int opb;
    bfd_boolean want_section;

    asymbol **sorted_syms = aux->sorted_symbols;
    long sorted_symcount  = aux->sorted_symcount;

    if (sorted_symcount < 1)
        return NULL;

    abfd = aux->abfd;
    sec  = aux->sec;
    opb  = inf->octets_per_byte;

    /* Perform a binary search looking for the closest symbol to the required
     * value. We are searching the range (min, max_count].
     */
    while (min + 1 < max_count) {
        asymbol *sym;

        thisplace = (max_count + min) / 2;
        sym       = sorted_syms[thisplace];

        if (bfd_asymbol_value(sym) > vma)
            max_count = thisplace;
        else if (bfd_asymbol_value(sym) < vma)
            min = thisplace;
        else {
            min = thisplace;
            break;
        }
    }

    /* The symbol we want is now in min, the low end of the range we were
     * searching. If there are several symbols with the same value, we want the
     * first (non-section/non-debugging) one.
     */
    thisplace = min;
    while (thisplace > 0 &&
           (bfd_asymbol_value(sorted_syms[thisplace]) ==
            bfd_asymbol_value(sorted_syms[thisplace - 1])) &&
           ((sorted_syms[thisplace - 1]->flags &
             (BSF_SECTION_SYM | BSF_DEBUGGING)) == 0))
        --thisplace;

    /* Prefer a symbol in the current section if we have multple symbols with
     * the same value, as can occur with overlays or zero size sections.
     */
    min = thisplace;
    while (min < max_count && (bfd_asymbol_value(sorted_syms[min]) ==
                               bfd_asymbol_value(sorted_syms[thisplace]))) {
        if (sorted_syms[min]->section == sec &&
            inf->symbol_is_valid(sorted_syms[min], inf)) {
            thisplace = min;

            if (place != NULL)
                *place = thisplace;

            return sorted_syms[thisplace];
        }
        ++min;
    }

    /* If the file is relocatable, and the symbol could be from this section,
     * prefer a symbol from this section over symbols from others, even if the
     * other symbol's value might be closer. Note that this may be wrong for
     * some symbol references if the sections have overlapping memory ranges,
     * but in that case there's no way to tell what's desired without looking at
     * the relocation table. Also give the target a chance to reject symbols.
     */
    want_section =
        (aux->require_sec || ((abfd->flags & HAS_RELOC) != 0 &&
                              vma >= bfd_get_section_vma(abfd, sec) &&
                              vma < (bfd_get_section_vma(abfd, sec) +
                                     bfd_section_size(abfd, sec) / opb)));
    if ((sorted_syms[thisplace]->section != sec && want_section) ||
        !inf->symbol_is_valid(sorted_syms[thisplace], inf)) {
        long i;
        long newplace = sorted_symcount;

        for (i = min - 1; i >= 0; i--) {
            if ((sorted_syms[i]->section == sec || !want_section) &&
                inf->symbol_is_valid(sorted_syms[i], inf)) {
                if (newplace == sorted_symcount)
                    newplace = i;

                if (bfd_asymbol_value(sorted_syms[i]) !=
                    bfd_asymbol_value(sorted_syms[newplace]))
                    break;

                /* Remember this symbol and keep searching until we reach an
                 * earlier address.
                 */
                newplace = i;
            }
        }

        if (newplace != sorted_symcount)
            thisplace = newplace;
        else {
            /* We didn't find a good symbol with a smaller value. Look for one
             * with a larger value.
             */
            for (i = thisplace + 1; i < sorted_symcount; i++) {
                if ((sorted_syms[i]->section == sec || !want_section) &&
                    inf->symbol_is_valid(sorted_syms[i], inf)) {
                    thisplace = i;
                    break;
                }
            }
        }

        if ((sorted_syms[thisplace]->section != sec && want_section) ||
            !inf->symbol_is_valid(sorted_syms[thisplace], inf))
            /* There is no suitable symbol.  */
            return NULL;
    }

    /* If we have not found an exact match for the specified address and we have
     * dynamic relocations available, then we can produce a better result by
     * matching a relocation to the address and using the symbol associated with
     * that relocation.
     */
    if (!want_section && aux->dynrelbuf != NULL &&
        sorted_syms[thisplace]->value != vma
        /* If we have matched a synthetic symbol, then stick with that.  */
        && (sorted_syms[thisplace]->flags & BSF_SYNTHETIC) == 0) {
        long rel_count;
        arelent **rel_pp;

        for (rel_count = aux->dynrelcount, rel_pp = aux->dynrelbuf;
             rel_count--;) {
            arelent *rel = rel_pp[rel_count];

            if (rel->address == vma &&
                rel->sym_ptr_ptr != NULL
                /* Absolute relocations do not provide a more helpful symbolic
                 * address.
                 */
                && !bfd_is_abs_section((*rel->sym_ptr_ptr)->section)) {
                if (place != NULL)
                    *place = thisplace;
                return *rel->sym_ptr_ptr;
            }

            /* We are scanning backwards, so if we go below the target address
             * we have failed.
             */
            if (rel_pp[rel_count]->address < vma)
                break;
        }
    }

    if (place != NULL)
        *place = thisplace;

    return sorted_syms[thisplace];
}

/* Print an address and the offset to the nearest symbol.  */
static void trdb_print_addr_with_sym(bfd *abfd, asection *sec, asymbol *sym,
                                     bfd_vma vma, struct disassemble_info *inf,
                                     bfd_boolean skip_zeroes, bool do_demangle,
                                     bool display_file_offsets)
{
    trdb_print_value(vma, inf, skip_zeroes);

    if (sym == NULL) {
        bfd_vma secaddr;

        (*inf->fprintf_func)(inf->stream, " <%s",
                             bfd_get_section_name(abfd, sec));
        secaddr = bfd_get_section_vma(abfd, sec);
        if (vma < secaddr) {
            (*inf->fprintf_func)(inf->stream, "-0x");
            trdb_print_value(secaddr - vma, inf, TRUE);
        } else if (vma > secaddr) {
            (*inf->fprintf_func)(inf->stream, "+0x");
            trdb_print_value(vma - secaddr, inf, TRUE);
        }
        (*inf->fprintf_func)(inf->stream, ">");
    } else {
        (*inf->fprintf_func)(inf->stream, " <");

        trdb_print_symname(abfd, inf, sym, do_demangle);

        if (bfd_asymbol_value(sym) == vma)
            ;
        /* Undefined symbols in an executables and dynamic objects do not have a
         * value associated with them, so it does not make sense to display an
         * offset relative to them. Normally we would not be provided with this
         * kind of symbol, but the target backend might choose to do so, and the
         * code in find_symbol_for_address might return an as yet unresolved
         * symbol associated with a dynamic reloc.
         */
        else if ((bfd_get_file_flags(abfd) & (EXEC_P | DYNAMIC)) &&
                 bfd_is_und_section(sym->section))
            ;
        else if (bfd_asymbol_value(sym) > vma) {
            (*inf->fprintf_func)(inf->stream, "-0x");
            trdb_print_value(bfd_asymbol_value(sym) - vma, inf, TRUE);
        } else if (vma > bfd_asymbol_value(sym)) {
            (*inf->fprintf_func)(inf->stream, "+0x");
            trdb_print_value(vma - bfd_asymbol_value(sym), inf, TRUE);
        }

        (*inf->fprintf_func)(inf->stream, ">");
    }

    if (display_file_offsets)
        inf->fprintf_func(inf->stream, " (File Offset: 0x%lx)",
                          (long int)(sec->filepos + (vma - sec->vma)));
}

/* Print an address (VMA), symbolically if possible. If SKIP_ZEROES is TRUE,
 * don't output leading zeroes.
 */
static void trdb_print_addr(bfd_vma vma, struct disassemble_info *inf,
                            bfd_boolean skip_zeroes, bool do_demangle,
                            bool display_file_offsets)
{
    struct trdb_disasm_aux *aux;
    asymbol *sym          = NULL;
    bfd_boolean skip_find = FALSE;

    aux                  = inf->application_data;
    long sorted_symcount = aux->sorted_symcount;

    if (sorted_symcount < 1) {
        (*inf->fprintf_func)(inf->stream, "0x");
        trdb_print_value(vma, inf, skip_zeroes);

        if (display_file_offsets)
            inf->fprintf_func(
                inf->stream, " (File Offset: 0x%lx)",
                (long int)(aux->sec->filepos + (vma - aux->sec->vma)));
        return;
    }

    if (aux->reloc != NULL && aux->reloc->sym_ptr_ptr != NULL &&
        *aux->reloc->sym_ptr_ptr != NULL) {
        sym = *aux->reloc->sym_ptr_ptr;

        /* Adjust the vma to the reloc.  */
        vma += bfd_asymbol_value(sym);

        if (bfd_is_und_section(bfd_get_section(sym)))
            skip_find = TRUE;
    }

    if (!skip_find)
        sym = find_symbol_for_address(vma, inf, NULL);

    trdb_print_addr_with_sym(aux->abfd, aux->sec, sym, vma, inf, skip_zeroes,
                             do_demangle, display_file_offsets);
}

/* Print VMA to INFO.  This function is passed to the disassembler
 * routine.
 */
void trdb_print_address(bfd_vma vma, struct disassemble_info *inf)
{
    struct trdb_disasm_aux *aux = inf->application_data;
    bool prefix_addresses       = aux->prefix_addresses;
    bool do_demangle            = aux->do_demangle;
    bool display_file_offsets   = aux->display_file_offsets;

    trdb_print_addr(vma, inf, !prefix_addresses, do_demangle,
                    display_file_offsets);
}

/* Determine if the given address has a symbol associated with it.  */
int trdb_symbol_at_address(bfd_vma vma, struct disassemble_info *inf)
{
    asymbol *sym;

    sym = find_symbol_for_address(vma, inf, NULL);

    return (sym != NULL && (bfd_asymbol_value(sym) == vma));
}

int trdb_alloc_dinfo_with_bfd(struct trdb_ctx *c, bfd *abfd,
                              struct disassembler_unit *dunit)
{
    /* TODO: make this configurable */
    char *machine              = NULL;
    enum bfd_endian endian     = BFD_ENDIAN_UNKNOWN;
    char *disassembler_options = NULL; /* TODO: take from context */
    int status                 = 0;

    if (!c || !abfd || !dunit)
        return -trdb_invalid;

    struct disassemble_info *dinfo = dunit->dinfo;
    struct trdb_disasm_aux *aux    = malloc(sizeof(*aux));
    if (!aux)
        return -trdb_nomem;

    /* make sure we properly initialize aux */
    *aux = (struct trdb_disasm_aux){0};

    struct bfd_target *xvec = NULL;

    long symcount         = 0;
    long dynsymcount      = 0;
    long synthcount       = 0;
    long sorted_symcount  = 0;
    asymbol **sorted_syms = NULL;
    asymbol **syms        = slurp_symtab(c, abfd, &symcount);
    asymbol **dynsyms     = slurp_dynamic_symtab(c, abfd, &dynsymcount);
    asymbol *synthsyms;

    synthcount = bfd_get_synthetic_symtab(abfd, symcount, syms, dynsymcount,
                                          dynsyms, &synthsyms);
    if (synthcount < 0)
        synthcount = 0;

    /* We make a copy of syms to sort. We don't want to sort syms because that
     * will screw up the relocs.
     */
    sorted_symcount = symcount ? symcount : dynsymcount;
    sorted_syms = malloc((sorted_symcount + synthcount) * sizeof(asymbol *));
    if (!sorted_syms) {
        status = -trdb_nomem;
        goto fail;
    }
    memcpy(sorted_syms, symcount ? syms : dynsyms,
           sorted_symcount * sizeof(asymbol *));

    sorted_symcount = remove_useless_symbols(sorted_syms, sorted_symcount);

    for (long i = 0; i < synthcount; ++i) {
        sorted_syms[sorted_symcount] = synthsyms + i;
        ++sorted_symcount;
    }

    /* Sort the symbols into section and symbol order.  */
    qsort(sorted_syms, sorted_symcount, sizeof(asymbol *), compare_symbols);

    /* TODO: here init disassemble_info */
    init_disassemble_info(dinfo, stdout, (fprintf_ftype)fprintf);

    dinfo->application_data = aux;

    aux->symbols            = syms;
    aux->symcount           = symcount;
    aux->dynamic_symbols    = dynsyms;
    aux->dynsymcount        = dynsymcount;
    aux->synthethic_symbols = synthsyms;
    aux->synthcount         = synthcount;
    aux->sorted_symcount    = sorted_symcount;
    aux->sorted_symbols     = sorted_syms;

    aux->abfd        = abfd;
    aux->require_sec = FALSE;
    aux->dynrelbuf   = NULL;
    aux->dynrelcount = 0;
    aux->reloc       = NULL;

    dinfo->print_address_func     = trdb_print_address;
    dinfo->symbol_at_address_func = trdb_symbol_at_address;

    if (machine != NULL) {
        const bfd_arch_info_type *inf = bfd_scan_arch(machine);

        if (inf == NULL) {
            err(c, "can't use supplied machine %s\n", machine);
            status = -trdb_arch_support;
            goto fail;
        }

        abfd->arch_info = inf;
    }

    if (endian != BFD_ENDIAN_UNKNOWN) {

        xvec = malloc(sizeof(struct bfd_target));
        if (!xvec) {
            status = -trdb_nomem;
            goto fail;
        }
        memcpy(xvec, abfd->xvec, sizeof(struct bfd_target));
        xvec->byteorder = endian;
        abfd->xvec      = xvec;
    }

    /* Use libopcodes to locate a suitable disassembler.  */
    dunit->disassemble_fn = disassembler(abfd);
    if (!dunit->disassemble_fn) {
        err(c, "can't disassemble for architecture %s\n",
            bfd_printable_arch_mach(bfd_get_arch(abfd), 0));
        status = -trdb_arch_support;
        goto fail;
    }

    dinfo->flavour                   = bfd_get_flavour(abfd);
    dinfo->arch                      = bfd_get_arch(abfd);
    dinfo->mach                      = bfd_get_mach(abfd);
    dinfo->disassembler_options      = disassembler_options;
    dinfo->octets_per_byte           = bfd_octets_per_byte(abfd);
    dinfo->skip_zeroes               = DEFAULT_SKIP_ZEROES;
    dinfo->skip_zeroes_at_end        = DEFAULT_SKIP_ZEROES_AT_END;
    dinfo->disassembler_needs_relocs = FALSE;

    /* Pre-load the dynamic relocs as we may need them during the disassembly.
     */
    {
        long relsize = bfd_get_dynamic_reloc_upper_bound(abfd);

        if (relsize > 0) {
            aux->dynrelbuf = malloc(relsize);
            if (!aux->dynrelbuf) {
                status = -trdb_nomem;
                goto fail;
            }

            aux->dynrelcount =
                bfd_canonicalize_dynamic_reloc(abfd, aux->dynrelbuf, dynsyms);
            if (aux->dynrelcount < 0) {
                status = -trdb_nomem;
                goto fail;
            }

            /* Sort the relocs by address.  */
            qsort(aux->dynrelbuf, aux->dynrelcount, sizeof(arelent *),
                  compare_relocs);
        }
    }
    dinfo->symtab      = sorted_syms;
    dinfo->symtab_size = sorted_symcount;

    return status;
fail:
    free(xvec);
    free(sorted_syms);
    free(aux);
    return status;
}

void trdb_free_dinfo_with_bfd(struct trdb_ctx *c, bfd *abfd,
                              struct disassembler_unit *dunit)

{
    /* if(abfd) */
    /* 	free(abfd->xvec); */
    if (!dunit)
        return;

    struct trdb_disasm_aux *aux = NULL;

    if (dunit->dinfo) {
        aux = dunit->dinfo->application_data;
        free(dunit->dinfo->symtab);
        /* this is allocated and freed in trdb_disassemble_section() itself*/
        /* free(dunit->dinfo->buffer); */
        dunit->dinfo->buffer_vma    = 0;
        dunit->dinfo->buffer_length = 0;
    }

    // TODO: free section in aux
    if (aux) {
        free(aux->dynrelbuf);
        free(aux->symbols);
        free(aux->dynamic_symbols);
        free(aux->synthethic_symbols);
        /* free(aux->sorted_symbols); */
        free(aux);
    }
}

void trdb_set_disassembly_conf(struct disassembler_unit *dunit,
                               uint32_t settings)
{
    struct trdb_disasm_aux *aux = dunit->dinfo->application_data;
    if (!aux)
        return;
    aux->config                = settings;
    aux->no_aliases            = settings & TRDB_NO_ALIASES;
    aux->prefix_addresses      = settings & TRDB_PREFIX_ADDRESSES;
    aux->do_demangle           = settings & TRDB_DO_DEMANGLE;
    aux->display_file_offsets  = settings & TRDB_DISPLAY_FILE_OFFSETS;
    aux->with_line_numbers     = settings & TRDB_LINE_NUMBERS;
    aux->with_source_code      = settings & TRDB_SOURCE_CODE;
    aux->with_function_context = settings & TRDB_FUNCTION_CONTEXT;
    aux->unwind_inlines        = settings & TRDB_INLINES;
}

int trdb_get_disassembly_conf(struct disassembler_unit *dunit, uint32_t *conf)
{
    struct trdb_disasm_aux *aux = dunit->dinfo->application_data;
    if (!aux)
        return -trdb_invalid;
    *conf = aux->config;
    return 0;
}

/* The number of preceding context lines to show when we start
   displaying a file for the first time.  */
#define SHOW_PRECEDING_CONTEXT_LINES (5)

/* Read a complete file into memory.  */
static const char *slurp_file(const char *fn, size_t *size)
{
#ifdef HAVE_MMAP
    int ps = getpagesize();
    size_t msize;
#endif
    const char *map;
    struct stat st;
    int fd = open(fn, O_RDONLY | O_BINARY);

    if (fd < 0)
        return NULL;
    if (fstat(fd, &st) < 0) {
        close(fd);
        return NULL;
    }
    *size = st.st_size;
#ifdef HAVE_MMAP
    msize = (*size + ps - 1) & ~(ps - 1);
    map   = mmap(NULL, msize, PROT_READ, MAP_SHARED, fd, 0);
    if (map != (char *)-1L) {
        close(fd);
        return map;
    }
#endif
    map = malloc(*size);
    if (!map || (size_t)read(fd, (char *)map, *size) != *size) {
        free((void *)map);
        map = NULL;
    }
    close(fd);
    return map;
}

#define line_map_decrease 5
/* Precompute array of lines for a mapped file. */
static const char **index_file(const char *map, size_t size,
                               unsigned int *maxline)
{
    const char *p, *lstart, *end;
    int chars_per_line = 45; /* First iteration will use 40.  */
    unsigned int lineno;
    const char **linemap        = NULL;
    unsigned long line_map_size = 0;

    lineno = 0;
    lstart = map;
    end    = map + size;

    for (p = map; p < end; p++) {
        if (*p == '\n') {
            if (p + 1 < end && p[1] == '\r')
                p++;
        } else if (*p == '\r') {
            if (p + 1 < end && p[1] == '\n')
                p++;
        } else
            continue;

        /* End of line found.  */

        if (linemap == NULL || line_map_size < lineno + 1) {
            unsigned long newsize;

            chars_per_line -= line_map_decrease;
            if (chars_per_line <= 1)
                chars_per_line = 1;
            line_map_size = size / chars_per_line + 1;
            if (line_map_size < lineno + 1)
                line_map_size = lineno + 1;
            newsize = line_map_size * sizeof(char *);
            linemap = (const char **)xrealloc(linemap, newsize);
        }

        linemap[lineno++] = lstart;
        lstart            = p + 1;
    }

    *maxline = lineno;
    return linemap;
}

/* Tries to open MODNAME, and if successful adds a node to print_files
   linked list and returns that node.  Returns NULL on failure.  */
static struct print_file_list *try_print_file_open(const char *origname,
                                                   const char *modname)
{
    struct print_file_list *p;

    p = malloc(sizeof(struct print_file_list));
    /* TODO: improve this handling */
    if (!p)
        return NULL;

    p->map = slurp_file(modname, &p->mapsize);
    if (p->map == NULL) {
        free(p);
        return NULL;
    }

    p->linemap     = index_file(p->map, p->mapsize, &p->maxline);
    p->last_line   = 0;
    p->max_printed = 0;
    p->filename    = origname;
    p->modname     = modname;
    p->next        = print_files;
    p->first       = 1;
    print_files    = p;
    return p;
}

/* If the source file, as described in the symtab, is not found
   try to locate it in one of the paths specified with -I
   If found, add location to print_files linked list.  */
static struct print_file_list *update_source_path(const char *filename)
{
    struct print_file_list *p;
    const char *fname;
    int i;

    p = try_print_file_open(filename, filename);
    if (p != NULL)
        return p;

    if (include_path_count == 0)
        return NULL;

    /* Get the name of the file.  */
    fname = lbasename(filename);

    /* If file exists under a new path, we need to add it to the list
       so that show_line knows about it.  */
    for (i = 0; i < include_path_count; i++) {
        char *modname = concat(include_paths[i], "/", fname, (const char *)0);

        p = try_print_file_open(filename, modname);
        if (p)
            return p;

        free(modname);
    }

    return NULL;
}

/* Print a source file line.  */
static void print_line(struct print_file_list *p, unsigned int linenum)
{
    const char *l;
    size_t len;

    --linenum;
    if (linenum >= p->maxline)
        return;
    l = p->linemap[linenum];
    /* Test fwrite return value to quiet glibc warning.  */
    len = strcspn(l, "\n\r");
    if (len == 0 || fwrite(l, len, 1, stdout) == 1)
        putchar('\n');
}

/* Print a range of source code lines. */
static void dump_lines(struct print_file_list *p, unsigned int start,
                       unsigned int end)
{
    if (p->map == NULL)
        return;
    printf("\n");
    while (start <= end) {
        print_line(p, start);
        start++;
    }
    printf("\n");
}

/* Show the line number, or the source line, in a disassembly
   listing.  */
static void show_line(bfd *abfd, asection *section, asymbol **syms,
                      bfd_vma addr_offset, struct trdb_disasm_aux *aux)
{
    const char *filename;
    const char *functionname;
    unsigned int linenumber;
    unsigned int discriminator;
    bfd_boolean reloc;
    char *path = NULL;

    bool with_line_numbers = aux->with_line_numbers;
    bool with_source_code  = aux->with_source_code;
    bool unwind_inlines    = aux->unwind_inlines;

    if (!with_line_numbers && !with_source_code)
        return;

    if (!bfd_find_nearest_line_discriminator(abfd, section, syms, addr_offset,
                                             &filename, &functionname,
                                             &linenumber, &discriminator))
        return;

    if (filename != NULL && *filename == '\0')
        filename = NULL;
    if (functionname != NULL && *functionname == '\0')
        functionname = NULL;

    if (filename && IS_ABSOLUTE_PATH(filename) && prefix) {
        char *path_up;
        const char *fname = filename;

        path = malloc(prefix_length + PATH_MAX + 1);
        /* TODO: improve this handling */
        if (!path)
            goto fail;

        if (prefix_length)
            memcpy(path, prefix, prefix_length);
        path_up = path + prefix_length;

        /* Build relocated filename, stripping off leading directories
           from the initial filename if requested.  */
        if (prefix_strip > 0) {
            int level = 0;
            const char *s;

            /* Skip selected directory levels.  */
            for (s = fname + 1; *s != '\0' && level < prefix_strip; s++)
                if (IS_DIR_SEPARATOR(*s)) {
                    fname = s;
                    level++;
                }
        }

        /* Update complete filename.  */
        strncpy(path_up, fname, PATH_MAX);
        path_up[PATH_MAX] = '\0';

        filename = path;
        reloc    = TRUE;
    } else
        reloc = FALSE;

    if (with_line_numbers) {
        if (functionname != NULL &&
            (prev_functionname == NULL ||
             strcmp(functionname, prev_functionname) != 0)) {
            printf("%s():\n", functionname);
            prev_line = -1;
        }
        if (linenumber > 0 &&
            (linenumber != prev_line || discriminator != prev_discriminator)) {
            if (discriminator > 0)
                printf("%s:%u (discriminator %u)\n",
                       filename == NULL ? "???" : filename, linenumber,
                       discriminator);
            else
                printf("%s:%u\n", filename == NULL ? "???" : filename,
                       linenumber);
        }
        if (unwind_inlines) {
            const char *filename2;
            const char *functionname2;
            unsigned line2;
            while (
                bfd_find_inliner_info(abfd, &filename2, &functionname2, &line2))
                printf("inlined by %s:%u (%s)\n", filename2, line2,
                       functionname2);
        }
    }

    if (with_source_code && filename != NULL && linenumber > 0) {
        struct print_file_list **pp, *p;
        unsigned l;

        for (pp = &print_files; *pp != NULL; pp = &(*pp)->next)
            if (filename_cmp((*pp)->filename, filename) == 0)
                break;
        p = *pp;

        if (p == NULL) {
            if (reloc)
                filename = xstrdup(filename);
            p = update_source_path(filename);
        }

        if (p != NULL && linenumber != p->last_line) {
            if (file_start_context && p->first)
                l = 1;
            else {
                l = linenumber - SHOW_PRECEDING_CONTEXT_LINES;
                if (l >= linenumber)
                    l = 1;
                if (p->max_printed >= l) {
                    if (p->max_printed < linenumber)
                        l = p->max_printed + 1;
                    else
                        l = linenumber;
                }
            }
            dump_lines(p, l, linenumber);
            if (p->max_printed < linenumber)
                p->max_printed = linenumber;
            p->last_line = linenumber;
            p->first     = 0;
        }
    }

    if (functionname != NULL &&
        (prev_functionname == NULL ||
         strcmp(functionname, prev_functionname) != 0)) {
        if (prev_functionname != NULL)
            free(prev_functionname);
        prev_functionname = malloc(strlen(functionname) + 1);
        /* TODO: improve this handling */
        if (!prev_functionname)
            goto fail;

        strcpy(prev_functionname, functionname);
    }

    if (linenumber > 0 && linenumber != prev_line)
        prev_line = linenumber;

    if (discriminator != prev_discriminator)
        prev_discriminator = discriminator;

fail:
    if (path)
        free(path);
    /* TODO: not sure if we are not freeing bad pointers */
    /* if(prev_functionname) */
    /* 	free(prev_functionname); */
}

/* sprintf to a "stream".  */
static int ATTRIBUTE_PRINTF_2 objdump_sprintf(SFILE *f, const char *format, ...)
{
    size_t n;
    va_list args;

    while (1) {
        size_t space = f->alloc - f->pos;

        va_start(args, format);
        n = vsnprintf(f->buffer + f->pos, space, format, args);
        va_end(args);

        if (space > n)
            break;

        f->alloc  = (f->alloc + n) * 2;
        f->buffer = (char *)xrealloc(f->buffer, f->alloc);
    }
    f->pos += n;

    return n;
}

/* Disassemble some data in memory between given values.  */
static void trdb_disassemble_bytes(struct disassemble_info *inf,
                                   disassembler_ftype disassemble_fn,
                                   bfd_boolean insns, bfd_byte *data,
                                   bfd_vma start_offset, bfd_vma rel_offset,
                                   arelent ***relppp, arelent **relppend)
{
    struct trdb_disasm_aux *aux;
    asection *section;
    int octets_per_line;
    int skip_addr_chars;
    bfd_vma addr_offset;
    unsigned int opb = inf->octets_per_byte;
    int octets       = opb;
    SFILE sfile;

    aux                    = inf->application_data;
    section                = aux->sec;
    bool prefix_addresses  = aux->prefix_addresses;
    bool with_line_numbers = aux->with_line_numbers;
    bool with_source_code  = aux->with_source_code;

    sfile.alloc  = 120;
    sfile.buffer = malloc(sfile.alloc);
    /* TODO: improve this handling */
    if (!sfile.buffer)
        goto fail;

    sfile.pos = 0;

    if (insn_width)
        octets_per_line = insn_width;
    else if (insns)
        octets_per_line = 4;
    else
        octets_per_line = 16;

    /* Figure out how many characters to skip at the start of an
       address, to make the disassembly look nicer.  We discard leading
       zeroes in chunks of 4, ensuring that there is always a leading
       zero remaining.  */
    skip_addr_chars = 0;
    if (!prefix_addresses) {
        char buf[30];

        bfd_sprintf_vma(aux->abfd, buf, section->vma + section->size / opb);

        while (buf[skip_addr_chars] == '0')
            ++skip_addr_chars;

        /* Don't discard zeros on overflow.  */
        if (buf[skip_addr_chars] == '\0' && section->vma != 0)
            skip_addr_chars = 0;

        if (skip_addr_chars != 0)
            skip_addr_chars = (skip_addr_chars - 1) & -4;
    }

    inf->insn_info_valid = 0;

    addr_offset = start_offset;

    bfd_boolean need_nl = FALSE;
    int previous_octets;

    /* Remember the length of the previous instruction.  */
    previous_octets = octets;
    octets          = 0;

    /* Make sure we don't use relocs from previous instructions.  */
    aux->reloc = NULL;

    char buf[50];
    int bpc = 0;
    int pb  = 0;

    if (with_line_numbers || with_source_code)
        show_line(aux->abfd, section, aux->symbols, addr_offset, aux);

    if (!prefix_addresses) {
        char *s;

        bfd_sprintf_vma(aux->abfd, buf, section->vma + addr_offset);
        for (s = buf + skip_addr_chars; *s == '0'; s++)
            *s = ' ';
        if (*s == '\0')
            *--s = '0';
        printf("%s\t", buf + skip_addr_chars);
    } else {
        aux->require_sec = TRUE;
        trdb_print_address(section->vma + addr_offset, inf);
        aux->require_sec = FALSE;
        putchar(' ');
    }

    sfile.pos            = 0;
    inf->fprintf_func    = (fprintf_ftype)objdump_sprintf;
    inf->stream          = &sfile;
    inf->bytes_per_line  = 0;
    inf->bytes_per_chunk = 0;
    inf->flags           = disassemble_all ? DISASSEMBLE_DATA : 0;
    /* if (machine) */
    /* 	inf->flags |= USER_SPECIFIED_MACHINE_TYPE; */

    if (inf->disassembler_needs_relocs &&
        (bfd_get_file_flags(aux->abfd) & EXEC_P) == 0 &&
        (bfd_get_file_flags(aux->abfd) & DYNAMIC) == 0 && *relppp < relppend) {
        bfd_signed_vma distance_to_rel;

        distance_to_rel = (**relppp)->address - (rel_offset + addr_offset);

        /* Check to see if the current reloc is associated with
           the instruction that we are about to disassemble.  */
        if (distance_to_rel == 0
            /* FIXME: This is wrong.  We are trying to catch
               relocs that are addressed part way through the
               current instruction, as might happen with a packed
               VLIW instruction.  Unfortunately we do not know the
               length of the current instruction since we have not
               disassembled it yet.  Instead we take a guess based
               upon the length of the previous instruction.  The
               proper solution is to have a new target-specific
               disassembler function which just returns the length
               of an instruction at a given address without trying
               to display its disassembly. */
            || (distance_to_rel > 0 &&
                distance_to_rel < (bfd_signed_vma)(previous_octets / opb))) {
            inf->flags |= INSN_HAS_RELOC;
            aux->reloc = **relppp;
        }
    }

    octets = (*disassemble_fn)(section->vma + addr_offset, inf);

    inf->stop_vma     = 0;
    inf->fprintf_func = (fprintf_ftype)fprintf;
    inf->stream       = stdout;
    if (insn_width == 0 && inf->bytes_per_line != 0)
        octets_per_line = inf->bytes_per_line;
    if (octets < (int)opb) {
        if (sfile.pos)
            printf("%s\n", sfile.buffer);
        if (octets >= 0) {
            /* non_fatal (_("disassemble_fn returned length %d"), */
            printf("disassemble_fn returned length %d", octets);
            /* exit_status = 1; */
        }
        /* break; */
    }

    if (prefix_addresses ? show_raw_insn > 0 : show_raw_insn >= 0) {
        bfd_vma j;

        /* If ! prefix_addresses and ! wide_output, we print
           octets_per_line octets per line.  */
        pb = octets;
        if (pb > octets_per_line && !prefix_addresses && !wide_output)
            pb = octets_per_line;

        if (inf->bytes_per_chunk)
            bpc = inf->bytes_per_chunk;
        else
            bpc = 1;

        for (j = addr_offset * opb; j < addr_offset * opb + pb; j += bpc) {
            int k;

            if (bpc > 1 && inf->display_endian == BFD_ENDIAN_LITTLE) {
                for (k = bpc - 1; k >= 0; k--)
                    printf("%02x", (unsigned)data[j + k]);
                putchar(' ');
            } else {
                for (k = 0; k < bpc; k++)
                    printf("%02x", (unsigned)data[j + k]);
                putchar(' ');
            }
        }

        for (; pb < octets_per_line; pb += bpc) {
            int k;

            for (k = 0; k < bpc; k++)
                printf("  ");
            putchar(' ');
        }

        /* Separate raw data from instruction by extra space.  */
        if (insns)
            putchar('\t');
        else
            printf("    ");
    }

    if (sfile.pos)
        printf("%s", sfile.buffer);

    if (prefix_addresses ? show_raw_insn > 0 : show_raw_insn >= 0) {
        while (pb < octets) {
            bfd_vma j;
            char *s;

            putchar('\n');
            j = addr_offset * opb + pb;

            bfd_sprintf_vma(aux->abfd, buf, section->vma + j / opb);
            for (s = buf + skip_addr_chars; *s == '0'; s++)
                *s = ' ';
            if (*s == '\0')
                *--s = '0';
            printf("%s:\t", buf + skip_addr_chars);

            pb += octets_per_line;
            if (pb > octets)
                pb = octets;
            for (; j < addr_offset * opb + pb; j += bpc) {
                int k;

                if (bpc > 1 && inf->display_endian == BFD_ENDIAN_LITTLE) {
                    for (k = bpc - 1; k >= 0; k--)
                        printf("%02x", (unsigned)data[j + k]);
                    putchar(' ');
                } else {
                    for (k = 0; k < bpc; k++)
                        printf("%02x", (unsigned)data[j + k]);
                    putchar(' ');
                }
            }
        }
    }

    if (!wide_output)
        putchar('\n');
    else
        need_nl = TRUE;

    if (need_nl)
        printf("\n");

    addr_offset += octets / opb;

fail:
    if (sfile.buffer)
        free(sfile.buffer);
}

void trdb_disassemble_instruction_with_bfd(struct trdb_ctx *c, bfd *abfd,
                                           bfd_vma addr,
                                           struct disassembler_unit *dunit)
{
    const struct elf_backend_data *bed;
    bfd_vma sign_adjust            = 0;
    struct disassemble_info *pinfo = dunit->dinfo;
    struct trdb_disasm_aux *paux   = dunit->dinfo->application_data;

    /* unsigned int                 opb = pinfo->octets_per_byte; */
    bfd_byte *data         = NULL;
    bfd_size_type datasize = 0;
    arelent **rel_pp       = NULL;
    arelent **rel_ppstart  = NULL;
    arelent **rel_ppend;
    bfd_vma stop_offset;
    asymbol *sym = NULL;
    long place   = 0;
    long rel_count;
    bfd_vma rel_offset;
    unsigned long addr_offset;

    asymbol **syms = paux->symbols;

    asymbol **sorted_syms = paux->sorted_symbols;
    long sorted_symcount  = paux->sorted_symcount;

    bool prefix_addresses      = paux->prefix_addresses;
    bool do_demangle           = paux->do_demangle;
    bool display_file_offsets  = paux->display_file_offsets;
    bool with_function_context = paux->with_function_context;

    /* get section section which vma points to, if any */
    asection *section = trdb_get_section_for_vma(abfd, addr);
    if (!section) {
        err(c, "no section for vma found\n");
        return;
    }

    /* Sections that do not contain machine
       code are not normally disassembled.  */
    if (!disassemble_all && ((section->flags & (SEC_CODE | SEC_HAS_CONTENTS)) !=
                             (SEC_CODE | SEC_HAS_CONTENTS))) {
        err(c, "section contains no machine code\n");
        return;
    }

    datasize = bfd_get_section_size(section);
    if (datasize == 0) {
        err(c, "section is empty\n");
        return;
    }

    /* Decide which set of relocs to use.  Load them if necessary.  */
    if (paux->dynrelbuf && dump_dynamic_reloc_info) {
        rel_pp    = paux->dynrelbuf;
        rel_count = paux->dynrelcount;
        /* Dynamic reloc addresses are absolute, non-dynamic are section
           relative.  REL_OFFSET specifies the reloc address corresponding
           to the start of this section.  */
        rel_offset = section->vma;
    } else {
        rel_count  = 0;
        rel_pp     = NULL;
        rel_offset = 0;

        if ((section->flags & SEC_RELOC) != 0 &&
            (dump_reloc_info || pinfo->disassembler_needs_relocs)) {
            long relsize;

            relsize = bfd_get_reloc_upper_bound(abfd, section);
            if (relsize < 0) {
                err(c, "%s\n", bfd_get_filename(abfd));
                return;
            }

            if (relsize > 0) {
                rel_ppstart = rel_pp = malloc(relsize);
                if (!rel_pp) {
                    err(c, "malloc: %s\n", strerror(errno));
                    goto fail;
                }
                rel_count = bfd_canonicalize_reloc(abfd, section, rel_pp, syms);
                if (rel_count < 0) {
                    err(c, "%s\n", bfd_get_filename(abfd));
                    return;
                }

                /* Sort the relocs by address.  */
                qsort(rel_pp, rel_count, sizeof(arelent *), compare_relocs);
            }
        }
    }
    rel_ppend = rel_pp + rel_count;

    /* load appropriate section, if we have to */
    if (!paux->sec || strcmp(paux->sec->name, section->name) != 0) {
        if (paux->sec && pinfo->buffer) {
            free(pinfo->buffer);
            pinfo->buffer = 0;
        }
        data = malloc(datasize);
        if (!data) {
            err(c, "malloc: %s\n", strerror(errno));
            goto fail;
        }

        if (!bfd_get_section_contents(abfd, section, data, 0, datasize)) {
            err(c, "bfd_get_section_contents: %s", bfd_errmsg(bfd_get_error()));
            goto fail;
        }

        paux->sec            = section;
        pinfo->buffer        = data;
        pinfo->buffer_vma    = section->vma;
        pinfo->buffer_length = datasize;
        pinfo->section       = section;
    }

    /* Skip over the relocs belonging to addresses below the
       start address.  */
    while (rel_pp < rel_ppend &&
           (*rel_pp)->address < rel_offset + addr - section->vma)
        ++rel_pp;

    /* printf (("\nDisassembly of section %s:\n"), section->name); */

    /* Find the nearest symbol forwards from our current position.  */
    paux->require_sec = TRUE;
    sym               = (asymbol *)find_symbol_for_address(addr, pinfo, &place);
    paux->require_sec = FALSE;

    /* PR 9774: If the target used signed addresses then we must make
       sure that we sign extend the value that we calculate for 'addr'
       in the loop below.  */
    if (bfd_get_flavour(abfd) == bfd_target_elf_flavour &&
        (bed = get_elf_backend_data(abfd)) != NULL && bed->sign_extend_vma)
        sign_adjust = (bfd_vma)1 << (bed->s->arch_size - 1);

    /* bfd_vma addr; */ // from parameters
    asymbol *nextsym;
    bfd_vma nextstop_offset;
    bfd_boolean insns;

    /* addr = section->vma + addr_offset; */
    addr = ((addr & ((sign_adjust << 1) - 1)) ^ sign_adjust) - sign_adjust;

    if (sym != NULL && bfd_asymbol_value(sym) <= addr) {
        int x;

        /* find first symbol value that is larger than addr */
        for (x = place; (x < sorted_symcount &&
                         (bfd_asymbol_value(sorted_syms[x]) <= addr));
             ++x)
            continue;

        pinfo->symbols     = sorted_syms + place;
        pinfo->num_symbols = x - place;
        pinfo->symtab_pos  = place;
    } else {
        pinfo->symbols     = NULL;
        pinfo->num_symbols = 0;
        pinfo->symtab_pos  = -1;
    }

    if (!prefix_addresses) {
        if (with_function_context && bfd_asymbol_value(sym) == addr) {
            pinfo->fprintf_func(pinfo->stream, "\n\n");
            trdb_print_addr_with_sym(abfd, section, sym, addr, pinfo, FALSE,
                                     do_demangle, display_file_offsets);
            pinfo->fprintf_func(pinfo->stream, ":\n");
        }
    }

    if (sym != NULL && bfd_asymbol_value(sym) > addr)
        nextsym = sym;
    else if (sym == NULL)
        nextsym = NULL;
    else {
#define is_valid_next_sym(SYM)                                                 \
    ((SYM)->section == section &&                                              \
     (bfd_asymbol_value(SYM) > bfd_asymbol_value(sym)) &&                      \
     pinfo->symbol_is_valid(SYM, pinfo))

        /* Search forward for the next appropriate symbol in
           SECTION.  Note that all the symbols are sorted
           together into one big array, and that some sections
           may have overlapping addresses.  */
        while (place < sorted_symcount &&
               !is_valid_next_sym(sorted_syms[place]))
            ++place;

        if (place >= sorted_symcount)
            nextsym = NULL;
        else
            nextsym = sorted_syms[place];
    }

    if (sym != NULL && bfd_asymbol_value(sym) > addr)
        nextstop_offset = bfd_asymbol_value(sym) - section->vma;
    else if (nextsym == NULL)
        nextstop_offset = stop_offset;
    else
        nextstop_offset = bfd_asymbol_value(nextsym) - section->vma;

    if (nextstop_offset > stop_offset || nextstop_offset <= addr_offset)
        nextstop_offset = stop_offset;

    /* If a symbol is explicitly marked as being an object
       rather than a function, just dump the bytes without
       disassembling them.  */
    if (disassemble_all || sym == NULL || sym->section != section ||
        bfd_asymbol_value(sym) > addr ||
        ((sym->flags & BSF_OBJECT) == 0 &&
         (strstr(bfd_asymbol_name(sym), "gnu_compiled") == NULL) &&
         (strstr(bfd_asymbol_name(sym), "gcc2_compiled") == NULL)) ||
        (sym->flags & BSF_FUNCTION) != 0)
        insns = TRUE;
    else
        insns = FALSE;

    trdb_disassemble_bytes(pinfo, dunit->disassemble_fn, insns, pinfo->buffer,
                           addr - section->vma, rel_offset, &rel_pp, rel_ppend);

    addr_offset = nextstop_offset;
    sym         = nextsym;

    if (rel_ppstart != NULL)
        free(rel_ppstart);
    return;

fail:
    if (!rel_pp)
        free(rel_pp);
    if (!pinfo->buffer)
        free(pinfo->buffer);
}

void trdb_dump_section_names(bfd *abfd)
{
    bfd_map_over_sections(abfd, trdb_dump_section_header, NULL);
}

void trdb_dump_section_header(bfd *abfd, asection *section, void *ignored)
{
    bfd_printf_vma(abfd, bfd_get_section_vma(abfd, section));
    printf("%s\n", bfd_get_section_name(abfd, section));
}

void trdb_dump_bin_info(bfd *abfd)
{
    printf("information about binary:\n");
    printf("flavour: %u\n", bfd_get_flavour(abfd));
    printf("name   : %s\n", bfd_printable_name(abfd));
    printf("size   : %d\n", bfd_get_arch_size(abfd));
    printf("mach   : %s\n", bfd_printable_arch_mach(bfd_get_arch(abfd), 0));
    printf("endian : %d\n", abfd->xvec->byteorder);
}

void trdb_dump_target_list()
{
    printf("available target list:\n");
    const char **list = bfd_target_list();
    for (unsigned int i = 0; list[i]; i++) {
        printf("%s\n", list[i]);
    }
}

bool trdb_vma_in_section(bfd *abfd, asection *section, bfd_vma vma)
{
    return (vma >= section->vma &&
            vma < (section->vma + bfd_section_size(abfd, section)));
}

asection *trdb_get_section_for_vma(bfd *abfd, bfd_vma vma)
{
    asection *p;
    for (p = abfd->sections; p != NULL; p = p->next) {
        if (trdb_vma_in_section(abfd, p, vma)) {
            return p;
        }
    }
    return NULL;
}

void trdb_disassemble_section(bfd *abfd, asection *section, void *inf)
{
    /* Do not disassemble sections without machine code*/
    if ((section->flags & (SEC_CODE | SEC_HAS_CONTENTS)) !=
        (SEC_CODE | SEC_HAS_CONTENTS)) {
        return;
    }
    struct disassembler_unit *dunit = inf;
    if (!dunit) {
        fprintf(stderr, "disassembler_unit is NULL\n");
        return;
    }

    struct disassemble_info *dinfo    = dunit->dinfo;
    disassembler_ftype disassemble_fn = dunit->disassemble_fn;
    if (!dinfo || !disassemble_fn) {
        fprintf(stderr, "unitialized member of disassembler_unit");
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

    dinfo->buffer        = data;
    dinfo->buffer_vma    = section->vma;
    dinfo->buffer_length = datasize;
    dinfo->section       = section;

    struct trdb_disasm_aux *aux = dinfo->application_data;
    if (aux)
        aux->sec = section;

    printf("disassembly of section %s:\n", section->name);
    while (addr_offset < stop_offset) {
        /* print instr address */
        (*dinfo->fprintf_func)(dinfo->stream, "0x%016jx  ",
                               (uintmax_t)(section->vma + addr_offset));
        int size = (*disassemble_fn)(section->vma + addr_offset, dinfo);
        addr_offset += size;
        (*dinfo->fprintf_func)(dinfo->stream, "\n");
        if (size <= 0) {
            fprintf(stderr, "encountered instruction with %d bytes, stopping",
                    size);
            break;
        }
    }

    free(data);
}

void trdb_disassemble_block(size_t len, bfd_byte data[len],
                            struct disassembler_unit *dunit)
{
    if (!dunit) {
        fprintf(stderr, "disassembler_unit is NULL\n");
        return;
    }

    struct disassemble_info *dinfo    = dunit->dinfo;
    disassembler_ftype disassemble_fn = dunit->disassemble_fn;
    if (!dinfo || !disassemble_fn) {
        fprintf(stderr, "unitialized member of disassembler_unit");
        return;
    }

    size_t pc            = 0;
    dinfo->buffer        = data;
    dinfo->buffer_vma    = pc;
    dinfo->buffer_length = len;

    while (pc < len) {
        int size = (*disassemble_fn)(pc, dinfo);
        pc += size;
        (*dinfo->fprintf_func)(dinfo->stream, "\n");
        if (size <= 0) {
            fprintf(stderr, "encountered instruction with %d bytes, stopping",
                    size);
            break;
        }
    }
}

void trdb_disassemble_single_instruction(uint32_t instr, uint32_t addr,
                                         struct disassembler_unit *dunit)
{
    if (!dunit) {
        fprintf(stderr, "disassembler_unit is NULL\n");
        return;
    }

    struct disassemble_info *dinfo    = dunit->dinfo;
    disassembler_ftype disassemble_fn = dunit->disassemble_fn;
    if (!dinfo || !disassemble_fn) {
        fprintf(stderr, "unitialized member of disassembler_unit\n");
        return;
    }

    size_t len     = 8;
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
    dinfo->buffer        = data;
    dinfo->buffer_vma    = addr;
    dinfo->buffer_length = len;

    int size = (*disassemble_fn)(addr, dinfo);
    (*dinfo->fprintf_func)(dinfo->stream, "\n");
    if (size <= 0) {
        fprintf(stderr, "encountered instruction with %d bytes, stopping",
                size);
    }

    free(data);
}

void trdb_disassemble_single_instruction_slow(uint32_t instr, uint32_t addr)
{
    struct disassembler_unit dunit = {0};
    struct disassemble_info dinfo  = {0};
    dunit.dinfo                    = &dinfo;

    trdb_init_disassembler_unit_for_pulp(&dunit, NULL);

    trdb_disassemble_single_instruction(instr, addr, &dunit);
}
