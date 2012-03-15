#ifndef SHAREDMEMORY_H
#define SHAREDMEMORY_H

#include "MwaDaqMap.h"
#include "DaqMwaMap.h"
#include "RecordingData.h"
#include "SpikeThresholding.h"
#include "BlueSpikeTimeStamp.h"
#include "SpikeTimeStamp.h"
#include "TemplateMatchingData.h"
#include "KernelTaskCtrl.h"
#include "RtTasksData.h"


#define SHARED_MEM_NAME  "SHMEM"
#ifdef KERNELSPIKE_H
#define SHARED_MEM_SIZE sizeof(SharedMemStruct)
#else
#define SHARED_MEM_SIZE 0
#endif





typedef struct __SharedMemStruct
{
	DaqMwaMap				daq_mwa_map;
	MwaDaqMap				mwa_daq_map;
	RecordingData			recording_data;
	RecordingData			filtered_recording_data;
	SpikeThresholding			spike_thresholding;
	BlueSpikeTimeStamp 		blue_spike_time_stamp;
	SpikeTimeStamp 			spike_time_stamp;
	TemplateMatchingData		template_matching_data;
	KernelTaskCtrl			kernel_task_ctrl;
	RtTasksData				rt_tasks_data;
} SharedMemStruct;

SharedMemStruct *shared_memory;


#endif
