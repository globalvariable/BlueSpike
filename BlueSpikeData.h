#ifndef BLUE_SPIKE_DATA_H
#define BLUE_SPIKE_DATA_H


#include "MwaDaqMap.h"
#include "DaqMwaMap.h"
#include "RecordingData.h"
#include "SpikeThresholding.h"
#include "BlueSpikeTimeStamp.h"
#include "SpikeTimeStamp.h"
#include "TemplateMatchingData.h"
#include "BlueSpikeCtrl.h"

#define BLUE_SPIKE_DATA_SHM_NAME		"BLUESP"		

typedef struct __BlueSpikeData
{
	DaqMwaMap				daq_mwa_map;
	MwaDaqMap				mwa_daq_map;
	RecordingData			recording_data;
	RecordingData			filtered_recording_data;
	SpikeThresholding			spike_thresholding;
	BlueSpikeTimeStamp 		blue_spike_time_stamp;
	SpikeTimeStamp 			spike_time_stamp;
	TemplateMatchingData		template_matching_data;
	BlueSpikeCtrl				blue_spike_ctrl;
} BlueSpikeData;

#endif
