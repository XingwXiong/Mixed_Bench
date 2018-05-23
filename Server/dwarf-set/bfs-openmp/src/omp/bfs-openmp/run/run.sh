#!/bin/bash

graph500=../graph500-2.1.4

run_one() {
  scale=$1
  efactor=$2
  dumpfile=$3
  #./make-edgelist -s 10 -e 3 -V -o make-edgelist.dump
  #./omp-csr/omp-csr -s 10 -e 3 -V -o make-edgelist.dump
  ${graph500}/omp-csr/omp-csr -n 3 -s ${scale} -e ${efactor} -V -o ${dumpfile} 1>run.1.txt.${scale}.my_dump.m 2>run.2.txt.${scale}.my_dump.m
  ${graph500}/seq-csr/seq-csr -n 3 -s ${scale} -e ${efactor} -V -o ${dumpfile} 1>run.1.txt.${scale}.my_dump.s 2>run.2.txt.${scale}.my_dump.s
}


run_one 10 3 ../dataset/g.10_2640.dump
run_one 16 5 ../dataset/g.16_297651.dump
run_one 20 7 ../dataset/g.20_6952392.dump
#run_one 21 8 ../dataset/g.21_15284140.dump
#run_one 22 9 ../dataset/g.22_33600654.dump
#run_one 24 10 ../dataset/g.24_162390705.dump


