#define SPIKEENDHANDLINGDATA_H

#define SPIKE_END_HANDLING_DATA_BUFF_SIZE 10000

#ifndef TIMESTAMP_H
#include "TimeStamp.h"
#endif

typedef struct __SpikeEndHandlingItem
{
	int 			recording_data_buff_idx;    // index of SpikeEndData in filtered recording data buffer 
	int 			mwa;
	int 			chan;
	TimeStamp	peak_time;
} SpikeEndHandlingItem;

typedef SpikeEndHandlingItem	SpikeEndHandlingBuff[SPIKE_END_HANDLING_DATA_BUFF_SIZE];

typedef struct __SpikeEndHandling
{
	SpikeEndHandlingBuff		spike_end_handling_buff;
	int						buff_start_idx;					// circular buffer, start reading from here. it discards the previous items
	int						buff_write_idx;
	int						spike_end_buff_prev_idx;			// item of spike_end_buffer which has been read finally
	int 						filtered_recording_data_prev_idx[MAX_NUM_OF_MWA][MAX_NUM_OF_CHAN_PER_MWA];	
} SpikeEndHandling;

