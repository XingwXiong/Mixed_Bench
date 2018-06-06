#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
source ../configs.sh

# Setup commands
mkdir -p results

# Run specjbb
TBENCH_PATH=../harness

export LD_LIBRARY_PATH=${TBENCH_PATH}:${LD_LIBRARY_PATH}

export CLASSPATH=./build/dist/jbb.jar:./build/dist/check.jar:${TBENCH_PATH}/tbench.jar

export PATH=${JDK_PATH}/bin:${PATH}

export TBENCH_QPS=2000 
export TBENCH_MINSLEEPNS=10000

APP_NAME=specjbb
TBENCH_SERVER_PORT=10006 \
./client &
echo $! > client.pid
echo 'wait-----------------------------ttttttttttttttttttttttttttttttttttttttt'
#wait $(cat client.pid)
echo 'ok-----------------------------------next'

