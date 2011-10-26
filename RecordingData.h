#define RECORDING_DATA_H

#ifndef TYPES_H
#include "Types.h"
#endif

#define RECORDING_DATA_BUFF_SIZE 40000

#define MAX_NUM_OF_MWA 2
#define MAX_NUM_OF_CHAN_PER_MWA 8


typedef struct __RecordingData
{
	float data[MAX_NUM_OF_MWA][MAX_NUM_OF_CHAN_PER_MWA];
} RecordingData;




