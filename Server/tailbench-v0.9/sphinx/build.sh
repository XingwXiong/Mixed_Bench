#!/bin/bash

if [[ ! -d sphinx-install ]]
then
    ROOTDIR=$PWD
    mkdir sphinx-install

    # Build and install sphinxbase
    tar -xzvf sphinxbase-5prealpha.tar.gz
    cd sphinxbase-5prealpha
    ./configure --prefix=${ROOTDIR}/sphinx-install
    make clean all
    # make check
    make install
    cd -

    # Build and install pocketsphinx
    tar -xzvf pocketsphinx-5prealpha.tar.gz
    cd pocketsphinx-5prealpha
    ./configure --prefix=${ROOTDIR}/sphinx-install
    make clean all
    # make check
    make install
    cd -
fi

# Build decoder
make
