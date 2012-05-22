#ifndef BLUE_SPIKE_CTRL_H
#define BLUE_SPIKE_CTRL_H

#include <stdbool.h>

typedef struct __BlueSpikeCtrl
{
	bool 		highpass_150Hz_on;
	bool 		highpass_400Hz_on;
	bool 		lowpass_8KHz_on;
	bool		turn_daq_card_on;
	bool		daq_card_mapped;
} BlueSpikeCtrl;

#endif
