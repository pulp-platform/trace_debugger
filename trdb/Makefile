# trdb - Trace Debugger Software for the PULP platform
#
# Copyright (C) 2018 Robert Balas
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>.
#
#
# Author: Robert Balas (balasr@student.ethz.ch)
# Description: Build the C model and decompression tools for the trace debugger

CFLAGS          = -Wall -Wextra -Wno-missing-field-initializers \
			-Wno-unused-function -Wno-missing-braces \
			-O2 -g -march=native \
			-DENABLE_LOGGING -DNDEBUG
CFLAGS_DBG      =
# we need gnu11 and no-strict-aliasing
ALL_CFLAGS      = -std=gnu11 -fno-strict-aliasing $(CFLAGS)
ALL_CFLAGS_DBG  = -std=gnu11 -Wall -Wextra -Wno-missing-field-initializers \
			-Wno-unused-function -Wno-missing-braces \
			-O0 -g -fno-strict-aliasing \
			-fsanitize=address -fno-omit-frame-pointer \
			-DENABLE_LOGGING -DENABLE_DEBUG $(CFLAGS_DBG)\
# -fsanitize=undefined \
# -fsanitize=leak \


QUESTASIM_PATH  = /usr/pack/modelsim-10.5c-kgf/questasim

# Prebuilt libbfd, libopcodes, libz and libiberty because it is annoying to
# build pulp-riscv-binutils-gdb. If you decide to use your own build, then
# change this path

BINUTILS_PATH = lib

LIB_DIRS        = $(addprefix $(BINUTILS_PATH)/,opcodes bfd libiberty zlib)
LIBS            = bfd opcodes iberty z dl c

INCLUDE_DIRS    = ./ ./include ./internal \
		$(addprefix $(BINUTILS_PATH)/,include bfd) \
		$(QUESTASIM_PATH)/include


LDFLAGS         = $(addprefix -L, $(LIB_DIRS))
LDLIBS          = $(addprefix -l, $(LIBS))

SRCS            = $(wildcard *.c)
OBJS            = $(SRCS:.c=.o)
INCLUDES        = $(addprefix -I, $(INCLUDE_DIRS))

DPI_SRCS        = $(wildcard dpi/*.c)
DPI_OBJS        = $(DPI_SRCS:.c=.o)

HEADERS         = $(wildcard include/*.h)

# executables
CLI_BIN         = main/trdb
BENCH_BIN       = benchmark/benchmarks
TESTS_BIN       = test/tests

MAINS           = $(CLI_BIN) $(BENCH_BIN) $(TESTS_BIN)
MAINS_SRCS      = $(addsuffix .c,$(MAINS))
MAINS_OBJS      = $(addsuffix .o,$(MAINS))

# different libs (sv_lib for simulators, others for software)
SV_LIB          = libtrdbsv
LIB             = libtrdb
STATIC_LIB      = libtrdb

# header file dependency generation
DEPDIR          := .d
DEPDIRS         := $(addsuffix /$(DEPDIR),. main benchmark test)
# goal: make gcc put a dependency file called obj.Td (derived from subdir/obj.o)
# in subdir/.d/
DEPFLAGS        = -MT $@ -MMD -MP -MF $(@D)/$(DEPDIR)/$(patsubst %.o,%.Td,$(@F))
# move gcc generated header dependencies to DEPDIR
# this rename step is here to make the header dependency generation "atomic"
POSTCOMPILE     = @mv -f $(@D)/$(DEPDIR)/$(patsubst %.o,%.Td,$(@F)) \
			$(@D)/$(DEPDIR)/$(patsubst %.o,%.d,$(@F)) && touch $@

# GNU recommendations for install targets
prefix          = /usr/local
exec_prefix     = $(prefix)
bindir          = $(exec_prefix)/bin
libdir          = $(exec_prefix)/lib
includedir      = $(prefix)/include

INSTALL         = install
INSTALL_PROGRAM = $(INSTALL)
INSTALL_DATA    = ${INSTALL} -m 644

CTAGS           = ctags
VALGRIND        = valgrind
DOXYGEN         = doxygen


# compilation targets
all: $(MAINS)

debug: ALL_CFLAGS = $(ALL_CFLAGS_DBG)
debug: all

sv-lib: ALL_CFLAGS += -fPIC
sv-lib: $(SV_LIB).so

lib: ALL_CFLAGS += -fPIC
lib: $(LIB).so

static-lib: ALL_CFLAGS += -fPIC
static-lib: $(STATIC_LIB).a

#compilation boilerplate
$(MAINS): %: %.o $(OBJS)
	$(CC) $(ALL_CFLAGS) $(INCLUDES) -o $@ $(LDFLAGS) $^ $(LDLIBS)

$(SV_LIB).so: $(OBJS) $(DPI_OBJS)
	$(LD) -shared -E --exclude-libs ALL -o $(SV_LIB).so $(LDFLAGS) \
		$(OBJS) $(DPI_OBJS) $(LDLIBS)

$(LIB).so: $(OBJS) $(DPI_OBJS)
	$(LD) -shared -E --exclude-libs ALL -o $(LIB).so $(LDFLAGS) \
		$(OBJS) $(LDLIBS)

$(STATIC_LIB).a: $(OBJS)
	$(AR) -rs $@ $(OBJS)

# $@ = name of target
# $< = first dependency
%.o: %.c
%.o: %.c $(DEPDIR)/%.d $(DEPDIRS)
	$(CC) $(DEPFLAGS) $(ALL_CFLAGS) $(INCLUDES) $(LDFLAGS) \
		-c $(CPPFLAGS) $< -o $@ $(LDLIBS)
	$(POSTCOMPILE)

# check if we need to create the dependencies folders (gcc doesn't)
$(DEPDIRS):
	$(shell mkdir -p $(DEPDIRS) > /dev/null)
# make won't fail if the dependency file doesn't exist
$(addsuffix /$(DEPDIR)/%.d,. main benchmark test): ;

# prevent automatic deletion as intermediate file
.PRECIOUS: $(addsuffix /$(DEPDIR)/%.d,. main benchmark test)

# run targets
.PHONY: run
run: $(CLI_BIN)
	./$(CLI_BIN)

.PHONY: test
test: $(TESTS_BIN)
	./$(TESTS_BIN)

.PHONY: benchmark
benchmark: $(BENCH_BIN)
	./$(BENCH_BIN)

.PHONY: check
check: test

.PHONY: valgrind-test
valgrind-test: $(TESTS_BIN)
	$(VALGRIND) -v --leak-check=full --track-origins=yes ./$(TESTS_BIN)

.PHONY: valgrind-main
valgrind-main: $(CLI_BIN)
	$(VALGRIND) -v --leak-check=full --track-origins=yes ./$(CLI_BIN)

.PHONY: valgrind-benchmark
valgrind-benchmark: $(BENCH_BIN)
	$(VALGRIND) -v --leak-check=full --track-origins=yes ./$(BENCH_BIN)

# all install targets
install-headers: $(HEADERS)
	$(INSTALL_DATA) $(HEADERS) $(DESTDIR)$(includedir)

install-static-lib: static-lib
	$(INSTALL_DATA) $(STATIC_LIB).a $(DESTDIR)$(libdir)/$(STATIC_LIB).a

install-lib: lib
	$(INSTALL_DATA) $(LIB).so $(DESTDIR)$(libdir)/$(LIB).so

install-trdb: $(CLI_BIN)
	$(INSTALL_PROGRAM) $(CLI_BIN) $(DESTDIR)$(bindir)/$(CLI_BIN)

install: install-static-lib install-lib install-trdb

install-strip:
	$(MAKE) INSTALL_PROGRAM='$(INSTALL_PROGRAM) -s' install

uninstall:
	rm $(DESTDIR)$(bindir)/$(CLI_BIN)
	rm $(DESTDIR)$(libdir)/$(LIB).so
	rm $(DESTDIR)$(libdir)/$(STATIC_LIB).a
	rm $(DESTDIR)$(includedir)/$(HEADERS)

# emacs tag generation
.PHONY: TAGS
TAGS:
	$(CTAGS) -R -e -h=".c.h" --tag-relative=always \
		. $(LIB_DIRS) $(INCLUDE_DIRS) $(BINUTILS_PATH)/bfd

# documentation
docs: doxyfile $(SRCS) $(MAINS_SRCS)
	$(DOXYGEN) doxyfile

# patched spike to produce traces
spike-generate-traces: spike riscv-tests/benchmarks/build.ok
	for benchmark in riscv-tests/benchmarks/*.riscv; do \
		./trdb-spike \
			--ust-trace=riscv-traces/$$(basename $$benchmark).cvs \
			$$benchmark; \
	done


spike: riscv-isa-sim/build.ok riscv-fesvr/build.ok

riscv-isa-sim/build.ok: riscv-fesvr/build.ok
	rm -rf riscv-isa-sim
	git clone https://github.com/pulp-platform/riscv-isa-sim
	cd riscv-isa-sim && \
		LDFLAGS="-L../riscv-fesvr" ./configure --with-isa=RV32IMC
	+cd riscv-isa-sim && ln -s ../riscv-fesvr/fesvr . && \
		$(MAKE) && touch build.ok & cd ..
	echo "#!/usr/bin/env bash" > trdb-spike
	echo "LD_LIBRARY_PATH=./riscv-isa-sim:./riscv-fesvr ./riscv-isa-sim/spike \"\$$@\"" \
		>> trdb-spike
	chmod u+x trdb-spike


riscv-fesvr/build.ok:
	rm -rf riscv-fesvr
	git clone https://github.com/riscv/riscv-fesvr.git riscv-fesvr
	+cd riscv-fesvr && ./configure && $(MAKE) && touch build.ok

# riscv-benchmark-tests
# have your $RISCV point to your compiler
riscv-tests/benchmarks/build.ok:
	rm -rf riscv-tests
	git clone https://github.com/riscv/riscv-tests/ --recursive
	cd riscv-tests/benchmarks && XLEN=32 $(MAKE) && touch build.ok
# cleanup
.PHONY: clean
clean:
	rm -rf $(MAINS) $(SV_LIB).so $(STATIC_LIB).a $(LIB).so \
		$(OBJS) $(MAINS_OBJS) $(DPI_OBJS) $(DEPDIRS)

.PHONY: distclean
distclean: clean
	rm -f TAGS
	rm -rf doc/*
	rm -rf trdb-spike
	rm -rf riscv-fesvr
	rm -rf riscv-isa-sim
	rm -rf riscv-tests

# include auto generated header dependency information
include $(wildcard $(addsuffix /*.d,$(DEPDIRS)))
