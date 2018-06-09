#!/bin/bash
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
source ${DIR}/../configs.sh
QPS=50

if [[ -z "${NTHREADS}" ]]; then NTHREADS=1; fi
if [[ -z "${SERVER_IP}" ]]; then SERVER_IP=127.0.0.1; fi 

echo $SERVER_IP

APP_NAME=xapian \
TBENCH_SERVER_PORT=10000 \
TBENCH_SERVER=${SERVER_IP} TBENCH_RANDSEED=${RANDOM} TBENCH_QPS=${QPS} TBENCH_MINSLEEPNS=100000 \
    TBENCH_TERMS_FILE=${DATA_ROOT}/xapian/terms.in \
    ${DIR}/xapian_networked_client
echo $! > client.pid

