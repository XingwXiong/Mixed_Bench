#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
source ${DIR}/../configs.sh

if [[ -z "${NTHREADS}" ]]; then NTHREADS=1; fi

MAXREQS=100
WARMUPREQS=10

SERVER_IP=0.0.0.0

TBENCH_SERVER=${SERVER_IP} \
TBENCH_SERVER_PORT=10002 \
TBENCH_RANDSEED=${RANDOM} \
TBENCH_MAXREQS=${MAXREQS} TBENCH_WARMUPREQS=${WARMUPREQS} \
    ${TAIL_ROOT}/masstree/mttest_server_networked -j${NTHREADS} mycsba masstree &

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
