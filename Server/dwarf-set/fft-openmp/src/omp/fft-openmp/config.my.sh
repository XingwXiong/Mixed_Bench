#!/bin/bash

if [ ! -f Makefile ]; then
  echo "No Makefile..."
  exit -1
fi

already_rt=$(grep "^CFlags_. =" Makefile | grep " \-lrt" | wc -l)
if [ ${already_rt} -ne 0 ]; then
  echo "-rt already exist in Makefile"
  exit 0
fi

glibc_v=$(ldd --version | head -1 | awk '{print $NF}')
#glibc_v=$(getconf GNU_LIBC_VERSION | awk '{print $NF}')

#glibc_v=1.5
#glibc_v=2.5

v1=$(echo ${glibc_v} | awk -F '.' '{print $1}')
v2=$(echo ${glibc_v} | awk -F '.' '{print $2}')

# cmp to 2.17
need_rt=0
if [ $v1 -lt 2 ]; then
  echo "glibc too old..."
  exit -1
elif [ $v1 -eq 2 ]; then
  if [ $v2 -lt 17 ]; then
    echo "need -lrt"
    need_rt=1
  fi
fi

#sed '/^CFlags_. =/s/$/abcdefg$/' Makefile | less
#sed 's/^CFlags_. =/& -lrt/' Makefile | less
if [ $need_rt -eq 1 ]; then
  sed -i 's/^CFlags_. =/& -lrt/' Makefile
fi

