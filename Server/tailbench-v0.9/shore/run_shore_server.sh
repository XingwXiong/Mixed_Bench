#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
source ${DIR}/../configs.sh

THREADS=1
REQS=1000000
WARMUP=10
MAXREQS=100

kill -9 $(cat server.pid)
rm -f log scratch cmdfile db-tpcc-1 diskrw shore.conf info server.pid client.pid

# Setup
TMP=$(mktemp -d --tmpdir=${SCRATCH_DIR})
echo $TMP
ln -sf $TMP scratch
mkdir scratch/log && ln -sf scratch/log log
mkdir scratch/diskrw && ln -sf scratch/diskrw diskrw

cp ${DATA_ROOT}/shore/db-tpcc-1 scratch/ && \
    ln -sf scratch/db-tpcc-1 db-tpcc-1
chmod 644 scratch/db-tpcc-1

cp shore-kits/run-templates/cmdfile.template cmdfile
sed -i -e "s#@NTHREADS#$THREADS#g" cmdfile
sed -i -e "s#@REQS#$REQS#g" cmdfile

cp shore-kits/run-templates/shore.conf.template \
    shore.conf
sed -i -e "s#@NTHREADS#$THREADS#g" shore.conf

# Launch Server

TBENCH_SERVER_PORT=10003 \
TBENCH_MAXREQS=${MAXREQS} TBENCH_WARMUPREQS=${WARMUP} \
shore-kits/shore_kits_server_networked -i cmdfile &
echo $! > server.pid

sleep 5

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

