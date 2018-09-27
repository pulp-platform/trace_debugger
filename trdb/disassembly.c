/*
 * trdb - Trace Debugger Software for the PULP platform
 *
 * Copyright (C) 2018 ETH Zurich and University of Bologna
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
#include <errno.h>
#include "disassembly.h"
#include "trace_debugger.h"
#include "utils.h"
#include "bfd.h"
#include "elf-bfd.h"
#include "dis-asm.h"
/* fix nameconflict with basename in libiberty and libgen */
#define HAVE_DECL_BASENAME 1
#include "libiberty.h"
#include "demangle.h"

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

    an = bfd_asymbol_name(a);
    bn = bfd_asymbol_name(b);
    anl = strlen(an);
    bnl = strlen(bn);

    /* The symbols gnu_compiled and gcc2_compiled convey no real
       information, so put them after other symbols with the same value.  */
    af = (strstr(an, "gnu_compiled") != NULL
          || strstr(an, "gcc2_compiled") != NULL);
    bf = (strstr(bn, "gnu_compiled") != NULL
          || strstr(bn, "gcc2_compiled") != NULL);

    if (af && !bf)
        return 1;
    if (!af && bf)
        return -1;

        /* We use a heuristic for the file name, to try to sort it after
           more useful symbols.  It may not work on non Unix systems, but it
           doesn't really matter; the only difference is precisely which
           symbol names get printed.  */

#define file_symbol(s, sn, snl)                                                \
    (((s)->flags & BSF_FILE) != 0                                              \
     || ((sn)[(snl)-2] == '.'                                                  \
         && ((sn)[(snl)-1] == 'o' || (sn)[(snl)-1] == 'a')))

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

    if (bfd_get_flavour(bfd_asymbol_bfd(a)) == bfd_target_elf_flavour
        && bfd_get_flavour(bfd_asymbol_bfd(b)) == bfd_target_elf_flavour) {
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
    return strcmp(name, ".plt") == 0 || strcmp(name, ".got") == 0
           || strcmp(name, ".plt.got") == 0;
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
        if ((sym->flags & (BSF_DEBUGGING | BSF_SECTION_SYM))
            && !is_significant_symbol_name(sym->name))
            continue;
        if (bfd_is_und_section(sym->section)
            || bfd_is_com_section(sym->section))
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
    name = bfd_asymbol_name(sym);
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
    long min = 0;
    long max_count = aux->sorted_symcount;
    long thisplace;
    bfd *abfd;
    asection *sec;
    unsigned int opb;
    bfd_boolean want_section;

    asymbol **sorted_syms = aux->sorted_symbols;
    long sorted_symcount = aux->sorted_symcount;

    if (sorted_symcount < 1)
        return NULL;

    abfd = aux->abfd;
    sec = aux->sec;
    opb = inf->octets_per_byte;

    /* Perform a binary search looking for the closest symbol to the required
     * value. We are searching the range (min, max_count].
     */
    while (min + 1 < max_count) {
        asymbol *sym;

        thisplace = (max_count + min) / 2;
        sym = sorted_syms[thisplace];

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
    while (thisplace > 0
           && (bfd_asymbol_value(sorted_syms[thisplace])
               == bfd_asymbol_value(sorted_syms[thisplace - 1]))
           && ((sorted_syms[thisplace - 1]->flags
                & (BSF_SECTION_SYM | BSF_DEBUGGING))
               == 0))
        --thisplace;

    /* Prefer a symbol in the current section if we have multple symbols with
     * the same value, as can occur with overlays or zero size sections.
     */
    min = thisplace;
    while (min < max_count
           && (bfd_asymbol_value(sorted_syms[min])
               == bfd_asymbol_value(sorted_syms[thisplace]))) {
        if (sorted_syms[min]->section == sec
            && inf->symbol_is_valid(sorted_syms[min], inf)) {
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
    want_section = (aux->require_sec
                    || ((abfd->flags & HAS_RELOC) != 0
                        && vma >= bfd_get_section_vma(abfd, sec)
                        && vma < (bfd_get_section_vma(abfd, sec)
                                  + bfd_section_size(abfd, sec) / opb)));
    if ((sorted_syms[thisplace]->section != sec && want_section)
        || !inf->symbol_is_valid(sorted_syms[thisplace], inf)) {
        long i;
        long newplace = sorted_symcount;

        for (i = min - 1; i >= 0; i--) {
            if ((sorted_syms[i]->section == sec || !want_section)
                && inf->symbol_is_valid(sorted_syms[i], inf)) {
                if (newplace == sorted_symcount)
                    newplace = i;

                if (bfd_asymbol_value(sorted_syms[i])
                    != bfd_asymbol_value(sorted_syms[newplace]))
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
                if ((sorted_syms[i]->section == sec || !want_section)
                    && inf->symbol_is_valid(sorted_syms[i], inf)) {
                    thisplace = i;
                    break;
                }
            }
        }

        if ((sorted_syms[thisplace]->section != sec && want_section)
            || !inf->symbol_is_valid(sorted_syms[thisplace], inf))
            /* There is no suitable symbol.  */
            return NULL;
    }

    /* If we have not found an exact match for the specified address and we have
     * dynamic relocations available, then we can produce a better result by
     * matching a relocation to the address and using the symbol associated with
     * that relocation.
     */
    if (!want_section && aux->dynrelbuf != NULL
        && sorted_syms[thisplace]->value != vma
        /* If we have matched a synthetic symbol, then stick with that.  */
        && (sorted_syms[thisplace]->flags & BSF_SYNTHETIC) == 0) {
        long rel_count;
        arelent **rel_pp;

        for (rel_count = aux->dynrelcount, rel_pp = aux->dynrelbuf;
             rel_count--;) {
            arelent *rel = rel_pp[rel_count];

            if (rel->address == vma
                && rel->sym_ptr_ptr != NULL
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
        else if ((bfd_get_file_flags(abfd) & (EXEC_P | DYNAMIC))
                 && bfd_is_und_section(sym->section))
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
    asymbol *sym = NULL;
    bfd_boolean skip_find = FALSE;

    aux = inf->application_data;
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

    if (aux->reloc != NULL && aux->reloc->sym_ptr_ptr != NULL
        && *aux->reloc->sym_ptr_ptr != NULL) {
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
    bool prefix_addresses = false; /* TODO: make this configurable */
    bool do_demangle = false;
    bool display_file_offsets = false;
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
    char *machine = NULL;
    enum bfd_endian endian = BFD_ENDIAN_UNKNOWN;
    char *disassembler_options = NULL; /* TODO: take from context */

    int status = 0;

    struct disassemble_info *dinfo = dunit->dinfo;
    struct trdb_disasm_aux *aux = malloc(sizeof(*aux));
    if (!aux) {
        err(c, "malloc: %s\n", strerror(errno));
        return -ENOMEM;
    }

    struct bfd_target *xvec = NULL;

    long symcount = 0;
    long dynsymcount = 0;
    long synthcount = 0;
    long sorted_symcount = 0;
    asymbol **sorted_syms = NULL;
    asymbol **syms = slurp_symtab(c, abfd, &symcount);
    asymbol **dynsyms = slurp_dynamic_symtab(c, abfd, &dynsymcount);
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
        err(c, "malloc:%s\n", strerror(errno));
        status = -ENOMEM;
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

    aux->symbols = syms;
    aux->symcount = symcount;
    aux->dynamic_symbols = dynsyms;
    aux->dynsymcount = dynsymcount;
    aux->synthethic_symbols = synthsyms;
    aux->synthcount = synthcount;
    aux->sorted_symcount = sorted_symcount;
    aux->sorted_symbols = sorted_syms;

    aux->abfd = abfd;
    aux->require_sec = FALSE;
    aux->dynrelbuf = NULL;
    aux->dynrelcount = 0;
    aux->reloc = NULL;

    dinfo->print_address_func = trdb_print_address;
    dinfo->symbol_at_address_func = trdb_symbol_at_address;

    if (machine != NULL) {
        const bfd_arch_info_type *inf = bfd_scan_arch(machine);

        if (inf == NULL) {
            err(c, "can't use supplied machine %s\n", machine);
            status = -1;
            goto fail;
        }

        abfd->arch_info = inf;
    }

    if (endian != BFD_ENDIAN_UNKNOWN) {

        xvec = malloc(sizeof(struct bfd_target));
        if (!xvec) {
            err(c, "malloc: %s\n", strerror(errno));
            status = -ENOMEM;
            goto fail;
        }
        memcpy(xvec, abfd->xvec, sizeof(struct bfd_target));
        xvec->byteorder = endian;
        abfd->xvec = xvec;
    }

    /* Use libopcodes to locate a suitable disassembler.  */
    dunit->disassemble_fn = disassembler(abfd);
    if (!dunit->disassemble_fn) {
        err(c, "can't disassemble for architecture %s\n",
            bfd_printable_arch_mach(bfd_get_arch(abfd), 0));
        status = -1;
        goto fail;
    }

    dinfo->flavour = bfd_get_flavour(abfd);
    dinfo->arch = bfd_get_arch(abfd);
    dinfo->mach = bfd_get_mach(abfd);
    dinfo->disassembler_options = disassembler_options;
    dinfo->octets_per_byte = bfd_octets_per_byte(abfd);
    dinfo->skip_zeroes = DEFAULT_SKIP_ZEROES;
    dinfo->skip_zeroes_at_end = DEFAULT_SKIP_ZEROES_AT_END;
    dinfo->disassembler_needs_relocs = FALSE;

    /* Pre-load the dynamic relocs as we may need them during the disassembly.
     */
    {
        long relsize = bfd_get_dynamic_reloc_upper_bound(abfd);

        if (relsize > 0) {
            aux->dynrelbuf = malloc(relsize);
            if (!aux->dynrelbuf) {
                err(c, "malloc: %s\n", strerror(errno));
                return -ENOMEM;
            }

            aux->dynrelcount =
                bfd_canonicalize_dynamic_reloc(abfd, aux->dynrelbuf, dynsyms);
            if (aux->dynrelcount < 0) {
                err(c, "reloc: %s\n", (bfd_get_filename(abfd)));
                return -ENOMEM;
            }

            /* Sort the relocs by address.  */
            qsort(aux->dynrelbuf, aux->dynrelcount, sizeof(arelent *),
                  compare_relocs);
        }
    }
    dinfo->symtab = sorted_syms;
    dinfo->symtab_size = sorted_symcount;

    return status;
fail:
    free(xvec);
    free(sorted_syms);
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
    }

    if (aux) {
        free(aux->dynrelbuf);
        free(aux->symbols);
        free(aux->dynamic_symbols);
        free(aux->synthethic_symbols);
        free(aux);
    }
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
