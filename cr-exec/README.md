
# replay_tools

The UTwinVM replay tools need cgroup support

pre-requisites: linux cgroup support -> cpu, blkio, network_cls

### For each VM client:

1. Init all CPU network and blkio cgroup
```
sh cpu_replay/cgroup_init.sh
sh tx/cgroup_init.sh $nic_interface #for example, sh tx/cgroup_init.sh ens3
sh diskio_replay/cgroup_init.sh
```

2. Set up for script
```
Use fio for disk io replay
Read: fio just randomly reads the system disk. 
Write: fio write may cause the problem, so for each VM, we have to mount an unformatted blk device. 
For example, mount an extra blk device as /dev/sda2 
Please remember to change the script in disk_io_change, the cgroup needs the major:minor number of such block devices for write restriction. 
Major and Minor numbers can be obtained by using lsblk. Example: 252:0 for block device and 8:0 for system disk. 

Network:
For both disk and network, we are set up the maximum limited time.
By default, it is set as 600s. 

```
3. set up the origin csv
```
we have three Python scripts and a C program, and each of them will read a CSV file as input.
1. replay_final.py -> record.csv and cpu_overhead.csv
2. replay.py -> record.csv
3. memory/memory_replay.c -> memory_record

for all Python scripts, you have to tell which row of virt-top output(which VM), is the one you going to replay.
The number starts with 1 instead of 0.

For example, for replay vm1, the which_row_of_replay_csv has to be 1, and which_row_of_cpu_over_head_csv has to be 1 too. 

there is a helper.py to help you check which row.

helper.py will print the data of the row you have chosen. 

for the C program, the a.out is the executable file, the provided memory_record is the file from the wasted_log_lib/analyze-results/cloudRep_{500,250,100,50,3}ms/processed/vm1/mem_usage_vm.log

all input files please put AT SAME PLACE with the scripts or executable

```
### For HOST replay rx

4. First init different cgroup
```
check replay_rx, and set up a different group.

For example, set up tagged ID starting from 10:10, 10:11, 10:12 ...

cgroup_init.sh $interface $number_of_vm_to_connect

i.e.  
rx/cgroup_init.sh br1 10

the tagged ID will start from 10:10 to 10:20
and this info will be used in host_rx.py

at host_rx.py
set up:
    number_of_machines=6
    cgroup_num=10
    port=5001
    ip=81

then it will set up an iperf server and client that connect to each of those 10 VMs.

```
5. setup csv
```
host_rx.py uses rx.csv as input, which is the original log. 
```


6. replay without cpu
```
To replay without cpu
Please check replay_rx and also change the IP address of the guest VM that is going to replay.

please run those scripts on all machines at the same time, and try to use the broadcast command.

Inside VM: 
sudo sh python3 replay.py
On the Host:
sudo sh python3 host_rx.py

In the meantime, you should start recording also. 

After running REMEMBER TO CLEAN THE MACHINES! USE KILL or KILLALL.

```

7. replay with CPU
```
The record of a replay with CPU, the CSV file should be renamed as cpu_overhead.csv

which has the CPU overhead of other replays(blk, network, memory).

Please also get the row of the VM, and remember, this time, the row should be the VM that just ran the replay without CPU

instead of the original VM that we are going to replay.

you still can use the helper.py to check if you are not sure.(the number starts with 1 instead of 0)

This time, the replay CPU will minus the replay overhead first then replay.

Inside VM:
Please check host_rx.py and also change the IP address of the guest VM that going to replay.

sudo sh python3 replay_final.py
outside:
sudo sh python3 host_rx.py 
In the meanwhile, you should start recording. 


```
