#!/bin/bash

if [[ -z "${DWARF_DIR}" ]]; then DWARF_DIR=/root/dwarf-set; fi

${DWARF_DIR}/wordcount-openmp/src/omp/wordcount-openmp/run/multi_thread ${DWARF_DIR}/wordcount-openmp/src\
/omp/wordcount-openmp/dataset\
/test.data.01/ha.txt ${DWARF_DIR}/wordcount-openmp/src/omp/wordcount-openmp/run/output/m.01 3
