#define RECORDINGDATA_H

#ifndef TYPES_H
#include "Types.h"
#endif

#ifndef MICROWIREARRAY_H
#include "MicroWireArray.h"
#endif

#define RECORDING_DATA_BUFF_SIZE 40000
#define RECORDING_DATA_BUFF_SIZE_1 39999
#define RECORDING_DATA_BUFF_SIZE_2 39998
#define RECORDING_DATA_BUFF_SIZE_3 39997
#define RECORDING_DATA_BUFF_SIZE_4 39996

typedef float RecordingDataChanBuff[RECORDING_DATA_BUFF_SIZE];

typedef RecordingDataChanBuff RecordingDataBuff[MAX_NUM_OF_MWA][MAX_NUM_OF_CHAN_PER_MWA];

typedef struct __RecordingData
{
	RecordingDataBuff	recording_data_buff;
	int				buff_idx_write[MAX_NUM_OF_MWA][MAX_NUM_OF_CHAN_PER_MWA];
} RecordingData;

