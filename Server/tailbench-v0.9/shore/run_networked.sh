#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
source ${DIR}/../configs.sh

THREADS=1
QPS=400
WARMUPREQS=10
MAXREQS=20

DUMMYREQS=100 # set this really high so MAXREQS controls execution



# Launch Client
TBENCH_QPS=${QPS} TBENCH_MINSLEEPNS=10000 \
     chrt -r 99 shore-kits/shore_kits_client_networked -i cmdfile &
echo $! > client.pid

wait $(cat client.pid)

# Clean up
./kill_networked.sh

rm -f log scratch cmdfile db-tpcc-1 diskrw shore.conf info server.pid \
    client.pid

../utilities/parselats.py ./lats.bin

mv lats.bin lats.net.bin
mv lats.txt lats.net.txt
