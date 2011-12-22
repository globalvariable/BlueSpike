#ifndef SPIKEENDHANDLINGDATA_H
#define SPIKEENDHANDLINGDATA_H

#include "../TimeStamp.h"
#include "../MicroWireArray.h"

#define SPIKE_END_HANDLING_DATA_BUFF_SIZE MAX_NUM_OF_MWA*MAX_NUM_OF_CHAN_PER_MWA*1000      // 1 seconds of buffer at worst scenario (if each channel spikes every millisecond )

#define SPIKE_MIN_END_SAMP_NUM   15

typedef struct __SpikeEndHandlingItem
{
	int 			end_of_spike_in_filtered_recording_data_buff;    // index of SpikeEndData in filtered recording data buffer 
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
} SpikeEndHandling;


#endif
