#!/bin/bash
make clean
MODE=perf make -j16
MODE=perf make -j16 dbtest
