#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
source ${DIR}/../configs.sh

if [[ -z "${SERVER_IP}" ]]; then SERVER_IP=127.0.0.1; fi

THREADS=1
QPS=1
AUDIO_SAMPLES='audio_samples'

kill -9 $(cat client.pid)
rm -f client.pid

APP_NAME=sphinx \
TBENCH_SERVER=${SERVER_IP} \
TBENCH_SERVER_PORT=10004 \
TBENCH_QPS=${QPS} TBENCH_MINSLEEPNS=10000 TBENCH_AN4_CORPUS=${DATA_ROOT}/sphinx \
    TBENCH_AUDIO_SAMPLES=${AUDIO_SAMPLES} ${TAIL_ROOT}/sphinx/decoder_client_networked &
echo $! > client.pid

