#ifndef RT_TASK_STATS_H
#define RT_TASK_STATS_H

#include <gtk/gtk.h>
#include <stdbool.h>
#include <rtai_sched.h>
#include "../../SharedMemory.h"
#include "../Misc/Misc.h"

bool check_rt_task_specs_to_init(unsigned int cpu_id, unsigned int cpu_thread_id, TimeStamp period);
bool write_rt_task_specs_to_rt_tasks_data(unsigned int cpu_id, unsigned int cpu_thread_id, TimeStamp period, unsigned int positive_jitter_threshold, unsigned int negative_jitter_threshold);
bool delete_rt_task_from_rt_tasks_data(unsigned int cpu_id, unsigned int cpu_thread_id, TimeStamp period);
void evaluate_and_save_jitter(unsigned int cpu_id, unsigned int cpu_thread_id, unsigned int previous_period_end_time, unsigned int current_period_end_time);
void evaluate_and_save_period_run_time(unsigned int cpu_id, unsigned int cpu_thread_id, unsigned int process_start_time, unsigned int process_end_time);


#endif
