#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
source ${DIR}/../configs.sh

if [[ -z "${NTHREADS}" ]]; then NTHREADS=1; fi
if [[ -z "${SERVER_IP}" ]]; then SERVER_IP=127.0.0.1; fi

QPS=500


APP_NAME=img-dnn \
TBENCH_SERVER=${SERVER_IP} \
TBENCH_SERVER_PORT=10001 \
TBENCH_RANDSEED=${RANDOM} \
TBENCH_QPS=${QPS} TBENCH_MNIST_DIR=${DATA_ROOT}/img-dnn/mnist \
${TAIL_ROOT}/img-dnn/img-dnn_client_networked &

echo $! > client.pid

