#include "RtTaskStats.h"

int main( int argc, char *argv[])
{
	unsigned int i, j;
   	shared_memory = (SharedMemStruct*)rtai_malloc(nam2num(SHARED_MEM_NAME), SHARED_MEM_SIZE);
	if (shared_memory == NULL)
	{
		printf("rtai_malloc() failed (maybe /dev/rtai_shm is missing)!\n");
		return -1;
   	}
	printf("-----------------------------------------  RT_TASK_STATS ----------------------------------------\n");
        printf("SHARED_MEMORY_SIZE: 		%lu\n", sizeof(SharedMemStruct));
        printf("SYSTEM_TIME:		 	%llu\n", (long long unsigned int)shared_memory->rt_tasks_data.current_system_time);
        printf("MAX_NUM_OF_CPUS: 		%u\n", MAX_NUM_OF_CPUS);	
        printf("MAX_NUM_OF_THREADS_PER_CPU: 	%u\n", MAX_NUM_OF_THREADS_PER_CPU);	
        printf("TOTAL_NUM_OF_RT_TASKS: 		%u\n", shared_memory->rt_tasks_data.num_of_total_rt_tasks);	

	for (i = 0; i < MAX_NUM_OF_CPUS; i ++)
	{
		for (j = 0; j < MAX_NUM_OF_THREADS_PER_CPU; j++)
		{	
			printf("---------------------  CPU %u  THREAD %u  -----------------\n", i, j);
			printf("CPU_RT_TASK_PERIOD: 		%lu\n", (long unsigned int) shared_memory->rt_tasks_data.cpu_rt_task_data[i].rt_task_period);	
	       		printf("NUM_OF_TASKS_AT_CPU: 		%u\n", shared_memory->rt_tasks_data.cpu_rt_task_data[i].num_of_rt_tasks_at_cpu);			
       		 	printf("NUM_OF_TASKS_AT_THREAD: 	%u\n", shared_memory->rt_tasks_data.cpu_rt_task_data[i].cpu_thread_rt_task_data[j].num_of_rt_tasks_at_cpu_thread);
       		 	printf("PERIOD_RUN_TIME - MAX: 		%lu\n", (long unsigned int)shared_memory->rt_tasks_data.cpu_rt_task_data[i].cpu_thread_rt_task_data[j].max_period_run_time);		
       		 	printf("JITTER - MAX_POSITIVE: 		%lu\n", (long unsigned int)shared_memory->rt_tasks_data.cpu_rt_task_data[i].cpu_thread_rt_task_data[j].max_positive_jitter);		
	 		printf("JITTER - MAX_NEGATIVE: 		%lu\n", (long unsigned int)shared_memory->rt_tasks_data.cpu_rt_task_data[i].cpu_thread_rt_task_data[j].max_negative_jitter);					
		}		
	}
	printf("---------------------------------------------------------------------------------------------------------------\n");
	return 0;
}
