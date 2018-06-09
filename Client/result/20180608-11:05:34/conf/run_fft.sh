#!/bin/bash
if [[ -z "${DWARF_DIR}" ]]; then DWARF_DIR=/root/dwarf-set; fi

${DWARF_DIR}/fft-openmp/src/omp/fft-openmp/run/single_thread \
${DWARF_DIR}/fft-openmp/src/omp/fft-openmp/dataset/data ${DWARF_DIR}/fft-openmp/src/omp/fft-openmp\
/run/output/s.0

#./multi_thread ../dataset/part-00000 ./output/m.0 3 > multi_thread.output_console

