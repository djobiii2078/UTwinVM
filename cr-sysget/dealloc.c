#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

void print_time(int number, struct timespec start, struct timespec end)
{	
 	printf("[%d bytes] Time taken: %.5f seconds\n",
	 	((double)end.tv_sec + 1.0e-9*end.tv_nsec) - ((double)start.tv_sec + 1.0e-9*start.tv_nsec));


}

int main(void){
	int[] array_to_test = [1024,1024*4,1024*1024];
	struct timespect start={0,0}, end={0,0};
	int *to_allocate;

	int counter = 0;
	int testing_size = sizeof(array_to_test)/sizeof(array_to_test[0]);
	int j = 0;
	int sizeof_allocate = 0; 
	srand(time(NULL));
	
	printf("========== CR-sysget - mem_rand_read_write ==========");
	
	for(counter=0; counter<testing_size; counter++){
		to_allocate = malloc(sizeof(int)*(array_to_test[counter]/sizeof(int)));
		//initialise read and write
		to_allocate = {0}
		
		sizeof_allocate = sizeof(to_allocate)/sizeof(sizeof_allocate[0]);

		clock_gettime(CLOCK_MONOTONIC,start);
		for(j = 0; j<sizeof_allocate; j++)
		{
			to_allocate[j] = to_allocate[rand()%sizeof_allocate]+1;
		}

		clock_gettime(CLOCK_MONOTONIC,end);
		print_time(array_to_test[counter],start,end);
		free(to_allocate);

	}

}