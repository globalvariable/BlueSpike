#ifndef RT_TASKS_DATA_H
#define RT_TASKS_DATA_H

#include "../Cpu/CpuInfo.h"
#include "../KernelTaskCtrl/KernelTaskCtrl.h"
#include "../TimeStamp/TimeStamp.h"

#define MAX_NUM_OF_RT_TASKS_PER_CPU_THREAD	10
#define MAX_NUM_OF_RT_TASKS_NAME_LENGTH		40

typedef struct __CpuThreadTasksRtData
{
	char		task_name[MAX_NUM_OF_RT_TASKS_NAME_LENGTH];
	unsigned int	max_task_run_time;			// each task checks its max run time in a task period and writes here if exceeds
	unsigned int	max_positive_jitter;				// period appeared longer than submitted period. 
	unsigned int	max_negative_jitter;				// period appeared shorter than submitted period. 
	unsigned int	positive_jitter_threshold;					// determine a jitter threshold and write here.  
	unsigned int	negative_jitter_threshold;					// determine a jitter threshold and write here.  
	unsigned int	num_of_positive_jitter_exceeding_threshold;	// determine a threshold and writes threshold-exceeding jitters here.  
	unsigned int	num_of_negative_jitter_exceeding_threshold;	// determine a threshold and writes threshold-exceeding jitters here.  
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
	unsigned int		num_of_total_rt_tasks;	
	CpusRtData 		cpus_rt_task_data[MAX_NUM_OF_CPUS];
	TimeStamp		current_system_time;		// shared clocking set by kernelspike and others read it. kernel_spike detects exact spike times and other tasks adjusts themselves according to kernelspike
	TimeStamp		previous_system_time;
	KernelTaskCtrl	kernel_task_ctrl;
} RtTasksData;


#define START_RT_TIMER_PERIOD 				50000		// 50 us

// KernelSpike
#define KERNELSPIKE_PERIOD 							1000000		//1 ms
#define KERNELSPIKE_TASK_PRIORITY 						1
#define KERNELSPIKE_STACK_SIZE 						10000
#define KERNELSPIKE_CPU_ID 							1
#define KERNELSPIKE_CPU_THREAD_ID					0
#define KERNELSPIKE_CPU_THREAD_TASK_ID				0
#define KERNELSPIKE_PASS_DATA 						0
#define KERNELSPIKE_SIGNAL 							0
#define KERNELSPIKE_USES_FLOATING_POINT 				1
#define KERNELSPIKE_POSITIVE_JITTER_THRES	 			100000		// 100 us
#define KERNELSPIKE_NEGATIVE_JITTER_THRES	 		100000

// TrialHandler
#define TRIAL_HANDLER_PERIOD 						2000000		//2 ms
#define TRIAL_HANDLER_TASK_NAME					nam2num("TRIALHND")
#define TRIAL_HANDLER_TASK_PRIORITY				300
#define TRIAL_HANDLER_STACK_SIZE 					10000
#define TRIAL_HANDLER_MSG_SIZE 					1000
#define TRIAL_HANDLER_POLICY 						SCHED_FIFO
#define TRIAL_HANDLER_CPU_ID						0
#define TRIAL_HANDLER_CPU_THREAD_ID				0
#define TRIAL_HANDLER_CPU_THREAD_TASK_ID		0
#define TRIAL_HANDLER_POSITIVE_JITTER_THRES	 	200000		//0.1 ms
#define TRIAL_HANDLER_NEGATIVE_JITTER_THRES	 	200000

// TrialDurationHandler
#define TRIAL_DURATION_HANDLER_PERIOD 						TRIAL_HANDLER_PERIOD		//1 ms
#define TRIAL_DURATION_HANDLER_TASK_NAME					nam2num("TRIALDUR")
#define TRIAL_DURATION_HANDLER_TASK_PRIORITY				301
#define TRIAL_DURATION_HANDLER_STACK_SIZE 					10000
#define TRIAL_DURATION_HANDLER_MSG_SIZE 						1000
#define TRIAL_DURATION_HANDLER_POLICY 						SCHED_FIFO
#define TRIAL_DURATION_HANDLER_CPU_ID						TRIAL_HANDLER_CPU_ID
#define TRIAL_DURATION_HANDLER_CPU_THREAD_ID				TRIAL_HANDLER_CPU_THREAD_ID
#define TRIAL_DURATION_HANDLER_CPU_THREAD_TASK_ID			TRIAL_HANDLER_CPU_THREAD_TASK_ID + 1
#define TRIAL_DURATION_HANDLER_POSITIVE_JITTER_THRES	 	TRIAL_HANDLER_POSITIVE_JITTER_THRES		//0.1 ms
#define TRIAL_DURATION_HANDLER_NEGATIVE_JITTER_THRES	 	TRIAL_HANDLER_NEGATIVE_JITTER_THRES

// ExpEnviHandler
#define EXP_ENVI_HANDLER_PERIOD 						TRIAL_HANDLER_PERIOD		//1 ms
#define EXP_ENVI_HANDLER_TASK_NAME					nam2num("ENVIHND")
#define EXP_ENVI_HANDLER_TASK_PRIORITY				302
#define EXP_ENVI_HANDLER_STACK_SIZE 					10000
#define EXP_ENVI_HANDLER_MSG_SIZE 					1000
#define EXP_ENVI_HANDLER_POLICY 						SCHED_FIFO
#define EXP_ENVI_HANDLER_CPU_ID						TRIAL_HANDLER_CPU_ID
#define EXP_ENVI_HANDLER_CPU_THREAD_ID				TRIAL_HANDLER_CPU_THREAD_ID
#define EXP_ENVI_HANDLER_CPU_THREAD_TASK_ID		TRIAL_DURATION_HANDLER_CPU_THREAD_TASK_ID + 1
#define EXP_ENVI_HANDLER_POSITIVE_JITTER_THRES	 	TRIAL_HANDLER_POSITIVE_JITTER_THRES		//0.1 ms
#define EXP_ENVI_HANDLER_NEGATIVE_JITTER_THRES	 	TRIAL_HANDLER_NEGATIVE_JITTER_THRES

// ExpEnviDurationHandler
#define EXP_ENVI_DURATION_HANDLER_PERIOD 						TRIAL_HANDLER_PERIOD		//1 ms
#define EXP_ENVI_DURATION_HANDLER_TASK_NAME					nam2num("ENVIDHND")
#define EXP_ENVI_DURATION_HANDLER_TASK_PRIORITY				303
#define EXP_ENVI_DURATION_HANDLER_STACK_SIZE 					10000
#define EXP_ENVI_DURATION_HANDLER_MSG_SIZE 						1000
#define EXP_ENVI_DURATION_HANDLER_POLICY 						SCHED_FIFO
#define EXP_ENVI_DURATION_HANDLER_CPU_ID						TRIAL_HANDLER_CPU_ID
#define EXP_ENVI_DURATION_HANDLER_CPU_THREAD_ID				TRIAL_HANDLER_CPU_THREAD_ID
#define EXP_ENVI_DURATION_HANDLER_CPU_THREAD_TASK_ID			EXP_ENVI_HANDLER_CPU_THREAD_TASK_ID + 1 
#define EXP_ENVI_DURATION_HANDLER_POSITIVE_JITTER_THRES	 	TRIAL_HANDLER_POSITIVE_JITTER_THRES		//0.1 ms
#define EXP_ENVI_DURATION_HANDLER_NEGATIVE_JITTER_THRES	 	TRIAL_HANDLER_NEGATIVE_JITTER_THRES

// MovObjHandler
#define MOV_OBJ_HANDLER_PERIOD 						TRIAL_HANDLER_PERIOD		//1 ms
#define MOV_OBJ_HANDLER_TASK_NAME					nam2num("MOBJHND")
#define MOV_OBJ_HANDLER_TASK_PRIORITY				304
#define MOV_OBJ_HANDLER_STACK_SIZE 					10000
#define MOV_OBJ_HANDLER_MSG_SIZE 					1000
#define MOV_OBJ_HANDLER_POLICY 						SCHED_FIFO
#define MOV_OBJ_HANDLER_CPU_ID						TRIAL_HANDLER_CPU_ID
#define MOV_OBJ_HANDLER_CPU_THREAD_ID				TRIAL_HANDLER_CPU_THREAD_ID
#define MOV_OBJ_HANDLER_CPU_THREAD_TASK_ID			EXP_ENVI_DURATION_HANDLER_CPU_THREAD_TASK_ID + 1
#define MOV_OBJ_HANDLER_POSITIVE_JITTER_THRES	 	TRIAL_HANDLER_POSITIVE_JITTER_THRES		//0.1 ms
#define MOV_OBJ_HANDLER_NEGATIVE_JITTER_THRES	 	TRIAL_HANDLER_NEGATIVE_JITTER_THRES

// MovObjDurationHandler
#define MOV_OBJ_DURATION_HANDLER_PERIOD 						TRIAL_HANDLER_PERIOD		//1 ms
#define MOV_OBJ_DURATION_HANDLER_TASK_NAME					nam2num("MOBDHND")
#define MOV_OBJ_DURATION_HANDLER_TASK_PRIORITY					305
#define MOV_OBJ_DURATION_HANDLER_STACK_SIZE 					10000
#define MOV_OBJ_DURATION_HANDLER_MSG_SIZE 						1000
#define MOV_OBJ_DURATION_HANDLER_POLICY 						SCHED_FIFO
#define MOV_OBJ_DURATION_HANDLER_CPU_ID						TRIAL_HANDLER_CPU_ID
#define MOV_OBJ_DURATION_HANDLER_CPU_THREAD_ID				TRIAL_HANDLER_CPU_THREAD_ID
#define MOV_OBJ_DURATION_HANDLER_CPU_THREAD_TASK_ID			MOV_OBJ_HANDLER_CPU_THREAD_TASK_ID + 1
#define MOV_OBJ_DURATION_HANDLER_POSITIVE_JITTER_THRES	 	TRIAL_HANDLER_POSITIVE_JITTER_THRES		//0.1 ms
#define MOV_OBJ_DURATION_HANDLER_NEGATIVE_JITTER_THRES	 	TRIAL_HANDLER_NEGATIVE_JITTER_THRES

// FirstBMICageInterfacer
#define FIRST_BMI_CAGE_INTERF_PERIOD 						TRIAL_HANDLER_PERIOD		//1 ms
#define FIRST_BMI_CAGE_INTERF_TASK_NAME					nam2num("FIRSTBCM")
#define FIRST_BMI_CAGE_INTERF_TASK_PRIORITY				306
#define FIRST_BMI_CAGE_INTERF_STACK_SIZE 					10000
#define FIRST_BMI_CAGE_INTERF_MSG_SIZE 					1000
#define FIRST_BMI_CAGE_INTERF_POLICY 						SCHED_FIFO
#define FIRST_BMI_CAGE_INTERF_CPU_ID						TRIAL_HANDLER_CPU_ID
#define FIRST_BMI_CAGE_INTERF_CPU_THREAD_ID				TRIAL_HANDLER_CPU_THREAD_ID
#define FIRST_BMI_CAGE_INTERF_CPU_THREAD_TASK_ID		MOV_OBJ_DURATION_HANDLER_CPU_THREAD_TASK_ID + 1
#define FIRST_BMI_CAGE_INTERF_POSITIVE_JITTER_THRES	 	TRIAL_HANDLER_POSITIVE_JITTER_THRES		//0.1 ms
#define FIRST_BMI_CAGE_INTERF_NEGATIVE_JITTER_THRES	 	TRIAL_HANDLER_NEGATIVE_JITTER_THRES

// BlueSpike Buffer Handler
#define BLUE_SPIKE_BUFF_HANDLER_PERIOD 						TRIAL_HANDLER_PERIOD		//1 ms
#define BLUE_SPIKE_BUFF_HANDLER_TASK_NAME					nam2num("BLUEHAND")
#define BLUE_SPIKE_BUFF_HANDLER_TASK_PRIORITY				100
#define BLUE_SPIKE_BUFF_HANDLER_STACK_SIZE 					10000
#define BLUE_SPIKE_BUFF_HANDLER_MSG_SIZE		 			1000
#define BLUE_SPIKE_BUFF_HANDLER_POLICY			 			SCHED_FIFO
#define BLUE_SPIKE_BUFF_HANDLER_CPU_ID						TRIAL_HANDLER_CPU_ID
#define BLUE_SPIKE_BUFF_HANDLER_CPU_THREAD_ID				TRIAL_HANDLER_CPU_THREAD_ID + 1
#define BLUE_SPIKE_BUFF_HANDLER_CPU_THREAD_TASK_ID		0
#define BLUE_SPIKE_BUFF_HANDLER_POSITIVE_JITTER_THRES	 	TRIAL_HANDLER_POSITIVE_JITTER_THRES		//0.1 ms
#define BLUE_SPIKE_BUFF_HANDLER_NEGATIVE_JITTER_THRES	 	TRIAL_HANDLER_NEGATIVE_JITTER_THRES

// Trial Handler 2 Neural Network Messages Handler
#define TRIAL_HAND_2_NEURAL_NET_MSGS_HANDLER_PERIOD 					TRIAL_HANDLER_PERIOD		//1 ms
#define TRIAL_HAND_2_NEURAL_NET_MSGS_HANDLER_TASK_NAME				nam2num("TRTONN")
#define TRIAL_HAND_2_NEURAL_NET_MSGS_HANDLER_TASK_PRIORITY			101
#define TRIAL_HAND_2_NEURAL_NET_MSGS_HANDLER_STACK_SIZE 				10000
#define TRIAL_HAND_2_NEURAL_NET_MSGS_HANDLER_MSG_SIZE		 		1000
#define TRIAL_HAND_2_NEURAL_NET_MSGS_HANDLER_POLICY			 		SCHED_FIFO
#define TRIAL_HAND_2_NEURAL_NET_MSGS_HANDLER_CPU_ID					TRIAL_HANDLER_CPU_ID
#define TRIAL_HAND_2_NEURAL_NET_MSGS_HANDLER_CPU_THREAD_ID			TRIAL_HANDLER_CPU_THREAD_ID + 1
#define TRIAL_HAND_2_NEURAL_NET_MSGS_HANDLER_CPU_THREAD_TASK_ID	BLUE_SPIKE_BUFF_HANDLER_CPU_THREAD_TASK_ID + 1
#define TRIAL_HAND_2_NEURAL_NET_MSGS_HANDLER_POSITIVE_JITTER_THRES 	TRIAL_HANDLER_POSITIVE_JITTER_THRES		//0.1 ms
#define TRIAL_HAND_2_NEURAL_NET_MSGS_HANDLER_NEGATIVE_JITTER_THRES 	TRIAL_HANDLER_NEGATIVE_JITTER_THRES

// Mov Obj Handler 2 Neural Network Messages Handler
#define MOV_OBJ_HAND_2_NEURAL_NET_MSGS_HANDLER_PERIOD 					TRIAL_HANDLER_PERIOD		//1 ms
#define MOV_OBJ_HAND_2_NEURAL_NET_MSGS_HANDLER_TASK_NAME				nam2num("MOTONN")
#define MOV_OBJ_HAND_2_NEURAL_NET_MSGS_HANDLER_TASK_PRIORITY			102
#define MOV_OBJ_HAND_2_NEURAL_NET_MSGS_HANDLER_STACK_SIZE 				10000
#define MOV_OBJ_HAND_2_NEURAL_NET_MSGS_HANDLER_MSG_SIZE		 		1000
#define MOV_OBJ_HAND_2_NEURAL_NET_MSGS_HANDLER_POLICY			 		SCHED_FIFO
#define MOV_OBJ_HAND_2_NEURAL_NET_MSGS_HANDLER_CPU_ID					TRIAL_HANDLER_CPU_ID
#define MOV_OBJ_HAND_2_NEURAL_NET_MSGS_HANDLER_CPU_THREAD_ID			TRIAL_HANDLER_CPU_THREAD_ID + 1
#define MOV_OBJ_HAND_2_NEURAL_NET_MSGS_HANDLER_CPU_THREAD_TASK_ID	TRIAL_HAND_2_NEURAL_NET_MSGS_HANDLER_CPU_THREAD_TASK_ID + 1
#define MOV_OBJ_HAND_2_NEURAL_NET_MSGS_HANDLER_POSITIVE_JITTER_THRES 	TRIAL_HANDLER_POSITIVE_JITTER_THRES		//0.1 ms
#define MOV_OBJ_HAND_2_NEURAL_NET_MSGS_HANDLER_NEGATIVE_JITTER_THRES 	TRIAL_HANDLER_NEGATIVE_JITTER_THRES

// Trial Handler 2 SpikeGenerator Messages Handler
#define TRIAL_HAND_2_SPIKE_GEN_MSGS_HANDLER_PERIOD 					TRIAL_HANDLER_PERIOD		//1 ms
#define TRIAL_HAND_2_SPIKE_GEN_MSGS_HANDLER_TASK_NAME				nam2num("TRTOSG")
#define TRIAL_HAND_2_SPIKE_GEN_MSGS_HANDLER_TASK_PRIORITY				103
#define TRIAL_HAND_2_SPIKE_GEN_MSGS_HANDLER_STACK_SIZE 				10000
#define TRIAL_HAND_2_SPIKE_GEN_MSGS_HANDLER_MSG_SIZE		 			1000
#define TRIAL_HAND_2_SPIKE_GEN_MSGS_HANDLER_POLICY			 		SCHED_FIFO
#define TRIAL_HAND_2_SPIKE_GEN_MSGS_HANDLER_CPU_ID					TRIAL_HANDLER_CPU_ID
#define TRIAL_HAND_2_SPIKE_GEN_MSGS_HANDLER_CPU_THREAD_ID			TRIAL_HANDLER_CPU_THREAD_ID + 1
#define TRIAL_HAND_2_SPIKE_GEN_MSGS_HANDLER_CPU_THREAD_TASK_ID		MOV_OBJ_HAND_2_NEURAL_NET_MSGS_HANDLER_CPU_THREAD_TASK_ID + 1
#define TRIAL_HAND_2_SPIKE_GEN_MSGS_HANDLER_POSITIVE_JITTER_THRES 	TRIAL_HANDLER_POSITIVE_JITTER_THRES		//0.1 ms
#define TRIAL_HAND_2_SPIKE_GEN_MSGS_HANDLER_NEGATIVE_JITTER_THRES 	TRIAL_HANDLER_NEGATIVE_JITTER_THRES

// SpikeGenerator
#define SPIKE_GENERATOR_PERIOD 						TRIAL_HANDLER_PERIOD		//1 ms
#define SPIKE_GENERATOR_TASK_NAME					nam2num("SPIKEGEN")
#define SPIKE_GENERATOR_TASK_PRIORITY				201
#define SPIKE_GENERATOR_STACK_SIZE 					10000
#define SPIKE_GENERATOR_MSG_SIZE 						1000
#define SPIKE_GENERATOR_POLICY 						SCHED_FIFO
#define SPIKE_GENERATOR_CPU_ID						TRIAL_HANDLER_CPU_ID
#define SPIKE_GENERATOR_CPU_THREAD_ID				TRIAL_HANDLER_CPU_THREAD_ID + 1
#define SPIKE_GENERATOR_CPU_THREAD_TASK_ID			TRIAL_HAND_2_SPIKE_GEN_MSGS_HANDLER_CPU_THREAD_TASK_ID + 1
#define SPIKE_GENERATOR_POSITIVE_JITTER_THRES	 	TRIAL_HANDLER_POSITIVE_JITTER_THRES		//0.1 ms
#define SPIKE_GENERATOR_NEGATIVE_JITTER_THRES	 	TRIAL_HANDLER_NEGATIVE_JITTER_THRES

// InSilicoSimulators
#define IZ_PS_NETWORK_SIM_PERIOD						2000000		//1 ms
#define IZ_PS_NETWORK_SIM_TASK_NAME					"IZPS"
#define IZ_PS_NETWORK_SIM_TASK_PRIORITY				202
#define IZ_PS_NETWORK_SIM_STACK_SIZE 					10000
#define IZ_PS_NETWORK_SIM_MSG_SIZE 					1000
#define IZ_PS_NETWORK_SIM_POLICY 						SCHED_FIFO
#define IZ_PS_NETWORK_SIM_CPU_ID						2
#define IZ_PS_NETWORK_SIM_CPU_THREAD_ID				0
#define IZ_PS_NETWORK_SIM_POSITIVE_JITTER_THRES	 	200000		// 200 us
#define IZ_PS_NETWORK_SIM_NEGATIVE_JITTER_THRES	 	200000

#define IZ_PS_NETWORK_SIM_NUM_OF_DEDICATED_CPUS	2	// cpus startiing from IZ_PS_NETWORK_SIM_CPU_ID are dedicated to SNN simulations




#endif