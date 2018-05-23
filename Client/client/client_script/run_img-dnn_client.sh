#!/bin/bash

source ${CLIENT_DIR}/client/client_script/configs.sh
QPS=300
THREADS=1
REQS=100000 # Set this very high; the harness controls maxreqs
NUM=${CUR_NUM} \
APP_NAME="img-dnn" \
TBENCH_SERVER=${SERVER_IP} \
TBENCH_SERVER_PORT=10001 \
TBENCH_RANDSEED=${RANDOM} \
 TBENCH_QPS=${QPS} TBENCH_MNIST_DIR=${DATA_ROOT}/img-dnn/mnist ${CLIENT_DIR}/client/client_process/img-dnn_client_networked 


