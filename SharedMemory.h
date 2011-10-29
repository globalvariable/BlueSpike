#define SHAREDMEMORY_H

#include "DaqMwaMap.h"
#include "RecordingData.h"
#include "SpikeEnd.h"
#include "SpikeTimeStamp.h"
#include "ExpEnviEventTimeStamp.h"
#include "MovingObjEventTimeStamp.h"
#include "TemplateMatchingData.h"
#include "KernelTaskCtrl.h"
#include "KernelTaskStat.h"

#define SHARED_MEM_NAME  "SHMEM"
#ifdef KERNELSPIKE_H
#define SHARED_MEM_SIZE sizeof(SharedMemStruct)
#else
#define SHARED_MEM_SIZE 0
#endif


typedef struct __SharedMemStruct
{
	DaqMwaMap				daq_mwa_map;
	RecordingData				recording_data;
	RecordingData				filtered_recording_data;
	SpikeEnd					spike_end;
	SpikeTimeStamp 			spike_time_stamp;
	ExpEnviEventTimeStamp		exp_envi_event_time_stamp;
	MovingObjEventTimeStamp	mov_obj_event_time_stamp;
	ExpEnviCommandTimeStamp 	exp_envi_command_time_stamp;	
	TemplateMatchingData		template_matching_data;
	KernelTaskCtrl				kernel_task_ctrl;
	KernelTaskStat				kernel_task_stat;	
} SharedMemStruct;

SharedMemStruct *shared_memory;

