#!/bin/bash
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

#make result dir with timestamp
dir=$(date +%Y%m%d-%H:%M:%S)
mkdir -p result/${dir}/conf

export CLIENT_DIR=$DIR
export RESULT_DIR=$DIR/result/${dir}/
export TAIL_ROOT=/root/tailbench-v0.9
export DWARF_DIR=/root/dwarf-set
export SERVER_IP=172.17.0.51
export CLIENT_IP=172.17.0.1
export NUM=1

mode="parallel" #"parallel"or"serial"
BS_line=(2)
compute_line=(8) 
#compute_line=(5 6 7 8 9 10 11 + 6 7) 
#cmd_line=(1 5 + 0 2 7 3 + 10 11)

app_name=("xapian" "img-dnn" "masstree" "shore" "sphinx" "silo" "specjbb" "moses" \
    "sort" "wordcount" "md5" "union" "bfs" "mtx_mul" "fft")

#The ARRAY_SERVER_CMD and ARRAY_CMD are make in order to start up server and run application
ARRAY_SERVER_CMD=("${TAIL_ROOT}/xapian/run_xapian_server.sh ;" \
"${TAIL_ROOT}/img-dnn/run_img-dnn_server.sh ;" \
"${TAIL_ROOT}/masstree/run_masstree_server.sh ;" \
"${TAIL_ROOT}/shore/run_shore_server.sh ;" \
"${TAIL_ROOT}/sphinx/run_sphinx_server.sh ;" \
"${TAIL_ROOT}/silo/run_silo_server.sh ;" \
"${TAIL_ROOT}/specjbb/run_specjbb_server.sh ;" \
"${TAIL_ROOT}/moses/run_moses_server.sh ;")

ARRAY_CMD=("${TAIL_ROOT}/xapian/run_xapian_client.sh" \
"${TAIL_ROOT}/img-dnn/run_img-dnn_client.sh" \
"${TAIL_ROOT}/masstree/run_masstree_client.sh" \
"${TAIL_ROOT}/shore/run_shore_client.sh" \
"${TAIL_ROOT}/sphinx/run_sphinx_client.sh" \
"${TAIL_ROOT}/silo/run_silo_client.sh" \
"${TAIL_ROOT}/specjbb/run_specjbb_client.sh" \
"${TAIL_ROOT}/moses/run_moses_client.sh" \
"${DWARF_DIR}/sort-openmp/src/omp/sort-openmp/run/run_sort.sh > \
${RESULT_DIR}dwarf-sort_" \
"${DWARF_DIR}/wordcount-openmp/src/omp/wordcount-openmp/run/run_wordcount.sh >\
 ${RESULT_DIR}dwarf-wordcount_" \
"${DWARF_DIR}/md5-openmp/src/omp/md5-openmp/run/run_md5.sh > \
${RESULT_DIR}dwarf-md5_" \
"${DWARF_DIR}/union-openmp/src/omp/union-openmp/run/run_union.sh > \
${RESULT_DIR}dwarf-union_" \
"${DWARF_DIR}/bfs-openmp/src/omp/bfs-openmp/run/run_bfs.sh > \
${RESULT_DIR}dwarf-bfs_" \
"${DWARF_DIR}/mtx_mul/src/omp/mtx_mul/run/run_matrix.sh  > \
${RESULT_DIR}dwarf-mtx_mul_" \
"${DWARF_DIR}/fft-openmp/src/omp/fft-openmp/run/run_fft.sh > \
${RESULT_DIR}dwarf-fft_")

# app_name=("xapian" "img-dnn" "masstree" "shore" "sphinx" "silo" "specjbb" "moses" \
#The ARRAY_SERVER_CONF and ARRAY_CONF are make in order to save the conf.
ARRAY_SERVER_CONF=("scp root@${SERVER_IP}:${TAIL_ROOT}/xapian/run_xapian_server.sh" \
"scp root@${SERVER_IP}:${TAIL_ROOT}/img-dnn/run_img-dnn_server.sh" \
"scp root@${SERVER_IP}:${TAIL_ROOT}/masstree/run_masstree_server.sh" \
"scp root@${SERVER_IP}:${TAIL_ROOT}/shore/run_shore_server.sh" \
"scp root@${SERVER_IP}:${TAIL_ROOT}/sphinx/run_sphinx_server.sh" \
"scp root@${SERVER_IP}:${TAIL_ROOT}/silo/run_silo_server.sh" \
"scp root@${SERVER_IP}:${TAIL_ROOT}/specjbb/run_specjbb_server.sh" \
"scp root@${SERVER_IP}:${TAIL_ROOT}/moses/run_moses_server.sh" )

ARRAY_CONF=("cp ${TAIL_ROOT}/xapian/run_xapian_client.sh" \
"cp ${TAIL_ROOT}/img-dnn/run_img-dnn_client.sh" \
"cp ${TAIL_ROOT}/masstree/run_masstree_client.sh" \
"cp ${TAIL_ROOT}/shore/run_shore_client.sh" \
"cp ${TAIL_ROOT}/sphinx/run_sphinx_client.sh" \
"cp ${TAIL_ROOT}/silo/run_silo_client.sh" \
"cp ${TAIL_ROOT}/specjbb/run_specjbb_client.sh" \
"cp ${TAIL_ROOT}/moses/run_moses_client.sh" \
"scp root@${SERVER_IP}:${DWARF_DIR}/sort-openmp/src/omp/sort-openmp/run/run_sort.sh" \
"scp root@${SERVER_IP}:${DWARF_DIR}/wordcount-openmp/src/omp/wordcount-openmp/run/run_wordcount.sh" \
"scp root@${SERVER_IP}:${DWARF_DIR}/md5-openmp/src/omp/md5-openmp/run/run_md5.sh" \
"scp root@${SERVER_IP}:${DWARF_DIR}/union-openmp/src/omp/union-openmp/run/run_union.sh" \
"scp root@${SERVER_IP}:${DWARF_DIR}/bfs-openmp/src/omp/bfs-openmp/run/run_bfs.sh" \
"scp root@${SERVER_IP}:${DWARF_DIR}/mtx_mul/src/omp/mtx_mul/run/run_matrix.sh" \
"scp root@${SERVER_IP}:${DWARF_DIR}/fft-openmp/src/omp/fft-openmp/run/run_fft.sh")

excute_func(){
cmd_line=($1)
sign=$2 #sign for bs or compute

wait_expect=""
for((i=0;i<${#ARRAY_CMD[*]};i++))
do
  cmd_count[i]=1 #array for times of excuting
done
cmd=""
link=" & "
cmd_server=""
count=${#cmd_line[*]}
echo length:$count

if [ $count -eq 0 ]
then
   echo "excute line length equal 0 , return!"
   return
fi
echo "excute line:"${cmd_line[*]}
start_tm=0
end_tm=0
compute_app_num=0

if [ $mode == "parallel" ];then
   for ((i=0;i<$count;i++))
   do
        if [ ${cmd_line[$i]} == "+" ]
        then
            echo "[cmd_server]spawn ssh -f -n root@${SERVER_IP} \"/root/tailbench-v0.9/server_func.py ${cmd_server}\""
	        /usr/bin/expect <<-EOF
	        spawn ssh -f -n root@${SERVER_IP} "/root/tailbench-v0.9/server_func.py ${cmd_server}"
	        $wait_expect
EOF
	        echo "[cmd] $cmd"
            eval $cmd
        	cmd=""
	        cmd_server=""
	        wait_expect=""
	        wait
	        ./clean.sh
            echo "======================================="
        else
	        if [ ${cmd_line[$i]} -lt ${#ARRAY_SERVER_CMD[*]} ]
	        then
	            #cmd_server="$cmd_server ""${ARRAY_SERVER_CMD[${cmd_line[$i]}]}"
	            cmd_server="$cmd_server ${cmd_line[$i]}"
                cmd="$cmd"" NUM=${cmd_count[${cmd_line[$i]}]} ""${ARRAY_CMD[${cmd_line[$i]}]}""$link"
	            ${ARRAY_SERVER_CONF[${cmd_line[$i]}]} ${RESULT_DIR}conf
	            echo "${ARRAY_CONF[${cmd_line[$i]}]}"" ${RESULT_DIR}conf"
	            eval "${ARRAY_CONF[${cmd_line[$i]}]}"" ${RESULT_DIR}conf"
	            wait_expect="${wait_expect};""expect \"start ok\""
	        else
        	    let compute_app_num+=1
	            start_tm=$(eval "date +%s%N")
	            NUM=${cmd_count[${cmd_line[$i]}]}
	            cmd="$cmd""ssh root@${SERVER_IP} DWARF_DIR=${DWARF_DIR} ${ARRAY_CMD[${cmd_line[$i]}]}""${NUM}.result""$link"
	            echo ${ARRAY_CONF[${cmd_line[$i]}]} ${RESULT_DIR}conf
	            eval ${ARRAY_CONF[${cmd_line[$i]}]} ${RESULT_DIR}conf
            fi
	        let cmd_count[${cmd_line[$i]}]=${cmd_count[${cmd_line[$i]}]}+1
        fi
    done
    echo "[cmd_serever]spawn ssh -f -n root@${SERVER_IP} \"/root/tailbench-v0.9/server_func.py ${cmd_server}\""
    /usr/bin/expect <<-EOF
	    spawn ssh -f -n root@${SERVER_IP} "/root/tailbench-v0.9/server_func.py ${cmd_server}"
        $wait_expect
EOF
    echo "[cmd]${cmd}"
    eval $cmd
    wait
    end_tm=$(eval "date +%s%N")
else
    for ((i=0;i<$count;i++))
    do
        if [ ${cmd_line[$i]} == "+" ]
        then
            continue
        fi
        NUM=${cmd_count[${cmd_line[$i]}]}
        if [ ${cmd_line[$i]} -lt ${#ARRAY_SERVER_CMD[*]} ]
        then
	        #spawn ssh -X -f -n root@${SERVER_IP} "${ARRAY_SERVER_CMD[${cmd_line[$i]}]}"
	        /usr/bin/expect <<-EOF
	            spawn ssh -X -f -n root@${SERVER_IP} "/root/tailbench-v0.9/server_func.py ${cmd_line[$i]}"
	            expect "start ok"
EOF
	        scp root@${SERVER_IP}:"${ARRAY_SERVER_CONF[${cmd_line[$i]}]} ${RESULT_DIR}conf"
	        eval "${ARRAY_CONF[${cmd_line[$i]}]}"" ${RESULT_DIR}conf"
	        ${ARRAY_CMD[${cmd_line[$i]}]}
        else
            let compute_app_num+=1
	        start_tm=$(eval "date +%s%N") #start compute time
	        eval "${ARRAY_CMD[${cmd_line[$i]}]}""$NUM.result"
	        eval "ssh root@${SERVER_IP} ""${ARRAY_CONF[${cmd_line[$i]}]}"" ${RESULT_DIR}conf"
        fi
        wait
        let cmd_count[${cmd_line[$i]}]=${cmd_count[${cmd_line[$i]}]}+1
        ./clean.sh
    done
    end_tm=$(eval "date +%s%N") #end compute time
fi
if [ $sign -eq 1 ]
then
    echo okkkkkk
    use_time=$[$[end_tm-start_tm] / 1000000000]
    echo "compute app time:${use_time} ""compute app num:${compute_app_num}"
    throughput_rate=$(echo "scale=5;${compute_app_num}/${use_time}"|bc)
    echo throughput_rate:${throughput_rate}
    echo -e "compute application throughput_rate:${throughput_rate}" >> ${RESULT_DIR}all_result.txt
fi
}

count_bs=${#BS_line[*]}
count_compute=${#compute_line[*]}
excute_func "${BS_line[*]}" 0 &
excute_func "${compute_line[*]}" 1 &

wait

cmd_line=("${BS_line[@]}" "${compute_line[@]}")
#generate result
for((i=0;i<${#ARRAY_CMD[*]};i++))
do
    cmd_count[i]=1
done
count=${#cmd_line[*]}
for ((i=0;i<$count;i++))
do
    if [ ${cmd_line[$i]} == "+" ]
    then
        continue
    fi
    echo -e ${app_name[${cmd_line[$i]}]} | tee -a ${RESULT_DIR}all_result.txt
    echo -e -------------------------- | tee -a ${RESULT_DIR}all_result.txt
    if [ ${cmd_line[$i]} -lt ${#ARRAY_SERVER_CMD[*]} ]
    then
        while [ ! -f ${RESULT_DIR}${app_name[${cmd_line[$i]}]}_${cmd_count[${cmd_line[$i]}]}_lats.bin ]
        do
            continue
        done
        eval "${TAIL_ROOT}/utilities/parselats.py ${RESULT_DIR}${app_name[${cmd_line[$i]}]}_${cmd_count[${cmd_line[$i]}]}\
_lats.bin 0 | tee -a ${RESULT_DIR}all_result.txt"
        eval "${TAIL_ROOT}/utilities/parselats.py ${RESULT_DIR}${app_name[${cmd_line[$i]}]}_${cmd_count[${cmd_line[$i]}]}\
_lats.bin 90 | tee -a ${RESULT_DIR}all_result.txt"
        eval "${TAIL_ROOT}/utilities/parselats.py ${RESULT_DIR}${app_name[${cmd_line[$i]}]}_${cmd_count[${cmd_line[$i]}]}\
_lats.bin 90 | tee -a ${RESULT_DIR}all_result.txt"
        eval "${TAIL_ROOT}/utilities/parselats.py ${RESULT_DIR}${app_name[${cmd_line[$i]}]}_${cmd_count[${cmd_line[$i]}]}\
_lats.bin 95 | tee -a ${RESULT_DIR}all_result.txt"
        eval "${TAIL_ROOT}/utilities/parselats.py ${RESULT_DIR}${app_name[${cmd_line[$i]}]}_${cmd_count[${cmd_line[$i]}]}\
_lats.bin 100 | tee -a ${RESULT_DIR}all_result.txt"
        cp lats.txt ${RESULT_DIR}${app_name[${cmd_line[$i]}]}_${cmd_count[${cmd_line[$i]}]}.txt
    else
        eval "find -samefile ${RESULT_DIR}dwarf-${app_name[${cmd_line[$i]}]}_${cmd_count[${cmd_line[$i]}]}\
.result | xargs grep time | tee -a ${RESULT_DIR}all_result.txt"
    fi
    echo -e | tee -a ${RESULT_DIR}all_result.txt
    let cmd_count[${cmd_line[$i]}]=${cmd_count[${cmd_line[$i]}]}+1
done
#clean up

./clean.sh
