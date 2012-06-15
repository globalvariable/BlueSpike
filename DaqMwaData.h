#ifndef DAQ_MWA_DATA_H
#define DAQ_MWA_DATA_H


#include "DaqCard.h"
#include "MicroWireArray.h"

typedef struct   __DaqMwaMapItem
{
	unsigned int mwa;
	unsigned int channel;
} DaqMwaMapItem;

typedef struct   __MwaDaqMapItem
{
	unsigned int daq_card;
	unsigned int daq_chan;
} MwaDaqMapItem;

typedef DaqMwaMapItem   DaqMwaMap[MAX_NUM_OF_DAQ_CARD][MAX_NUM_OF_CHANNEL_PER_DAQ_CARD];
typedef MwaDaqMapItem   MwaDaqMap[MAX_NUM_OF_MWA][MAX_NUM_OF_CHAN_PER_MWA];


typedef struct   __DaqMwaData
{
	bool		daq_cards_on;
	DaqMwaMap	daq_mwa_map;
	MwaDaqMap	mwa_daq_map;
} DaqMwaData;




#endif
