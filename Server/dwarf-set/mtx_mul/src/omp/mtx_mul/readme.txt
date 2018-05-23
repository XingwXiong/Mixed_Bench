to make:
cd src/omp/mtx_mul
make all

to help:
cd src/omp/mtx_mul/run
./multi_thread
./single_thread

to run: NOTE: thread_cnt is the last arg for multi_thread!
cd src/omp/mtx_mul/run
./multi_thread ../dataset/part-00000 ../dataset/part-00000 ./output/out.s.mtx 3
./single_thread ../dataset/part-00000 ../dataset/part-00000 ./output/out.m.mtx

run:
  input: two sparse matrix files, in coo format; (x,y,val) one line, x, y begin with 0;
  output: on sparse matrix file, in coo format;

