#!/usr/bin/env bash
set -euo pipefail
../trdb --full-address --binary-format pulp --compress -o my_packets ../data/trdb_stimuli
../trdb --full-address --binary-format pulp --dump my_packets
../trdb --full-address --binary-format pulp --bfd ../data/interrupt --extract my_packets
../trdb --full-address -dSsl --binary-format pulp --bfd ../data/interrupt --extract my_packets
rm my_packets
