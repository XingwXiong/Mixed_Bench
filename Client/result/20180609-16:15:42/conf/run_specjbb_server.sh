#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
source ${DIR}/../configs.sh 

# Setup commands
mkdir -p results

# Run specjbb
TBENCH_PATH=../harness

export LD_LIBRARY_PATH=${TBENCH_PATH}:${LD_LIBRARY_PATH}

export CLASSPATH=./build/dist/jbb.jar:./build/dist/check.jar:${TBENCH_PATH}/tbench.jar

export PATH=${JDK_PATH}/bin:${PATH}

REQUESTS=15000
WARMUPREQS=1000 

if [[ -d libtbench_jni.so ]] 
then
    rm libtbench_jni.so
fi
ln -sf libtbench_networked_jni.so libtbench_jni.so


#if [[ -d server.pid ]]; then kill -9 $(cat server.pid); rm -f server.pid; echo "okay"; fi
kill -9 $(cat server.pid)
rm -f server.pid
sleep 1

TBENCH_SERVER_PORT=10006 \
TBENCH_RANDSEED=${RANDOM} TBENCH_MAXREQS=${REQUESTS} TBENCH_WARMUPREQS=${WARMUPREQS} \
nohup ${JDK_PATH}/bin/java -Djava.library.path=. -XX:ParallelGCThreads=1 \
    -XX:+UseSerialGC -XX:NewRatio=1 -XX:NewSize=7000m -Xloggc:gc.log \
    -Xms10000m -Xmx10000m -Xrs spec.jbb.JBBmain -propfile SPECjbb_mt.props >server.log 2>&1 &

echo $! > server.pid
#wait $(cat server.pid)

while [ 1 ]
do
    netstat -nlp | grep 10006
    if [ $? == 0 ]
      then
          echo start ok
          break
      else
          echo -e '#\c'
    fi
done
