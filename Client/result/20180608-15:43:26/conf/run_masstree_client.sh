#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
source ${DIR}/../configs.sh

if [[ -z "${NTHREADS}" ]]; then NTHREADS=1; fi
if [[ -z "${SERVER_IP}" ]]; then SERVER_IP=127.0.0.1; fi

QPS=1

echo ${SERVER_IP}

APP_NAME=masstree \
TBENCH_SERVER_PORT=10002 \
TBENCH_SERVER=${SERVER_IP} \
TBENCH_RANDSEED=${RANDOM} \
TBENCH_QPS=${QPS} TBENCH_MINSLEEPNS=10000  \
${TAIL_ROOT}/masstree/mttest_client_networked &

#echo $! > client.pid

#wait $(cat client.pid)

#rm -f *.pid
