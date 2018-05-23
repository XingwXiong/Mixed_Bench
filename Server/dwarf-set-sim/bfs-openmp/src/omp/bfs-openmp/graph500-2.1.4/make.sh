#!/bin/bash

make 1>make.1.txt 2>make.2.txt
echo ""
wc make.2.txt

