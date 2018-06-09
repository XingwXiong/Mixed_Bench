#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
source ${DIR}/../configs.sh

if [[ -z "${NTHREADS}" ]]; then NTHREADS=1; fi

MAXREQS=1000
WARMUPREQS=100

TBENCH_SERVER_PORT=10002 \
TBENCH_RANDSEED=${RANDOM} \
TBENCH_MAXREQS=${MAXREQS} TBENCH_WARMUPREQS=${WARMUPREQS} \
    nohup ${TAIL_ROOT}/masstree/mttest_server_networked -j${NTHREADS} mycsba masstree >server.log 2>&1 &

while [ 1 ]
do
    netstat -nlp | grep 10002
    if [ $? == 0 ]
      then
          echo start ok
          break
      else
          echo -e '#\c'
    fi
done
