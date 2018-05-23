#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
source /home/fanfanda/tailbench-v0.9/configs.sh

THREADS=1
REQS=100000 # Set this very high; the harness controls maxreqs

TBENCH_SERVER=hw114 \
TBENCH_SERVER_PORT=10001 \
TBENCH_RANDSEED=${RANDOM} \
 TBENCH_QPS=500 TBENCH_MNIST_DIR=${DATA_ROOT}/img-dnn/mnist ${TAIL_DIR}/img-dnn/img-dnn_client_networked &

echo $! > client.pid

