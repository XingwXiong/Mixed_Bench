#!/bin/bash

source configs.sh

if [[ -z "${SERVER_IP}" ]]; then SERVER_IP=127.0.0.1; fi

cd ${TAIL_ROOT}/xapian/ && ${TAIL_ROOT}/xapian/run_xapian_client.sh &

cd ${TAIL_ROOT}/img-dnn && ${TAIL_ROOT}/img-dnn/run_img-dnn_client.sh &

cd  ${TAIL_ROOT}/masstree/ && ${TAIL_ROOT}/masstree/run_masstree_client.sh &

cd  ${TAIL_ROOT}/shore && ${TAIL_ROOT}/shore/run_shore_client.sh &

cd ${TAIL_ROOT}/sphinx && ${TAIL_ROOT}/sphinx/run_sphinx_client.sh &

cd ${TAIL_ROOT}/silo && ${TAIL_ROOT}/silo/run_silo_client.sh &

cd ${TAIL_ROOT}/moses && ${TAIL_ROOT}/moses/run_moses_client.sh &

cd ${TAIL_ROOT}/specjbb && ${TAIL_ROOT}/specjbb/run_specjbb_client.sh &
