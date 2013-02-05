#ifndef RECORDINGDATA_H
#define RECORDINGDATA_H

#include "MicroWireArray.h"

#define RECORDING_DATA_BUFF_SIZE 40000
#define RECORDING_DATA_BUFF_SIZE_1 39999
#define RECORDING_DATA_BUFF_SIZE_2 39998
#define RECORDING_DATA_BUFF_SIZE_3 39997
#define RECORDING_DATA_BUFF_SIZE_4 39996

typedef double RecordingDataSample;

typedef RecordingDataSample RecordingDataChanBuff[RECORDING_DATA_BUFF_SIZE];

typedef RecordingDataChanBuff RecordingDataBuff[MAX_NUM_OF_MWA][MAX_NUM_OF_CHAN_PER_MWA];

typedef struct __RecordingData
{
	RecordingDataBuff	recording_data_buff;
	int				buff_idx_write[MAX_NUM_OF_MWA][MAX_NUM_OF_CHAN_PER_MWA];      // Do not read data pointed by buff_idx_write. Read up to buff_idx_write by excluding buff_idx_write
	int				buff_idx_prev[MAX_NUM_OF_MWA][MAX_NUM_OF_CHAN_PER_MWA];     
} RecordingData;


#endif
