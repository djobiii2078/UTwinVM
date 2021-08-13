CC=gcc
all: mem_alloc_set mem_read_write fork_time

fork_time: fork_time.c 
				$(CC) -o fork_time.o fork_time.c 
mem_alloc_set: mem_alloc_set.c 
				$(CC) -o mem_alloc_set.o mem_alloc_set.c 
mem_read_write: mem_read_write.c 
				$(CC) -o mem_read_write.o mem_read_write.c 
clean:
		rm -f *.o 


