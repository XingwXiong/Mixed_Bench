#!/bin/bash
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
source ${DIR}/../configs.sh 
#source ../configs.sh

THREADS=1
REQUESTS=10000

REQS=100000 # Set this very high; the harness controls maxreqs

TBENCH_SERVER_PORT=10001 \
TBENCH_WARMUPREQS=5000 TBENCH_MAXREQS=${REQUESTS} ${TAIL_ROOT}/img-dnn/img-dnn_server_networked \
    -r ${THREADS} -f ${DATA_ROOT}/img-dnn/models/model.xml -n ${REQS} &

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
