#!/usr/bin/env bash
git clone "$SPIKE_TEST_TRACES_URL" spike-traces

mkdir "$TRDB_HOME/trdb/data/cvs"

cp "$TRDB_HOME/spike-traces/"* "$TRDB_HOME/trdb/data/cvs/"
