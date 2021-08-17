#include <stdio.h>
#include <stdlib.h>
#include <sys/sysinfo.h>
#include <unistd.h>
#include <string.h>
static int malloc_memory=0;
//max memory replay 1TB
static void** pool;
struct MEM_INFO
{
    unsigned int total;
    unsigned int free;
    unsigned int buffers;
    unsigned int cached;
    unsigned int active;
    unsigned int inactive;
    unsigned int apages;
    unsigned int mapped;
    unsigned int shmem;
    unsigned int slab;
    unsigned int sr;
    unsigned int su;
    unsigned int ks;
    unsigned int pt;
};
typedef struct MEM_INFO Mem_info;
int current_sys_memory()
{
	Mem_info *o=malloc(sizeof(Mem_info));
	FILE* fpMemInfo = fopen("/proc/meminfo", "r");
	if (NULL == fpMemInfo)
    	{
        	return 0;
    	}
    	int i = 0;
    	int value;
    	char name[10240];
	char name2[1024];
    	char line[1024];
    	int nFiledNumber = 3;
    	int nMemberNumber = 14;
   
    	while (fgets(line, sizeof(line) - 1, fpMemInfo))
    	{
		sscanf(line,"%s %u %s",name ,&value, name2);
		if (!strcmp(name, "MemTotal:"))
        	{
         	   	++i;
            		o->total = value;
        	}
        	else if (!strcmp(name, "MemAvailable:"))
        	{
            		++i;
            		o->free = value;
        	}
        	else if (!strcmp(name, "Buffers:"))
        	{
            		++i;
 		        o->buffers = value;
        	}
        	else if (!strcmp(name, "Cached:"))
	        {
	  		++i;
	                o->cached = value;
        	}
		else if (!strcmp(name, "Active:"))
                {
                        ++i;
                        o->active = value;
                }
		else if (!strcmp(name, "Inactive:"))
                {
                        ++i;
                        o->inactive = value;
                }
		else if (!strcmp(name, "AnonPages:"))
                {
                        ++i;
                        o->apages = value;
                }
		else if (!strcmp(name, "Mapped:"))
                {
                        ++i;
                        o->mapped = value;
                }
		else if (!strcmp(name, "Shmem:"))
                {
                        ++i;
                        o->shmem = value;
                }
		else if (!strcmp(name, "Slab:"))
                {
                        ++i;
                        o->slab = value;
                }
		else if (!strcmp(name, "SReclaimable:"))
                {
                        ++i;
                        o->sr = value;
                }
		else if (!strcmp(name, "SUnreclaim"))
                {
                        ++i;
                        o->su = value;
                }
		else if (!strcmp(name, "KernelStack:"))
                {
                        ++i;
                        o->ks = value;
                }
		else if (!strcmp(name, "PageTables:"))
                {
                        ++i;
                        o->pt = value;
                }

        	if (i == nMemberNumber)
        	{
            		break;
        	}
    	}
	fclose(fpMemInfo);
	return o->total - (o->free - o->buffers - o->cached - o->active - o->inactive - o->apages - o->mapped - o->shmem - o->slab - o->sr - o->su - o->ks - o->pt);
}

void add_memory(int __memory)
{
	int i;
	for(i=malloc_memory; i<malloc_memory+__memory;i++)
	{
		pool[i]= malloc(1024*1024*sizeof(void));
		memset(pool[i],1,1024*1024*sizeof(void));
	}
	malloc_memory+=__memory;
}
void remove_memory(int __memory)
{
	while(malloc_memory!=__memory)
	{
		malloc_memory--;
		memset(pool[malloc_memory],0,1024*1024*sizeof(void));
		free(pool[malloc_memory]);
	}
}
void free_all()
{
	remove_memory(0);
}


int main()
{
	pool = malloc(1024*1024*10*sizeof(void));
	FILE * fp;
    	char * line = NULL;
    	size_t len = 0;
    	ssize_t read;
	struct sysinfo s_info;
	int replay_memory,sys_memory,__memory;
	sys_memory=current_sys_memory(s_info);
    	fp = fopen("./memory_record", "r");
   	if (fp == NULL)
	{
		printf("No memory_record file, exit...\n");
        	exit(EXIT_FAILURE);
	}
    	while ((read = getline(&line, &len, fp)) != -1) {
		replay_memory=atoi(line)*0.9;
		if(replay_memory<sys_memory){ free_all();goto finish;}
		__memory=(replay_memory-sys_memory)/(sizeof(void)*1024);
		
		if(__memory==0) goto finish;
		if(__memory-malloc_memory >0)
			add_memory(__memory-malloc_memory);
		else if (__memory-malloc_memory <0)
			remove_memory(__memory);
		else
			goto finish;
finish:
		usleep(1000000);
    	}

    	fclose(fp);
	if (line)
		free(line);
	return 0;
}

