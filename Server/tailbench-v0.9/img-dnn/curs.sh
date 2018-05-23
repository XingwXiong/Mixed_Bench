#!/bin/bash
source ../configs.sh

THREADS=1
REQS=100000 # Set this very high; the harness controls maxreqs

TBENCH_SERVER_PORT=10001 \
TBENCH_WARMUPREQS=5000 TBENCH_MAXREQS=10000 ./img-dnn_server_networked \
    -r ${THREADS} -f ${DATA_ROOT}/img-dnn/models/model.xml -n ${REQS} &

while [ 1 ]
do
  nc -z -w 10 172.18.11.114 10000
  if [ $! == 0 ]
  then
        echo yes
        break
  fi
done
