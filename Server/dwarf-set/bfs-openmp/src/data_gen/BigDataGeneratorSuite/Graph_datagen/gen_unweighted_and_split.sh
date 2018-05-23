###################################################################
#        File Name: gen_unweighted_and_split.sh
#      Description: gen unweighted direct graph, and split;
#           Author: He Xiwen
#            Email: hexiwen2000@163.com
#     Created Time: 2015-11-10 09:54:33
#    Modified Time: 2015-11-10 09:55:27
#          License: 
###################################################################

# unweighted direct graph, so these edge could appear in the file:
#   v v
#   u v
#   v u
# use '\t' to split two vertices in one edge. "u\tv" in one line;

#!/bin/bash

# all user defined variables begin;
# f_cnt: split one graph file into $f_cnt small files;
f_cnt=2
# v_i: there are $((1<<v_i)) vertices in graph;
v_i=17
fname_prefix="gen_graph"
# v_o: the one graph filename; actually the final output filename 
#   is defined by $fname and $sp_prefix
v_o=${fname_prefix}"_"${v_i}".txt"
# v_m: input matrix; different matrixs represent different models;
v_m="0.9532 0.5502; 0.4439 0.2511" # for amazon_gen
#v_m="0.9999 0.5887; 0.6254 0.3676" # for facebook_gen
#v_m="0.8305 0.5573; 0.4638 0.3021" # for google
#echo ${v_o}
#echo ${v_m}
#echo ${v_i}
# all user defined variables end;

./gen_kronecker_graph -o:${v_o} -m:"${v_m}" -i:${v_i} 1>out.txt
sed -i 1,4d ${v_o}

total_vertices=$(head -14 out.txt | tail -1 | awk '{print $2}')
total_edges=$(head -14 out.txt | tail -1 | awk '{print $4}')
#echo ${total_vertices}
#echo ${total_edges}
fname=${fname_prefix}"_"${v_i}"_"${total_edges}".txt"
mv ${v_o} ${fname}
rm out.txt

# split: method 1
## total_edges: also total_lines
#total_lines=${total_edges}
#one_f_lines=$((total_lines/f_cnt))
##echo ${total_lines}"/"${f_cnt}"="${one_f_lines}
#for (( i=1; i<${f_cnt}; ++i )); do
#  one_f_name=${fname_prefix}"_"${v_i}"_"${one_f_lines}"_"${i}".txt"
#  #echo $((i*one_f_lines))
#  #echo ${one_f_lines}
#  #echo ${one_f_name}
#  head -$((i*one_f_lines)) ${fname} | tail -${one_f_lines} > ${one_f_name}
#done
#one_f_lines=$((total_lines-one_f_lines*(f_cnt-1)))
#one_f_name=${fname_prefix}"_"${v_i}"_"${one_f_lines}"_"${i}".txt"
##echo ${total_lines}
##echo ${one_f_lines}
##echo ${one_f_name}
#tail -${one_f_lines} ${fname} > ${one_f_name}

# split: method 2
# total_edges: also total_lines
total_lines=${total_edges}
one_f_lines=$((total_lines/f_cnt+1))
#echo ${total_lines}"/"${f_cnt}+1"="${one_f_lines}
sp_prefix=${fname_prefix}"_"${v_i}"_"${total_lines}
mkdir ${sp_prefix}
cd ${sp_prefix}
sp_prefix=${fname_prefix}"_"${v_i}"_"${one_f_lines}"_"
split -l ${one_f_lines} --numeric-suffixes=1 "../"${fname} ${sp_prefix}
cd - >/dev/null

