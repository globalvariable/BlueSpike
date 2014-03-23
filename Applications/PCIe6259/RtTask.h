#ifndef RT_TASK_H
#define RT_TASK_H


#include <gtk/gtk.h>
#include <stdbool.h>
#include <rtai_shm.h>
#include <rtai_nam2num.h>
#include "ConfigDaq/DaqCard.h"
#include "RecordingData.h"
#include "SpikeTimeStamp.h"
#include "SpikeSorting.h"
#include "../../../BlueSpike/Library/Misc/Misc.h"
#include "../../../BlueSpike/System/RtTasksData/RtTasksData.h"
#include "../../../BlueSpike/Library/RtTaskStats/RtTaskStats.h"
#include "../../../BlueSpike/System/ShmSemNum/ShmSemNum.h"


#define BLUESPIKE_PERIODIC_PERIOD 							BLUESPIKE_DAQ_PERIOD		//1 ms
#define BLUESPIKE_PERIODIC_TASK_NAME						nam2num("BLUPER")
#define BLUESPIKE_PERIODIC_TASK_PRIORITY 					2

#define BLUESPIKE_PERIODIC_STACK_SIZE 					BLUESPIKE_DAQ_STACK_SIZE
#define BLUESPIKE_PERIODIC_MSG_SIZE 						BLUESPIKE_DAQ_MSG_SIZE
#define BLUESPIKE_PERIODIC_CPU_ID 							BLUESPIKE_DAQ_CPU_ID 
#define BLUESPIKE_PERIODIC_CPU_THREAD_ID					BLUESPIKE_DAQ_CPU_THREAD_ID
#define BLUESPIKE_PERIODIC_CPU_THREAD_TASK_ID			0	
#define BLUESPIKE_PERIODIC_PASS_DATA 						BLUESPIKE_DAQ_PASS_DATA
#define BLUESPIKE_PERIODIC_SIGNAL 							BLUESPIKE_DAQ_SIGNAL
#define BLUESPIKE_PERIODIC_POLICY 							BLUESPIKE_DAQ_POLICY
#define BLUESPIKE_PERIODIC_USES_FLOATING_POINT 			BLUESPIKE_DAQ_USES_FLOATING_POINT 
#define BLUESPIKE_PERIODIC_POSITIVE_JITTER_THRES	 		BLUESPIKE_DAQ_POSITIVE_JITTER_THRES		// 100 us
#define BLUESPIKE_PERIODIC_NEGATIVE_JITTER_THRES	 	BLUESPIKE_DAQ_NEGATIVE_JITTER_THRES

#define BLUESPIKE_DAQ_PERIOD 							SAMPLING_INTERVAL*NUM_OF_SCAN		//1 ms
#define BLUESPIKE_DAQ_TASK_NAME						nam2num("BLUDAQ")
#define BLUESPIKE_DAQ_TASK_PRIORITY 					1

#define BLUESPIKE_DAQ_STACK_SIZE 						10000
#define BLUESPIKE_DAQ_MSG_SIZE 						1000
#define BLUESPIKE_DAQ_CPU_ID 							1
#define BLUESPIKE_DAQ_CPU_THREAD_ID					0
#define BLUESPIKE_DAQ_CPU_THREAD_TASK_ID			1
#define BLUESPIKE_DAQ_PASS_DATA 						0
#define BLUESPIKE_DAQ_SIGNAL 							0
#define BLUESPIKE_DAQ_POLICY 							SCHED_FIFO
#define BLUESPIKE_DAQ_USES_FLOATING_POINT 			1
#define BLUESPIKE_DAQ_POSITIVE_JITTER_THRES	 		100000		// 100 us
#define BLUESPIKE_DAQ_NEGATIVE_JITTER_THRES	 		100000

bool start_periodic_task(void);
bool start_acquisition(void);

RtTasksData *rt_tasks_data;



#endif
