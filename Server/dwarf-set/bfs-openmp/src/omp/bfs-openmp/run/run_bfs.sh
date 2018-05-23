#!/bin/bash

graph500=${DWARF_DIR}/bfs-openmp/src/omp/bfs-openmp/graph500-2.1.4

run_one() {
  scale=$1
  efactor=$2
  dumpfile=$3
  #./make-edgelist -s 10 -e 3 -V -o make-edgelist.dump
  #./omp-csr/omp-csr -s 10 -e 3 -V -o make-edgelist.dump
  ${graph500}/omp-csr/omp-csr -n 3 -s ${scale} -e ${efactor} -V -o ${dumpfile} 1>run.1.txt.${scale}.my_dump.m 2
  #${graph500}/seq-csr/seq-csr -n 3 -s ${scale} -e ${efactor} -V -o ${dumpfile} 1>run.1.txt.${scale}.my_dump.s 2 > \
${RESULT_DIR}/bfs_${CUR_NUM}_run.2.${scale}.my_dump.s
}
dumpfile=${DWARF_DIR}/bfs-openmp/src/omp/bfs-openmp/dataset/g.10_2636.dump
${graph500}/omp-csr/omp-csr -n 3 -s 10 -e 3 -V -o ${dumpfile} 2
#run_one 10 3 ${DWARF_DIR}/bfs-openmp/src/omp/bfs-openmp/dataset/g.10_2636.dump
#run_one 16 5 ../dataset/g.16_297651.dump
#run_one 17 7 ${DWARF_DIR}/bfs-openmp/src/omp/bfs-openmp/dataset/g.17_654356.dump
#run_one 21 8 ../dataset/g.21_15284140.dump
#run_one 22 9 ../dataset/g.22_33600654.dump
#run_one 24 10 ../dataset/g.24_162390705.dump


