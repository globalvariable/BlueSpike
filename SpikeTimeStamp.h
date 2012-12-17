#ifndef SPIKETIMESTAMP_H
#define SPIKETIMESTAMP_H

#include "System/TimeStamp/TimeStamp.h"
#include "MicroWireArray.h"

#define SPIKE_TIME_STAMP_BUFF_SIZE MAX_NUM_OF_MWA*MAX_NUM_OF_CHAN_PER_MWA*1000      // 1 seconds of buffer at worst scenario (if each channel spikes every millisecond )

typedef struct __SpikeTimeStampItem
{
	TimeStamp 		peak_time;				// spike peak time
	unsigned int		mwa_or_layer;				// mwa from which spike sorted
	unsigned int		channel_or_neuron_group;		// channel from which spike sorted
	unsigned int		unit_or_neuron;				// channel from which spike sorted
} SpikeTimeStampItem;

typedef SpikeTimeStampItem	SpikeTimeStampBuff[SPIKE_TIME_STAMP_BUFF_SIZE];

typedef struct __SpikeTimeStamp
{
	SpikeTimeStampBuff		spike_time_stamp_buff;
	unsigned int				buff_idx_write;
} SpikeTimeStamp;



#endif
