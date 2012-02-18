#ifndef RTDATA_H
#define RTDATA_H

#include "CpuInfo.h"

typedef struct __CpuThreadRtTasksData
{
	unsigned int	num_of_rt_tasks_at_cpu_thread;
	TimeStamp	max_period_run_time;		// each task checks its max run time in a task period and writes here if exceeds
	TimeStamp	min_period_run_time;
	TimeStamp	max_positive_jitter;			// each task checks its max jitter
	TimeStamp	min_negative_jitter;
	unsigned int	num_of_jitter_exceeding_threshold;	// each task determine a threshold and writes threshold-exceeding jitters here.  			
} CpuThreadRtTasksData;


typedef struct __CpuRtTasksData
{
	CpuThreadRtTasksData	cpu_thread_rt_task_data[MAX_NUM_OF_THREADS_PER_CPU];		// this cpu has an rt_task assiged.
	unsigned int			num_of_rt_tasks_at_cpu;	
	TimeStamp			rt_task_period;
} CpuRtTasksData;

typedef struct __RtTasksData
{
	unsigned int		num_of_total_rt_tasks;	
	CpuRtTasksData 	cpu_rt_task_data[MAX_NUM_OF_CPUS];
} RtTasksData;




// KernelSpike
#define KERNELSPIKE_PERIOD 				1000000
#define KERNELSPIKE_TASK_PRIORITY 			1
#define KERNELSPIKE_STACK_SIZE 			10000
#define KERNELSPIKE_CPU_ID 				1
#define KERNELSPIKE_CPU_THREAD_ID			0
#define KERNELSPIKE_PASS_DATA 			0
#define KERNELSPIKE_SIGNAL 				0
#define KERNELSPIKE_USES_FLOATING_POINT 	1




#endif
