#define SPIKETIMESTAMP_H

#ifndef TYPES_H
#include "Types.h"
#endif

#define SPIKE_TIMESTAMP_BUFF_SIZE 10000

typedef struct __SpikeTimeStampItem
{
	TimeStamp 	time;
	char 		mwa;      		// mwa from which spike sorted
	char 		channel;		// channel from which spike sorted
	char 		unit;			// channel from which spike sorted
} SpikeTimeStampItem;

typedef SpikeTimeStampItem	SpikeTimeStampBuff[SPIKE_TIMESTAMP_BUFF_SIZE];

typedef struct __SpikeTimeStamp
{
	SpikeTimeStampBuff		spike_timestamp_buff;
	int					buff_idx_write;
} SpikeTimeStamp;
