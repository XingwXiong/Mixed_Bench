#!/bin/bash
source ../configs.sh

NSERVERS=1
WARMUPREQS=1000
REQUESTS=3000

APP_NAME=xapian \
TBENCH_SERVER_PORT=10000 \
TBENCH_RANDSEED=${RANDOM} TBENCH_MAXREQS=${REQUESTS} TBENCH_WARMUPREQS=${WARMUPREQS} \
    ${TAIL_ROOT}/xapian/xapian_networked_server -n ${NSERVERS} -d ${DATA_ROOT}/xapian/wiki \
    -r 1000000000 &

while [ 1 ]
do
    netstat -nlp | grep 10000
    if [ $? == 0 ]
      then
          echo start ok
          break
      else
          echo -e '#\c'
    fi
done
