#!/bin/bash
while ((1))
do
        sleep 0.25
        free -m | awk 'NR==2{printf "%.2f\n",$3*100/$2 }' >> mem
done

