#ifndef RT_TASK_H
#define RT_TASK_H


#include <gtk/gtk.h>
#include <stdbool.h>
#include <rtai_shm.h>
#include <rtai_nam2num.h>

#include "../../../BlueSpike/Library/Misc/Misc.h"
#include "../../../BlueSpike/System/RtTasksData/RtTasksData.h"
#include "../../../BlueSpike/Library/RtTaskStats/RtTaskStats.h"
#include "../../../BlueSpike/System/ShmSemNum/ShmSemNum.h"




bool start_periodic_task(void);
bool start_acquisition(void);





#endif
