#define SPIKETIMESTAMP_H

#ifndef TIMESTAMP_H
#include "TimeStamp.h"
#endif

#define SPIKE_TIMESTAMP_BUFF_SIZE 10000

typedef struct __SpikeTimeStampItem
{
	TimeStamp 	peak_time;	// spike peak time
	char 		mwa;      		// mwa from which spike sorted
	char 		channel;		// channel from which spike sorted
	char 		unit;			// channel from which spike sorted
} SpikeTimeStampItem;

typedef SpikeTimeStampItem	SpikeTimeStampBuff[SPIKE_TIMESTAMP_BUFF_SIZE];

typedef struct __SpikeTimeStamp
{
	SpikeTimeStampBuff		spike_timestamp_buff;
	int					buff_idx_write;
	int					spike_end_buff_read_idx;
	int					spike_end_recording_data_read_idx[MAX_NUM_OF_MWA][MAX_NUM_OF_CHAN_PER_MWA];
} SpikeTimeStamp;
