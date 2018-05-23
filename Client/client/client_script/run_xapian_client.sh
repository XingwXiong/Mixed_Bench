#!/bin/bash
source ${CLIENT_DIR}/client/client_script/configs.sh

QPS=100

NUM=${CUR_NUM} \
APP_NAME="xapian" \
TBENCH_SERVER=${SERVER_IP} \
TBENCH_SERVER_PORT=10000 \
TBENCH_RANDSEED=${RANDOM} TBENCH_QPS=${QPS} TBENCH_MINSLEEPNS=100000 \
    TBENCH_TERMS_FILE=${DATA_ROOT}/xapian/terms.in \
    chrt -r 99 ${CLIENT_DIR}/client/client_process/xapian_networked_client 

