#!/bin/bash

#> make.1.txt
#> make.2.txt

#graph500=./graph500-2.1.4

#gcc -g -std=c99 -I${graph500} my_make_edgelist.c ${graph500}/xalloc.c ${graph500}/generator/utils.c -lm -lrt -o my_make_edgelist 1>>make.1.txt 2>>make.2.txt

make all 2>make.2.txt
echo ""
wc make.2.txt

