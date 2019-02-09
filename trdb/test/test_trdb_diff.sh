#!/usr/bin/env bash
set -euo pipefail
../main/trdb --binary-format pulp --compress -o my_packets ../data/trdb_stimuli
../main/trdb --binary-format pulp --dump my_packets
../main/trdb --binary-format pulp --bfd ../data/interrupt --extract my_packets
../main/trdb -dSsl --binary-format pulp --bfd ../data/interrupt --extract my_packets
rm my_packets
