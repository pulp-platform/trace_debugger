# Copyright 2018 Robert Balas
# Copyright and related rights are licensed under the Solderpad Hardware
# License, Version 0.51 (the "License"); you may not use this file except in
# compliance with the License.  You may obtain a copy of the License at
# http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
# or agreed to in writing, software, hardware and materials distributed under
# this License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
# CONDITIONS OF ANY KIND, either express or implied. See the License for the
# specific language governing permissions and limitations under the License.

# Author: Robert Balas (balasr@student.ethz.ch)
# Description: All in one

VERILATOR		= verilator

LINTER			= $(VERILATOR) --lint-only
MAKE			= make
CTAGS			= ctags

VVERSION                = "10.7b"

VLIB			= vlib-$(VVERSION)
VWORK			= work

VLOG			= vlog-$(VVERSION)
VLOG_FLAGS		= -64 -lint #-pedanticerrors
VLOG_LOG                = vloggy

VOPT			= vopt-$(VVERSION)
VOPT_FLAGS		= -64 -debugdb -fsmdebug +acc -check_synthesis #=mnprft -pedanticerrors

VSIM			= vsim-$(VVERSION)
VSIM_FLAGS		=
ALL_VSIM_FLAGS          = $(VSIM_FLAGS) -64
VSIM_SCRIPT             = tb/scripts/vsim.tcl

RTLSRC_TB_PKG		:= $(wildcard include/trdb_tb*.sv)
RTLSRC_TB_TOP		:= $(wildcard tb/*_top.sv)
RTLSRC_TB		:= $(wildcard tb/*.sv) \
				$(wildcard tb/dummy/*.sv)
RTLSRC_PKG		:= $(wildcard include/trdb_pkg.sv)
RTLSRC			:= $(wildcard rtl/*.sv) \
				$(wildcard rtl/tech_cells_generic/*.sv) \
				$(wildcard rtl/common_cells/*.sv)

RTLSRC_VLOG_TB_TOP	:= $(basename $(notdir $(RTLSRC_TB_TOP)))
RTLSRC_VOPT_TB_TOP	:= $(addsuffix _vopt, $(RTLSRC_VLOG_TB_TOP))

DPINAME			= trdb/dpi/autogen_trdb_sv.h
DPISRC			= $(RTLSRC_TB_PKG)
SV_LIB			= trdb/libtrdbsv
CFLAGS                  = -Wall -Wextra -Wno-missing-field-initializers \
				-Wno-unused-function -Wno-missing-braces \
				-O2 -g -march=native \
				-DENABLE_LOGGING -DNDEBUG


ALL_TESTS               = $(wildcard test/*.cvs)
ALL_TESTS_64            = $(wildcard test-64/*.cvs)
ALL_TEST_RESULTS        = $(addsuffix .test, $(basename $(ALL_TESTS)))
ALL_TEST_RESULTS_64     = $(addsuffix .test, $(basename $(ALL_TESTS_64)))

# rtl related targets
.PHONY: lint
lint: $(RTLSRC_PKG) $(RTLSRC) $(RTLSRC_TB_PKG) $(RTLSRC_TB)
	$(LINTER) -I. -Iinclude/ -Itb/ $(RTLSRC_PKG) $(RTLSRC) \
		$(RTLSRC_TB_PKG) $(RTLSRC_TB)


# driver related targets
.PHONY: driver-all
driver-all: check-env
	$(MAKE) -C driver/lowlevel all
	$(MAKE) -C driver/rt all

.PHONY: driver-run
driver-run: check-env
	$(MAKE) -C driver/lowlevel run

.PHONY: driver-clean
driver-clean: check-env
	$(MAKE) -C driver/lowlevel clean
	$(MAKE) -C driver/rt clean


# check if environment is setup properly
check-env:
	@if test "$(PULP_SDK_HOME)" = "" ; then \
		echo "PULP_SDK_HOME not set"; \
		exit 1; \
	fi

# make sure that we also compile the library in 64-bit mode if we request it
ifeq ($(findstring +define+TRDB_ARCH64,$(VLOG_FLAGS)),+define+TRDB_ARCH64)
CFLAGS+=-DTRDB_ARCH64
endif

# c model and decompression tools
.PHONY: c-all
c-all:
	$(MAKE) -C trdb all

.PHONY: c-lib
c-lib:
	$(MAKE) -C trdb lib

.PHONY: c-sv-lib
c-sv-lib:
	$(MAKE) -C trdb sv-lib CFLAGS="$(CFLAGS)"

.PHONY: c-run
c-run:
	$(MAKE) -C trdb run

.PHONY: c-clean
c-clean:
	$(MAKE) -C trdb clean

.PHONY: c-docs
c-docs:
	$(MAKE) -C trdb docs

# testbench compilation and optimization
.lib-rtl:
	$(VLIB) $(VWORK)
	touch .lib-rtl

.build-rtl: .lib-rtl $(RTLSRC_PKG) $(RTLSRC) $(RTLSRC_TB_PKG) $(RTLSRC_TB)
	$(VLOG) -work $(VWORK) $(VLOG_FLAGS) $(RTLSRC_PKG) $(RTLSRC) \
	$(RTLSRC_TB_PKG) $(RTLSRC_TB)
	touch .build-rtl

.opt-rtl: .build-rtl
	$(VOPT) -work $(VWORK) $(VOPT_FLAGS) $(RTLSRC_VLOG_TB_TOP) -o \
	$(RTLSRC_VOPT_TB_TOP)
	touch .opt-rtl

vlib: .lib-rtl
vlog: .build-rtl
tb-all: .opt-rtl

.PHONY: dpiheader
dpiheader: tb-all
	$(VLOG) -work $(VWORK) -l $(VLOG_LOG) -dpiheader $(DPINAME) $(DPISRC)

# run tb and exit
.PHONY: tb-run
tb-run: ALL_VSIM_FLAGS += -c
tb-run: tb-all c-sv-lib
	$(VSIM) -work $(VWORK) -sv_lib $(SV_LIB) $(ALL_VSIM_FLAGS) \
	$(RTLSRC_VOPT_TB_TOP) -do 'source $(VSIM_SCRIPT); exit -f'

# run tb and drop into interactive shell
.PHONY: tb-run-sh
tb-run-sh: ALL_VSIM_FLAGS += -c
tb-run-sh: tb-all c-sv-lib
	$(VSIM) -work $(VWORK) -sv_lib $(SV_LIB) $(ALL_VSIM_FLAGS) \
	$(RTLSRC_VOPT_TB_TOP) -do $(VSIM_SCRIPT)

# run tb with simulator gui
.PHONY: tb-run-gui
tb-run-gui: ALL_VSIM_FLAGS += -gui -debugdb
tb-run-gui: tb-all c-sv-lib
	$(VSIM) -work $(VWORK) -sv_lib $(SV_LIB) $(ALL_VSIM_FLAGS) \
	$(RTLSRC_VOPT_TB_TOP) -do $(VSIM_SCRIPT)


.PHONY: tb-clean
tb-clean:
	if [ -d $(VWORK) ]; then rm -r $(VWORK); fi
	rm -f transcript vsim.wlf .build-rtl .opt-rtl .lib-rtl

generate-tests:
	$(MAKE) -C trdb spike-traces-32
	cp trdb/riscv-traces-32/*.cvs test/

generate-tests-64:
	$(MAKE) -C trdb spike-traces-64
	cp trdb/riscv-traces-64/*.cvs test-64/

test: $(ALL_TEST_RESULTS)
	grep -B 2 -A 6 "Simulation Results" test/*.riscv.test \
		| tee test/summary.test
	@if [ ! -s test/summary.test ]; then                \
		echo "EMPTY RESULTS";                       \
		exit 1;                                     \
	elif grep -q "TEST FAIL" test/summary.test; then    \
		echo "ATLEAST ONE FAILURE";                 \
		exit 1;                                     \
	else                                                \
		echo "ALL TESTS PASSED";                    \
		exit 0;                                     \
	fi


test-64: VLOG_FLAGS+=+define+TRDB_ARCH64
test-64: CFLAGS+=-DTRDB_ARCH64
test-64: $(ALL_TEST_RESULTS_64)
	grep -B 2 -A 6 "Simulation Results" test-64/*.riscv.test \
		| tee test-64/summary.test
	@if [ ! -s test-64/summary.test ]; then             \
		echo "EMPTY RESULTS";                       \
		exit 1;                                     \
	elif grep -q "TEST FAIL" test-64/summary.test; then \
		echo "ATLEAST ONE FAILURE";                 \
		exit 1;                                     \
	else                                                \
		echo "ALL TESTS PASSED";                    \
		exit 0;                                     \
	fi


# we use separate wlf otherwise vsim complains
%.test: %.cvs tb-all c-sv-lib
	$(VSIM) -work $(VWORK) -sv_lib $(SV_LIB) $(ALL_VSIM_FLAGS) -c \
		+cvs +implicitret +testname=$< -wlf $@.wlf \
		$(RTLSRC_VOPT_TB_TOP) -do 'source $(VSIM_SCRIPT); exit -f' > $@

.PHONY: test-clean
test-clean:
	rm -rf test/*.test test/*.wlf test/*.dbg
	rm -rf test-64/*.test test-64/*.wlf test-64/*.dbg

# general targets
.PHONY: TAGS
TAGS: check-env
	echo "Generating TAGS for driver..."
	echo "Generating TAGS for RTL..."
	$(CTAGS) -R -e -h=".c.h.sv.svh" --tag-relative=always \
		--exclude=$(PULP_SDK_HOME) --exclude=trdb/ \
		. $(PULP_PROJECT_HOME) \
		rtl/ tb/

.PHONY: docs
docs: c-docs

.PHONY: all
all: driver-all tb-all c-all

.PHONY: clean
clean: tb-clean c-clean test-clean

.PHONY: distclean
distclean: clean
	rm -f TAGS
