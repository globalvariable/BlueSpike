#include "RtTaskStats.h"

bool check_rt_task_specs_to_init(RtTasksData *rt_tasks_data, unsigned int cpu_id, unsigned int cpu_thread_id, TimeStamp period)
{
	if (rt_tasks_data == NULL)
		return print_message(ERROR_MSG ,"BlueSpike", "RtTaskStats", "check_rt_task_specs_to_init", "rt_tasks_data == NULL.");		
	if ((rt_tasks_data->cpu_rt_task_data[cpu_id].rt_task_period != 0) && (rt_tasks_data->cpu_rt_task_data[cpu_id].rt_task_period != period)) 
		return print_message(ERROR_MSG ,"BlueSpike", "RtTaskStats", "check_rt_task_specs_to_init", "Inconvenient period submitted for rt_task.");	
	if (rt_tasks_data->num_of_total_rt_tasks == 0)
		return print_message(ERROR_MSG ,"BlueSpike", "RtTaskStats", "check_rt_task_specs_to_init", "num_of_total_rt_tasks == 0.");
	return TRUE;
}

bool write_rt_task_specs_to_rt_tasks_data(RtTasksData *rt_tasks_data, unsigned int cpu_id, unsigned int cpu_thread_id, TimeStamp period, unsigned int positive_jitter_threshold, unsigned int negative_jitter_threshold)
{
	if (!check_rt_task_specs_to_init(rt_tasks_data, cpu_id, cpu_thread_id, period))
		return print_message(ERROR_MSG ,"BlueSpike", "RtTaskStats", "write_rt_task_specs_to_rt_tasks_data", "! check_rt_task_specs_to_init().");
	if ((positive_jitter_threshold != rt_tasks_data->cpu_rt_task_data[cpu_id].cpu_thread_rt_task_data[cpu_thread_id].positive_jitter_threshold) && (rt_tasks_data->cpu_rt_task_data[cpu_id].cpu_thread_rt_task_data[cpu_thread_id].positive_jitter_threshold != 0))
		print_message(WARNING_MSG ,"BlueSpike", "RtTaskStats", "write_rt_task_specs_to_rt_tasks_data", "positive_jitter_threshold != previously_assigned_jitter_threshold.");		
	if ((negative_jitter_threshold != rt_tasks_data->cpu_rt_task_data[cpu_id].cpu_thread_rt_task_data[cpu_thread_id].negative_jitter_threshold) && (rt_tasks_data->cpu_rt_task_data[cpu_id].cpu_thread_rt_task_data[cpu_thread_id].negative_jitter_threshold != 0))
		print_message(WARNING_MSG ,"BlueSpike", "RtTaskStats", "write_rt_task_specs_to_rt_tasks_data", "negative_jitter_threshold != previously_assigned_jitter_threshold.");			
	rt_tasks_data->cpu_rt_task_data[cpu_id].rt_task_period = period;			
	rt_tasks_data->num_of_total_rt_tasks++;
	rt_tasks_data->cpu_rt_task_data[cpu_id].num_of_rt_tasks_at_cpu++;
	rt_tasks_data->cpu_rt_task_data[cpu_id].cpu_thread_rt_task_data[cpu_thread_id].num_of_rt_tasks_at_cpu_thread++;
	rt_tasks_data->cpu_rt_task_data[cpu_id].cpu_thread_rt_task_data[cpu_thread_id].positive_jitter_threshold = positive_jitter_threshold;
	rt_tasks_data->cpu_rt_task_data[cpu_id].cpu_thread_rt_task_data[cpu_thread_id].negative_jitter_threshold = negative_jitter_threshold;
	return TRUE;
}

bool delete_rt_task_from_rt_tasks_data(RtTasksData *rt_tasks_data, unsigned int cpu_id, unsigned int cpu_thread_id, TimeStamp period)
{
	if (!check_rt_task_specs_to_init(rt_tasks_data, cpu_id, cpu_thread_id, period))
		return print_message(ERROR_MSG ,"BlueSpike", "RtTaskStats", "delete_rt_task_from_rt_tasks_data", "! check_rt_task_specs_to_init().");	
	rt_tasks_data->cpu_rt_task_data[cpu_id].cpu_thread_rt_task_data[cpu_thread_id].num_of_rt_tasks_at_cpu_thread--;
	if (rt_tasks_data->cpu_rt_task_data[cpu_id].cpu_thread_rt_task_data[cpu_thread_id].num_of_rt_tasks_at_cpu_thread == 0)	// no task remained at cpu thread
	{
		rt_tasks_data->cpu_rt_task_data[cpu_id].cpu_thread_rt_task_data[cpu_thread_id].max_period_run_time = 0;
		rt_tasks_data->cpu_rt_task_data[cpu_id].cpu_thread_rt_task_data[cpu_thread_id].max_positive_jitter = 0;
		rt_tasks_data->cpu_rt_task_data[cpu_id].cpu_thread_rt_task_data[cpu_thread_id].max_negative_jitter = 0;
		rt_tasks_data->cpu_rt_task_data[cpu_id].cpu_thread_rt_task_data[cpu_thread_id].positive_jitter_threshold = 0;
		rt_tasks_data->cpu_rt_task_data[cpu_id].cpu_thread_rt_task_data[cpu_thread_id].negative_jitter_threshold = 0;						
		rt_tasks_data->cpu_rt_task_data[cpu_id].cpu_thread_rt_task_data[cpu_thread_id].num_of_positive_jitter_exceeding_threshold = 0;	
		rt_tasks_data->cpu_rt_task_data[cpu_id].cpu_thread_rt_task_data[cpu_thread_id].num_of_negative_jitter_exceeding_threshold = 0;	
	}	
	rt_tasks_data->cpu_rt_task_data[cpu_id].num_of_rt_tasks_at_cpu--;
	if (rt_tasks_data->cpu_rt_task_data[cpu_id].num_of_rt_tasks_at_cpu == 0)		// no task remained at cpu
		rt_tasks_data->cpu_rt_task_data[cpu_id].rt_task_period = 0;		
	rt_tasks_data->num_of_total_rt_tasks--;	// cannot be zero since KernelSpike is always up.
	return TRUE;
}

void evaluate_and_save_jitter(RtTasksData *rt_tasks_data, unsigned int cpu_id, unsigned int cpu_thread_id, unsigned int previous_period_end_time, unsigned int current_period_end_time)
{
	unsigned int period, jitter, submitted_period;
	CpuThreadRtTasksData *cpu_thread_data = &(rt_tasks_data->cpu_rt_task_data[cpu_id].cpu_thread_rt_task_data[cpu_thread_id]) ; 

	period = current_period_end_time - previous_period_end_time;	
	submitted_period = rt_tasks_data->cpu_rt_task_data[cpu_id].rt_task_period;
	if (period > submitted_period)
	{
		jitter = period - submitted_period;
		if (jitter > cpu_thread_data->max_positive_jitter)
			cpu_thread_data->max_positive_jitter = jitter;
		if (jitter > cpu_thread_data->positive_jitter_threshold)
			cpu_thread_data->num_of_positive_jitter_exceeding_threshold++;
	}
	else
	{
		jitter = submitted_period - period;
		if (jitter > cpu_thread_data->max_negative_jitter)
			cpu_thread_data->max_negative_jitter = jitter;
		if (jitter > cpu_thread_data->negative_jitter_threshold)
			cpu_thread_data->num_of_negative_jitter_exceeding_threshold++;
	}
	return;
}

void evaluate_and_save_period_run_time(RtTasksData *rt_tasks_data, unsigned int cpu_id, unsigned int cpu_thread_id, unsigned int process_start_time, unsigned int process_end_time)
{
	unsigned int period_run_time;
	CpuThreadRtTasksData *cpu_thread_data = &(rt_tasks_data->cpu_rt_task_data[cpu_id].cpu_thread_rt_task_data[cpu_thread_id]); 

	period_run_time = process_end_time - process_start_time;

	if (period_run_time > cpu_thread_data->max_period_run_time)
		cpu_thread_data->max_period_run_time = period_run_time;
	return;
}
