#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
source ${CLIENT_DIR}/client/client_script/configs.sh

THREADS=1
QPS=100
WARMUPREQS=400
MAXREQS=400

DUMMYREQS=1000 # set this really high so MAXREQS controls execution

# Launch Client
NUM=${CUR_NUM} \
APP_NAME="shore" \
TBENCH_SERVER=${SERVER_IP} \
TBENCH_SERVER_PORT=10003 \
TBENCH_QPS=${QPS} TBENCH_MINSLEEPNS=10000 \
     chrt -r 99 ${CLIENT_DIR}/client/client_process/shore_kits_client_networked -i cmdfile 

