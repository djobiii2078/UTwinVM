
# replay_tools

The replay tools are very easy, leverage on cgroup. 

pre-requisites: linux cgroup support -> cpu, blkio, network_cls

### For each VM client:

1. online all cgroup
```
sh cpu_replay/cgroup_init.sh
sh tx/cgroup_init.sh $nic_interface #for example, sh tx/cgroup_init.sh ens3
sh diskio_replay/cgroup_init.sh
```

2. Set up for script
```
diskio we use cgroup to control, however we are using fio. 
For read, fio just randread the system disk. 
but for write, fio write may cause the problem, and write to a directory is not fully sufficient, 
so for each VM, we have to mount a unformatted blk device. 
For exaple, /dev/sda2 
Please remember to change the function cmd in disk_io_change, the cgroup need the major:minor number of such device for write restriction. 
you can get this using lsblk. for our case is 252:0 and 8:0 for system disk. 

Network:
Both disk and network we are using the command that can setup the time. please change it if the replay time is larger than it. by default it set as 600s

```
3. set up the origin csv
```
we have three python scripts and a C program, each of them will read a csv file as input.
1. replay_final.py -> record.csv and cpu_overhead.csv
2. replay.py -> record.csv
3. memory/memory_replay.c -> memory_record

for all python scripts, you have to tell which raw of virt-top output vm is the one you going to replay.
The number start with 1 instead of 0.

For example, for replay vm1, the which_row_of_replay_csv has to be 1, and which_row_of_cpu_over_head_csv has to be 1 too. 

there is a helper.py to help you check which raw. helper.py will print the data of the raw you have choosen. 

for C program, the a.out is the exeutable file, the memory_record is the file from the wasted_log_lib/analyze-results/cloudRep_{500,250,100,50,3}ms/processed/vm1/mem_usage_vm.log

all input files please put AT SAME PLACE with the scripts or executable

```
### For HOST replay rx

4. online different cgroup
```
check replay_rx, and set up different cgroup. For example, set up tagged ID start from 10:10, 10:11, 10:12 ...
cgroup_init.sh $interface $number_of_vm_to_connect 
i.e.  
rx/cgroup_init.sh br1 10
the tagged id will start from 10:10 to 10:20
and this will be used in host_rx.py
at host_rx.py
set up:
    number_of_machines=6
    cgroup_num=10
    port=5001
    ip=81
then it will set up a iperf server and client that connect to each of those 10 VMs.

```
5. setup csv
```
host_rx.py use rx.csv as imput, which is the original log. 
```




6. replay without cpu
```
To replay without cpu
Please checkout replay_rx and also change the IP address of the guest vm that going to replay.
please run those scripts on all machine at same time, try use boradcast command.

from inside: 
sudo sh python3 replay.py
outside:
sudo sh python3 host_rx.py

In the meanwhile you should start record also. 

After run REMEMBER TO CLEAN THE MACHINES! USE KILL or KILLALL.

```
After run REMEMBER TO CLEAN THE MACHINES! USE KILL or KILLALL.

7. replay with cpu
```
The record of replay without cpu csv file should rename as cpu_overhead.csv
which has the cpu overhead of others replay. 
Please also tell the raw of the vm, and remember, this time, the raw should be the vm that just run the replay without cpu
instead of the original vm that we are going to fake.
you still can use helper to check if you are not sure. The number start with 1 instead of 0!!!

REMEMBER TO CLEAN THE MACHINES! USE KILL or KILLALL. 


This time, the replay cpu will minus the replay overhead first then replay.
Then inside:
Please checkout host_rx.py and also change the IP address of the guest vm that going to replay.

sudo sh python3 replay_final.py
outside:
sudo sh python3 host_rx.py 
In the meanwhile you should start record. 


```
