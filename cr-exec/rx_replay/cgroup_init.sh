#!/bin/bash
NUM=$2
BASE=60
#Major 10, minor from 10 to $NUM-1
TARGET=`expr $BASE + $NUM - 1`
sudo tc qdisc add dev $1 root handle 10: htb
for i in $(seq $BASE 1 $TARGET)
do
	sudo mkdir -p /sys/fs/cgroup/net_cls/replay$i
	sudo sh -c "echo 0x1000$i > /sys/fs/cgroup/net_cls/replay$i/net_cls.classid"
	sudo tc class add dev $1 parent 10: classid 10:$i htb rate 10mbit
	sudo tc filter add dev $1 parent 10:$i protocol ip prio 10 handle 1: cgroup
done

