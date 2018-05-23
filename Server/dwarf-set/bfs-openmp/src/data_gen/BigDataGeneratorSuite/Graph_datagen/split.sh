###################################################################
#        File Name: split.sh
#      Description: split existent graph file to several files;
#           Author: He Xiwen
#            Email: hexiwen2000@163.com
#     Created Time: 2015-11-10 10:03:22
#    Modified Time: 2015-11-10 10:05:06
#          License: 
###################################################################

#!/bin/bash


Usage="$0 pre_fix_vi_tl.txt"
if [ $# != 1 ]; then
  echo "Usage: "${Usage}
  exit 1
fi

fname_ori=$1
fname=$1
#echo ${fname}
# fname like this: gen_graph_8_545.txt
if [ $(echo ${fname} | awk -F '.' '{print $2}')"x" != "txt""x" ]; then
  echo "Wrong input_file"
  exit 1
fi
fname=$(echo ${fname} | awk -F '.' '{print $1}')
#echo ${fname}
v_i=$(echo ${fname} | awk -F '_' '{print $3}')
total_lines=$(echo ${fname} | awk -F '_' '{print $4}')
if [ -z ${v_i} ] || [ -z ${total_lines} ]; then
  echo "no v_i or total_lines"
  exit 1
fi
#echo "vi: "${v_i}
#echo "t_l: "${total_lines}
f_cnt=4
fname_prefix=$(echo ${fname} | awk -F '_' '{print $1}')"_"$(echo ${fname} | awk -F '_' '{print $2}')
#echo ${fname_prefix}
mkdir ${fname}
cd ${fname}
one_f_lines=$((total_lines/f_cnt+1))
#echo ${total_lines}"/"${f_cnt}+1"="${one_f_lines}
sp_prefix=${fname_prefix}"_"${v_i}"_"${one_f_lines}"_"
split -l ${one_f_lines} --numeric-suffixes=1 "../"${fname_ori} ${sp_prefix}

cd - > /dev/null

