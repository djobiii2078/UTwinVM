#!/bin/bash
#Periodically get the number of context switches dones inside the VMs.
#We achieve this by obtaining the number of total flush tlb events done and substract from
#the number of INVLG, MMIO_EXITS
# #context_switch = #total_flush_tlbs - (INVLG + MMIO_EXITS)
# djob + Tong

if [ "$#" -gt 2 ];
then
	
	echo "Error, you passed only $# params. Please, specify the observability period (optional) and the total observation time."
	echo "Usage: $0 [<period>] {<observationtime>]"
	echo "By default, the script collects data each scheduler tick & period=10s."
	echo "If no period is specified i.e, just one parameter passed, it will consider the period as (cat /proc/sys/kernel/sched_min_granularity_ns)*10^-9"
	exit 

fi

echo "==================================="
echo "Event Logger v0.0.1"
echo "==================================="

period=10

if [ "$#" -eq 1 ];
then
	period=$(echo "scale=3;$(cat /proc/sys/kernel/sched_min_granularity_ns)/1000000000" | bc)
	observationtime=$1
fi

if [ "$#" -eq 2 ];
then
	period=$1
	observationtime=$2
fi

if [ "$#" -eq 0 ];
then
	echo "Nothing to do".
	echo "No inputs"
	exit
fi

echo "Period: $period --- Observationtime: $observationtime"
nbVMs=$(virsh --connect qemu:///system list | grep running | wc -l)
listVMs=($(virsh --connect qemu:///system list | grep running | awk '{print $2}'))

#Each VM stats will be dumped in the folder vm_<number> in the folder cloudRep_<seed_number>
randNUM=$RANDOM

#This function logs events for a particular VM.
#Basically, it logs every event such as VM_EXIT, I/O requests, CPU usage, mem usage changes
#It logs the events in the format 
#<timestamp> <event> <value>
#ex: 
#	10:111:2300 INCREASE_CPU_USAGE 30
#	10:221:1230	INCREASE_NET_RQ 431
#	10:113:2319	INCREASE_MEM_USAGE	21
#	10:113:2103	VM_I/O EXIT
#	10:113:2210	VNMI EXIT
#	10:113:2901	INPUT_PACKET 200 1KB

echo 1 >/sys/kernel/debug/tracing/events/kvm/enable

function logEventforVM(){
	vmName=$1
	pidQemu=$(ps -aux | grep $vmName | head -n 1 | awk '{print $2}')
	
	statDir=$(ls /sys/kernel/debug/kvm | grep $pidQemu)

	virsh --connect qemu:///system dommemstat --period 1 $vmName


	#echo "watch: $statDir "
	#inotifywait -r -m -e modify /sys/kernel/debug/kvm/$statDir/ | 

	#while read -r filename event; do 
	#	echo "update to $filename: $(cat /sys/kernel/debug/kvm/$statDir/$filename)" >> cloudRep_profiling/cloudRep_$randNUM/vm_$vmCounter/kvm_events_$vmName
	#	watch=$(cat cloudRep_profiling/cloudRep_$randNUM/watch)
	#	if [ "$watch" == "false"]; 
	#	then 
	#		exit 0 
	#	fi
	#done 
	while true; do 
		sleep $2

		dateLog=$(date +"%s.%3N")
#=======
#		dateLog=$(date + "%s.%3N")
#>>>>>>> 9a238655387ecbca8504bca6826115938869ff16
		for file in /sys/kernel/debug/kvm/$statDir/*
		do
			dirH="no"

			if [ -f $file ]; 
			then
			 	dirH="y"
			fi

			if [ "$dirH" == "y" ];
			then 
				readValue=$(cat $file)
				filename=$(echo $file | awk -F'/' '{print $7}')

				if [ -f "cloudRep_profiling/cloudRep_$randNUM/$vmName/$filename" ]; then
					prevValue=$(cat cloudRep_profiling/cloudRep_$randNUM/$vmName/$filename)
					if (( readValue > prevValue )); then
						echo "$dateLog update to $file: $readValue" >> cloudRep_profiling/cloudRep_$randNUM/vm_$vmCounter/kvm_events_$vmName
					fi
				else
					mkdir -p cloudRep_profiling/cloudRep_$randNUM/$vmName 
					touch cloudRep_profiling/cloudRep_$randNUM/$vmName/$filename
				fi 
				echo $readValue > cloudRep_profiling/cloudRep_$randNUM/$vmName/$filename 
			fi

		done

		free_mem=$(virsh --connect qemu:///system dommemstat $vmName | grep unused | awk '{print $2}')
		total_mem=$(virsh --connect qemu:///system dommemstat $vmName | grep available | awk '{print $2}')
		diff=$(expr $total_mem - $free_mem)
#<<<<<<< HEAD
#		echo "Free memory $free_mem"
#=======
#		echo "Free memory $free_mem"
#>>>>>>> 9a238655387ecbca8504bca6826115938869ff16

		echo "$dateLog $diff" >> cloudRep_profiling/cloudRep_$randNUM/vm_$vmCounter/memory_usage 


		watch=$(cat cloudRep_profiling/cloudRep_$randNUM/watch)
		if [ "$watch" == "false" ]; 
		then 
			exit 0 
		fi
	done  
}


for ((vmCounter = 0; vmCounter <= nbVMs; vmCounter++));
do
	mkdir -p cloudRep_profiling/cloudRep_$randNUM/vm_$vmCounter
	echo ${listVMs[$vmCounter]} > cloudRep_profiling/cloudRep_$randNUM/vm_$vmCounter/vmName
done

timePassed=0
linuxver=$(uname -r | awk -F'-' '{print $1}')
echo "true" > cloudRep_profiling/cloudRep_$randNUM/watch

#Get cpu%, disk%, disk tx request, disk rx request, network tx request, network rx, request
#<<<<<<< HEAD
nohup  virt-top --csv cloudRep_profiling/cloudRep_$randNUM/$1output.csv --block-in-bytes --connect qemu:///system -d $period --script &
#=======
#nohup virt-top --csv cloudRep_profiling/cloudRep_$randNUM/$1output.csv --connect qemu:///system -d $period &
#>>>>>>> 9a238655387ecbca8504bca6826115938869ff16
toKill=$!

#Get KVM events (context switches)

	echo "Watching KVM events for every VMs"
	for ((vmCounter = 0; vmCounter < nbVMs; vmCounter++));
	do
		#The code here is different depending on your Linux Kernel Version
		echo "Watching ${listVMs[$vmCounter]}"
		logEventforVM ${listVMs[$vmCounter]} $period & 
#<<<<<<< HEAD
#		logMemoryforVM ${listVMs[$vmCounter]} &	
	done

	while [ "$(echo "$timePassed>$observationtime" | bc)" -ne "1" ]; do
	sleep $period
	timePassed=$(echo "$timePassed+$period" | bc)
#	echo "$timePassed"
	echo "$timePassed>$observationtime" | bc
#=======
#		logMemoryforVM ${listVMs[$vmCounter]} &	
#	done

#while [ "$timePassed" -le "$observationtime" ]; do
#	sleep $period
#	timePassed=$((timePassed+period))
#>>>>>>> 9a238655387ecbca8504bca6826115938869ff16
done

kill -9 $toKill 
#stop watch on kvm files 
#<<<<<<< HEAD
echo "false" > cloudRep_profiling/cloudRep_$randNUM/watch
#=======
echo "false" >> cloudRep_profiling/cloudRep_$randNUM/watch
#>>>>>>> 9a238655387ecbca8504bca6826115938869ff16
echo 0 >/sys/kernel/debug/tracing/events/kvm/enable

#Parsing virt-top results 
echo "Profiling successfully finished."
echo "KVM events and log usage in cloudRep_profiling/cloudRep_$randNUM/<vmName>"
echo "Global resource usage in $1output.csv"
echo "[TODO] You can now run the parser on the files in cloudRep_profiling/cloudRep_$randNUM"
echo "Collecting kvm_trace_pipe .... (Upon completion, check the log in cloudRep_profiling/cloudRep_$randNUM/kvm_log_trace"
{
    sleep 60
    kill $$
} &
cat /sys/kernel/debug/tracing/trace_pipe > cloudRep_profiling/cloudRep_$randNUM/kvm_log_trace 

