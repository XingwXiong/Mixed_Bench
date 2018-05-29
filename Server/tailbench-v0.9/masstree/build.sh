#!/bin/bash

autoconf
./configure --disable-assertions --with-malloc=tcmalloc 

#./configure --disable-assertions --with-malloc=tcmalloc CPPFLAGS=${PWD}/../harness/hiredis/include LIBS=${PWD}/../harness/hiredis/lib/libhiredis.a 
make -j16
