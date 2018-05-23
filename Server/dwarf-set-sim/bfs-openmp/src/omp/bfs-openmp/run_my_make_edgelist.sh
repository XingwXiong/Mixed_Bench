#!/bin/bash

if [ $# != 1 ]; then
  echo "Usage: $0 input_f"
  exit 1
fi
ifpath=$1
ifname=${ifpath##*/}
idname=${ifpath%/*}
if [ "s"${ifpath} = "s"${idname} ]; then
  idname=./
fi
scale=$(echo ${ifname} | awk -F '_' '{print $3}')
nedge=$(echo ${ifname} | awk -F '_' '{print $4}' | awk -F '.' '{print $1}')
ofpath=${idname}/g.${scale}_${nedge}.dump
#echo ${nedge}
#echo ${ifpath}
#echo ${ofpath}

./my_make_edgelist ${nedge} ${ifpath} ${ofpath}

#echo "10"
#./my_make_edgelist 2640 ./dataset/gen_graph_10_2640.txt ./dataset/g.10_2640.dump
#echo "16"
#./my_make_edgelist 297651 ./dataset/gen_graph_16_297651.txt ./dataset/g.16_297651.dump
#echo "20"
#./my_make_edgelist 6952392 ./dataset/gen_graph_20_6952392.txt ./dataset/g.20_6952392.dump
#echo "21"
#./my_make_edgelist 15284140 ./dataset/gen_graph_21_15284140.txt ./dataset/g.21_15284140.dump
#echo "22"
#./my_make_edgelist 33600654 ./dataset/gen_graph_22_33600654.txt ./dataset/g.22_33600654.dump
#echo "24"
#./my_make_edgelist 162390705 ./dataset/gen_graph_24_162390705.txt ./dataset/g.24_162390705.dump


