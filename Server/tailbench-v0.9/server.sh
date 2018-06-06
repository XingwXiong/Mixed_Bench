#!/bin/bash

source configs.sh

cd ${TAIL_ROOT}/xapian/ && ${TAIL_ROOT}/xapian/run_xapian_server.sh &

cd ${TAIL_ROOT}/img-dnn && ${TAIL_ROOT}/img-dnn/run_img-dnn_server.sh &

cd  ${TAIL_ROOT}/masstree/ && ${TAIL_ROOT}/masstree/run_masstree_server.sh &

cd  ${TAIL_ROOT}/shore && ${TAIL_ROOT}/shore/run_shore_server.sh &

cd ${TAIL_ROOT}/sphinx && ${TAIL_ROOT}/sphinx/run_sphinx_server.sh &

cd ${TAIL_ROOT}/silo && ${TAIL_ROOT}/silo/run_silo_server.sh &

cd ${TAIL_ROOT}/moses && ${TAIL_ROOT}/moses/run_moses_server.sh &

cd ${TAIL_ROOT}/specjbb && ${TAIL_ROOT}/specjbb/run_specjbb_server.sh &


sleep 10

while [ 1 ]
do
    netstat -nlp | grep 1000
    if [ $? == 0 ]
      then
          echo start ok
          break
      else
          echo -e '#\c'
    fi
done
