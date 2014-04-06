#include "RtTaskStats.h"

bool check_rt_task_specs_to_init(RtTasksData *rt_tasks_data, unsigned int cpu_id, unsigned int cpu_thread_id, unsigned int cpu_thread_task_id, TimeStamp period, bool bluespike_rt_task)
{
	if (rt_tasks_data == NULL)
		return print_message(ERROR_MSG ,"BlueSpike", "RtTaskStats", "check_rt_task_specs_to_init", "rt_tasks_data == NULL.");		
	if ((rt_tasks_data->cpus_rt_task_data[cpu_id].rt_task_period != 0) && (rt_tasks_data->cpus_rt_task_data[cpu_id].rt_task_period != period)) 
		return print_message(ERROR_MSG ,"BlueSpike", "RtTaskStats", "check_rt_task_specs_to_init", "Inconvenient period submitted for rt_task.");
	if ((!bluespike_rt_task) & (rt_tasks_data->num_of_total_rt_tasks == 0))
		return print_message(ERROR_MSG ,"BlueSpike", "RtTaskStats", "check_rt_task_specs_to_init", "(!bluespike_rt_task) & (rt_tasks_data->num_of_total_rt_tasks == 0).");
	if (rt_tasks_data->cpus_rt_task_data[cpu_id].cpu_threads_rt_data[cpu_thread_id].cpu_thread_tasks_rt_data[cpu_thread_task_id].task_name[0] != 0)
		return print_message(ERROR_MSG ,"BlueSpike", "RtTaskStats", "check_rt_task_specs_to_init", "There is a task at this cpu/thread/task id == 0.");
	return TRUE;
}

bool write_rt_task_specs_to_rt_tasks_data(RtTasksData *rt_tasks_data, unsigned int cpu_id, unsigned int cpu_thread_id, unsigned int cpu_thread_task_id, TimeStamp period, unsigned int positive_jitter_threshold, unsigned int negative_jitter_threshold, char *task_name, bool bluespike_rt_task)
{
	if (!check_rt_task_specs_to_init(rt_tasks_data, cpu_id, cpu_thread_id, cpu_thread_task_id, period, bluespike_rt_task))
		return print_message(ERROR_MSG ,"BlueSpike", "RtTaskStats", "write_rt_task_specs_to_rt_tasks_data", "! check_rt_task_specs_to_init().");
	rt_tasks_data->cpus_rt_task_data[cpu_id].rt_task_period = period;			
	rt_tasks_data->num_of_total_rt_tasks++;
	rt_tasks_data->cpus_rt_task_data[cpu_id].num_of_rt_tasks_at_cpu++;
	rt_tasks_data->cpus_rt_task_data[cpu_id].cpu_threads_rt_data[cpu_thread_id].num_of_rt_tasks_at_cpu_thread++;
	strcpy(rt_tasks_data->cpus_rt_task_data[cpu_id].cpu_threads_rt_data[cpu_thread_id].cpu_thread_tasks_rt_data[cpu_thread_task_id].task_name, task_name);
	rt_tasks_data->cpus_rt_task_data[cpu_id].cpu_threads_rt_data[cpu_thread_id].cpu_thread_tasks_rt_data[cpu_thread_task_id].positive_jitter_threshold = positive_jitter_threshold;
	rt_tasks_data->cpus_rt_task_data[cpu_id].cpu_threads_rt_data[cpu_thread_id].cpu_thread_tasks_rt_data[cpu_thread_task_id].negative_jitter_threshold = negative_jitter_threshold;
	return TRUE;
}

bool delete_rt_task_from_rt_tasks_data(RtTasksData *rt_tasks_data, unsigned int cpu_id, unsigned int cpu_thread_id, unsigned int cpu_thread_task_id, bool bluespike_rt_task)
{
	rt_tasks_data->num_of_total_rt_tasks--;
	rt_tasks_data->cpus_rt_task_data[cpu_id].num_of_rt_tasks_at_cpu--;
	if (rt_tasks_data->cpus_rt_task_data[cpu_id].num_of_rt_tasks_at_cpu == 0)		// no task remained at cpu
		rt_tasks_data->cpus_rt_task_data[cpu_id].rt_task_period = 0;
	rt_tasks_data->cpus_rt_task_data[cpu_id].cpu_threads_rt_data[cpu_thread_id].num_of_rt_tasks_at_cpu_thread--;	

	rt_tasks_data->cpus_rt_task_data[cpu_id].cpu_threads_rt_data[cpu_thread_id].cpu_thread_tasks_rt_data[cpu_thread_task_id].task_name[0] = 0;		
	rt_tasks_data->cpus_rt_task_data[cpu_id].cpu_threads_rt_data[cpu_thread_id].cpu_thread_tasks_rt_data[cpu_thread_task_id].max_task_run_time = 0;		
	rt_tasks_data->cpus_rt_task_data[cpu_id].cpu_threads_rt_data[cpu_thread_id].cpu_thread_tasks_rt_data[cpu_thread_task_id].max_positive_jitter = 0;	
	rt_tasks_data->cpus_rt_task_data[cpu_id].cpu_threads_rt_data[cpu_thread_id].cpu_thread_tasks_rt_data[cpu_thread_task_id].max_negative_jitter = 0;	
	rt_tasks_data->cpus_rt_task_data[cpu_id].cpu_threads_rt_data[cpu_thread_id].cpu_thread_tasks_rt_data[cpu_thread_task_id].positive_jitter_threshold = 0;		
	rt_tasks_data->cpus_rt_task_data[cpu_id].cpu_threads_rt_data[cpu_thread_id].cpu_thread_tasks_rt_data[cpu_thread_task_id].negative_jitter_threshold = 0;	
	rt_tasks_data->cpus_rt_task_data[cpu_id].cpu_threads_rt_data[cpu_thread_id].cpu_thread_tasks_rt_data[cpu_thread_task_id].max_positive_jitter = 0;		
	rt_tasks_data->cpus_rt_task_data[cpu_id].cpu_threads_rt_data[cpu_thread_id].cpu_thread_tasks_rt_data[cpu_thread_task_id].max_negative_jitter = 0;		
	rt_tasks_data->cpus_rt_task_data[cpu_id].cpu_threads_rt_data[cpu_thread_id].cpu_thread_tasks_rt_data[cpu_thread_task_id].num_of_positive_jitter_exceeding_threshold = 0;		
	rt_tasks_data->cpus_rt_task_data[cpu_id].cpu_threads_rt_data[cpu_thread_id].cpu_thread_tasks_rt_data[cpu_thread_task_id].num_of_negative_jitter_exceeding_threshold = 0;
		

	return TRUE;
}

void evaluate_and_save_jitter(RtTasksData *rt_tasks_data, unsigned int cpu_id, unsigned int cpu_thread_id, unsigned int cpu_thread_task_id, unsigned int previous_period_end_time, unsigned int current_period_end_time)
{
	unsigned int period, jitter, submitted_period;
	CpuThreadTasksRtData *cpu_thread_tasks_rt_data = &(rt_tasks_data->cpus_rt_task_data[cpu_id].cpu_threads_rt_data[cpu_thread_id].cpu_thread_tasks_rt_data[cpu_thread_task_id]) ; 

	period = current_period_end_time - previous_period_end_time;	
	submitted_period = rt_tasks_data->cpus_rt_task_data[cpu_id].rt_task_period;
	if (period > submitted_period)
	{
		jitter = period - submitted_period;
		if (jitter > cpu_thread_tasks_rt_data->max_positive_jitter)
			cpu_thread_tasks_rt_data->max_positive_jitter = jitter;
		if (jitter > cpu_thread_tasks_rt_data->positive_jitter_threshold)
			cpu_thread_tasks_rt_data->num_of_positive_jitter_exceeding_threshold++;
	}
	else
	{
		jitter = submitted_period - period;
		if (jitter > cpu_thread_tasks_rt_data->max_negative_jitter)
			cpu_thread_tasks_rt_data->max_negative_jitter = jitter;
		if (jitter > cpu_thread_tasks_rt_data->negative_jitter_threshold)
			cpu_thread_tasks_rt_data->num_of_negative_jitter_exceeding_threshold++;
	}
	return;
}

void evaluate_and_save_period_run_time(RtTasksData *rt_tasks_data, unsigned int cpu_id, unsigned int cpu_thread_id, unsigned int cpu_thread_task_id, unsigned int process_start_time, unsigned int process_end_time)
{
	unsigned int task_run_time;
	CpuThreadTasksRtData *cpu_thread_tasks_rt_data = &(rt_tasks_data->cpus_rt_task_data[cpu_id].cpu_threads_rt_data[cpu_thread_id].cpu_thread_tasks_rt_data[cpu_thread_task_id]) ; 

	task_run_time = process_end_time - process_start_time;

	if (task_run_time > cpu_thread_tasks_rt_data->max_task_run_time)
		cpu_thread_tasks_rt_data->max_task_run_time = task_run_time;
	return;
}
