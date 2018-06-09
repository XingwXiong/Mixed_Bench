#!/bin/bash
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

#make result dir with timestamp
dir=$(date +%Y%m%d-%H:%M:%S)
mkdir -p result/${dir}/conf

export CLIENT_DIR=$DIR
export RESULT_DIR=$DIR/result/${dir}/
export TAIL_ROOT=/root/tailbench-v0.9
export DWARF_DIR=/root/dwarf-set
export SERVER_IP=172.17.0.2
export CLIENT_IP=172.17.0.1
export CUR_NUM=1

app_name=("xapian" "img-dnn" "masstree" "shore" "sphinx" "sort" "wordcount" "md5" "union" \
"bfs" "mtx_mul" "fft")

online_name=("xapian" "img-dnn" "masstree" "shore" "sphinx" )
