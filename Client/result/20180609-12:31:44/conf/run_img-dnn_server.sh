#!/bin/bash
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
source ${DIR}/../configs.sh 
#source ../configs.sh

THREADS=1
REQUESTS=8000

REQS=100000 # Set this very high; the harness controls maxreqs

TBENCH_SERVER_PORT=10001 \
TBENCH_WARMUPREQS=1000 TBENCH_MAXREQS=${REQUESTS} nohup  ${TAIL_ROOT}/img-dnn/img-dnn_server_networked \
    -r ${THREADS} -f ${DATA_ROOT}/img-dnn/models/model.xml -n ${REQS} >${DIR}/server.log 2>&1 &

while [ 1 ]
do
    netstat -nlp | grep 10001
    if [ $? == 0 ]
      then
          echo start ok
          break
      else
          echo -e '#\c'
    fi
done
