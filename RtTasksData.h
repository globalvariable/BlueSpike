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
	TimeStamp		current_system_time;		// shared clocking set by kernelspike and others read it. kernel_spike detects exact spike times and other tasks adjusts themselves according to kernelspike
	TimeStamp		previous_system_time;
} RtTasksData;


#define START_RT_TIMER_PERIOD 				50000		// 50 us

// KernelSpike
#define KERNELSPIKE_PERIOD 							500000		//0.5 ms
#define KERNELSPIKE_TASK_PRIORITY 						1
#define KERNELSPIKE_STACK_SIZE 						10000
#define KERNELSPIKE_CPU_ID 							1
#define KERNELSPIKE_CPU_THREAD_ID					0
#define KERNELSPIKE_PASS_DATA 						0
#define KERNELSPIKE_SIGNAL 							0
#define KERNELSPIKE_USES_FLOATING_POINT 				1
#define KERNELSPIKE_POSITIVE_JITTER_THRES	 			100000		// 100 us
#define KERNELSPIKE_NEGATIVE_JITTER_THRES	 		100000

// BlueSpike Buffer Handler
#define BLUE_SPIKE_BUFF_HANDLER_PERIOD 						KERNELSPIKE_PERIOD		//0.5 ms
#define BLUE_SPIKE_BUFF_HANDLER_TASK_NAME					nam2num("BLUEHAND")
#define BLUE_SPIKE_BUFF_HANDLER_PRIORITY						100
#define BLUE_SPIKE_BUFF_HANDLER_STACK_SIZE 					10000
#define BLUE_SPIKE_BUFF_HANDLER_MSG_SIZE		 			1000
#define BLUE_SPIKE_BUFF_HANDLER_POLICY			 			SCHED_FIFO
#define BLUE_SPIKE_BUFF_HANDLER_CPU_ID						KERNELSPIKE_CPU_ID
#define BLUE_SPIKE_BUFF_HANDLER_CPU_THREAD_ID				KERNELSPIKE_CPU_THREAD_ID + 1
#define BLUE_SPIKE_BUFF_HANDLER_POSITIVE_JITTER_THRES	 	KERNELSPIKE_POSITIVE_JITTER_THRES		// 100 us
#define BLUE_SPIKE_BUFF_HANDLER_NEGATIVE_JITTER_THRES	 	KERNELSPIKE_POSITIVE_JITTER_THRES

// TrialHandler
#define TRIAL_HANDLER_PERIOD 						10000000		//10 ms
#define TRIAL_HANDLER_TASK_NAME					nam2num("TRIALHND")
#define TRIAL_HANDLER_TASK_PRIORITY				200
#define TRIAL_HANDLER_STACK_SIZE 					10000
#define TRIAL_HANDLER_MSG_SIZE 					1000
#define TRIAL_HANDLER_POLICY 						SCHED_FIFO
#define TRIAL_HANDLER_CPU_ID						0
#define TRIAL_HANDLER_CPU_THREAD_ID				0
#define TRIAL_HANDLER_POSITIVE_JITTER_THRES	 	1000000		//1 ms
#define TRIAL_HANDLER_NEGATIVE_JITTER_THRES	 	1000000

// TrialDurationHandler
#define TRIAL_DURATION_HANDLER_PERIOD 						10000000		//10 ms
#define TRIAL_DURATION_HANDLER_TASK_NAME					nam2num("TRIALDUR")
#define TRIAL_DURATION_HANDLER_TASK_PRIORITY				201
#define TRIAL_DURATION_HANDLER_STACK_SIZE 					10000
#define TRIAL_DURATION_HANDLER_MSG_SIZE 						1000
#define TRIAL_DURATION_HANDLER_POLICY 						SCHED_FIFO
#define TRIAL_DURATION_HANDLER_CPU_ID						0
#define TRIAL_DURATION_HANDLER_CPU_THREAD_ID				0
#define TRIAL_DURATION_HANDLER_POSITIVE_JITTER_THRES	 	1000000		//1 ms
#define TRIAL_DURATION_HANDLER_NEGATIVE_JITTER_THRES	 	1000000

// ExpEnviHandler
#define EXP_ENVI_HANDLER_PERIOD 						10000000		//10 ms
#define EXP_ENVI_HANDLER_TASK_NAME					nam2num("ENVIHND")
#define EXP_ENVI_HANDLER_TASK_PRIORITY				202
#define EXP_ENVI_HANDLER_STACK_SIZE 					10000
#define EXP_ENVI_HANDLER_MSG_SIZE 					1000
#define EXP_ENVI_HANDLER_POLICY 						SCHED_FIFO
#define EXP_ENVI_HANDLER_CPU_ID						0
#define EXP_ENVI_HANDLER_CPU_THREAD_ID				0
#define EXP_ENVI_HANDLER_POSITIVE_JITTER_THRES	 	1000000		//1 ms
#define EXP_ENVI_HANDLER_NEGATIVE_JITTER_THRES	 	1000000

// ExpEnviDurationHandler
#define EXP_ENVI_DURATION_HANDLER_PERIOD 						10000000		//10 ms
#define EXP_ENVI_DURATION_HANDLER_TASK_NAME					nam2num("ENVIDHND")
#define EXP_ENVI_DURATION_HANDLER_TASK_PRIORITY				203
#define EXP_ENVI_DURATION_HANDLER_STACK_SIZE 					10000
#define EXP_ENVI_DURATION_HANDLER_MSG_SIZE 						1000
#define EXP_ENVI_DURATION_HANDLER_POLICY 						SCHED_FIFO
#define EXP_ENVI_DURATION_HANDLER_CPU_ID						0
#define EXP_ENVI_DURATION_HANDLER_CPU_THREAD_ID				0
#define EXP_ENVI_DURATION_HANDLER_POSITIVE_JITTER_THRES	 	1000000		//1 ms
#define EXP_ENVI_DURATION_HANDLER_NEGATIVE_JITTER_THRES	 	1000000

// MovObjHandler
#define MOV_OBJ_HANDLER_PERIOD 						10000000		//10 ms
#define MOV_OBJ_HANDLER_TASK_NAME					nam2num("MOBJHND")
#define MOV_OBJ_HANDLER_TASK_PRIORITY				204
#define MOV_OBJ_HANDLER_STACK_SIZE 					10000
#define MOV_OBJ_HANDLER_MSG_SIZE 					1000
#define MOV_OBJ_HANDLER_POLICY 						SCHED_FIFO
#define MOV_OBJ_HANDLER_CPU_ID						0
#define MOV_OBJ_HANDLER_CPU_THREAD_ID				0
#define MOV_OBJ_HANDLER_POSITIVE_JITTER_THRES	 	1000000		//1 ms
#define MOV_OBJ_HANDLER_NEGATIVE_JITTER_THRES	 	1000000

// MovObjDurationHandler
#define MOV_OBJ_DURATION_HANDLER_PERIOD 						10000000		//10 ms
#define MOV_OBJ_DURATION_HANDLER_TASK_NAME					nam2num("MOBJDHND")
#define MOV_OBJ_DURATION_HANDLER_TASK_PRIORITY					205
#define MOV_OBJ_DURATION_HANDLER_STACK_SIZE 					10000
#define MOV_OBJ_DURATION_HANDLER_MSG_SIZE 						1000
#define MOV_OBJ_DURATION_HANDLER_POLICY 						SCHED_FIFO
#define MOV_OBJ_DURATION_HANDLER_CPU_ID						0
#define MOV_OBJ_DURATION_HANDLER_CPU_THREAD_ID				0
#define MOV_OBJ_DURATION_HANDLER_POSITIVE_JITTER_THRES	 	1000000		//1 ms
#define MOV_OBJ_DURATION_HANDLER_NEGATIVE_JITTER_THRES	 	1000000

// FirstBMICageInterfacer
#define FIRST_BMI_CAGE_INTERF_PERIOD 						10000000		//10 ms
#define FIRST_BMI_CAGE_INTERF_TASK_NAME					nam2num("FIRSTBCM")
#define FIRST_BMI_CAGE_INTERF_TASK_PRIORITY				206
#define FIRST_BMI_CAGE_INTERF_STACK_SIZE 					10000
#define FIRST_BMI_CAGE_INTERF_MSG_SIZE 					1000
#define FIRST_BMI_CAGE_INTERF_POLICY 						SCHED_FIFO
#define FIRST_BMI_CAGE_INTERF_CPU_ID						0
#define FIRST_BMI_CAGE_INTERF_CPU_THREAD_ID				0
#define FIRST_BMI_CAGE_INTERF_POSITIVE_JITTER_THRES	 	1000000		//1 ms
#define FIRST_BMI_CAGE_INTERF_NEGATIVE_JITTER_THRES	 	1000000

// SpikeGenerator
#define SPIKE_GENERATOR_PERIOD 						KERNELSPIKE_PERIOD		//0.5 ms
#define SPIKE_GENERATOR_TASK_NAME					nam2num("SPIKEGEN")
#define SPIKE_GENERATOR_TASK_PRIORITY				101
#define SPIKE_GENERATOR_STACK_SIZE 					10000
#define SPIKE_GENERATOR_MSG_SIZE 						1000
#define SPIKE_GENERATOR_POLICY 						SCHED_FIFO
#define SPIKE_GENERATOR_CPU_ID						KERNELSPIKE_CPU_ID
#define SPIKE_GENERATOR_CPU_THREAD_ID				KERNELSPIKE_CPU_THREAD_ID + 1
#define SPIKE_GENERATOR_POSITIVE_JITTER_THRES	 	KERNELSPIKE_POSITIVE_JITTER_THRES		// 100 us
#define SPIKE_GENERATOR_NEGATIVE_JITTER_THRES	 	KERNELSPIKE_POSITIVE_JITTER_THRES

// InSilicoSimulators
#define IZ_PS_NETWORK_SIM_PERIOD						1000000		//1 ms
#define IZ_PS_NETWORK_SIM_PERIOD_SIM_TASK_NAME		nam2num("IZPSSIM")
#define IZ_PS_NETWORK_SIM_PERIOD_SIM_TASK_PRIORITY	102
#define IZ_PS_NETWORK_SIM_STACK_SIZE 					10000
#define IZ_PS_NETWORK_SIM_MSG_SIZE 					1000
#define IZ_PS_NETWORK_SIM_POLICY 						SCHED_FIFO
#define IZ_PS_NETWORK_SIM_CPU_ID						2
#define IZ_PS_NETWORK_SIM_CPU_THREAD_ID				0
#define IZ_PS_NETWORK_SIM_POSITIVE_JITTER_THRES	 	200000		// 200 us
#define IZ_PS_NETWORK_SIM_NEGATIVE_JITTER_THRES	 	200000





#endif
