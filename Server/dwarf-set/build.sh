#!/bin/bash
source ./config.sh
#dwarf_set=('bfs-openmp' 'fft-openmp' 'matrixMult' 'md5-openmp' 'sort-openmp' 'union-openmp' 'wordcount-openmp''openmp-pagerank')
dwarf_set=('bfs-openmp' 'fft-openmp' 'mtx_mul' 'md5-openmp' 'sort-openmp' 'union-openmp' 'wordcount-openmp')

for item in ${dwarf_set[@]}; do
    make -C ${DWARF_DIR}/${item}/src/omp/${item} -j16 clean
    make -C ${DWARF_DIR}/${item}/src/omp/${item} -j16
    echo "===================="
done

