#include "RtTask.h"


static TimeStamp current_daq_time_ns = 0;
static TimeStamp previous_daq_time_ns = 0;

static int rt_daq_threads[MAX_NUM_OF_DAQ_CARD];
static int rt_periodic_task_thread;

static bool rt_periodic_task_stay_alive = 1;

static void *rt_daq_handler(void *args);
static void *rt_periodic_handler(void *args);



bool start_periodic_task(void)
{
	rt_tasks_data = rtai_malloc(SHM_NUM_RT_TASKS_DATA, sizeof(RtTasksData));
	if (rt_tasks_data == NULL) 
		return print_message(ERROR_MSG ,"PCIe6259", "RtTask", "start_periodic_task", "rt_tasks_data == NULL.");

   	spike_time_stamp = rtai_malloc(SHM_NUM_KERNEL_SPIKE_SPIKE_TIME_STAMP, sizeof(SpikeTimeStamp));
	if (spike_time_stamp == NULL) 
		return print_message(ERROR_MSG ,"PCIe6259", "RtTask", "main", "spike_time_stamp == NULL.");


	rt_periodic_task_thread = rt_thread_create(rt_periodic_handler, NULL, 10000);

	return TRUE;
}


static void *rt_periodic_handler(void *args)
{
	RT_TASK *handler;
        RTIME period;
	unsigned int prev_time, curr_time;
	
   	rt_tasks_data = rtai_malloc(SHM_NUM_RT_TASKS_DATA, sizeof(RtTasksData));
	if (rt_tasks_data == NULL)  {
		print_message(ERROR_MSG ,"PCIe6259", "RtTask", "rt_periodic_handler", "rt_tasks_data == NULL."); exit(1); }
	if (! check_rt_task_specs_to_init(rt_tasks_data, BLUESPIKE_PERIODIC_CPU_ID, BLUESPIKE_PERIODIC_CPU_THREAD_ID, BLUESPIKE_PERIODIC_CPU_THREAD_TASK_ID, BLUESPIKE_PERIODIC_PERIOD, TRUE))  {
		print_message(ERROR_MSG ,"PCIe6259", "RtTask", "rt_periodic_handler", "! check_rt_task_specs_to_init()."); exit(1); }	
        if (! (handler = rt_task_init_schmod(BLUESPIKE_PERIODIC_TASK_NAME, BLUESPIKE_PERIODIC_TASK_PRIORITY, BLUESPIKE_PERIODIC_STACK_SIZE, BLUESPIKE_PERIODIC_MSG_SIZE,BLUESPIKE_PERIODIC_POLICY, 1 << ((BLUESPIKE_PERIODIC_CPU_ID*MAX_NUM_OF_CPU_THREADS_PER_CPU)+BLUESPIKE_PERIODIC_CPU_THREAD_ID)))) {
		print_message(ERROR_MSG ,"PCIe6259", "RtTask", "rt_periodic_handler", "handler = rt_task_init_schmod()."); exit(1); }
	if (! write_rt_task_specs_to_rt_tasks_data(rt_tasks_data, BLUESPIKE_PERIODIC_CPU_ID, BLUESPIKE_PERIODIC_CPU_THREAD_ID, BLUESPIKE_PERIODIC_CPU_THREAD_TASK_ID, BLUESPIKE_PERIODIC_PERIOD, BLUESPIKE_PERIODIC_POSITIVE_JITTER_THRES, BLUESPIKE_PERIODIC_NEGATIVE_JITTER_THRES, "BlueSpike", TRUE) ) {
		print_message(ERROR_MSG ,"PCIe6259", "RtTask", "rt_periodic_handler", "! write_rt_task_specs_to_rt_tasks_data()."); exit(1); }	

	start_rt_timer(nano2count(START_RT_TIMER_PERIOD));
        period = nano2count(BLUESPIKE_PERIODIC_PERIOD);
        rt_task_make_periodic(handler, rt_get_time() + period, period);

	prev_time = rt_get_cpu_time_ns();	

        mlockall(MCL_CURRENT | MCL_FUTURE);
	rt_make_hard_real_time();		// do not forget this // check the task by nano /proc/rtai/scheduler (HD/SF) 

        while (rt_periodic_task_stay_alive) 
	{
        	rt_task_wait_period();
		curr_time = rt_get_cpu_time_ns();
		evaluate_and_save_jitter(rt_tasks_data, BLUESPIKE_PERIODIC_CPU_ID, BLUESPIKE_PERIODIC_CPU_THREAD_ID, BLUESPIKE_PERIODIC_CPU_THREAD_TASK_ID, prev_time, curr_time);
		prev_time = curr_time;
		// routines

		evaluate_and_save_period_run_time(rt_tasks_data, BLUESPIKE_PERIODIC_CPU_ID, BLUESPIKE_PERIODIC_CPU_THREAD_ID, BLUESPIKE_PERIODIC_CPU_THREAD_TASK_ID, curr_time, rt_get_cpu_time_ns());
	}

	rt_make_soft_real_time();
        rt_task_delete(handler);
	print_message(INFO_MSG ,"PCIe6259", "RtTask", "rt_periodic_handler", "rt_task_delete().");	

        return 0; 
}


bool start_acquisition(void)
{
	unsigned int i;
	for (i = 0; i < MAX_NUM_OF_DAQ_CARD; i++)
	{
		ni6259_comedi_dev_ids[i] = i;
		rt_daq_threads[i] = rt_thread_create(rt_daq_handler, &(ni6259_comedi_dev_ids[i]), 10000);	// support to handle daq cards on multiple threads.  To handle on multiple CPUs assign daq handling tasks to specific CPUs via rt_task_init_schmod()
	}

	return TRUE;
}


static void *rt_daq_handler(void *args)
{
	RT_TASK *handler;
	unsigned int prev_time, curr_time;
	unsigned int daq_num;
	unsigned int period_occured;	
	long int cb_val = 0, cb_retval = 0;
	lsampl_t *daq_data;

        if (! (handler = rt_task_init_schmod(BLUESPIKE_DAQ_TASK_NAME, BLUESPIKE_DAQ_TASK_PRIORITY, BLUESPIKE_DAQ_STACK_SIZE, BLUESPIKE_DAQ_MSG_SIZE,BLUESPIKE_DAQ_POLICY, 1 << ((BLUESPIKE_DAQ_CPU_ID*MAX_NUM_OF_CPU_THREADS_PER_CPU)+BLUESPIKE_DAQ_CPU_THREAD_ID)))) {
		print_message(ERROR_MSG ,"PCIe6259", "RtTask", "rt_daq_handler", "handler = rt_task_init_schmod()."); exit(1); }

	daq_num = *((unsigned int*)args);

        mlockall(MCL_CURRENT | MCL_FUTURE);
	rt_make_hard_real_time();		// do not forget this // check the task by nano /proc/rtai/scheduler (HD/SF) 

	if (! open_daq_card(daq_num))  {
		print_message(ERROR_MSG ,"PCIe6259", "RtTask", "rt_daq_handler", "! open_daq_cards()."); exit(1); }		

	rt_comedi_register_callback(ni6259_comedi_dev[daq_num], COMEDI_SUBDEVICE_AI, COMEDI_CB_EOS, NULL, handler);

	if (! config_daq_card(daq_num))  {
		print_message(ERROR_MSG ,"PCIe6259", "RtTask", "rt_daq_handler", "! config_daq_cards()."); exit(1); }		

	daq_data = ni6259_data[daq_num];

	prev_time = rt_get_cpu_time_ns();
	
        while (daq_cards_on) 
	{
		cb_val = 0;
		cb_retval += rt_comedi_wait(&cb_val);

		curr_time = rt_get_cpu_time_ns();
		period_occured = curr_time - prev_time;
		current_daq_time_ns += period_occured;
		evaluate_and_save_jitter(rt_tasks_data, BLUESPIKE_DAQ_CPU_ID, BLUESPIKE_DAQ_CPU_THREAD_ID, BLUESPIKE_DAQ_CPU_THREAD_TASK_ID+daq_num, prev_time, curr_time);
		previous_daq_time_ns = current_daq_time_ns;	   ///   carefully handle 	previous_daq_time_ns. spike_sorting fnction handles peak_time according to previous_daq_time_ns.
		prev_time = curr_time;


		// routines
		if (!(cb_val & COMEDI_CB_EOS))
		{
			print_message(ERROR_MSG ,"PCIe6259", "RtTask", "rt_daq_handler", "! (cb_val & COMEDI_CB_EOS)."); 
			break;
		}

		rt_comedi_command_data_read(ni6259_comedi_dev[daq_num], COMEDI_SUBDEVICE_AI, MAX_NUM_OF_CHANNEL_PER_DAQ_CARD*NUM_OF_SCAN, daq_data);

		pthread_mutex_lock(&(daq_mwa_map[daq_num].mutex));   // do not allow mapping change by configdaqgui during processing retrieved data. 	
	
		handle_recording_data(daq_num, daq_data);
		spike_sorting(daq_num, previous_daq_time_ns);

		pthread_mutex_unlock(&(daq_mwa_map[daq_num].mutex)); 
		// routines	
		evaluate_and_save_period_run_time(rt_tasks_data, BLUESPIKE_DAQ_CPU_ID, BLUESPIKE_DAQ_CPU_THREAD_ID, BLUESPIKE_DAQ_CPU_THREAD_TASK_ID+daq_num, curr_time, rt_get_cpu_time_ns());		
        }
	close_daq_cards(daq_num);
	rt_make_soft_real_time();
        rt_task_delete(handler);
	print_message(INFO_MSG ,"PCIe6259", "RtTask", "rt_daq_handler", "rt_task_delete().");	

        return 0; 
}



