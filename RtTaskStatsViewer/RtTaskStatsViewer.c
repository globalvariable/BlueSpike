#include "RtTaskStatsViewer.h"

static RtTasksData 			*rt_tasks_data = NULL; 

int main( int argc, char *argv[])
{
	unsigned int i, j, k;
	rt_tasks_data = (RtTasksData*)rtai_malloc(nam2num(RT_TASKS_DATA_SHM_NAME), 0);
	if (rt_tasks_data == NULL) 
		return print_message(ERROR_MSG ,"RtTaskStatsViewer", "RtTaskStatsViewer", "main", "rt_tasks_data == NULL.");

	if (argc == 1)
	{
		printf("-----------------------------------------  RT_TASK_STATS ----------------------------------------\n");
		printf("SYSTEM_TIME:		 	%llu\n", (long long unsigned int)rt_tasks_data->current_system_time);
		printf("MAX_NUM_OF_CPUS: 		%u\n", MAX_NUM_OF_CPUS);	
		printf("MAX_NUM_OF_CPU_THREADS_PER_CPU: 	%u\n", MAX_NUM_OF_CPU_THREADS_PER_CPU);	
		printf("MAX_NUM_OF_RT_TASKS_PER_CPU_THREAD: 	%u\n", MAX_NUM_OF_RT_TASKS_PER_CPU_THREAD);	
		printf("TOTAL_NUM_OF_RT_TASKS: 		%u\n", rt_tasks_data->num_of_total_rt_tasks);	

		for (i = 0; i < MAX_NUM_OF_CPUS; i ++)
		{
			printf("===============================================\n");
			printf("------------------------------------ CPU %u ------------------------------------\n", i);
			printf("CPU_RT_TASK_PERIOD:		%lu\n", (long unsigned int) rt_tasks_data->cpus_rt_task_data[i].rt_task_period);
			printf("NUM_OF_RT_TASKS_AT_CPU:	%u\n", rt_tasks_data->cpus_rt_task_data[i].num_of_rt_tasks_at_cpu);				
			for (j = 0; j < MAX_NUM_OF_CPU_THREADS_PER_CPU; j++)
			{
				printf("----------------------------------  CPU %u THREAD %u ----------------------\n", i, j);
		       		printf("NUM_OF_RT_TASKS_AT_CPU_THREAD: 	%u\n", rt_tasks_data->cpus_rt_task_data[i].cpu_threads_rt_data[j].num_of_rt_tasks_at_cpu_thread);
				for(k = 0; k < MAX_NUM_OF_RT_TASKS_PER_CPU_THREAD; k++)
				{
					printf("---------------------  CPU %u  THREAD %u TASK %u -----------------\n", i, j, k);
		       		 	printf("TASK_NAME: 			%s\n", rt_tasks_data->cpus_rt_task_data[i].cpu_threads_rt_data[j].cpu_thread_tasks_rt_data[k].task_name);		
		       		 	printf("TASK_RUN_TIME - MAX: 		%u\n", rt_tasks_data->cpus_rt_task_data[i].cpu_threads_rt_data[j].cpu_thread_tasks_rt_data[k].max_task_run_time);		
		       		 	printf("JITTER - MAX_POSITIVE: 		%u\n", rt_tasks_data->cpus_rt_task_data[i].cpu_threads_rt_data[j].cpu_thread_tasks_rt_data[k].max_positive_jitter);		
			 		printf("JITTER - MAX_NEGATIVE: 		%u\n", rt_tasks_data->cpus_rt_task_data[i].cpu_threads_rt_data[j].cpu_thread_tasks_rt_data[k].max_negative_jitter);	
		       		 	printf("JITTER - POSITIVE_THRES:	%u\n", rt_tasks_data->cpus_rt_task_data[i].cpu_threads_rt_data[j].cpu_thread_tasks_rt_data[k].positive_jitter_threshold);		
			 		printf("JITTER - NEGATIVE_THRES:	%u\n", rt_tasks_data->cpus_rt_task_data[i].cpu_threads_rt_data[j].cpu_thread_tasks_rt_data[k].negative_jitter_threshold);	
		       		 	printf("JITTER - POSITIVE_ALARM:	%u\n", rt_tasks_data->cpus_rt_task_data[i].cpu_threads_rt_data[j].cpu_thread_tasks_rt_data[k].num_of_positive_jitter_exceeding_threshold);		
			 		printf("JITTER - NEGATIVE_ALARM:	%u\n", rt_tasks_data->cpus_rt_task_data[i].cpu_threads_rt_data[j].cpu_thread_tasks_rt_data[k].num_of_negative_jitter_exceeding_threshold);
				}					
			}		
		}
		printf("---------------------------------------------------------------------------------------------------------------\n");
	}
	else if (argc == 2)
	{
		if (strcmp(argv[1], "clear_all" ) == 0)
		{
			for (i = 0; i < MAX_NUM_OF_CPUS; i ++)
			{
				for (j = 0; j < MAX_NUM_OF_CPU_THREADS_PER_CPU; j++)
				{	
					for(k = 0; k < MAX_NUM_OF_RT_TASKS_PER_CPU_THREAD; k++)
					{
		       		 		rt_tasks_data->cpus_rt_task_data[i].cpu_threads_rt_data[j].cpu_thread_tasks_rt_data[k].max_task_run_time = 0;	
						rt_tasks_data->cpus_rt_task_data[i].cpu_threads_rt_data[j].cpu_thread_tasks_rt_data[k].max_positive_jitter = 0;		
						rt_tasks_data->cpus_rt_task_data[i].cpu_threads_rt_data[j].cpu_thread_tasks_rt_data[k].max_negative_jitter = 0;
						rt_tasks_data->cpus_rt_task_data[i].cpu_threads_rt_data[j].cpu_thread_tasks_rt_data[k].num_of_positive_jitter_exceeding_threshold = 0;		
						rt_tasks_data->cpus_rt_task_data[i].cpu_threads_rt_data[j].cpu_thread_tasks_rt_data[k].num_of_negative_jitter_exceeding_threshold = 0;		
					}			
				}		
			}			
		}		
		else
		{
			print_message(ERROR_MSG ,"RtTaskStatsViewer", "RtTaskStatsViewer", "main", "Unknown argument.\n");			
		}
	}
	else if (argc == 3)
	{
		if (strcmp(argv[1], "task" ) == 0)
		{
			for (i = 0; i < MAX_NUM_OF_CPUS; i ++)
			{
				for (j = 0; j < MAX_NUM_OF_CPU_THREADS_PER_CPU; j++)
				{	
					for(k = 0; k < MAX_NUM_OF_RT_TASKS_PER_CPU_THREAD; k++)
					{
						if ((strcmp(argv[2], rt_tasks_data->cpus_rt_task_data[i].cpu_threads_rt_data[j].cpu_thread_tasks_rt_data[k].task_name) != 0))
							continue;
						printf("---------------------  CPU %u  THREAD %u TASK %u -----------------\n", i, j, k);
			       		 	printf("TASK_NAME: 			%s\n", rt_tasks_data->cpus_rt_task_data[i].cpu_threads_rt_data[j].cpu_thread_tasks_rt_data[k].task_name);		
			       		 	printf("TASK_RUN_TIME - MAX: 		%u\n", rt_tasks_data->cpus_rt_task_data[i].cpu_threads_rt_data[j].cpu_thread_tasks_rt_data[k].max_task_run_time);		
			       		 	printf("JITTER - MAX_POSITIVE: 		%u\n", rt_tasks_data->cpus_rt_task_data[i].cpu_threads_rt_data[j].cpu_thread_tasks_rt_data[k].max_positive_jitter);		
				 		printf("JITTER - MAX_NEGATIVE: 		%u\n", rt_tasks_data->cpus_rt_task_data[i].cpu_threads_rt_data[j].cpu_thread_tasks_rt_data[k].max_negative_jitter);	
			       		 	printf("JITTER - POSITIVE_THRES:	%u\n", rt_tasks_data->cpus_rt_task_data[i].cpu_threads_rt_data[j].cpu_thread_tasks_rt_data[k].positive_jitter_threshold);		
				 		printf("JITTER - NEGATIVE_THRES:	%u\n", rt_tasks_data->cpus_rt_task_data[i].cpu_threads_rt_data[j].cpu_thread_tasks_rt_data[k].negative_jitter_threshold);	
			       		 	printf("JITTER - POSITIVE_ALARM:	%u\n", rt_tasks_data->cpus_rt_task_data[i].cpu_threads_rt_data[j].cpu_thread_tasks_rt_data[k].num_of_positive_jitter_exceeding_threshold);		
				 		printf("JITTER - NEGATIVE_ALARM:	%u\n", rt_tasks_data->cpus_rt_task_data[i].cpu_threads_rt_data[j].cpu_thread_tasks_rt_data[k].num_of_negative_jitter_exceeding_threshold);
					}			
				}		
			}
			print_message(ERROR_MSG ,"RtTaskStatsViewer", "RtTaskStatsViewer", "main", "Unknown task name.\n");			
		}		
		else if  (strcmp(argv[1], "clear_cpu" ) == 0)
		{
			if ((atoi(argv[2]) < MAX_NUM_OF_CPUS) && (atoi(argv[2]) >= 0))
			{
				for (j = 0; j < MAX_NUM_OF_CPU_THREADS_PER_CPU; j++)
				{	
					for(k = 0; k < MAX_NUM_OF_RT_TASKS_PER_CPU_THREAD; k++)
					{
						rt_tasks_data->cpus_rt_task_data[atoi(argv[2])].cpu_threads_rt_data[j].cpu_thread_tasks_rt_data[k].task_name[0] = 0;	
			       		 	rt_tasks_data->cpus_rt_task_data[atoi(argv[2])].cpu_threads_rt_data[j].cpu_thread_tasks_rt_data[k].max_task_run_time = 0;		
			       		 	rt_tasks_data->cpus_rt_task_data[atoi(argv[2])].cpu_threads_rt_data[j].cpu_thread_tasks_rt_data[k].max_positive_jitter = 0;		
				 		rt_tasks_data->cpus_rt_task_data[atoi(argv[2])].cpu_threads_rt_data[j].cpu_thread_tasks_rt_data[k].max_negative_jitter = 0;	
			       		 	rt_tasks_data->cpus_rt_task_data[atoi(argv[2])].cpu_threads_rt_data[j].cpu_thread_tasks_rt_data[k].positive_jitter_threshold = 0;		
				 		rt_tasks_data->cpus_rt_task_data[atoi(argv[2])].cpu_threads_rt_data[j].cpu_thread_tasks_rt_data[k].negative_jitter_threshold = 0;	
			       		 	rt_tasks_data->cpus_rt_task_data[atoi(argv[2])].cpu_threads_rt_data[j].cpu_thread_tasks_rt_data[k].num_of_positive_jitter_exceeding_threshold = 0;		
				 		rt_tasks_data->cpus_rt_task_data[atoi(argv[2])].cpu_threads_rt_data[j].cpu_thread_tasks_rt_data[k].num_of_negative_jitter_exceeding_threshold = 0;
					}			
				}				
			}
			else
			{
				print_message(ERROR_MSG ,"RtTaskStatsViewer", "RtTaskStatsViewer", "main", "Inconvenient cpu num(./RtTaskStatsViewer clear_cpu XX).\n");			
			}	
		}
		else 
		{
			print_message(ERROR_MSG ,"RtTaskStatsViewer", "RtTaskStatsViewer", "main", "Unknown argument.\n");			
		}		
	}
	else if  (argc == 4)
	{
		if  (strcmp(argv[1], "clear_cpu_thread" ) == 0)
		{
			if ((atoi(argv[2]) < MAX_NUM_OF_CPUS) && (atoi(argv[2]) >= 0))
			{
				if ((atoi(argv[3]) < MAX_NUM_OF_CPU_THREADS_PER_CPU) && (atoi(argv[3]) >= 0))
				{	
					for(k = 0; k < MAX_NUM_OF_RT_TASKS_PER_CPU_THREAD; k++)
					{
			       		 	rt_tasks_data->cpus_rt_task_data[atoi(argv[2])].cpu_threads_rt_data[atoi(argv[3])].cpu_thread_tasks_rt_data[k].task_name[0] = 0;
			       		 	rt_tasks_data->cpus_rt_task_data[atoi(argv[2])].cpu_threads_rt_data[atoi(argv[3])].cpu_thread_tasks_rt_data[k].max_task_run_time = 0;		
			       		 	rt_tasks_data->cpus_rt_task_data[atoi(argv[2])].cpu_threads_rt_data[atoi(argv[3])].cpu_thread_tasks_rt_data[k].max_positive_jitter = 0;		
				 		rt_tasks_data->cpus_rt_task_data[atoi(argv[2])].cpu_threads_rt_data[atoi(argv[3])].cpu_thread_tasks_rt_data[k].max_negative_jitter = 0;	
			       		 	rt_tasks_data->cpus_rt_task_data[atoi(argv[2])].cpu_threads_rt_data[atoi(argv[3])].cpu_thread_tasks_rt_data[k].positive_jitter_threshold = 0;		
				 		rt_tasks_data->cpus_rt_task_data[atoi(argv[2])].cpu_threads_rt_data[atoi(argv[3])].cpu_thread_tasks_rt_data[k].negative_jitter_threshold = 0;	
			       		 	rt_tasks_data->cpus_rt_task_data[atoi(argv[2])].cpu_threads_rt_data[atoi(argv[3])].cpu_thread_tasks_rt_data[k].num_of_positive_jitter_exceeding_threshold = 0;		
				 		rt_tasks_data->cpus_rt_task_data[atoi(argv[2])].cpu_threads_rt_data[atoi(argv[3])].cpu_thread_tasks_rt_data[k].num_of_negative_jitter_exceeding_threshold = 0;
					}			
				}
				else
				{
					print_message(ERROR_MSG ,"RtTaskStatsViewer", "RtTaskStatsViewer", "main", "Inconvenient cpu thread num(./RtTaskStatsViewer clear_cpu_thread XX XX).\n");					
				}				
			}
			else
			{
				print_message(ERROR_MSG ,"RtTaskStatsViewer", "RtTaskStatsViewer", "main", "Inconvenient cpu num(./RtTaskStatsViewer clear_cpu XX).\n");			
			}
		}
		else
		{
			print_message(ERROR_MSG ,"RtTaskStatsViewer", "RtTaskStatsViewer", "main", "Unknown argument.\n");					
		}			
	}
	else 
	{
		print_message(ERROR_MSG ,"RtTaskStatsViewer", "RtTaskStatsViewer", "main", "Too many arguments.\n");					
	}
	rtai_free(nam2num(RT_TASKS_DATA_SHM_NAME), rt_tasks_data);	
	return 0;
}
