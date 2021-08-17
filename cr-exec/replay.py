import csv
import time
import os
import subprocess
from itertools import islice

#########################################
#########################################
which_row_of_replay_csv=1
which_row_of_cpu_over_head_csv=8
#########################################
#########################################

vm_time=[]
with open('record.csv', newline='', encoding='utf-8') as f:
    reader = csv.reader(f)
    for row in islice(reader, 1, None):
        vm=[[0 for x in range(6)] for y in range(10)]
        c=0
        for i in range(19,len(row)-1):
            if (i-19)%10==0:
                vm[c][0]=row[i+2]
                vm[c][1]=row[i+4]
                vm[c][2]=row[i+5]
                vm[c][3]=row[i+6]
                vm[c][4]=row[i+7]
                vm[c][5]=row[i+8]
                c+=1
        vm_time.append(vm)
#print(vm_time)
def print_log():
    for vv in vm_time:
        for i in range(0,len(vv)):
            #print("vm"+str(i+1),end=' ')
            for j in vv[i]:
                print(j,' ',end='')
            print()
        print()

def cpu_of_vm(i):
    cpu=[]
    if i==0:
        i=1
    for vv in vm_time:
        cpu.append(vv[i-1][0])
    return cpu
def network_rx_of_vm(i):
    rx=[]
    if i==0:
        i=1
    for vv in vm_time:
        rx.append(vv[i-1][4])
    return rx
def network_tx_of_vm(i):
    tx=[]
    if i==0:
        i=1
    for vv in vm_time:
        tx.append(vv[i-1][5])
    return tx

def disk_read_of_vm(i):
    read=[]
    if i==0:
        i=1
    for vv in vm_time:
        read.append(vv[i-1][2])
    return read
def disk_write_of_vm(i):
    write=[]
    if i==0:
        i=1
    for vv in vm_time:
        write.append(vv[i-1][3])
    return write
def cpu_change(i):
    if i == "":
        i=-1
    u=float(i)
    ii=u*100000
    ii=int(ii)
    if ii<=1000:
        ii=1000
    cmd="echo "+str(ii)+" >/sys/fs/cgroup/cpu/replay/cpu.cfs_quota_us "
    subprocess.Popen(cmd,shell=True,stdout=None)

def disk_io_change(i,j):
    if i == '0' or i == '':
        i = '1000'
    if j=='0' or j == '':
        j= '1000'
    j=str(int(j))
    cmd1="echo " + "8:0 " + i +" > /sys/fs/cgroup/blkio/replay/blkio.throttle.read_bps_device  "
    cmd2="echo " + "252:0 " + j +" > /sys/fs/cgroup/blkio/replay/blkio.throttle.write_bps_device  "
    #cmd3="cat /sys/fs/cgroup/blkio/replay/blkio.throttle.write_bps_device"
    subprocess.Popen(cmd1,shell=True,stdout=None)
    subprocess.Popen(cmd2,shell=True,stdout=None)
def network_change(i):
    if i == '':
        return 
    j=int(i)
    j=int((j*27)/1024)
    if j == 0:
        j=10000
    i = str(j)
    cmd="sudo tc class change dev ens3 parent 10: classid 10:1 htb rate "+ i+"kbit"
    subprocess.Popen(cmd,shell=True,stdout=None)

cpu=cpu_of_vm(which_row_of_replay_csv)
tx=network_tx_of_vm(which_row_of_replay_csv)
rx=network_rx_of_vm(which_row_of_replay_csv)
read=disk_read_of_vm(which_row_of_replay_csv)
write=disk_write_of_vm(which_row_of_replay_csv)


disk_io_change("10240","10240")
network_change("10000")
network_tx_cmd = "sudo cgexec -g net_cls:replay iperf -c 192.168.10.1 -p 5001 -t 600 -u -b 50mb"
network_rx_cmd = "sudo iperf -s -u"
disk_write_cmd = "sudo cgexec -g blkio:replay fio -name iops -rw=randwrite -bs=4m -runtime=600  -filename /dev/vda -direct=1 --ioengine=libaio  >/dev/null"
disk_read_cmd = "sudo cgexec -g blkio:replay fio -filename=/dev/sda2 -direct=1 -rw=read  -bs=4k -size=1G  -name=seqread  -runtime=200 > /dev/null"
a=subprocess.Popen(disk_write_cmd,shell=True,stdout=None)
b=subprocess.Popen(disk_read_cmd,shell=True,stdout=None)
c=subprocess.Popen("sudo ./memory_replay/a.out ",shell=True,stdout=None)
d=subprocess.Popen(network_rx_cmd,shell=True,stdout=None)
#f=subprocess.Popen("sudo cgexec -g cpu:replay python3 ./cpu_replay/fake_cpu.py",shell=True,stdout=None)
time.sleep(3)
e=subprocess.Popen(network_tx_cmd,shell=True,stdout=None)

c=0
for i,d,j,k,l in zip(tx,rx,read,write,cpu):
    start=time.time()
    disk_io_change(j,k)
    network_change(i)
    #cpu_change(l)
    end=time.time()
    if end-start<1:
        time.sleep(1-(end-start))

