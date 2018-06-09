#!/bin/bash
source ./config.sh
if [[ -z "${DWARF_DIR}" ]]; then DWARF_DIR=/root/dwarf-set; fi

dwarf_name=('bfs-openmp' 'fft-openmp' 'mtx_mul' 'md5-openmp' 'sort-openmp' 'union-openmp' 'wordcount-openmp')
bash_name=('bfs' 'fft' 'matrix' 'md5' 'sort' 'union' 'wordcount')

for i in ${#dwarf_set[@]}; do
    ${DWARF_DIR}/${dwarf_name[$i]}/src/omp/${dwarf_name[$i]}/run/run_${bash_name[$i]}.sh
    echo "===================="
done

