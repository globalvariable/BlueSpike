#ifndef SPIKE_SORTING_H
#define SPIKE_SORTING_H

#include "ConfigDaq/MicroWireArray.h"
#include "ConfigDaq/DaqMwaData.h"
#include "TemplateMatching.h"
#include "SpikeThresholding.h"

#define SPIKE_END_HANDLING_DATA_BUFF_SIZE 	1000      // 1 seconds of buffer at worst scenario (if each channel spikes every millisecond )

#define SPIKE_MIN_END_SAMP_NUM   (NUM_OF_SAMP_PER_SPIKE / 2)

#define IN_SPIKE_SAMPLE_CNTR_MAX	((1000000/SAMPLING_INTERVAL)/3)		// not a spike if lasts longer than  ~320 us  

typedef struct __SpikeEndHandlingItem
{
	int 			end_of_spike_in_filtered_recording_data_buff;    // index of SpikeEndData in filtered recording data buffer 
	TimeStamp	peak_time;
} SpikeEndHandlingItem;

typedef SpikeEndHandlingItem	SpikeEndHandlingBuff[SPIKE_END_HANDLING_DATA_BUFF_SIZE];

typedef struct __SpikeEndHandling
{
	SpikeEndHandlingBuff		spike_end_handling_buff;
	int						buff_start_idx;					// circular buffer, start reading from here. it discards the previous items
	int						buff_write_idx;
} SpikeEndHandlingChan;

typedef SpikeEndHandlingChan	SpikeEndHandling[MAX_NUM_OF_MWA][MAX_NUM_OF_CHAN_PER_MWA];

SpikeEndHandling	spike_end_handling;

void spike_sorting(unsigned int daq_num, TimeStamp previous_daq_time_ns);



#endif
