#ifndef RT_TASK_STATS_H
#define RT_TASK_STATS_H

#include <gtk/gtk.h>
#include <stdbool.h>
#include <rtai_sched.h>
#include <rtai_shm.h>
#include "../../System/RtTasksData/RtTasksData.h"
#include "../Misc/Misc.h"

bool check_rt_task_specs_to_init(RtTasksData *rt_tasks_data, unsigned int cpu_id, unsigned int cpu_thread_id, unsigned int cpu_thread_task_id, TimeStamp period, bool bluespike_rt_task);
bool write_rt_task_specs_to_rt_tasks_data(RtTasksData *rt_tasks_data, unsigned int cpu_id, unsigned int cpu_thread_id, unsigned int cpu_thread_task_id, TimeStamp period, unsigned int positive_jitter_threshold, unsigned int negative_jitter_threshold, unsigned int run_time_threshold, char *task_name, bool bluespike_rt_task);
bool delete_rt_task_from_rt_tasks_data(RtTasksData *rt_tasks_data, unsigned int cpu_id, unsigned int cpu_thread_id, unsigned int cpu_thread_task_id, bool bluespike_rt_task);
void evaluate_and_save_jitter(RtTasksData *rt_tasks_data, unsigned int cpu_id, unsigned int cpu_thread_id, unsigned int cpu_thread_task_id, RTIME occurred_time_count, RTIME expected_time_count);
void evaluate_and_save_period_run_time(RtTasksData *rt_tasks_data, unsigned int cpu_id, unsigned int cpu_thread_id, unsigned int cpu_thread_task_id, RTIME process_start_time, RTIME process_end_time);
void write_run_time_to_averaging_buffer(RtTasksData *rt_tasks_data, unsigned int cpu_id, unsigned int cpu_thread_id, unsigned int cpu_thread_task_id, RTIME process_start_time, RTIME process_end_time);

#endif
