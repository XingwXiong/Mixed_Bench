#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
source ${DIR}/../configs.sh 

if [[ -z "${SERVER_IP}" ]]; then SERVER_IP=127.0.0.1; fi

# Setup commands
mkdir -p results

# Run specjbb

QPS=2000

export LD_LIBRARY_PATH=${TBENCH_PATH}:${LD_LIBRARY_PATH}

export CLASSPATH=./build/dist/jbb.jar:./build/dist/check.jar:${TBENCH_PATH}/tbench.jar

export PATH=${JDK_PATH}/bin:${PATH}

APP_NAME=specjbb \
TBENCH_SERVER=${SERVER_IP} TBENCH_SERVER_PORT=10006 \
TBENCH_QPS=${QPS} TBENCH_MINSLEEPNS=10000 \
TBENCH_RANDSEED=${RANDOM} \
./client &

echo $! > client.pid
echo 'wait-----------------------------ttttttttttttttttttttttttttttttttttttttt'
wait $(cat client.pid)
echo 'ok-----------------------------------next'
# Teardown
rm -rf libtbench_jni.so gc.log results


