import csv
import time
import os
import subprocess
from itertools import islice
vm_time=[]
with open('rx.csv', newline='', encoding='utf-8') as f:
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
def network_rx_of_vm(i):
    rx=[]
    if i==0:
        i=1
    for vv in vm_time:
        rx.append(vv[i-1][4])
    return rx

def network_change(i,major,minor):
    if i == '':
        return 
    j=int(i)
    j=int((j*27)/1024)
    if j == 0:
        j=1000
    i = str(j)
    cmd="sudo tc class change dev br1 parent 10: classid "+str(major)+":"+str(minor)+" htb rate "+ i+"kbit"
    subprocess.Popen(cmd,shell=True,stdout=None)
c=0


def init():
    number_of_machines=2
    cgroup_num=60
    port=5001
    ip=51
    for i in range(0,number_of_machines):
        print(port)
        network_rx_cmd = "sudo iperf -s -u -p "+str(port)
        subprocess.Popen(network_rx_cmd,shell=True,stdout=None)
        port+=1
    time.sleep(3)
    for i in range(0,number_of_machines):
        network_tx_cmd = "sudo cgexec -g net_cls:replay"+str(cgroup_num)+" iperf -c 192.168.10."+str(ip)+"  -t 600 -u -b 100mb"
        print(network_tx_cmd)
        subprocess.Popen(network_tx_cmd,shell=True,stdout=None)
        ip+=1
        cgroup_num+=1

rx=[]

for i in range(1,3):
    rx.append(network_rx_of_vm(i))
print("Please remember sudo")
init()
major=10
for i in range(0,len(rx[0])):
    start=time.time()
    minor=60
    for j in range(0,len(rx)):
        network_change(rx[j][i],major,minor+j)
    end=time.time()
    time.sleep(1-(end-start))
    end=time.time() 

