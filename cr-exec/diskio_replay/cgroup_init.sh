#!/bin/bash
sudo mkdir -p /sys/fs/cgroup/blkio/replay
sudo echo "8:0 1048576" > /sys/fs/cgroup/blkio/replay/blkio.throttle.write_bps_device
sudo echo "8:0 1048576" > /sys/fs/cgroup/blkio/replay/blkio.throttle.read_bps_device
