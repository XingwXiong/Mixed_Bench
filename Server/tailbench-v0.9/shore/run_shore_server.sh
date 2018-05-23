#!/bin/bash

#DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
source ${TAIL_DIR}/configs.sh
THREADS=1
QPS=400
WARMUPREQS=100
MAXREQS=200

DUMMYREQS=1000 # set this really high so MAXREQS controls execution

# Point this to an appropriate location on your system
SCRATCH_DIR=/home/fanfanda/tailbench-scratchData

# Setup
TMP=$(mktemp -d --tmpdir=${SCRATCH_DIR})
ln -s $TMP scratch
mkdir scratch/log && ln -s scratch/log log
mkdir scratch/diskrw && ln -s scratch/diskrw diskrw

cp ${DATA_ROOT}/shore/db-tpcc-1 scratch/ && \
    ln -s scratch/db-tpcc-1 db-tpcc-1
chmod 644 scratch/db-tpcc-1

cp ${TAIL_DIR}/shore/shore-kits/run-templates/cmdfile.template cmdfile
sed -i -e "s#@NTHREADS#$THREADS#g" cmdfile
sed -i -e "s#@REQS#$DUMMYREQS#g" cmdfile

cp ${TAIL_DIR}/shore/shore-kits/run-templates/shore.conf.template \
    shore.conf
sed -i -e "s#@NTHREADS#$THREADS#g" shore.conf
# Launch Server
TBENCH_SERVER_PORT=10003 \
TBENCH_RANDSEED=${RANDOM} \
TBENCH_MAXREQS=${MAXREQS} TBENCH_WARMUPREQS=${WARMUPREQS} \
    chrt -r 99 ${TAIL_DIR}/shore/shore-kits/shore_kits_server_networked -i cmdfile &

while [ 1 ]
do
    netstat -nlp | grep 10003
    if [ $? == 0 ]
      then
          echo start ok
          break
      else
          echo -e '#\c'
    fi
done
