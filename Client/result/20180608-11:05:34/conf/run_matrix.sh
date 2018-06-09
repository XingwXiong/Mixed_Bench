#!/bin/bash

if [[ -z "${DWARF_DIR}" ]]; then DWARF_DIR=/root/dwarf-set; fi

${DWARF_DIR}/mtx_mul/src/omp/mtx_mul/run/single_thread ${DWARF_DIR}/mtx_mul/src/omp/mtx_mul\
/dataset/part-00000 ${DWARF_DIR}/mtx_mul/src/omp/mtx_mul/dataset/part-00000 \
${DWARF_DIR}/mtx_mul/src/omp/mtx_mul/run/output/part-00000.out.s.mtx

#./multi_thread ../dataset/part-00000 ../dataset/part-00000 ./output/part-00000.out.m.mtx 4

