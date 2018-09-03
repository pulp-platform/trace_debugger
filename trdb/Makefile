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

CC		= gcc
CPPFLAGS	=
override CFLAGS		+= -std=gnu11 -Wall -O2 -fno-strict-aliasing \
			-Wno-unused-function -DNDEBUG
override CFLAGS_DEBUG	+= -std=gnu11 -Wall -g -fno-strict-aliasing \
			-Wno-unused-function

QUESTASIM_PATH = /usr/pack/modelsim-10.5c-kgf/questasim/
PULP_BINUTILS_PATH = /scratch/balasr/pulp-riscv-binutils-gdb/

LIB_PATHS       = $(PULP_BINUTILS_PATH)/opcodes \
		$(PULP_BINUTILS_PATH)/bfd \
		$(PULP_BINUTILS_PATH)/libiberty \
		$(PULP_BINUTILS_PATH)/zlib

INCLUDE_PATHS   = $(PULP_BINUTILS_PATH)/include \
		$(QUESTASIM_INCLUDE)/include \
		/usr/include

MORE_TAG_PATHS  = $(PULP_BINUTILS_PATH)/bfd

LDFLAGS		= $(addprefix -L, $(LIB_PATHS))
LDLIBS		= -l:libbfd.a -l:libopcodes.a -l:libiberty.a -l:libz.a -ldl

MAIN_SRCS	= $(wildcard main/*.c)
MAIN_OBJS	= $(MAIN_SRCS:.c=.o)

TEST_SRCS	= $(wildcard test/*.c)
TEST_OBJS	= $(TEST_SRCS:.c=.o)

SRCS		= $(wildcard *.c)
OBJS		= $(SRCS:.c=.o)
INCLUDES	= $(addprefix -I, $(INCLUDE_PATHS))
BIN		= trdb
TEST_BIN	= tests


CTAGS		= ctags

VALGRIND	= valgrind

DOC		= doxygen


all: $(BIN) $(TEST_BIN)

debug: CFLAGS = $(CFLAGS_DEBUG)
debug: all

$(BIN): $(OBJS) $(MAIN_OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $(LDFLAGS) $(MAIN_OBJS) $(OBJS) \
		$(LDLIBS)

$(TEST_BIN): $(OBJS) $(TEST_OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $(LDFLAGS) $(TEST_OBJS) $(LDLIBS)

# $@ = name of target
# $< = first dependency
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES)  $(LDFLAGS) -c $< -o $@ $(LDLIBS)

.PHONY: run
run:
	./$(BIN)

.PHONY: test
test:
	./$(TEST_BIN)

.PHONY: valgrind-test
valgrind-test:
	$(VALGRIND) -v --leak-check=full --track-origins=yes ./$(TEST_BIN)

.PHONY: valgrind-main
valgrind-main:
	$(VALGRIND) -v --leak-check=full --track-origins=yes ./$(BIN)

.PHONY: TAGS
TAGS:
	$(CTAGS) -R -e -h=".c.h" --tag-relative=always . $(LIB_PATHS) \
	$(INCLUDE_PATHS) $(MORE_TAG_PATHS)

docs: doxyfile $(SRCS) $(MAIN_SRCS) $(TEST_SRCS)
	$(DOC) doxyfile

.PHONY: clean
clean:
	rm -rf $(BIN) $(TEST_BIN) $(OBJS) $(MAIN_OBJS) $(TEST_OBJS)

.PHONY: distclean
distclean: clean
	rm -f TAGS
	rm -rf doc/*
