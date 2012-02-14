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
#include "KernelTaskStat.h"
#include "RtData.h"
#include "SharedMemoryAdditionalData.h"


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
	RecordingData				recording_data;
	RecordingData				filtered_recording_data;
	SpikeThresholding			spike_thresholding;
	BlueSpikeTimeStamp 		blue_spike_time_stamp;
	SpikeTimeStamp 			spike_time_stamp;
	void					*additional_data[MAX_NUM_OF_SHM_ADDITIONAL_DATA];	
	TemplateMatchingData		template_matching_data;
	KernelTaskCtrl				kernel_task_ctrl;
	KernelTaskStat				kernel_task_stat;
} SharedMemStruct;

SharedMemStruct *shared_memory;


#endif
