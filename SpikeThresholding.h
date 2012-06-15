#ifndef SPIKETHRESHOLDING_H
#define SPIKETHRESHOLDING_H

#include <stdbool.h>

typedef struct __SpikeThresholding
{
	float 			amplitude_thres[MAX_NUM_OF_MWA][MAX_NUM_OF_CHAN_PER_MWA];
} SpikeThresholding;


#endif

