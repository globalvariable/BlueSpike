#include "RecordingData.h"
#include "SpikeTimeStamps.h"
#include "EventTimeStamps.h"
#include "CommandTimeStamps.h"



typedef struct __SharedMemStruct
{
	RecordingData 		RecordingDataBuff[RECORDING_DATA_BUFF_SIZE][MAX_NUM_OF_MWA][MAX_NUM_OF_CHAN_PER_MWA];
	SpikeTimeStamp 	SpikeTimeStampBuff[SPIKE_TIMESTAMPS_BUFF_SIZE];
	EventTimeStamp 	EventTimeStampsBuff[EVENT_TIMESTAMPS_BUFF_SIZE];
	CommandTimeStamp 	CommandTimeStampsBuff[COMMAND_TIMESTAMPS_BUFF_SIZE];	
} SharedMemStruct;

SharedMemStruct *SharedMemory;

