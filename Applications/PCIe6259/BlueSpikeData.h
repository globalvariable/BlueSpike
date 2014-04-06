#ifndef BLUESPIKE_DATA_H
#define BLUESPIKE_DATA_H

//   THIS FILE SHOULD ONLY BE INCLUDED BY THIS APPLICATION. 
//	IF OTHER PROGRAMS INCLUDE DECLARED DATA BELOW WILL BE ALLOCATED BY THEM UNNECESSARILY

#include "RecordingData.h"
#include "SortedSpikes.h"
#include "BlueSpikeTimeStamp.h"
#include "SpikeThresholding.h"
#include "SpikeSorting.h"
#include "TemplateMatching.h"
#include "ConfigDaq/DaqMwaData.h"
#include "RtTask.h"

RtTasksData *rt_tasks_data;
SortedSpikes *sorted_spikes;
RecordingData *recording_data;
BlueSpikeTimeStamp blue_spike_time_stamp;
SpikeThresholding spike_thresholding;
SpikeEndHandling	spike_end_handling;








#endif
