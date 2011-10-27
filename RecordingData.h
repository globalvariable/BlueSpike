#define RECORDINGDATA_H

#ifndef TYPES_H
#include "Types.h"
#endif

#ifndef MICROWIREARRAY_H
#include "MicroWireArray.h"
#endif

#define RECORDING_DATA_BUFF_SIZE 40000


typedef float RecordingDataBuff[MAX_NUM_OF_MWA][MAX_NUM_OF_CHAN_PER_MWA][RECORDING_DATA_BUFF_SIZE];

typedef struct __RecordingData
{
	RecordingDataBuff	recording_data_buff;
	int				buff_idx_write[MAX_NUM_OF_MWA][MAX_NUM_OF_CHAN_PER_MWA];
} RecordingData;

