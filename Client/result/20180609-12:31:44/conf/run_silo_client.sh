#!/bin/bash
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
source ${DIR}/../configs.sh

if [[ -z "${SERVER_IP}" ]]; then SERVER_IP=127.0.0.1; fi

# ops-per-worker is set to a very large value, so that TBENCH_MAXREQS controls how
# many ops are performed
NUM_WAREHOUSES=1
NUM_THREADS=1

QPS=1000

kill -9 $(cat client.pid)
rm -f client.pid

APP_NAME=silo \
TBENCH_SERVER=${SERVER_IP} TBENCH_SERVER_PORT=10005 \
TBENCH_QPS=${QPS} TBENCH_MINSLEEPNS=10000 \
${TAIL_ROOT}/silo/out-perf.masstree/benchmarks/dbtest_client_networked &

echo $! > client.pid

wait $(cat client.pid)

