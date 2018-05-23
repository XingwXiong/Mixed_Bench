#!/bin/bash

make graph500_mpi_simple 1>make.1.txt 2>make.2.txt
echo ""
wc make.2.txt

