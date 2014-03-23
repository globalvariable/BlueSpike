#ifndef TEMPLATEMATCHING_H
#define TEMPLATEMATCHING_H


#include <stdbool.h>
#include <math.h>
#include "ConfigDaq/MicroWireArray.h"
#include "SpikeTimeStamp.h"
#include "BlueSpikeTimeStamp.h"
#include "RecordingData.h"

#define NUM_OF_SAMP_PER_SPIKE 	24    // adjusted according to 31.25 KHz sampling rate

#define TEMPLATE_PROBAB_THRES_NULL 0
typedef unsigned int UnitNum;
typedef double TemplateProbabilityThres;



typedef struct __TemplateMatchingUnitData
{
	double 					template[NUM_OF_SAMP_PER_SPIKE];   //mean
	double 					inv_S[NUM_OF_SAMP_PER_SPIKE][NUM_OF_SAMP_PER_SPIKE];  // inverted covariance
	double 					sqrt_det_S;
	double 					log_det_S;	
	TemplateProbabilityThres 	probability_thres;
	bool 					sorting_on;
	bool 					include_unit;
	TemplateProbabilityThres 	alarm_thres;
	bool 					alarm_on;
	unsigned int				alarm_count;
} TemplateMatchingUnitData;


typedef TemplateMatchingUnitData TemplateMatchingData[MAX_NUM_OF_MWA][MAX_NUM_OF_CHAN_PER_MWA][MAX_NUM_OF_UNIT_PER_CHAN+1]; // +1 for not sorted


void run_template_matching(int mwa, int chan, int filtered_recording_data_buff_idx, TimeStamp peak_time);


TemplateMatchingData template_matching_data;

#endif
