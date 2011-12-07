#define SPIKETIMESTAMP_H

#ifndef TIMESTAMP_H
#include "TimeStamp.h"
#endif

#include "MicroWireArray.h"

#define SPIKE_TIMESTAMP_BUFF_SIZE MAX_NUM_OF_MWA*MAX_NUM_OF_CHAN_PER_MWA*1000      // 1 seconds of buffer at worst scenario (if each channel spikes every millisecond )

typedef struct __SpikeTimeStampItem
{
	TimeStamp 	peak_time;	// spike peak time
	int	 		mwa;      		// mwa from which spike sorted
	int 			channel;		// channel from which spike sorted
	int 			unit;			// channel from which spike sorted
	int			recording_data_buff_idx;   // end of spike at filtered recording data
	bool		include_unit;	// spike well-sorted 
} SpikeTimeStampItem;

typedef SpikeTimeStampItem	SpikeTimeStampBuff[SPIKE_TIMESTAMP_BUFF_SIZE];

typedef struct __SpikeTimeStamp
{
	SpikeTimeStampBuff		spike_timestamp_buff;
	int					buff_idx_write;
} SpikeTimeStamp;
