#ifndef BLUESPIKETIMESTAMP_H
#define BLUESPIKETIMESTAMP_H

#include <stdbool.h>
#include "System/TimeStamp/TimeStamp.h"
#include "MicroWireArray.h"

#define BLUE_SPIKE_TIME_STAMP_BUFF_SIZE MAX_NUM_OF_MWA*MAX_NUM_OF_CHAN_PER_MWA*1000      // 1 seconds of buffer at worst scenario (if each channel spikes every millisecond )

typedef struct __BlueSpikeTimeStampItem
{
	TimeStamp 	peak_time;	// spike peak time
	int	 		mwa;      		// mwa from which spike sorted
	int 			channel;		// channel from which spike sorted
	int 			unit;			// channel from which spike sorted
	int			recording_data_buff_idx;   // end of spike at filtered recording data
	bool		include_unit;	// spike well-sorted 
} BlueSpikeTimeStampItem;

typedef BlueSpikeTimeStampItem	BlueSpikeTimeStampBuff[BLUE_SPIKE_TIME_STAMP_BUFF_SIZE];

typedef struct __BlueSpikeTimeStamp
{
	BlueSpikeTimeStampBuff		blue_spike_time_stamp_buff;
	int						buff_idx_write;
} BlueSpikeTimeStamp;



#endif
