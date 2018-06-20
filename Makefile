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

verilator ?= verilator

LINTER = $(verilator) --lint-only
MAKE = make

srcfiles := $(wildcard rtl/*.sv)

.Phony: lint verilate driver driver-run driver-clean

lint: $(srcfiles)
	$(LINTER) $(srcfiles)

driver-all:
	$(MAKE) -C driver/ all

driver-run:
	$(MAKE) -C driver/ run

driver-clean:
	$(MAKE) -C driver/ clean
