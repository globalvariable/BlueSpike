#ifndef DAQ_MWA_DATA_H
#define DAQ_MWA_DATA_H

#include <stdbool.h>

#include "DaqCard.h"
#include "MicroWireArray.h"

typedef struct   __DaqMwaMapItem
{
	unsigned int mwa;
	unsigned int channel;
} DaqMwaMapItem;

typedef struct   __DaqMwaMap
{
	DaqMwaMapItem   map[MAX_NUM_OF_CHANNEL_PER_DAQ_CARD];
	pthread_mutex_t 	mutex;
} DaqMwaMap;


DaqMwaMap	daq_mwa_map[MAX_NUM_OF_DAQ_CARD];



bool initialize_daq_mwa_map_data(void);
bool config_daq_mwa_map_data_range(unsigned int daq_num, unsigned int daq_chan_start, unsigned int daq_chan_end, unsigned int mwa_num, unsigned int mwa_chan_start, unsigned int mwa_chan_end);
bool config_daq_mwa_map_data(unsigned int daq_num, unsigned int daq_chan, unsigned int mwa_num, unsigned int mwa_chan);
bool cancel_daq_mwa_map_data(void);
bool interrogate_daq_mwa_map(void);

#endif
