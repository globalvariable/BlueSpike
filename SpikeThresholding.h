#define SPIKETHRESHOLDING_H

typedef struct __SpikeThresholding
{
	float 			amplitude_thres[MAX_NUM_OF_MWA][MAX_NUM_OF_CHAN_PER_MWA];
	bool 			in_spike[MAX_NUM_OF_MWA][MAX_NUM_OF_CHAN_PER_MWA];
} SpikeThresholding;


