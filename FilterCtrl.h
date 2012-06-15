#ifndef FILTER_CTRL_H
#define FILTER_CTRL_H

#include <stdbool.h>

typedef struct __FilterCtrl
{
	bool 		highpass_150Hz_on;
	bool 		highpass_400Hz_on;
	bool 		lowpass_8KHz_on;
} FilterCtrl;

#endif
