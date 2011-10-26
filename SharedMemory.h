#define SHAREDMEMORY_H

#include "RecordingData.h"
#include "SpikeEndData.h"
#include "SpikeTimeStamps.h"
#include "EventTimeStamps.h"
#include "CommandTimeStamps.h"
#include "PerformanceData.h"
#include "TemplateMatchingData.h"
#include "ControllingData.h"



typedef struct __SharedMemStruct
{
	RecordingData 			recording_data_buff[RECORDING_DATA_BUFF_SIZE];
	RecordingData 			filtered_recording_data_buff[RECORDING_DATA_BUFF_SIZE];
	SpikeEndData			spike_end_data_buff[SPIKE_END_DATA_BUFF_SIZE];
	SpikeTimeStamp 		spike_time_stamp_buff[SPIKE_TIMESTAMPS_BUFF_SIZE];
	EventTimeStamp 		event_time_stamps_buff[EVENT_TIMESTAMPS_BUFF_SIZE];
	CommandTimeStamp 	command_time_stamps_buff[COMMAND_TIMESTAMPS_BUFF_SIZE];	
	PerformanceData		performance_data_buff[PERFORMANCE_DATA_BUFF_SIZE];	
	int 					recording_data_buff_read_idx;	
	int 					filtered_recording_data_buff_read_idx;	
	int 					spike_end_data_buff_read_idx;			
	int 					spike_timestamp_buff_read_idx;
	int 					event_timestamp_buff_read_idx;	
	int 					command_timestamp_buff_read_idx;	
	int 					performance_data_buff_read_idx;		
	TemplateMatchingData	template_matching_data;
	ControllingData			controlling_data;
} SharedMemStruct;

SharedMemStruct *shared_memory;

