#!/bin/bash
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

#make result dir with timestamp
dir=$(date +%Y%m%d-%H:%M:%S)
mkdir -p result/${dir}/conf

export CLIENT_DIR=$DIR
export RESULT_DIR=/home/fanfanda/Client/result/${dir}/
export TAIL_DIR=/home/fanfanda/Server/tailbench-v0.9
export DWARF_DIR=/home/fanfanda/Server/dwarf-set
export REDIS_DIR=/home/fanfanda/redis-cluster
export SERVER_IP=172.18.11.103
export CLIENT_IP=172.18.11.103
export CUR_NUM=1

mode="serial" #"parallel"or"serial"
BS_line=(0 1 + 1)
compute_line=(2 3)

app_name=("xapian" "redis" "sort" "wordcount")

#The ARRAY_SERVER_CMD and ARRAY_CMD are make in order to start up server and run application
ARRAY_SERVER_CMD=("export TAIL_DIR=/home/fanfanda/Server/tailbench-v0.9;${TAIL_DIR}/xapian/run_xapian_server.sh &" \
"export REDIS_DIR=/home/fanfanda/redis-cluster;${REDIS_DIR}/run_redis_cluster.sh ")

ARRAY_CMD=("${CLIENT_DIR}/client/client_script/run_xapian_client.sh" \
"${REDIS_DIR}/multi-benchmark.py > ${RESULT_DIR}redis_" \
"${DWARF_DIR}/sort-openmp/src/omp/sort-openmp/run/run_sort.sh > ${RESULT_DIR}dwarf-sort_" \
"${DWARF_DIR}/wordcount-openmp/src/omp/wordcount-openmp/run/run_wordcount.sh > ${RESULT_DIR}dwarf-wordcount_")

#The ARRAY_SERVER_CONF and ARRAY_CONF are make in order to save the conf.
ARRAY_SERVER_CONF=("scp ${TAIL_DIR}/xapian/run_xapian_server.sh" \
"scp ${REDIS_DIR}/run_redis_cluster.sh")

ARRAY_CONF=("cp ${CLIENT_DIR}/client/client_script/run_xapian_client.sh" \
"cp ${REDIS_DIR}/multi-benchmark.py" \
"scp ${DWARF_DIR}/sort-openmp/src/omp/sort-openmp/run/run_sort.sh" \
"scp ${DWARF_DIR}/wordcount-openmp/src/omp/wordcount-openmp/run/run_wordcount.sh")

SERVER_NUM=${#ARRAY_SERVER_CMD[*]}

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
			/usr/bin/expect <<-EOF
			spawn ssh -f -n ${SERVER_IP} "${cmd_server}"
			$wait_expect
			EOF
			eval $cmd
			cmd=""
			cmd_server=""
			wait_expect=""
			wait
			./clean.sh
		else
			if [ ${cmd_line[$i]} -lt ${SERVER_NUM} ]
			then
				ssh ${SERVER_IP} ${ARRAY_SERVER_CONF[${cmd_line[$i]}]} ${CLIENT_IP}:${RESULT_DIR}conf
				if [ ${cmd_line[$i]} -eq $(( ${SERVER_NUM} - 1 )) ]; then
					cmd="$cmd""export CUR_NUM=${cmd_count[${cmd_line[$i]}]};""${ARRAY_CMD[${cmd_line[$i]}]}${CUR_NUM}.result""$link"
				else
					cmd_server="${cmd_server} ""${ARRAY_SERVER_CMD[${cmd_line[$i]}]}"
					cmd="$cmd""export CUR_NUM=${cmd_count[${cmd_line[$i]}]};""${ARRAY_CMD[${cmd_line[$i]}]}""$link"
				fi
				eval "${ARRAY_CONF[${cmd_line[$i]}]}"" ${RESULT_DIR}conf"
				wait_expect="${wait_expect};""expect \"start ok\""
			else
				let compute_app_num+=1
				start_tm=$(eval "date +%s%N")
				CUR_NUM=${cmd_count[${cmd_line[$i]}]}
				cmd="$cmd""ssh ${SERVER_IP} export DWARF_DIR=${DWARF_DIR};""${ARRAY_CMD[${cmd_line[$i]}]}${CUR_NUM}.result""$link"
				eval "ssh ${SERVER_IP} ""${ARRAY_CONF[${cmd_line[$i]}]}"" ${CLIENT_IP}:${RESULT_DIR}conf"
			fi
			let cmd_count[${cmd_line[$i]}]=${cmd_count[${cmd_line[$i]}]}+1
		fi
	done
	/usr/bin/expect <<-EOF
	spawn ssh -f -n ${SERVER_IP} "${cmd_server}"
	$wait_expect
	EOF
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
		CUR_NUM=${cmd_count[${cmd_line[$i]}]}
		if [ ${cmd_line[$i]} -lt ${SERVER_NUM} ]
		then
			#eval ${ARRAY_SERVER_CMD[${cmd_line[$i]}]}
			#ssh -X -f -n 172.18.11.114 "eval ${ARRAY_SERVER_CMD[${cmd_line[$i]}]}"
			ssh ${SERVER_IP} "${ARRAY_SERVER_CONF[${cmd_line[$i]}]} ${CLIENT_IP}:${RESULT_DIR}conf"
			eval "${ARRAY_CONF[${cmd_line[$i]}]}"" ${RESULT_DIR}conf"
			#echo ${ARRAY_CMD[${cmd_line[$i]}]}
			#sleep 4
			if [ ${cmd_line[$i]} -eq $(( ${SERVER_NUM} - 1 )) ]; then
				eval "${ARRAY_CMD[${cmd_line[$i]}]}${CUR_NUM}.result"
			else
				/usr/bin/expect <<-EOF
				spawn ssh -X -f -n ${SERVER_IP} "${ARRAY_SERVER_CMD[${cmd_line[$i]}]}"
				expect "start ok"
				EOF
				eval "${ARRAY_CMD[${cmd_line[$i]}]}"
			fi
		else
			let compute_app_num+=1
			start_tm=$(eval "date +%s%N") #start compute time
			eval "${ARRAY_CMD[${cmd_line[$i]}]}""${CUR_NUM}.result"
			eval "ssh ${SERVER_IP} ""${ARRAY_CONF[${cmd_line[$i]}]}"" ${CLIENT_IP}:${RESULT_DIR}conf"
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
for ((i=0;i<${#ARRAY_CMD[*]};i++))
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
	echo -e ${app_name[${cmd_line[$i]}]} >> ${RESULT_DIR}all_result.txt
	echo -e -------------------------- >> ${RESULT_DIR}all_result.txt
	# if it's the app of tailbench
	if [ ${cmd_line[$i]} -lt $(( ${SERVER_NUM} - 1 )) ]
	then
		eval "./parselats.py ${RESULT_DIR}${app_name[${cmd_line[$i]}]}_${cmd_count[${cmd_line[$i]}]}_lats.bin 0 >> ${RESULT_DIR}all_result.txt"
		eval "./parselats.py ${RESULT_DIR}${app_name[${cmd_line[$i]}]}_${cmd_count[${cmd_line[$i]}]}_lats.bin 80 >> ${RESULT_DIR}all_result.txt"
		eval "./parselats.py ${RESULT_DIR}${app_name[${cmd_line[$i]}]}_${cmd_count[${cmd_line[$i]}]}_lats.bin 90 >> ${RESULT_DIR}all_result.txt"
		eval "./parselats.py ${RESULT_DIR}${app_name[${cmd_line[$i]}]}_${cmd_count[${cmd_line[$i]}]}_lats.bin 95 >> ${RESULT_DIR}all_result.txt"
		eval "./parselats.py ${RESULT_DIR}${app_name[${cmd_line[$i]}]}_${cmd_count[${cmd_line[$i]}]}_lats.bin 100 >> ${RESULT_DIR}all_result.txt"
		cp lats.txt ${RESULT_DIR}${app_name[${cmd_line[$i]}]}_${cmd_count[${cmd_line[$i]}]}.txt
	elif [ ${cmd_line[$i]} -eq $(( ${SERVER_NUM} - 1 )) ]
	then
		eval "find -samefile ${RESULT_DIR}${app_name[${cmd_line[$i]}]}_${cmd_count[${cmd_line[$i]}]}.result | xargs cat >> ${RESULT_DIR}all_result.txt"
	else
		eval "find -samefile ${RESULT_DIR}dwarf-${app_name[${cmd_line[$i]}]}_${cmd_count[${cmd_line[$i]}]}.result | xargs grep time >> ${RESULT_DIR}all_result.txt"
	fi
	echo -e >> ${RESULT_DIR}all_result.txt
	let cmd_count[${cmd_line[$i]}]=${cmd_count[${cmd_line[$i]}]}+1
done
#clean up

./clean.sh