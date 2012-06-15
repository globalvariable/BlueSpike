#ifndef KERNELTASKCTRL_H
#define KERNELTASKCTRL_H

#include <stdbool.h>

typedef struct __KernelTaskCtrl
{
	bool		kill_all_rt_tasks;		// write here from any task to stop all rt tasks. KernelSpike will stop rt_timer. Do not stop rt_timer from any other rt_task. Freezes. . 
} KernelTaskCtrl;


#endif
