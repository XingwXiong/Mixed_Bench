#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
source ${DIR}/../configs.sh

THREADS=1
WARMUPREQS=500
MAXREQS=500

BINDIR=${DIR}/bin

# Setup
cp moses.ini.template moses.ini
sed -i -e "s#@DATA_ROOT#$DATA_ROOT#g" moses.ini

kill -9 $(cat server.pid)
rm -f server.pid

# Launch Server
TBENCH_SERVER_PORT=10007 \
TBENCH_MAXREQS=${MAXREQS} TBENCH_WARMUPREQS=${WARMUPREQS} \
    ${BINDIR}/moses_server_networked -config ./moses.ini \
    -input-file ${DATA_ROOT}/moses/testTerms \
    -threads ${THREADS} -num-tasks 1000000 -verbose 0 &

echo $! > server.pid

while [ 1 ]
do
    netstat -nlp | grep 10007
    if [ $? == 0 ]
      then
          echo start ok
          break
      else
          echo -e '#\c'
    fi
done

