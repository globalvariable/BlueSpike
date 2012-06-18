#ifndef TEMPLATEMATCHINGDATA_H
#define TEMPLATEMATCHINGDATA_H

#include "RecordingData.h"
#include <stdbool.h>

#define MAX_NUM_OF_UNIT_PER_CHAN 4
#define NUM_OF_SAMP_PER_SPIKE 30

#define TEMPLATE_PROBAB_THRES_NULL 0
typedef unsigned int UnitNum;
typedef double TemplateProbabilityThres;



typedef struct __TemplateMatchingData
{
	double 					template[NUM_OF_SAMP_PER_SPIKE];   //mean
	double 					inv_S[NUM_OF_SAMP_PER_SPIKE][NUM_OF_SAMP_PER_SPIKE];  // inverted covariance
	double 					sqrt_det_S;
	double 					log_det_S;	
	TemplateProbabilityThres 	probability_thres;
	bool 					sorting_on;
	bool 					include_unit;
} TemplateMatchingUnitData;


typedef TemplateMatchingUnitData TemplateMatchingData[MAX_NUM_OF_MWA][MAX_NUM_OF_CHAN_PER_MWA][MAX_NUM_OF_UNIT_PER_CHAN];


#endif
