#!/bin/bash


export TAIL_DIR=/home/fanfanda/tailbench-v0.9

${TAIL_DIR}/xapian/run_xapian_server.sh &

${TAIL_DIR}/img-dnn/run_img-dnn_server.sh &

${TAIL_DIR}/masstree/run_masstree_server.sh &

${TAIL_DIR}/shore/run_shore_server.sh &

${TAIL_DIR}/sphinx/run_sphinx_server.sh &
