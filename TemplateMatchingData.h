#define TEMPLATEMATCHINGDATA_H

#ifndef RECORDINGDATA_H
#include "RecordingData.h"
#endif

#define NUM_OF_UNIT_PER_CHAN 3
#define NUM_OF_SAMP_PER_SPIKE 30

typedef struct __TemplateMatchingData
{
	double 	template[MAX_NUM_OF_MWA][MAX_NUM_OF_CHAN_PER_MWA][NUM_OF_UNIT_PER_CHAN][NUM_OF_SAMP_PER_SPIKE];   //mean
	double 	S[MAX_NUM_OF_MWA][MAX_NUM_OF_CHAN_PER_MWA][NUM_OF_UNIT_PER_CHAN][NUM_OF_SAMP_PER_SPIKE][NUM_OF_SAMP_PER_SPIKE]; //covariance
	double 	inv_S[MAX_NUM_OF_MWA][MAX_NUM_OF_CHAN_PER_MWA][NUM_OF_UNIT_PER_CHAN][NUM_OF_SAMP_PER_SPIKE][NUM_OF_SAMP_PER_SPIKE];
	double 	sqrt_det_S[MAX_NUM_OF_MWA][MAX_NUM_OF_CHAN_PER_MWA][NUM_OF_UNIT_PER_CHAN][NUM_OF_SAMP_PER_SPIKE][NUM_OF_SAMP_PER_SPIKE];
	double 	log_det_S[MAX_NUM_OF_MWA][MAX_NUM_OF_CHAN_PER_MWA][NUM_OF_UNIT_PER_CHAN];	
	double 	probability_thres[MAX_NUM_OF_MWA][MAX_NUM_OF_CHAN_PER_MWA][NUM_OF_UNIT_PER_CHAN];
	float 	amplitude_thres[MAX_NUM_OF_MWA][MAX_NUM_OF_CHAN_PER_MWA][NUM_OF_UNIT_PER_CHAN];
	bool 	sorting_on[MAX_NUM_OF_MWA][MAX_NUM_OF_CHAN_PER_MWA][NUM_OF_UNIT_PER_CHAN];
	bool 	include_unit[MAX_NUM_OF_MWA][MAX_NUM_OF_CHAN_PER_MWA][NUM_OF_UNIT_PER_CHAN];
} TemplateMatchingData;


