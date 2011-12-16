#ifndef SPIKEENDDATA_H
#define SPIKEENDDATA_H

#include "../TimeStamp.h"
#include "../MicroWireArray.h"

#define SPIKE_END_DATA_BUFF_SIZE 	MAX_NUM_OF_MWA*MAX_NUM_OF_CHAN_PER_MWA*1000      // 1 seconds of buffer at worst scenario (if each channel spikes every millisecond )

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

#endif
