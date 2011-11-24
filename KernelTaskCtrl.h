#define KERNELTASKCTRL_H

#include <stdbool.h>

typedef struct __KernelTaskCtrl
{
	bool 	highpass_150Hz_on;
	bool 	highpass_400Hz_on;
	bool 	lowpass_8KHz_on;
	bool 	sorting_on;
} KernelTaskCtrl;



