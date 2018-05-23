#/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
source ${CLIENT_DIR}/client/client_script/configs.sh

THREADS=1
QPS=1

AUDIO_SAMPLES="${CLIENT_DIR}/client/client_process/audio_samples"
NUM=${CUR_NUM} \
APP_NAME="sphinx" \
TBENCH_SERVER=${SERVER_IP} \
TBENCH_SERVER_PORT=10004 \
TBENCH_QPS=${QPS} TBENCH_MINSLEEPNS=10000 TBENCH_AN4_CORPUS=${DATA_ROOT}/sphinx \
    TBENCH_AUDIO_SAMPLES=${AUDIO_SAMPLES} ${CLIENT_DIR}/client/client_process/decoder_client_networked 
echo $! > client.pid

