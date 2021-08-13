#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<unistd.h>
#include<sys/wait.h>

struct timespec start, end;

float print_time(struct timespec start, struct timespec end)
{	
 	return ((double)end.tv_sec + 1.0e-9*end.tv_nsec) - ((double)start.tv_sec + 1.0e-9*start.tv_nsec);

}

int main(void){
	printf("======= CR-sysget fork_time ========");
	pid_t pid_proc;

	clock_gettime(CLOCK_MONOTONIC,&start);

	if(!(pid_proc=fork()))
	{
		clock_gettime(CLOCK_MONOTONIC,&end);
		printf("Time taken to init this tasks %.9f",print_time(start,end));
		exit(0);
	}

	if(pid_proc < 0)
	{
		perror("Fork()");
		waitpid(pid_proc,NULL,0);
	}

}