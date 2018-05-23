#!/bin/bash
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
source /home/fanfanda/tailbench-v0.9/configs.sh
NSERVERS=1
QPS=500
WARMUPREQS=1000
REQUESTS=3000


TBENCH_SERVER_PORT=10000 \
TBENCH_SERVER=hw114 TBENCH_RANDSEED=${RANDOM} TBENCH_QPS=${QPS} TBENCH_MINSLEEPNS=100000 \
    TBENCH_TERMS_FILE=${DATA_ROOT}/xapian/terms.in \
    chrt -r 99 ${DIR}/xapian_networked_client &
echo $! > client.pid

