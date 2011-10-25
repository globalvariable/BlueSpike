#ifndef TYPES_H
#include "Types.h"
#endif

#define SPIKE_TIMESTAMPS_BUFF_SIZE 10000

typedef struct __SpikeTimeStamp
{
	TimeStamp 	time;
	char 		mwa;      		// mwa from which spike sorted
	char 		channel;		// channel from which spike sorted
	char 		unit;			// channel from which spike sorted
} SpikeTimeStamp;


