#ifndef RTDATA_H
#define RTDATA_H

#include "CpuInfo.h"

typedef struct __CpuThreadRtTasksData
{
	unsigned int	num_of_rt_tasks_at_cpu_thread;
	TimeStamp	max_period_run_time;		// each task checks its max run time in a task period and writes here if exceeds
	TimeStamp	max_positive_jitter;			// each task checks its max jitter
	TimeStamp	max_negative_jitter;
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
	TimeStamp		current_system_time;
	TimeStamp		previous_system_time;
} RtTasksData;


#define START_RT_TIMER_PERIOD 			50000		// 50 us

// KernelSpike
#define KERNELSPIKE_PERIOD 				500000		//0.5 ms
#define KERNELSPIKE_TASK_PRIORITY 			1
#define KERNELSPIKE_STACK_SIZE 			10000
#define KERNELSPIKE_CPU_ID 				1
#define KERNELSPIKE_CPU_THREAD_ID			0
#define KERNELSPIKE_PASS_DATA 			0
#define KERNELSPIKE_SIGNAL 				0
#define KERNELSPIKE_USES_FLOATING_POINT 	1




#endif
