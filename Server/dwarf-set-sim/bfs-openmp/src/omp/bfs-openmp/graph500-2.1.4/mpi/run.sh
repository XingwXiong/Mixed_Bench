#!/bin/bash

TMPFILE="./tmp_a_file.txt"
#export TMPFILE
mpiexec -n 2 ./graph500_mpi_simple 10 16 1>out.1.txt 2>out.2.txt

