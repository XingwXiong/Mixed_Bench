#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
source ${DIR}/../configs.sh

if [[ -z "${NTHREADS}" ]]; then NTHREADS=1; fi
if [[ -z "${SERVER_IP}" ]]; then SERVER_IP=127.0.0.1; fi

REQS=1000000

# Setup
#TMP=$(mktemp -d --tmpdir=${SCRATCH_DIR})
#ln -sf $TMP scratch
#mkdir -p scratch/log && ln -sf scratch/log log
#mkdir -p scratch/diskrw && ln -sf scratch/diskrw diskrw

#cp ${DATA_ROOT}/shore/db-tpcc-1 scratch/ && \
#    ln -sf scratch/db-tpcc-1 db-tpcc-1
#chmod 644 scratch/db-tpcc-1

#cp shore-kits/run-templates/cmdfile.template cmdfile
#sed -i -e "s#@NTHREADS#$THREADS#g" cmdfile
#sed -i -e "s#@REQS#$REQS#g" cmdfile

#cp shore-kits/run-templates/shore.conf.template \
#    shore.conf
#sed -i -e "s#@NTHREADS#$THREADS#g" shore.conf


# Launch Client
APP_NAME=shore \
TBENCH_SERVER=${SERVER_IP} \
TBENCH_SERVER_PORT=10003 \
TBENCH_RANDSEED=${RANDOM} \
TBENCH_QPS=${QPS} TBENCH_MINSLEEPNS=100000 \
     shore-kits/shore_kits_client_networked -i cmdfile &
echo $! > client.pid

wait $(cat client.pid)

# Cleanup
rm -f log scratch cmdfile db-tpcc-1 diskrw shore.conf info server.pid \
    client.pid

