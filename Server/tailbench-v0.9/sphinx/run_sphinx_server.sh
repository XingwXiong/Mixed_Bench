#!/bin/bash

#DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
source ${TAIL_DIR}/configs.sh


THREADS=1
WARMUPREQS=10
MAXREQS=10

AUDIO_SAMPLES='audio_samples'

LD_LIBRARY_PATH=${TAIL_DIR}/sphinx/sphinx-install/lib:${LD_LIBRARY_PATH} \
TBENCH_SERVER_PORT=10004 TBENCH_MAXREQS=${MAXREQS} TBENCH_WARMUPREQS=${WARMUPREQS} \
    ${TAIL_DIR}/sphinx/decoder_server_networked -t $THREADS &

while [ 1 ]
do
    netstat -nlp | grep 10004
    if [ $? == 0 ]
      then
          echo start ok
          break
      else
          echo -e '#\c'
    fi
done
