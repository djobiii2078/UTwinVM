#!/bin/bash

#Measure the time
#to send packet of 1K and 4K sizes.

iperf -c $1 -l $2 -t 300 