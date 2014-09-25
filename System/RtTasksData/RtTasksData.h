#ifndef RT_TASKS_DATA_H
#define RT_TASKS_DATA_H

#include "../Cpu/CpuInfo.h"
#include "../KernelTaskCtrl/KernelTaskCtrl.h"
#include "../TimeStamp/TimeStamp.h"
#include <rtai_shm.h>

#define MAX_NUM_OF_RT_TASKS_PER_CPU_THREAD	10
#define MAX_NUM_OF_RT_TASKS_NAME_LENGTH		40

#define RUN_TIME_BUFFER_SIZE		1000

typedef struct __CpuThreadTasksRtData
{
	char		task_name[MAX_NUM_OF_RT_TASKS_NAME_LENGTH];
	RTIME	max_task_run_time;			// each task checks its max run time in a task period and writes here if exceeds
	RTIME	task_run_time_threshold;			// each task checks its max run time in a task period and writes here if exceeds
	RTIME	max_positive_jitter;				// period appeared longer than submitted period. 
	RTIME	max_negative_jitter;				// period appeared shorter than submitted period. 
	RTIME	positive_jitter_threshold;					// determine a jitter threshold and write here.  
	RTIME	negative_jitter_threshold;					// determine a jitter threshold and write here.  
	unsigned int	num_of_positive_jitter_exceeding_threshold;	// determine a threshold and writes threshold-exceeding jitters here.  
	unsigned int	num_of_negative_jitter_exceeding_threshold;	// determine a threshold and writes threshold-exceeding jitters here.  
	unsigned int	num_of_run_time_exceeding_threshold;	// determine a threshold and writes threshold-exceeding jitters here.  
	unsigned int	run_time_buff[RUN_TIME_BUFFER_SIZE];	// determine a threshold and writes threshold-exceeding jitters here.  
	unsigned int	run_time_buff_cntr;
} CpuThreadTasksRtData;

typedef struct __CpuThreadsRtData
{
	unsigned int				num_of_rt_tasks_at_cpu_thread;
	CpuThreadTasksRtData	cpu_thread_tasks_rt_data[MAX_NUM_OF_RT_TASKS_PER_CPU_THREAD];		
} CpuThreadsRtData;


typedef struct __CpusRtData
{
	unsigned int			num_of_rt_tasks_at_cpu;
	CpuThreadsRtData	cpu_threads_rt_data[MAX_NUM_OF_CPU_THREADS_PER_CPU];		
	TimeStamp			rt_task_period;
} CpusRtData;

typedef struct __RtTasksData
{
	RTIME			current_system_time;	// periodic task of HybridNetwork determines it. 	
	unsigned int		num_of_total_rt_tasks;	
	CpusRtData 		cpus_rt_task_data[MAX_NUM_OF_CPUS];
} RtTasksData;


#endif
