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

VERILATOR ?= verilator

LINTER = $(VERILATOR) --lint-only
MAKE = make
CTAGS = ctags

RTLSRC := $(wildcard rtl/*.sv)

# rtl related targets
.PHONY: lint
lint: $(rtlsrcfiles)
	$(LINTER) $(RTLSRC)

# driver related targets
.PHONY: driver-all
driver-all:
	$(MAKE) -C driver/lowlevel all
	$(MAKE) -C driver/rt all

.PHONY: driver-run
driver-run:
	$(MAKE) -C driver/lowlevel run

.PHONY: driver-clean
driver-clean:
	$(MAKE) -C driver/lowlevel clean
	$(MAKE) -C driver/rt clean

# check if environment is setup properly
check-env:
ifndef PULP_SDK_HOME
  $(error PULP_SDK_HOME is undefined)
endif

# general targets
TAGS: check-env
	$(CTAGS) -R -e -h=".c.h" --exclude=$(PULP_SDK_HOME) . $(PULP_PROJECT_HOME)

.PHONY: all
all: driver-all

.PHONY: clean
clean: driver-clean

.PHONY: distclean
distclean: clean
	rm -f TAGS
