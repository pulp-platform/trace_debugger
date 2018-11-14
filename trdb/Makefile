# trdb - Trace Debugger Software for the PULP platform
#
# Copyright (C) 2018 ETH Zurich and University of Bologna
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

CFLAGS		= -Wall -O2 -g -march=native -Wno-unused-function \
			-DENABLE_LOGGING -DNDEBUG
# we need gnu11 and no-strict-aliasing
ALL_CFLAGS	= -std=gnu11 -fno-strict-aliasing $(CFLAGS)
ALL_CFLAGS_DBG	= -std=gnu11 -Wall -O0 -g -fno-strict-aliasing \
			-Wno-unused-function -fsanitize=address \
			-DENABLE_LOGGING -DENABLE_DEBUG


QUESTASIM_PATH	= /usr/pack/modelsim-10.5c-kgf/questasim

# Prebuilt libbfd, libopcodes, libz and libiberty because it is annoying to
# build pulp-riscv-binutils-gdb. If you decide to use your own build, then
# change this path
PULP_BINUTILS_PATH = lib

LIB_PATHS       = $(PULP_BINUTILS_PATH)/opcodes \
		$(PULP_BINUTILS_PATH)/bfd \
		$(PULP_BINUTILS_PATH)/libiberty \
		$(PULP_BINUTILS_PATH)/zlib

INCLUDE_PATHS   = ./ \
		./include \
		./internal \
		$(PULP_BINUTILS_PATH)/include \
		$(PULP_BINUTILS_PATH)/bfd \
		$(QUESTASIM_PATH)/include \
		/usr/include

MORE_TAG_PATHS  = $(PULP_BINUTILS_PATH)/bfd

LIBS            = libbfd.a libopcodes.a libiberty.a libz.a
LDFLAGS		= $(addprefix -L, $(LIB_PATHS))
LDLIBS		= $(addprefix -l:, $(LIBS)) -ldl -lc

MAIN_SRCS	= $(wildcard main/*.c)
MAIN_OBJS	= $(MAIN_SRCS:.c=.o)

TEST_SRCS	= $(wildcard test/*.c)
TEST_OBJS	= $(TEST_SRCS:.c=.o)

BENCHMARK_SRCS  = $(wildcard benchmark/*.c)
BENCHMARK_OBJS  = $(BENCHMARK_SRCS:.c=.o)

DPI_SRCS	= $(wildcard dpi/*.c)
DPI_OBJS	= $(DPI_SRCS:.c=.o)

SRCS		= $(wildcard *.c)
OBJS		= $(SRCS:.c=.o)
INCLUDES	= $(addprefix -I, $(INCLUDE_PATHS))

HEADERS		= $(wildcard include/*.h)

BIN		= trdb
TEST_BIN	= tests
BENCHMARK_BIN   = benchmarks

# GNU recommendations
prefix          = /usr/local
exec_prefix     = $(prefix)
bindir          = $(exec_prefix)/bin
libdir          = $(exec_prefix)/lib
includedir      = $(prefix)/include

INSTALL         = install
INSTALL_PROGRAM = $(INSTALL)
INSTALL_DATA    = ${INSTALL} -m 644

# golden model lib for simulator
SV_LIB		= libtrdbsv
LIB		= libtrdb
STATIC_LIB      = libtrdb


CTAGS		= ctags

VALGRIND	= valgrind

DOC		= doxygen


# compilation targets
all: $(BIN) $(TEST_BIN) $(BENCHMARK_BIN)

debug: ALL_CFLAGS = $(ALL_CFLAGS_DEBUG)
debug: all

sv-lib: ALL_CFLAGS += -fPIC
sv-lib: $(SV_LIB).so

lib: ALL_CFLAGS += -fPIC
lib: $(LIB).so

static-lib: ALL_CFLAGS += -fPIC
static-lib: $(STATIC_LIB).a

# all install targets
install-headers: $(HEADERS)
	$(INSTALL_DATA) $(HEADERS) $(DESTDIR)$(includedir)

install-static-lib: static-lib
	$(INSTALL_DATA) $(STATIC_LIB).a $(DESTDIR)$(libdir)/$(STATIC_LIB).a

install-lib: lib
	$(INSTALL_DATA) $(LIB).so $(DESTDIR)$(libdir)/$(LIB).so

install-trdb: $(BIN)
	$(INSTALL_PROGRAM) $(BIN) $(DESTDIR)$(bindir)/$(BIN)

install: install-static-lib install-lib install-trdb

install-strip:
	$(MAKE) INSTALL_PROGRAM='$(INSTALL_PROGRAM) -s' install

uninstall:
	rm $(DESTDIR)$(bindir)/$(BIN)
	rm $(DESTDIR)$(libdir)/$(LIB).so
	rm $(DESTDIR)$(libdir)/$(STATIC_LIB).a
	rm $(DESTDIR)$(includedir)/$(HEADERS)

# compilation boilerplate
$(BIN): $(OBJS) $(MAIN_OBJS)
	$(CC) $(ALL_CFLAGS) $(INCLUDES) -o $@ $(LDFLAGS) $(MAIN_OBJS) $(OBJS) \
		$(LDLIBS)

$(TEST_BIN): $(OBJS) $(TEST_OBJS)
	$(CC) $(ALL_CFLAGS) $(INCLUDES) -o $@ $(LDFLAGS) $(TEST_OBJS) $(OBJS) \
		$(LDLIBS)

$(BENCHMARK_BIN): $(OBJS) $(BENCHMARK_OBJS)
	$(CC) $(ALL_CFLAGS) $(INCLUDES) -o $@ $(LDFLAGS) $(BENCHMARK_OBJS) \
		$(OBJS) $(LDLIBS)

$(SV_LIB).so: $(OBJS) $(DPI_OBJS)
#	gcc -o $(SV_LIB).so -shared $(LD_FLAGS) $(OBJS) $(DPI_OBJS)
	ld -shared -E --exclude-libs ALL -o $(SV_LIB).so $(LDFLAGS) \
		$(OBJS) $(DPI_OBJS) $(LDLIBS)

$(LIB).so: $(OBJS) $(DPI_OBJS)
	ld -shared -E --exclude-libs ALL -o $(LIB).so $(LDFLAGS) \
		$(OBJS) $(LDLIBS)

$(STATIC_LIB).a: $(OBJS)
	$(AR) -rs $@ $(OBJS)

# $@ = name of target
# $< = first dependency
%.o: %.c
	$(CC) $(ALL_CFLAGS) $(INCLUDES) $(LDFLAGS) \
		-c $(CPPFLAGS) $< -o $@ $(LDLIBS)

# run targets
.PHONY: run
run:
	./$(BIN)

.PHONY: test
test:
	./$(TEST_BIN)

.PHONY: benchmark
benchmark:
	./$(BENCHMARK_BIN)

.PHONY: check
check: test

.PHONY: valgrind-test
valgrind-test:
	$(VALGRIND) -v --leak-check=full --track-origins=yes ./$(TEST_BIN)

.PHONY: valgrind-main
valgrind-main:
	$(VALGRIND) -v --leak-check=full --track-origins=yes ./$(BIN)

.PHONY: valgrind-benchmark
valgrind-benchmark:
	$(VALGRIND) -v --leak-check=full --track-origins=yes ./$(BENCHMARK_BIN)

# emacs tag generation
.PHONY: TAGS
TAGS:
	$(CTAGS) -R -e -h=".c.h" --tag-relative=always . $(LIB_PATHS) \
		$(INCLUDE_PATHS) $(MORE_TAG_PATHS)

# documentation
docs: doxyfile $(SRCS) $(MAIN_SRCS) $(TEST_SRCS)
	$(DOC) doxyfile

# cleanup
.PHONY: clean
clean:
	rm -rf $(BIN) $(TEST_BIN) $(BENCHMARK_BIN) $(SV_LIB).so \
	$(STATIC_LIB).a $(LIB).so $(OBJS) $(MAIN_OBJS) $(TEST_OBJS) \
	$(BENCHMARK_OBJS) $(DPI_OBJS)

.PHONY: distclean
distclean: clean
	rm -f TAGS
	rm -rf doc/*
