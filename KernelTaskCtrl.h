#define KERNELTASKCTRL_H

#include <stdbool.h>

typedef struct __KernelTaskCtrl
{
	TimeStamp	current_time_ns;
	TimeStamp	previous_time_ns;	
	bool 		highpass_150Hz_on;
	bool 		highpass_400Hz_on;
	bool 		lowpass_8KHz_on;
	bool		kernel_task_idle;
	bool		turn_daq_card_on;
} KernelTaskCtrl;



