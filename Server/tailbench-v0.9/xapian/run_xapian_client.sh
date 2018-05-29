#!/bin/bash
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
source ../configs.sh
NSERVERS=1
QPS=100
WARMUPREQS=1000
REQUESTS=3000

mkdir -p result

REDIS_IP=172.17.0.1 REDIS_PORT=6379 REDIS_AUTH=xingwxingw \
APP_NAME=xapian RESULT_DIR=${PWD}/result \
TBENCH_SERVER_PORT=10000 \
TBENCH_SERVER=127.0.0.1 TBENCH_RANDSEED=${RANDOM} TBENCH_QPS=${QPS} TBENCH_MINSLEEPNS=100000 \
    TBENCH_TERMS_FILE=${DATA_ROOT}/xapian/terms.in \
    chrt -r 99 ${DIR}/xapian_networked_client
echo $! > client.pid

