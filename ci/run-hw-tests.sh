#!/usr/bin/env bash
set -e

ROOT=$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)

make -C ${ROOT} c-clean c-sv-lib tb-all tb-run
