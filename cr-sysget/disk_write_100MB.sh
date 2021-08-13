#!/bin/bash

dd if=/dev/urandom of=100.bin.o bs=1M count=100 iflag=direct | pv


