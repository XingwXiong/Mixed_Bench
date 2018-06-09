#!/bin/bash

if [[ -z "${DWARF_DIR}" ]]; then DWARF_DIR=/root/dwarf-set; fi

#./single_thread ../dataset/test.data.01/test.data.01.txt ../dataset/test.data.02/test.data.01.txt ./output/s.fmkdir ./output/s.d

${DWARF_DIR}/union-openmp/src/omp/union-openmp/run/multi_thread ${DWARF_DIR}/union-openmp/src/omp/union-openmp/dataset/test.data.01\
/ha.txt ${DWARF_DIR}/union-openmp/src/omp/union-openmp/dataset/test.data.02/test.data.01.txt ${DWARF_DIR}/union-openmp/src/omp\
/union-openmp/run/output/m.f 3

