#!/bin/bash

sudo mkdir -p /sys/fs/cgroup/net_cls/replay
sudo sh -c "echo 0x100001 > /sys/fs/cgroup/net_cls/replay/net_cls.classid"
sudo tc qdisc add dev $1 root handle 10: htb
sudo tc class add dev $1 parent 10: classid 10:1 htb rate 10mbit
sudo tc filter add dev $1 parent 10: protocol ip prio 10 handle 1: cgroup

