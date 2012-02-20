#ifndef RTDATA_H
#define RTDATA_H

#include "CpuInfo.h"

#define MAX_NUM_RT_TAKS_PER_THREAD	10
#define MAX_NUM_RT_TAKS_NAME_LENGTH	50

typedef struct __CpuThreadRtTasksData
{
	unsigned int	num_of_rt_tasks_at_cpu_thread;
	unsigned int	max_period_run_time;			// each task checks its max run time in a task period and writes here if exceeds
	unsigned int	max_positive_jitter;				// period appeared longer than submitted period. 
	unsigned int	max_negative_jitter;				// period appeared shorter than submitted period. 
	unsigned int	positive_jitter_threshold;					// determine a jitter threshold and write here.  
	unsigned int	negative_jitter_threshold;					// determine a jitter threshold and write here.  
	unsigned int	num_of_positive_jitter_exceeding_threshold;	// determine a threshold and writes threshold-exceeding jitters here.  
	unsigned int	num_of_negative_jitter_exceeding_threshold;	// determine a threshold and writes threshold-exceeding jitters here.  
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


#define START_RT_TIMER_PERIOD 				50000		// 50 us

// KernelSpike
#define KERNELSPIKE_PERIOD 					500000		//0.5 ms
#define KERNELSPIKE_TASK_PRIORITY 				1
#define KERNELSPIKE_STACK_SIZE 				10000
#define KERNELSPIKE_CPU_ID 					1
#define KERNELSPIKE_CPU_THREAD_ID				0
#define KERNELSPIKE_PASS_DATA 				0
#define KERNELSPIKE_SIGNAL 					0
#define KERNELSPIKE_USES_FLOATING_POINT 		1
#define KERNELSPIKE_POSITIVE_JITTER_THRES	 	100000		// 100 us
#define KERNELSPIKE_NEGATIVE_JITTER_THRES	 	100000

// TrialController
#define TRIAL_CONTROLLER_PERIOD 						10000000		//10 ms
#define TRIAL_CONTROLLER_TASK_NAME					nam2num("TRIALCTL")
#define TRIAL_CONTROLLER_TASK_PRIORITY				100
#define TRIAL_CONTROLLER_STACK_SIZE 					10000
#define TRIAL_CONTROLLER_MSG_SIZE 					1000
#define TRIAL_CONTROLLER_POLICY 						SCHED_FIFO
#define TRIAL_CONTROLLER_CPU_ID						0
#define TRIAL_CONTROLLER_CPU_THREAD_ID				0
#define TRIAL_CONTROLLER_POSITIVE_JITTER_THRES	 	1000000		//1 ms
#define TRIAL_CONTROLLER_NEGATIVE_JITTER_THRES	 	1000000


#endif
