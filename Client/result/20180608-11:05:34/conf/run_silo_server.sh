#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
source ${DIR}/../configs.sh

# ops-per-worker is set to a very large value, so that TBENCH_MAXREQS controls how
# many ops are performed
NUM_WAREHOUSES=1
NUM_THREADS=1

MAXREQS=10000
WARMUPREQS=5000

kill -9 $(cat server.pid)
rm -f server.pid

TBENCH_SERVER_PORT=10005 \
TBENCH_MAXREQS=${MAXREQS} TBENCH_WARMUPREQS=${WARMUPREQS} \
    ${TAIL_ROOT}/silo/out-perf.masstree/benchmarks/dbtest_server_networked --verbose --bench \
    tpcc --num-threads ${NUM_THREADS} --scale-factor ${NUM_WAREHOUSES} \
    --retry-aborted-transactions --ops-per-worker 10000000 &

echo $! > server.pid

while [ 1 ]
do
    netstat -nlp | grep 10005
    if [ $? == 0 ]
      then
          echo start ok
          break
      else
          echo -e '#\c'
    fi
done
