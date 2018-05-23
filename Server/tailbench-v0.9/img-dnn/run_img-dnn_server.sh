#!/bin/bash
source ${TAIL_DIR}/configs.sh

THREADS=1
REQS=100000 # Set this very high; the harness controls maxreqs
TBENCH_SERVER_PORT=10001 \
TBENCH_WARMUPREQS=5000 TBENCH_MAXREQS=10000 ${TAIL_DIR}/img-dnn/img-dnn_server_networked \
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
