#!/usr/bin/env bash
cd "$TRDB_HOME/trdb/test"
./test_trdb_diff.sh > diff_out
./test_trdb.sh > normal_out
diff <(tail diff_out) <(tail trdb_diff_expected)
diff <(tail normal_out) <(tail trdb_normal_expected)
