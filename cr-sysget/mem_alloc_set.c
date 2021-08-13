/**
 * Author: Djob Mvondo
 */
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

void print_time(int number, struct timespec start, struct timespec end)
{	
 	printf("[%d bytes] Time taken: %.5f seconds\n",number,
	 	((double)end.tv_sec + 1.0e-9*end.tv_nsec) - ((double)start.tv_sec + 1.0e-9*start.tv_nsec));

}

int main(void){
	int array_to_test[] = {1024,1024*10,1024*1024,1024*1024*100,1024*1024*500};
	struct timespec start,end={0,0};
	char *to_allocate;

	int counter = 0;
	int testing_size = sizeof(array_to_test)/sizeof(array_to_test[0]);
	
	printf("========== CR-sysget - mem_alloc ==========\n");
	for(counter=0; counter<testing_size; counter++){
		clock_gettime(CLOCK_MONOTONIC,&start);
		to_allocate = malloc(sizeof(char)*array_to_test[counter]);
		//initialise read and write
		memset(to_allocate,'0',array_to_test[counter]*sizeof(to_allocate[0]));

		clock_gettime(CLOCK_MONOTONIC,&end);
		print_time(array_to_test[counter],start,end);
		free(to_allocate);

	}

}