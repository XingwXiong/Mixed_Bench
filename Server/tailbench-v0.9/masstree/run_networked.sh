#!/bin/bash

if [[ -z "${NTHREADS}" ]]; then NTHREADS=1; fi

QPS=10
MAXREQS=300
WARMUPREQS=140

APP_NAME=masstree \
TBENCH_RANDSEED=${RANDOM} \
TBENCH_MAXREQS=${MAXREQS} TBENCH_WARMUPREQS=${WARMUPREQS}  \
    ./mttest_server_networked -j${NTHREADS} mycsba masstree &
echo $! > server.pid

sleep 5 # Allow server to come up

APP_NAME=masstree \
TBENCH_RANDSEED=${RANDOM} \
TBENCH_QPS=${QPS} TBENCH_MINSLEEPNS=10000  ./mttest_client_networked &
echo $! > client.pid

wait $(cat client.pid)

pkill -f mttest_server_networked mttest_client_networked
rm -f *.pid
