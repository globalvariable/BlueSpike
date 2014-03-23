#ifndef MAIN_H
#define MAIN_H


#define _GNU_SOURCE	// for sched_setaffinity(0, sizeof(mask), &mask))

#include <sched.h> // for sched_setaffinity(0, sizeof(mask), &mask))

#include "Gui.h"
#include "RtTask.h"

void create_gui(void);




#endif
