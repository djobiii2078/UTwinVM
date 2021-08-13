#!/bin/bash

dd if=/dev/urandom of=10.bin.o bs=1M count=10 iflag=direct | pv


