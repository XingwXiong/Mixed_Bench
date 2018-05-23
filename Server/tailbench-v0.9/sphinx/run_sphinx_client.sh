#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
source /home/fanfanda/tailbench-v0.9/configs.sh

THREADS=1
AUDIO_SAMPLES='audio_samples'

TBENCH_SERVER_PORT=10004 \
TBENCH_QPS=1 TBENCH_MINSLEEPNS=10000 TBENCH_AN4_CORPUS=${DATA_ROOT}/sphinx \
    TBENCH_AUDIO_SAMPLES=${AUDIO_SAMPLES} ./decoder_client_networked &
echo $! > client.pid

