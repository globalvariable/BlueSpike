#define SPIKEENDDATA_H

#define SPIKE_END_DATA_BUFF_SIZE 10000

#ifndef TIMESTAMP_H
#include "TimeStamp.h"
#endif

#include <stdbool.h>

typedef struct __SpikeEndItem
{
	int 			end_of_spike_in_filtered_recording_data_buff;    // index of SpikeEndData in filtered recording data buffer 
	int 			mwa;
	int 			chan;
	TimeStamp	peak_time;
} SpikeEndItem;

typedef SpikeEndItem	SpikeEndBuff[SPIKE_END_DATA_BUFF_SIZE];

typedef struct __SpikeEnd
{
	SpikeEndBuff		spike_end_buff;
	int				buff_idx_write;
	int				filtered_recording_data_start_idx_to_read[MAX_NUM_OF_MWA][MAX_NUM_OF_CHAN_PER_MWA];
} SpikeEnd;

#define SPIKE_MIN_END_SAMP_NUM   15
