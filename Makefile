# Copyright 2018 ETH Zurich and University of Bologna.
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

VLIB			= vlib
VWORK			= work

VLOG			= vlog
VLOG_FLAGS		= -pedanticerrors

VOPT			= vopt
VOPT_FLAGS		= -debugdb -fsmdebug -pedanticerrors +acc #=mnprft

VSIM			= vsim
VSIM_FLAGS		= -c
VSIM_DEBUG_FLAGS	= -gui

RTLSRC_TB_PKG		:= $(wildcard include/trdb_tb*.sv)
RTLSRC_TB_TOP		:= $(wildcard tb/*_top.sv)
RTLSRC_TB		:= $(wildcard tb/*.sv)
RTLSRC_PKG		:= $(wildcard include/trdb_pkg.sv)
RTLSRC			:= $(wildcard rtl/*.sv) \
				$(wildcard rtl/tech_cells_generic/*.sv) \
				$(wildcard rtl/common_cells/*.sv)

RTLSRC_VLOG_TB_TOP	:= $(basename $(notdir $(RTLSRC_TB_TOP)))
RTLSRC_VOPT_TB_TOP	:= $(addsuffix _vopt, $(RTLSRC_VLOG_TB_TOP))


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


# c model and decompression tools
.PHONY: c-all
c-all:
	$(MAKE) -C trdb all

.PHONY: c-run
c-run:
	$(MAKE) -C trdb run

.PHONY: c-clean
c-clean:
	$(MAKE) -C trdb clean


# testbench compilation and optimization
vlib:
	$(VLIB) $(VWORK)

vlog: vlib $(RTLSRC_TB)
	$(VLOG) -work $(VWORK) $(VLOG_FLAGS) $(RTLSRC_PKG) $(RTLSRC) \
	$(RTLSRC_TB_PKG) $(RTLSRC_TB)

.PHONY: tb-all
tb-all: vlog
	$(VOPT) -work $(VWORK) $(VOPT_FLAGS) $(RTLSRC_VLOG_TB_TOP) -o \
	$(RTLSRC_VOPT_TB_TOP)

.PHONY: tb-run
tb-run:
	$(VSIM) -work $(VWORK) $(VSIM_FLAGS) $(RTLSRC_VOPT_TB_TOP)

.PHONY: tb-run-gui
tb-run-gui: VSIM_FLAGS = $(VSIM_DEBUG_FLAGS)
tb-run-gui: tb-run

.PHONY: tb-clean
tb-clean:
	if [ -d $(VWORK) ]; then rm -r $(VWORK); fi
	rm -f transcript
	rm -f vsim.wlf


# general targets
TAGS: check-env
	$(CTAGS) -R -e -h=".c.h" --exclude=$(PULP_SDK_HOME) .\
	$(PULP_PROJECT_HOME)

.PHONY: all
all: driver-all tb-all c-all

.PHONY: clean
clean: driver-clean tb-clean c-clean

.PHONY: distclean
distclean: clean
	rm -f TAGS
