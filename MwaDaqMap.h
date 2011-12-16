#ifndef MWADAQMAP_H
#define MWADAQMAP_H

#include "DaqCard.h"
#include "MicroWireArray.h"

typedef struct   __MwaDaqMapItem
{
	int daq_card;
	int daq_chan;
} MwaDaqMapItem;

typedef MwaDaqMapItem   MwaDaqMap[MAX_NUM_OF_MWA][MAX_NUM_OF_CHAN_PER_MWA];



#endif
