#define DAQMWAMAP_H

#include "DaqCard.h"
#include "MicroWireArray.h"

typedef struct   __DaqMwaMapItem
{
	int mwa;
	int channel;
} DaqMwaMapItem;

typedef DaqMwaMapItem   DaqMwaMap[MAX_NUM_OF_DAQ_CARD][MAX_NUM_OF_CHANNEL_PER_DAQ_CARD];


