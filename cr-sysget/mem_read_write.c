/**
 * Author: Djob Mvondo
 */
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

int array_to_test[] = {1024,1024*10,1024*1024,1024*1024*100,1024*1024*500};
struct timespec start,end;



void print_time(int number, struct timespec start, struct timespec end)
{	
 	printf("[%d bytes] Time taken: %.5f seconds\n",number,
	 	((double)end.tv_sec + 1.0e-9*end.tv_nsec) - ((double)start.tv_sec + 1.0e-9*start.tv_nsec));


}

void mem_read_write(int counter){
	
		int j = 0;
		int sizeof_allocate = 0; 
		int* to_allocate= malloc(sizeof(int)*counter); 

		for(j = 0;j<counter;j++)
		{
			to_allocate[j] = 0;
		}

		//to_allocate = malloc(sizeof(int)*(counter/sizeof(int)));
		//initialise read and write
		
		//memset(to_allocate,0,counter*sizeof(to_allocate[0]));

		//sizeof_allocate = sizeof(to_allocate)/sizeof(to_allocate[0]);

		clock_gettime(CLOCK_MONOTONIC,&start);
		for(j = 0; j<counter; j++)
		{
			to_allocate[j] = to_allocate[rand()%counter]+1;
		}

		clock_gettime(CLOCK_MONOTONIC,&end);
		print_time(counter,start,end);
}

int main(void){
	int **to_allocate;

	int counter = 0;
	int testing_size = sizeof(array_to_test)/sizeof(array_to_test[0]);
	to_allocate = malloc(sizeof(int*)*testing_size);
	
	srand(time(NULL));
	
	printf("========== CR-sysget - mem_rand_read_write ==========\n");
	
	for(counter=0; counter<testing_size; counter++){
		
		mem_read_write(array_to_test[counter]);
	}

}