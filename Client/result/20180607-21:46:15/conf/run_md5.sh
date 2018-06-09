#!/bin/bash

if [[ -z "${DWARF_DIR}" ]]; then DWARF_DIR=/root/dwarf-set; fi

${DWARF_DIR}/md5-openmp/src/omp/md5-openmp/run/multi_thread ${DWARF_DIR}/md5-openmp/src/omp/md5-openmp/dataset/test.data.01/ha.txt \
${DWARF_DIR}/md5-openmp/src/omp/md5-openmp/run/output/m.f 3


