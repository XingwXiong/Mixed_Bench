gen data:
    cd ../../data_gen/BigDataGeneratorSuite/Graph_datagen/
    vim gen_unweighted_and_split.sh
    ./gen_unweighted_and_split.sh
then move the data generated to bfs/dataset/;
cd this dir;
    make my_make_edgelist
    ./run_my_make_edgelist.sh ./dataset/gen_graph_10_2640.txt
then data generated is tranformed to graph500 dump file.

to make:
    make all

to help:
    cd ./run
    ./multi_thread
    ./single_thread

to run: NOTE: thread_cnt is the last arg for multi_thread!
    cd ./run
    ./multi_thread 10 3 ../dataset/g.10_2640.dump 4
    ./single_thread 10 3 ../dataset/g.10_2640.dump

run:
  input: file;
  output: just some statistic info to stdout;

