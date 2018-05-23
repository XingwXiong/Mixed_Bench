在 “dwarf/src/omp/bfs/readme.txt” 中有说明。
    1。生成数据
        cd 到 dwarf/src/data_gen/BigDataGeneratorSuite/Graph_datagen/ 文件夹。
            dwarf是小矮人。data_gen是数据生成。BigDataGeneratorSuite（简称BDGS）是BigDataBench的数据生成 器，Graph_datagen是其中的图数据生成器。这个文件夹下有我改动的文件，跟官网的有些许不同。
        vim gen_unweighted_and_split.sh
            gen_unweighted_and_split.sh 是生成无权无向图的脚本，该脚本将运行BDGS的图数据生成器，然后处理其输出为无权无向图的数据，最后将单个无权无向图的数据文件切分成多个文件。
            数据生成的参数基本在这里头确定。打开这个脚本gen_unweighted_and_split.sh，在注释“# all user defined variables begin” 和注释 “# all user defined variables end”之间的部分，就是需要修改的参数，每个参数都有注释。参数分别是：
                f_cnt：前述的切分成的小文件的个数。这个可以不用修改，因为在bfs程序中并未用到，只做以后扩展用。
                v_i：用于确定图中点的个数，个数为 1<<v_i 个。这个要修改。
                fname_prefix：输出的数据文件的文件名前缀，文件名为 “前缀 v_i 总的边条数.txt”。这个可以不用修改。
                v_o：临时输出文件的文件名。这个不用改。
                v_m：数据生成所使用的模型，指数据生成使用的模型是从哪个实际的数据集中生成的，实际的数据集有amazon、facebook、google。默认 的使用amazon，这个可以不用改。
            需要说明的是，图数据的大小，一般可以用点个数和边个数来衡量，上述的参数中只能指定点个数，而边个数由BDGS使用点个数和所选模型自行计算得出。计算 方法在 dwarf/src/data_gen/BigDataGeneratorSuite/devLog.txt 中有提到。
            总之，改以下v_i就行了。v_i确定了点个数，结合模型确定了边个数。
        修改了这个脚本后，可能需要先make以生成BDGS的可执行文件gen_kronecker_graph。
        make
        现在可以运行之来生成数据了。
        ./gen_unweighted_and_split.sh
        ls -t # 按时间查看结果
        一般地，得到类似 gen_graph_3_10.txt 的数据集文件，其中 gen_graph 是前述的前缀，3 是v_i，10是实际的边的个数。
    2。 复制前述的数据集文件到 dwarf/src/omp/bfs/dataset 中。cd到 dwarf/src/omp/bfs/ 。
    3。 使用make 生成bfs的可执行文件。
        make all
        或者运行 make.sh 脚本
        ./make.sh
    4。使用刚才make生成的可执行文件 my_make_edgelist，将dataset文件夹下的数据集文件 gen_graph_3_10.txt 转换成graph500的数据文件。
        ./run_my_make_edgelist.sh ./dataset/gen_graph_3_10.txt
        这个脚本将数据文件gen_graph_3_10.txt 转换成 g.3_10.dump，这个就是graph500的输入文件了。
    5。运行graph500的bfs程序。
        这个在 run 文件夹中有相应的脚本来方便调用graph500的bfs程序。
        cd run/
        ./single_thread # 无参数运行显示Usage。
        运行的usage为：
            ./single_thread scale edgefactor input_file
            ./multi_thread scale edgefactor input_file thread_cnt
            其中，scale即为前述的v_i（名字取得不同是因为BDGS和graph500的不同），edgefactor等于取地板（边个数/点个数） （edgefactor的存在是因为BDGS和graph500在图的生成上的区别。BDGS是从实际数据中训练的，所以边个数和点个数是跟实际 数据的模型相关的，而graph500是用来测试性能的，其数据集的处理比较粗暴，边个数就是点个数乘以一个变量）。input_file是 graph500的数据文件。thread_cnt是多线程时的线程个数。下面是一个例子：
            ./single_thread 3 1 ../dataset/g.3_10.dump
            其中3是说图中有（1<<3）即8个点，1等于取地板（10/8）。
            所以，运行这个部分，edgefactor是需要自行计算的。当然也可以自动化地用脚本去实现，就是截取文件名g.3_10.dump中的3和10来计算这两个值。
