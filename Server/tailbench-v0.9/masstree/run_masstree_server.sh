#!/bin/bash

if [[ -z "${NTHREADS}" ]]; then NTHREADS=1; fi
SERVER_IP="172.18.11.114"
MAXREQS=30
WARMUPREQS=10
TBENCH_SERVER=${SERVER_IP} \
TBENCH_SERVER_PORT=10002 \
TBENCH_RANDSEED=${RANDOM} \
TBENCH_MAXREQS=${MAXREQS} TBENCH_WARMUPREQS=${WARMUPREQS} chrt -r 99 \
    ${TAIL_DIR}/masstree/mttest_server_networked -j${NTHREADS} mycsba masstree &

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
