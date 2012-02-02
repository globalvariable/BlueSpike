#ifndef KERNELTASKCTRL_H
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
	bool		daq_card_mapped;	
	bool		kill_all_rt_tasks;		// write here from any task to stop all rt tasks. KernelSpike will stop rt_timer. Do not stop rt_timer from any other rt_task. Freezes. . 
	bool 		recorder_recording;          // other tasks checks this flag to start_stop recording.
} KernelTaskCtrl;


#endif
