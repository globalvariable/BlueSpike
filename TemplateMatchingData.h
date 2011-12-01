#define TEMPLATEMATCHINGDATA_H

#ifndef RECORDINGDATA_H
#include "RecordingData.h"
#endif

#include <stdbool.h>

#define MAX_NUM_OF_UNIT_PER_CHAN 6
#define NUM_OF_SAMP_PER_SPIKE 30

typedef struct __TemplateMatchingData
{
	double 	template[NUM_OF_SAMP_PER_SPIKE];   //mean
	double 	S[NUM_OF_SAMP_PER_SPIKE][NUM_OF_SAMP_PER_SPIKE]; //covariance
	double 	inv_S[NUM_OF_SAMP_PER_SPIKE][NUM_OF_SAMP_PER_SPIKE];
	double 	sqrt_det_S;
	double 	log_det_S;	
	double 	probability_thres;
	bool 	sorting_on;
	bool 	include_unit;
} TemplateMatchingUnitData;


typedef TemplateMatchingUnitData TemplateMatchingData[MAX_NUM_OF_MWA][MAX_NUM_OF_CHAN_PER_MWA][MAX_NUM_OF_UNIT_PER_CHAN];



