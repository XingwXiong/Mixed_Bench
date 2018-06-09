#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
source ${DIR}/../configs.sh

if [[ -z "${SERVER_IP}" ]]; then SERVER_IP=127.0.0.1; fi

THREADS=1
QPS=100

BINDIR=${DIR}/bin

# Setup
cp moses.ini.template moses.ini
sed -i -e "s#@DATA_ROOT#$DATA_ROOT#g" moses.ini

kill -9 $(cat client.pid)
rm -f client.pid

# Launch Client
APP_NAME=moses \
TBENCH_SERVER=${SERVER_IP} TBENCH_SERVER_PORT=10007 \
TBENCH_QPS=${QPS} TBENCH_MINSLEEPNS=10000 \
    ${BINDIR}/moses_client_networked &
echo $! > client.pid

wait $(cat client.pid)
