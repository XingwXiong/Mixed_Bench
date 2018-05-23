to make:
cd src/omp/union
make all

to help:
cd src/omp/union/run
./multi_thread
./single_thread

to run: NOTE: thread_cnt is the last arg for multi_thread!
cd src/omp/union/run
./multi_thread ../dataset/test.data.01 ../dataset/test.data.02 ./output 3
./single_thread ../dataset/test.data.01 ../dataset/test.data.02 ./output

run:
  input: file or dir;
  output: same type as input; must exist before running this program if it is dir;

