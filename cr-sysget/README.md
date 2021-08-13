# CR-SYSGET

The component of CloudRep in charge of collecting the metrics that will characterise the virtualisation system. It evaluates four main virtualisation sub-system: 

- memory : mem_alloc_set.c and mem_read_write.c
- disk : disk_write_10MB.sh and disk_write_100MB.sh
- network : net_read.sh and net_send.sh
- CPU : fork_time.c 
 
