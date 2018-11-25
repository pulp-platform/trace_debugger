#!/usr/bin/env bash
set -euo pipefail
../trdb --binary-format pulp --compress -o my_packets ../data/trdb_stimuli
../trdb --binary-format pulp --dump my_packets
../trdb --binary-format pulp --bfd ../data/interrupt --extract my_packets
../trdb -dSsl --binary-format pulp --bfd ../data/interrupt --extract my_packets
rm my_packets
