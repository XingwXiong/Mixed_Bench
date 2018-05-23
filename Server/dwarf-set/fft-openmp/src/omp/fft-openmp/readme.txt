to make:
cd src/omp/fft
./config.my.sh
make all

to help:
cd src/omp/fft/run
./multi_thread
./single_thread

to run: NOTE: thread_cnt is the last arg for multi_thread!
cd src/omp/fft/run
./multi_thread ../dataset/part-00000 ./output/out.s.mtx 3
./single_thread ../dataset/part-00000 ./output/out.m.mtx

run:
  input: one sparse matrix file, in coo format; (x,y,val) one line, x, y begin with 0;
  output: on dense matrix file, in ascii or binary;

lib used:
  fftw3:
    install:
      1. download:
        download fftw3 form fftw.org, not from github.
          wget "http://fftw.org/fftw-3.3.4.tar.gz"
      2. compile
        tar zxf that.tar.gz;
        cd to that dir;
        then the normal "configure, make, make install" steps...
        note: --enable-openmp for omp and pthreads.
          ./configure --enable-openmp
          make
          sudo make install
        done.
        normally, installed in /usr/local/*
    not install:
      files needed in fftw3/;
      add to Makefile:
        -I ./fftw3/include/ -L ./fftw3/lib/

