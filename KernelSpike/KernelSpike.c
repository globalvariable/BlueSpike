/***************************************************************************
                          KernelSpike.c  -  description
                             -------------------
    copyright            : (C) 2011 by Mehmet Kocaturk
    email                : mehmet.kocaturk@boun.edu.tr
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "KernelSpike.h"

// to get rid of warning: the frame size of 2912 bytes is larger than 1024 bytes for run_template_matching func. Stack size for function is limited to 1024, define variables here.
static double template_matching_g_x[MAX_NUM_OF_UNIT_PER_CHAN];
static double template_matching_diff[MAX_NUM_OF_UNIT_PER_CHAN][NUM_OF_SAMP_PER_SPIKE];
static double template_matching_diff_temporary[MAX_NUM_OF_UNIT_PER_CHAN][NUM_OF_SAMP_PER_SPIKE];
static double template_matching_exponent[MAX_NUM_OF_UNIT_PER_CHAN];	
static double template_matching_probabl[MAX_NUM_OF_UNIT_PER_CHAN];	

static int spike_time_stamp_buff_size = SPIKE_TIME_STAMP_BUFF_SIZE;
static int blue_spike_time_stamp_buff_size = BLUE_SPIKE_TIME_STAMP_BUFF_SIZE;
static int spike_end_handling_buff_size = SPIKE_END_HANDLING_DATA_BUFF_SIZE;

static BlueSpikeData *blue_spike_data = NULL; 
static RtTasksData *rt_tasks_data = NULL; 

void rt_handler(long int t)
{
	int i, j, k, m, return_value;
	int front[MAX_NUM_OF_DAQ_CARD], back[MAX_NUM_OF_DAQ_CARD], num_byte[MAX_NUM_OF_DAQ_CARD], daq_chan_num[MAX_NUM_OF_DAQ_CARD] ;
	DaqMwaMap			*daq_mwa_map;
	RecordingData			*recording_data;
	RecordingData			*filtered_recording_data;
	KernelTaskCtrl			*kernel_task_ctrl;

	int *recording_data_write_idx;
	int mwa, mwa_chan;
	bool *highpass_150Hz_on, *highpass_400Hz_on, *lowpass_8KHz_on, *kernel_task_idle, *kill_all_rt_tasks, *daq_card_mapped; 
	TimeStamp *kern_curr_time, *kern_prev_time;
	unsigned int prev_time; // local_time  unsigned int
	unsigned int curr_time;		// local_time  unsigned int
	unsigned int period_occured;		
	
	int rt_task_kill_timer_cntr;

	prev_time= rt_get_cpu_time_ns();	
	
	rt_task_kill_timer_cntr = 0;
	 
	current_time_ns = 0;		// global time  long long unsigned int  // TimeStamp	
	previous_time_ns = 0;

	daq_cards_on = 0;

	daq_mwa_map = &blue_spike_data->daq_mwa_map;
	recording_data = &blue_spike_data->recording_data;
	filtered_recording_data = &blue_spike_data->filtered_recording_data;

	highpass_150Hz_on = &(blue_spike_data->blue_spike_ctrl.highpass_150Hz_on); 
	highpass_400Hz_on = &(blue_spike_data->blue_spike_ctrl.highpass_400Hz_on);
 	lowpass_8KHz_on = &(blue_spike_data->blue_spike_ctrl.lowpass_8KHz_on);
	daq_card_mapped = &(blue_spike_data->blue_spike_ctrl.daq_card_mapped);

	kernel_task_ctrl = &rt_tasks_data->kernel_task_ctrl;
	kernel_task_idle = &kernel_task_ctrl->kernel_task_idle;
	kill_all_rt_tasks = &kernel_task_ctrl->kill_all_rt_tasks;


	kern_curr_time = &rt_tasks_data->current_system_time;
	kern_prev_time = &rt_tasks_data->previous_system_time;
	
	for (i=0; i < MAX_NUM_OF_DAQ_CARD; i++)
	{
		front[i] = 0;
		back[i] = 0;
		daq_chan_num[i] = 0;
	}	

	while (!(*kill_all_rt_tasks)) 
	{
		rt_task_wait_period();
		
		*kernel_task_idle = 0;	
		curr_time = rt_get_cpu_time_ns();
		period_occured = curr_time - prev_time;
		current_time_ns += period_occured;
		evaluate_jitter(period_occured);
		previous_time_ns = current_time_ns;		
		prev_time = curr_time;

		if ((handle_daq_cards()) || (!(*daq_card_mapped)))
		{
			*kern_curr_time = current_time_ns;			// Recorder reaches current time after KernelSpike completes processing of all buffers. 
			*kern_prev_time = previous_time_ns;		
			*kernel_task_idle = 1;
			for (i=0; i < MAX_NUM_OF_DAQ_CARD; i++)
			{
				front[i] = 0;
				back[i] = 0;
				daq_chan_num[i] = 0;
			}
			*highpass_150Hz_on = 0; *highpass_400Hz_on = 0; *lowpass_8KHz_on = 0;	
			evaluate_period_run_time(curr_time);
			continue;
		}	
			
		for (i=0; i < MAX_NUM_OF_DAQ_CARD; i++)
		{	
			comedi_poll(ni6070_comedi_dev[i], COMEDI_SUBDEVICE_AI);
			num_byte[i] = comedi_get_buffer_contents(ni6070_comedi_dev[i], COMEDI_SUBDEVICE_AI);
			front[i] = front[i] + num_byte[i];
			if (front[i] >= comedi_buff_size[i])
				front[i] = front[i] - comedi_buff_size[i];
			if(num_byte[i] == 0)
			{
				printk("KernelSpike: CRITICAL ERROR: Must be exceeding rt_task_wait_period, num_byte[%d] = 0\n", i);
				printk("KernelSpike: CRITICAL ERROR: Task lsted too long > TICK_PERIOD.\n");
				continue;
			}
				
			for(j = 0; j < num_byte[i]; j += sizeof(sampl_t))
			{
				mwa = (*daq_mwa_map)[i][daq_chan_num[i]].mwa;
				mwa_chan = (*daq_mwa_map)[i][daq_chan_num[i]].channel;	
				if ((mwa != MAX_NUM_OF_MWA) && (mwa_chan != MAX_NUM_OF_CHAN_PER_MWA))	// No map for this channel
				{
					recording_data_write_idx = &(recording_data->buff_idx_write[mwa][mwa_chan]);

					if ((comedi_map_ptr[i]+back[i]+j) >= (comedi_map_ptr[i]+comedi_buff_size[i]))
					{
						recording_data->recording_data_buff[mwa][mwa_chan][*recording_data_write_idx] = (*(sampl_t *)(comedi_map_ptr[i] + back[i] + j - comedi_buff_size[i]) - BASELINE_QUANT_6070E) / VOLTAGE_MULTIPLIER_MV_6070E;
					}				
					else
					{
						recording_data->recording_data_buff[mwa][mwa_chan][*recording_data_write_idx] = (*(sampl_t *)(comedi_map_ptr[i] + back[i] + j) - BASELINE_QUANT_6070E) / VOLTAGE_MULTIPLIER_MV_6070E;
					}
					
					if (((*recording_data_write_idx) +1) == RECORDING_DATA_BUFF_SIZE) 
						(*recording_data_write_idx) = 0;
					else
						(*recording_data_write_idx)++;
				}
				else if (((mwa == MAX_NUM_OF_MWA) && (mwa_chan != MAX_NUM_OF_CHAN_PER_MWA)) || ((mwa != MAX_NUM_OF_MWA) && (mwa_chan == MAX_NUM_OF_CHAN_PER_MWA)))
				{
					printk("KernelSpike: BUG: mwa or mwa_chan is problematic\n");
					printk("KernelSpike: BUG: recording data might be corrupted\n");
					*kill_all_rt_tasks = 1;
					break;
				}
				
				(daq_chan_num[i])++;
				if (daq_chan_num[i] == MAX_NUM_OF_CHANNEL_PER_DAQ_CARD)
					daq_chan_num[i] = 0;
			}
				
			return_value = comedi_mark_buffer_read(ni6070_comedi_dev[i], COMEDI_SUBDEVICE_AI, num_byte[i]);
			if(return_value < 0)
			{
				printk("KernelSpike: ERROR: comedi_mark_buffer_read");
				*kill_all_rt_tasks = 1;
			}
			back[i] = front[i];
		}
			
		if ((*highpass_150Hz_on) || (*highpass_400Hz_on))
		{
			spike_end_handling_buff_control_cntr = 0;
			blue_spike_time_stamp_buff_control_cntr = 0;	
			for (k=0; k<MAX_NUM_OF_MWA; k++)
			{
				for (m=0; m<MAX_NUM_OF_CHAN_PER_MWA; m++)
				{
					filter_recording_data(recording_data, filtered_recording_data, k, m, *highpass_150Hz_on, *highpass_400Hz_on, *lowpass_8KHz_on);
					find_spike_end(filtered_recording_data, k, m);
				}
			}
			handle_spike_end_handling_buffer();
		}
		else
		{
			//   shouldn' t lose recording data write index when filtering is turned off.  
			for (k=0; k<MAX_NUM_OF_MWA; k++)
			{
				for (m=0; m<MAX_NUM_OF_CHAN_PER_MWA; m++)
				{
					filtered_recording_data->buff_idx_write[k][m] = recording_data->buff_idx_write[k][m];
				}
			} 	
			spike_end_handling.buff_start_idx = spike_end_handling.buff_write_idx;   // clear spike_end_handling_buffer
		} 
		
		*kern_curr_time = current_time_ns;			// Recorder reaches current time after KernelSpike completes processing of all buffers. 
		*kern_prev_time = previous_time_ns;
			
		print_warning_and_errors();

		evaluate_period_run_time(curr_time);

		*kernel_task_idle = 1;				
	}
	
	if (daq_cards_on)
		close_daq_cards();
		
	while (rt_task_kill_timer_cntr < 1000)   // wait 1000 msec before stopping rt_timer. wait for other rt_tasks to close. 
	{	
		rt_task_wait_period();
		rt_task_kill_timer_cntr++;
	}	
	stop_rt_timer();
	rt_task_delete(&rt_task0);	
    	rtai_kfree(nam2num(BLUE_SPIKE_DATA_SHM_NAME));	
    	rtai_kfree(nam2num(RT_TASKS_DATA_SHM_NAME));	
}



int __init xinit_module(void)
{
	int i, j;
	RTIME tick_period;
	
	printk("KernelSpike: insmod KernelSpike\n");

	rt_tasks_data = (RtTasksData*)rtai_kmalloc(nam2num(RT_TASKS_DATA_SHM_NAME), sizeof(RtTasksData));
	if (rt_tasks_data == NULL)
		return -ENOMEM;
	memset(rt_tasks_data, 0, sizeof(RtTasksData));
        printk("KernelSpike: RtTasksData Memory allocated.\n");
        printk("KernelSpike: sizeof(RtTasksData) : %lu.\n", sizeof(RtTasksData));

	blue_spike_data = (BlueSpikeData*)rtai_kmalloc(nam2num(BLUE_SPIKE_DATA_SHM_NAME), sizeof(BlueSpikeData));
	if (blue_spike_data == NULL)
		return -ENOMEM;
	memset(blue_spike_data, 0, sizeof(BlueSpikeData));
        printk("KernelSpike: BlueSpike Memory allocated.\n");
        printk("KernelSpike: sizeof(BlueSpikeData) : %lu.\n", sizeof(BlueSpikeData));
        
	for (i=0; i < MAX_NUM_OF_DAQ_CARD; i++)
	{
		for (j=0; j<MAX_NUM_OF_CHANNEL_PER_DAQ_CARD; j++)
		{
			blue_spike_data->daq_mwa_map[i][j].mwa = MAX_NUM_OF_MWA;
			blue_spike_data->daq_mwa_map[i][j].channel = MAX_NUM_OF_CHAN_PER_MWA;
		}
	}
	
	for (i=0; i < MAX_NUM_OF_MWA; i++)
	{
		for (j=0; j<MAX_NUM_OF_CHAN_PER_MWA; j++)
		{
			blue_spike_data->mwa_daq_map[i][j].daq_card = MAX_NUM_OF_DAQ_CARD;
			blue_spike_data->mwa_daq_map[i][j].daq_chan = MAX_NUM_OF_CHANNEL_PER_DAQ_CARD;
		}
	}

	rt_tasks_data->kernel_task_ctrl.kernel_task_idle = 1;

	rt_set_periodic_mode();
	rt_task_init_cpuid(&rt_task0, rt_handler, KERNELSPIKE_PASS_DATA, KERNELSPIKE_STACK_SIZE, KERNELSPIKE_TASK_PRIORITY, KERNELSPIKE_USES_FLOATING_POINT, KERNELSPIKE_SIGNAL, (KERNELSPIKE_CPU_ID*MAX_NUM_OF_THREADS_PER_CPU)+KERNELSPIKE_CPU_THREAD_ID);

	start_rt_timer(nano2count(START_RT_TIMER_PERIOD));
	tick_period = nano2count(KERNELSPIKE_PERIOD);
	rt_task_make_periodic(&rt_task0, rt_get_time() + tick_period, tick_period);

	rt_tasks_data->cpu_rt_task_data[KERNELSPIKE_CPU_ID].rt_task_period = KERNELSPIKE_PERIOD;
	rt_tasks_data->num_of_total_rt_tasks++;
	rt_tasks_data->cpu_rt_task_data[KERNELSPIKE_CPU_ID].num_of_rt_tasks_at_cpu++;
	rt_tasks_data->cpu_rt_task_data[KERNELSPIKE_CPU_ID].cpu_thread_rt_task_data[KERNELSPIKE_CPU_THREAD_ID].num_of_rt_tasks_at_cpu_thread++;
	rt_tasks_data->cpu_rt_task_data[KERNELSPIKE_CPU_ID].cpu_thread_rt_task_data[KERNELSPIKE_CPU_THREAD_ID].positive_jitter_threshold = KERNELSPIKE_POSITIVE_JITTER_THRES;
	rt_tasks_data->cpu_rt_task_data[KERNELSPIKE_CPU_ID].cpu_thread_rt_task_data[KERNELSPIKE_CPU_THREAD_ID].negative_jitter_threshold = KERNELSPIKE_NEGATIVE_JITTER_THRES;

	printk("KernelSpike: rt task created with %d nanoseconds period.\n", KERNELSPIKE_PERIOD);
	return 0;
}

void __exit xcleanup_module(void)
{
	int i;

	if (rt_tasks_data->num_of_total_rt_tasks != 1)
		printk("KernelSpike: BUG: There are other rt tasks !!!\n");			// Allow rmmod to run, otherwise it will lock

	rt_task_delete(&rt_task0);	
	stop_rt_timer();

	if (daq_cards_on)
	{
		for (i = 0; i<MAX_NUM_OF_DAQ_CARD; i++)
		{
			comedi_cancel(ni6070_comedi_dev[i], COMEDI_SUBDEVICE_AI);
			comedi_close(ni6070_comedi_dev[i]);
		}
	}
    	rtai_kfree(nam2num(BLUE_SPIKE_DATA_SHM_NAME));	
    	rtai_kfree(nam2num(RT_TASKS_DATA_SHM_NAME));	
    	printk("KernelSpike: rmmod KernelSpike\n");
	return;
}

module_init(xinit_module);
module_exit(xcleanup_module);
MODULE_LICENSE("GPL");

int ni6070_comedi_configure(int card_number)
{
	int i;
	memset(&ni6070_comedi_cmd[card_number], 0, sizeof(comedi_cmd));
	memset(&ni6070_comedi_chanlist[card_number], 0, sizeof(unsigned)*MAX_NUM_OF_CHANNEL_PER_DAQ_CARD);

	ni6070_comedi_cmd[card_number].subdev = COMEDI_SUBDEVICE_AI;
	ni6070_comedi_cmd[card_number].flags = 0;

	ni6070_comedi_cmd[card_number].start_src = TRIG_NOW;
	ni6070_comedi_cmd[card_number].start_arg = 0;

	ni6070_comedi_cmd[card_number].scan_begin_src = TRIG_TIMER;
	ni6070_comedi_cmd[card_number].scan_begin_arg = SAMPLING_INTERVAL;
	
	ni6070_comedi_cmd[card_number].convert_src = TRIG_TIMER;
	ni6070_comedi_cmd[card_number].convert_arg = 1000;

	ni6070_comedi_cmd[card_number].scan_end_src = TRIG_COUNT;
	ni6070_comedi_cmd[card_number].scan_end_arg = MAX_NUM_OF_CHANNEL_PER_DAQ_CARD;

	ni6070_comedi_cmd[card_number].stop_src = TRIG_NONE;
	ni6070_comedi_cmd[card_number].stop_arg = 0;

	ni6070_comedi_cmd[card_number].chanlist = ni6070_comedi_chanlist[card_number];
	ni6070_comedi_cmd[card_number].chanlist_len = MAX_NUM_OF_CHANNEL_PER_DAQ_CARD;

	for (i = 0 ; i < MAX_NUM_OF_CHANNEL_PER_DAQ_CARD ; i++)
	{
		ni6070_comedi_chanlist[card_number][i] = CR_PACK(i, VOLTAGE_RANGE_6070E, AREF_GROUND);
	}

	ni6070_comedi_cmd[card_number].data = NULL;
	ni6070_comedi_cmd[card_number].data_len = 0;

	print_cmd(card_number);
	printk("KernelSpike: test 1: %i\n", comedi_command_test(ni6070_comedi_dev[card_number], &ni6070_comedi_cmd[card_number]));
	print_cmd(card_number);
	printk("KernelSpike: test 2: %i\n", comedi_command_test(ni6070_comedi_dev[card_number],&ni6070_comedi_cmd[card_number]));
	print_cmd(card_number);

	return comedi_command(ni6070_comedi_dev[card_number], &ni6070_comedi_cmd[card_number]);
}

void print_cmd(int card_number)
{
	printk("KernelSpike: comedi_cmd[%d].subdev = %i\n", card_number, ni6070_comedi_cmd[card_number].subdev);
	printk("KernelSpike: comedi_cmd[%d].flags = %i\n", card_number, ni6070_comedi_cmd[card_number].flags);

	printk("KernelSpike: comedi_cmd[%d].start_src = %i\n", card_number, ni6070_comedi_cmd[card_number].start_src);
	printk("KernelSpike: comedi_cmd[%d].start_arg = %i\n", card_number, ni6070_comedi_cmd[card_number].start_arg);

	printk("KernelSpike: comedi_cmd[%d].scan_begin_src = %i\n", card_number, ni6070_comedi_cmd[card_number].scan_begin_src);
	printk("KernelSpike: comedi_cmd[%d].scan_begin_arg = %i\n", card_number, ni6070_comedi_cmd[card_number].scan_begin_arg);
	
	printk("KernelSpike: comedi_cmd[%d].convert_src = %i\n", card_number, ni6070_comedi_cmd[card_number].convert_src);
	printk("KernelSpike: comedi_cmd[%d].convert_arg = %i\n", card_number, ni6070_comedi_cmd[card_number].convert_arg);

	printk("KernelSpike: comedi_cmd[%d].scan_end_src = %i\n", card_number, ni6070_comedi_cmd[card_number].scan_end_src);
	printk("KernelSpike: comedi_cmd[%d].scan_end_arg = %i\n", card_number, ni6070_comedi_cmd[card_number].scan_end_arg);

	printk("KernelSpike: comedi_cmd[%d].stop_src = %i\n", card_number, ni6070_comedi_cmd[card_number].stop_src);
	printk("KernelSpike: comedi_cmd[%d].stop_arg = %i\n", card_number, ni6070_comedi_cmd[card_number].stop_arg);

	printk("KernelSpike: comedi_cmd[%d].chanlist_len = %i\n", card_number, ni6070_comedi_cmd[card_number].chanlist_len);
}


void filter_recording_data( RecordingData *recording_data, RecordingData *filtered_recording_data, int mwa, int mwa_chan, bool highpass_150Hz_on, bool highpass_400Hz_on, bool lowpass_8KHz_on)
{
	int idx, start_idx, end_idx;
	
	RecordingDataChanBuff	*recording_data_chan_buff;
	RecordingDataChanBuff	*filtered_recording_data_chan_buff;
	RecordingDataChanBuff	*highpass_filtered_recording_data_chan_buff;
	
	recording_data_chan_buff = &(recording_data->recording_data_buff[mwa][mwa_chan]);
	filtered_recording_data_chan_buff = &(filtered_recording_data->recording_data_buff[mwa][mwa_chan]);
	highpass_filtered_recording_data_chan_buff = &(highpass_filtered_recording_data.recording_data_buff[mwa][mwa_chan]);	
	
	start_idx = filtered_recording_data->buff_idx_write[mwa][mwa_chan];	
	end_idx = recording_data->buff_idx_write[mwa][mwa_chan];													

	idx = start_idx;		
												
	if ((lowpass_8KHz_on) && (highpass_400Hz_on))	 
	{
		while (idx != end_idx)
		{
			if (idx == 0)
			{
				(*highpass_filtered_recording_data_chan_buff)[idx]=	(0.921170993499942 * (*recording_data_chan_buff)[0]) +
														(-3.684683973999769 * (*recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_1]) +
														(5.527025960999653 * (*recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_2]) + 
														(-3.684683973999769 * (*recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_3]) +
 														(0.921170993499942 * (*recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_4])  -
														(-3.835825540647349 * (*highpass_filtered_recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_1]) -
														(5.520819136622230 * (*highpass_filtered_recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_2]) -
														(-3.533535219463017 * (*highpass_filtered_recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_3]) -
														(0.848555999266478 * (*highpass_filtered_recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_4]); 
														
				(*filtered_recording_data_chan_buff)[idx]=  			(0.046582906636444 * (*highpass_filtered_recording_data_chan_buff)[0])+
														(0.186331626545775 * (*highpass_filtered_recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_1]) +
														(0.279497439818662 * (*highpass_filtered_recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_2]) + 
														(0.186331626545775 * (*highpass_filtered_recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_3]) +
 														(0.046582906636444 * (*highpass_filtered_recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_4])  -
														(-0.782095198023338 * (*filtered_recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_1]) -
														(0.679978526916300 * (*filtered_recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_2]) -
														(-0.182675697753033 * (*filtered_recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_3]) -
														(0.030118875043169 * (*filtered_recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_4]); 														
														
			}
			else if (idx == 1)
			{
				(*highpass_filtered_recording_data_chan_buff)[idx]=	(0.921170993499942 * (*recording_data_chan_buff)[1]) +
														(-3.684683973999769 * (*recording_data_chan_buff)[0]) +
														(5.527025960999653 * (*recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_1]) + 
														(-3.684683973999769 * (*recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_2]) +
 														(0.921170993499942 * (*recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_3])  -
														(-3.835825540647349 * (*highpass_filtered_recording_data_chan_buff)[0]) -
														(5.520819136622230 * (*highpass_filtered_recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_1]) -
														(-3.533535219463017 * (*highpass_filtered_recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_2]) -
														(0.848555999266478 * (*highpass_filtered_recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_3]); 
																												
				(*filtered_recording_data_chan_buff)[idx]=  			(0.046582906636444 * (*highpass_filtered_recording_data_chan_buff)[1])+
														(0.186331626545775 * (*highpass_filtered_recording_data_chan_buff)[0]) +
														(0.279497439818662 * (*highpass_filtered_recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_1]) + 
														(0.186331626545775 * (*highpass_filtered_recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_2]) +
 														(0.046582906636444 * (*highpass_filtered_recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_3])  -
														(-0.782095198023338 * (*filtered_recording_data_chan_buff)[0]) -
														(0.679978526916300 * (*filtered_recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_1]) -
														(-0.182675697753033 * (*filtered_recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_2]) -
														(0.030118875043169 * (*filtered_recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_3]); 	
			}
			else if (idx == 2)
			{
				(*highpass_filtered_recording_data_chan_buff)[idx]=	(0.921170993499942 * (*recording_data_chan_buff)[2]) +
														(-3.684683973999769 * (*recording_data_chan_buff)[1]) +
														(5.527025960999653 * (*recording_data_chan_buff)[0]) + 
														(-3.684683973999769 * (*recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_1]) +
 														(0.921170993499942 * (*recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_2])  -
														(-3.835825540647349 * (*highpass_filtered_recording_data_chan_buff)[1]) -
														(5.520819136622230 * (*highpass_filtered_recording_data_chan_buff)[0]) -
														(-3.533535219463017 * (*highpass_filtered_recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_1]) -
														(0.848555999266478 * (*highpass_filtered_recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_2]); 
																																										
				(*filtered_recording_data_chan_buff)[idx]=  			(0.046582906636444 * (*highpass_filtered_recording_data_chan_buff)[2])+
														(0.186331626545775 * (*highpass_filtered_recording_data_chan_buff)[1]) +
														(0.279497439818662 * (*highpass_filtered_recording_data_chan_buff)[0]) + 
														(0.186331626545775 * (*highpass_filtered_recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_1]) +
 														(0.046582906636444 * (*highpass_filtered_recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_2])  -
														(-0.782095198023338 * (*filtered_recording_data_chan_buff)[1]) -
														(0.679978526916300 * (*filtered_recording_data_chan_buff)[0]) -
														(-0.182675697753033 * (*filtered_recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_1]) -
														(0.030118875043169 * (*filtered_recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_2]); 	
			}			
			else if (idx == 3)
			{
				(*highpass_filtered_recording_data_chan_buff)[idx]=	(0.921170993499942 * (*recording_data_chan_buff)[3]) +
														(-3.684683973999769 * (*recording_data_chan_buff)[2]) +
														(5.527025960999653 * (*recording_data_chan_buff)[1]) + 
														(-3.684683973999769 * (*recording_data_chan_buff)[0]) +
 														(0.921170993499942 * (*recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_1])  -
														(-3.835825540647349 * (*highpass_filtered_recording_data_chan_buff)[2]) -
														(5.520819136622230 * (*highpass_filtered_recording_data_chan_buff)[1]) -
														(-3.533535219463017 * (*highpass_filtered_recording_data_chan_buff)[0]) -
														(0.848555999266478 * (*highpass_filtered_recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_1]); 
																																																								
				(*filtered_recording_data_chan_buff)[idx]=  			(0.046582906636444 * (*highpass_filtered_recording_data_chan_buff)[3])+
														(0.186331626545775 * (*highpass_filtered_recording_data_chan_buff)[2]) +
														(0.279497439818662 * (*highpass_filtered_recording_data_chan_buff)[1]) + 
														(0.186331626545775 * (*highpass_filtered_recording_data_chan_buff)[0]) +
 														(0.046582906636444 * (*highpass_filtered_recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_1])  -
														(-0.782095198023338 * (*filtered_recording_data_chan_buff)[2]) -
														(0.679978526916300 * (*filtered_recording_data_chan_buff)[1]) -
														(-0.182675697753033 * (*filtered_recording_data_chan_buff)[0]) -
														(0.030118875043169 * (*filtered_recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_1]); 	
			}
			else
			{
				(*highpass_filtered_recording_data_chan_buff)[idx]=	(0.921170993499942 * (*recording_data_chan_buff)[idx]) +
														(-3.684683973999769 * (*recording_data_chan_buff)[idx-1]) +
														(5.527025960999653 * (*recording_data_chan_buff)[idx-2]) + 
														(-3.684683973999769 * (*recording_data_chan_buff)[idx-3]) +
 														(0.921170993499942 * (*recording_data_chan_buff)[idx-4])  -
														(-3.835825540647349 * (*highpass_filtered_recording_data_chan_buff)[idx-1]) -
														(5.520819136622230 * (*highpass_filtered_recording_data_chan_buff)[idx-2]) -
														(-3.533535219463017 * (*highpass_filtered_recording_data_chan_buff)[idx-3]) -
														(0.848555999266478 * (*highpass_filtered_recording_data_chan_buff)[idx-4]); 	
														
				(*filtered_recording_data_chan_buff)[idx]=  			(0.046582906636444 * (*highpass_filtered_recording_data_chan_buff)[idx])+
														(0.186331626545775 * (*highpass_filtered_recording_data_chan_buff)[idx-1]) +
														(0.279497439818662 * (*highpass_filtered_recording_data_chan_buff)[idx-2]) + 
														(0.186331626545775 * (*highpass_filtered_recording_data_chan_buff)[idx-3]) +
 														(0.046582906636444 * (*highpass_filtered_recording_data_chan_buff)[idx-4])  -
														(-0.782095198023338 * (*filtered_recording_data_chan_buff)[idx-1]) -
														(0.679978526916300 * (*filtered_recording_data_chan_buff)[idx-2]) -
														(-0.182675697753033 * (*filtered_recording_data_chan_buff)[idx-3]) -
														(0.030118875043169 * (*filtered_recording_data_chan_buff)[idx-4]); 						
			}
			idx++;	
			if (idx ==	RECORDING_DATA_BUFF_SIZE)
				idx = 0;			
		}
	}	
	else if ((lowpass_8KHz_on) && (highpass_150Hz_on))
	{
		while (idx != end_idx)
		{
			if (idx == 0)
			{
				(*highpass_filtered_recording_data_chan_buff)[idx]=	(0.969683064082198 * (*recording_data_chan_buff)[0]) +
														(-3.878732256328792 * (*recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_1])  +
														(5.818098384493188 * (*recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_2])  + 
														(-3.878732256328792 * (*recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_3])  + 
 														(0.969683064082198 * (*recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_4])  -
														(-3.938430361819402 * (*highpass_filtered_recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_1]) -
														(5.817179417349658 * (*highpass_filtered_recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_2]) -
														(-3.819034001378268 * (*highpass_filtered_recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_3]) -
														(0.940285244767841 * (*highpass_filtered_recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_4]);

				(*filtered_recording_data_chan_buff)[idx]=  			(0.046582906636444 * (*highpass_filtered_recording_data_chan_buff)[0])+
														(0.186331626545775 * (*highpass_filtered_recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_1]) +
														(0.279497439818662 * (*highpass_filtered_recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_2]) + 
														(0.186331626545775 * (*highpass_filtered_recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_3]) +
 														(0.046582906636444 * (*highpass_filtered_recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_4])  -
														(-0.782095198023338 * (*filtered_recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_1]) -
														(0.679978526916300 * (*filtered_recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_2]) -
														(-0.182675697753033 * (*filtered_recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_3]) -
														(0.030118875043169 * (*filtered_recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_4]); 	
			}
			else if (idx == 1)
			{
				(*highpass_filtered_recording_data_chan_buff)[idx]=	(0.969683064082198 * (*recording_data_chan_buff)[1]) +
														(-3.878732256328792 * (*recording_data_chan_buff)[0])  +
														(5.818098384493188 * (*recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_1])  + 
														(-3.878732256328792 * (*recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_2])  + 
 														(0.969683064082198 * (*recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_3])  -
														(-3.938430361819402 * (*highpass_filtered_recording_data_chan_buff)[0]) -
														(5.817179417349658 * (*highpass_filtered_recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_1]) -
														(-3.819034001378268 * (*highpass_filtered_recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_2]) -
														(0.940285244767841 * (*highpass_filtered_recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_3]);		
														
				(*filtered_recording_data_chan_buff)[idx]=  			(0.046582906636444 * (*highpass_filtered_recording_data_chan_buff)[1])+
														(0.186331626545775 * (*highpass_filtered_recording_data_chan_buff)[0]) +
														(0.279497439818662 * (*highpass_filtered_recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_1]) + 
														(0.186331626545775 * (*highpass_filtered_recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_2]) +
 														(0.046582906636444 * (*highpass_filtered_recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_3])  -
														(-0.782095198023338 * (*filtered_recording_data_chan_buff)[0]) -
														(0.679978526916300 * (*filtered_recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_1]) -
														(-0.182675697753033 * (*filtered_recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_2]) -
														(0.030118875043169 * (*filtered_recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_3]); 		
			}
			else if (idx == 2)
			{
				(*highpass_filtered_recording_data_chan_buff)[idx]=	(0.969683064082198 * (*recording_data_chan_buff)[2]) +
														(-3.878732256328792 * (*recording_data_chan_buff)[1])  +
														(5.818098384493188 * (*recording_data_chan_buff)[0])  + 
														(-3.878732256328792 * (*recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_1])  + 
 														(0.969683064082198 * (*recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_2])  -
														(-3.938430361819402 * (*highpass_filtered_recording_data_chan_buff)[1]) -
														(5.817179417349658 * (*highpass_filtered_recording_data_chan_buff)[0]) -
														(-3.819034001378268 * (*highpass_filtered_recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_1]) -
														(0.940285244767841 * (*highpass_filtered_recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_2]);
														
				(*filtered_recording_data_chan_buff)[idx]=  			(0.046582906636444 * (*highpass_filtered_recording_data_chan_buff)[2])+
														(0.186331626545775 * (*highpass_filtered_recording_data_chan_buff)[1]) +
														(0.279497439818662 * (*highpass_filtered_recording_data_chan_buff)[0]) + 
														(0.186331626545775 * (*highpass_filtered_recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_1]) +
 														(0.046582906636444 * (*highpass_filtered_recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_2])  -
														(-0.782095198023338 * (*filtered_recording_data_chan_buff)[1]) -
														(0.679978526916300 * (*filtered_recording_data_chan_buff)[0]) -
														(-0.182675697753033 * (*filtered_recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_1]) -
														(0.030118875043169 * (*filtered_recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_2]); 				
			}	
			else if (idx == 3)
			{
				(*highpass_filtered_recording_data_chan_buff)[idx]=	(0.969683064082198 * (*recording_data_chan_buff)[3]) +
														(-3.878732256328792 * (*recording_data_chan_buff)[2])  +
														(5.818098384493188 * (*recording_data_chan_buff)[1])  + 
														(-3.878732256328792 * (*recording_data_chan_buff)[0])  + 
 														(0.969683064082198 * (*recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_1])  -
														(-3.938430361819402 * (*highpass_filtered_recording_data_chan_buff)[2]) -
														(5.817179417349658 * (*highpass_filtered_recording_data_chan_buff)[1]) -
														(-3.819034001378268 * (*highpass_filtered_recording_data_chan_buff)[0]) -
														(0.940285244767841 * (*highpass_filtered_recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_1]);	
														
				(*filtered_recording_data_chan_buff)[idx]=  			(0.046582906636444 * (*highpass_filtered_recording_data_chan_buff)[3])+
														(0.186331626545775 * (*highpass_filtered_recording_data_chan_buff)[2]) +
														(0.279497439818662 * (*highpass_filtered_recording_data_chan_buff)[1]) + 
														(0.186331626545775 * (*highpass_filtered_recording_data_chan_buff)[0]) +
 														(0.046582906636444 * (*highpass_filtered_recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_1])  -
														(-0.782095198023338 * (*filtered_recording_data_chan_buff)[2]) -
														(0.679978526916300 * (*filtered_recording_data_chan_buff)[1]) -
														(-0.182675697753033 * (*filtered_recording_data_chan_buff)[0]) -
														(0.030118875043169 * (*filtered_recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_1]); 			
			}	
			else 
			{
				(*highpass_filtered_recording_data_chan_buff)[idx]=	(0.969683064082198 * (*recording_data_chan_buff)[idx]) +
														(-3.878732256328792 * (*recording_data_chan_buff)[idx-1])  +
														(5.818098384493188 * (*recording_data_chan_buff)[idx-2])  + 
														(-3.878732256328792 * (*recording_data_chan_buff)[idx-3])  + 
 														(0.969683064082198 * (*recording_data_chan_buff)[idx-4])  -
														(-3.938430361819402 * (*highpass_filtered_recording_data_chan_buff)[idx-1]) -
														(5.817179417349658 * (*highpass_filtered_recording_data_chan_buff)[idx-2]) -
														(-3.819034001378268 * (*highpass_filtered_recording_data_chan_buff)[idx-3]) -
														(0.940285244767841 * (*highpass_filtered_recording_data_chan_buff)[idx-4]);			
														
				(*filtered_recording_data_chan_buff)[idx]=  			(0.046582906636444 * (*highpass_filtered_recording_data_chan_buff)[idx])+
														(0.186331626545775 * (*highpass_filtered_recording_data_chan_buff)[idx-1]) +
														(0.279497439818662 * (*highpass_filtered_recording_data_chan_buff)[idx-2]) + 
														(0.186331626545775 * (*highpass_filtered_recording_data_chan_buff)[idx-3]) +
 														(0.046582906636444 * (*highpass_filtered_recording_data_chan_buff)[idx-4])  -
														(-0.782095198023338 * (*filtered_recording_data_chan_buff)[idx-1]) -
														(0.679978526916300 * (*filtered_recording_data_chan_buff)[idx-2]) -
														(-0.182675697753033 * (*filtered_recording_data_chan_buff)[idx-3]) -
														(0.030118875043169 * (*filtered_recording_data_chan_buff)[idx-4]); 
			}
			idx++;	
			if (idx ==	RECORDING_DATA_BUFF_SIZE)
				idx = 0;								
		}
	}	
	else if (highpass_400Hz_on)	 
	{
		while (idx != end_idx)
		{
			if (idx == 0)
			{
				(*filtered_recording_data_chan_buff)[idx]=			(0.921170993499942 * (*recording_data_chan_buff)[0]) +
														(-3.684683973999769 * (*recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_1]) +
														(5.527025960999653 * (*recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_2]) + 
														(-3.684683973999769 * (*recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_3]) +
 														(0.921170993499942 * (*recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_4])  -
														(-3.835825540647349 * (*filtered_recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_1]) -
														(5.520819136622230 * (*filtered_recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_2]) -
														(-3.533535219463017 * (*filtered_recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_3]) -
														(0.848555999266478 * (*filtered_recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_4]); 
			}
			else if (idx == 1)
			{
				(*filtered_recording_data_chan_buff)[idx]=			(0.921170993499942 * (*recording_data_chan_buff)[1]) +
														(-3.684683973999769 * (*recording_data_chan_buff)[0]) +
														(5.527025960999653 * (*recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_1]) + 
														(-3.684683973999769 * (*recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_2]) +
 														(0.921170993499942 * (*recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_3])  -
														(-3.835825540647349 * (*filtered_recording_data_chan_buff)[0]) -
														(5.520819136622230 * (*filtered_recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_1]) -
														(-3.533535219463017 * (*filtered_recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_2]) -
														(0.848555999266478 * (*filtered_recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_3]); 
			}
			else if (idx == 2)
			{
				(*filtered_recording_data_chan_buff)[idx]=			(0.921170993499942 * (*recording_data_chan_buff)[2]) +
														(-3.684683973999769 * (*recording_data_chan_buff)[1]) +
														(5.527025960999653 * (*recording_data_chan_buff)[0]) + 
														(-3.684683973999769 * (*recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_1]) +
 														(0.921170993499942 * (*recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_2])  -
														(-3.835825540647349 * (*filtered_recording_data_chan_buff)[1]) -
														(5.520819136622230 * (*filtered_recording_data_chan_buff)[0]) -
														(-3.533535219463017 * (*filtered_recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_1]) -
														(0.848555999266478 * (*filtered_recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_2]); 
			}			
			else if (idx == 3)
			{
				(*filtered_recording_data_chan_buff)[idx]=			(0.921170993499942 * (*recording_data_chan_buff)[3]) +
														(-3.684683973999769 * (*recording_data_chan_buff)[2]) +
														(5.527025960999653 * (*recording_data_chan_buff)[1]) + 
														(-3.684683973999769 * (*recording_data_chan_buff)[0]) +
 														(0.921170993499942 * (*recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_1])  -
														(-3.835825540647349 * (*filtered_recording_data_chan_buff)[2]) -
														(5.520819136622230 * (*filtered_recording_data_chan_buff)[1]) -
														(-3.533535219463017 * (*filtered_recording_data_chan_buff)[0]) -
														(0.848555999266478 * (*filtered_recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_1]); 
			}
			else
			{
				(*filtered_recording_data_chan_buff)[idx]=			(0.921170993499942 * (*recording_data_chan_buff)[idx]) +
														(-3.684683973999769 * (*recording_data_chan_buff)[idx-1]) +
														(5.527025960999653 * (*recording_data_chan_buff)[idx-2]) + 
														(-3.684683973999769 * (*recording_data_chan_buff)[idx-3]) +
 														(0.921170993499942 * (*recording_data_chan_buff)[idx-4])  -
														(-3.835825540647349 * (*filtered_recording_data_chan_buff)[idx-1]) -
														(5.520819136622230 * (*filtered_recording_data_chan_buff)[idx-2]) -
														(-3.533535219463017 * (*filtered_recording_data_chan_buff)[idx-3]) -
														(0.848555999266478 * (*filtered_recording_data_chan_buff)[idx-4]); 						
			}
			idx++;	
			if (idx ==	RECORDING_DATA_BUFF_SIZE)
				idx = 0;									
		}	
	}
	else if (highpass_150Hz_on)	 
	{
		while (idx != end_idx)
		{
			if (idx == 0)
			{
				(*filtered_recording_data_chan_buff)[idx]=			(0.969683064082198 * (*recording_data_chan_buff)[0]) +
														(-3.878732256328792 * (*recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_1])  +
														(5.818098384493188 * (*recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_2])  + 
														(-3.878732256328792 * (*recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_3])  + 
 														(0.969683064082198 * (*recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_4])  -
														(-3.938430361819402 * (*filtered_recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_1]) -
														(5.817179417349658 * (*filtered_recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_2]) -
														(-3.819034001378268 * (*filtered_recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_3]) -
														(0.940285244767841 * (*filtered_recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_4]);
			}
			else if (idx == 1)
			{
				(*filtered_recording_data_chan_buff)[idx]=			(0.969683064082198 * (*recording_data_chan_buff)[1]) +
														(-3.878732256328792 * (*recording_data_chan_buff)[0])  +
														(5.818098384493188 * (*recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_1])  + 
														(-3.878732256328792 * (*recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_2])  + 
 														(0.969683064082198 * (*recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_3])  -
														(-3.938430361819402 * (*filtered_recording_data_chan_buff)[0]) -
														(5.817179417349658 * (*filtered_recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_1]) -
														(-3.819034001378268 * (*filtered_recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_2]) -
														(0.940285244767841 * (*filtered_recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_3]);			
			}
			else if (idx == 2)
			{
				(*filtered_recording_data_chan_buff)[idx]=			(0.969683064082198 * (*recording_data_chan_buff)[2]) +
														(-3.878732256328792 * (*recording_data_chan_buff)[1])  +
														(5.818098384493188 * (*recording_data_chan_buff)[0])  + 
														(-3.878732256328792 * (*recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_1])  + 
 														(0.969683064082198 * (*recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_2])  -
														(-3.938430361819402 * (*filtered_recording_data_chan_buff)[1]) -
														(5.817179417349658 * (*filtered_recording_data_chan_buff)[0]) -
														(-3.819034001378268 * (*filtered_recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_1]) -
														(0.940285244767841 * (*filtered_recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_2]);			
			}	
			else if (idx == 3)
			{
				(*filtered_recording_data_chan_buff)[idx]=			(0.969683064082198 * (*recording_data_chan_buff)[3]) +
														(-3.878732256328792 * (*recording_data_chan_buff)[2])  +
														(5.818098384493188 * (*recording_data_chan_buff)[1])  + 
														(-3.878732256328792 * (*recording_data_chan_buff)[0])  + 
 														(0.969683064082198 * (*recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_1])  -
														(-3.938430361819402 * (*filtered_recording_data_chan_buff)[2]) -
														(5.817179417349658 * (*filtered_recording_data_chan_buff)[1]) -
														(-3.819034001378268 * (*filtered_recording_data_chan_buff)[0]) -
														(0.940285244767841 * (*filtered_recording_data_chan_buff)[RECORDING_DATA_BUFF_SIZE_1]);			
			}	
			else 
			{
				(*filtered_recording_data_chan_buff)[idx]=			(0.969683064082198 * (*recording_data_chan_buff)[idx]) +
														(-3.878732256328792 * (*recording_data_chan_buff)[idx-1])  +
														(5.818098384493188 * (*recording_data_chan_buff)[idx-2])  + 
														(-3.878732256328792 * (*recording_data_chan_buff)[idx-3])  + 
 														(0.969683064082198 * (*recording_data_chan_buff)[idx-4])  -
														(-3.938430361819402 * (*filtered_recording_data_chan_buff)[idx-1]) -
														(5.817179417349658 * (*filtered_recording_data_chan_buff)[idx-2]) -
														(-3.819034001378268 * (*filtered_recording_data_chan_buff)[idx-3]) -
														(0.940285244767841 * (*filtered_recording_data_chan_buff)[idx-4]);			
			}
			idx++;	
			if (idx ==	RECORDING_DATA_BUFF_SIZE)
				idx = 0;
		}
	}
	filtered_recording_data->buff_idx_write[mwa][mwa_chan] = end_idx;   
	filtered_recording_data->buff_idx_prev[mwa][mwa_chan] = start_idx;   	
}

void find_spike_end(RecordingData *filtered_recording_data, int mwa, int mwa_chan)
{
	RecordingDataChanBuff	*filtered_recording_data_chan_buff;
	int previous_acquisition_time_cntr =0; 
	int idx, start_idx, end_idx, min_idx, i, spike_end_idx, handle_for_peak_idx;
	float amplitude_thres, min;
	bool *in_spike;
	int *in_spike_sample_cntr;
	SpikeEndHandlingBuff 	*spike_end_handling_buff;	
	TimeStamp peak_time;			

	amplitude_thres = blue_spike_data->spike_thresholding.amplitude_thres[mwa][mwa_chan];
	if (amplitude_thres == 0.0)
	{
		return;
	}
	
	filtered_recording_data_chan_buff = &(filtered_recording_data->recording_data_buff[mwa][mwa_chan]);
	spike_end_handling_buff = &(spike_end_handling.spike_end_handling_buff);
	
	start_idx = filtered_recording_data->buff_idx_prev[mwa][mwa_chan];
	end_idx = filtered_recording_data->buff_idx_write[mwa][mwa_chan];
	in_spike = &(blue_spike_data->spike_thresholding.in_spike[mwa][mwa_chan]);
	in_spike_sample_cntr = &(blue_spike_data->spike_thresholding.in_spike_sample_cntr[mwa][mwa_chan]);
	idx = start_idx;

	while (idx != end_idx)
	{	
		previous_acquisition_time_cntr++;
		
		if (((*filtered_recording_data_chan_buff)[idx] <  amplitude_thres) && (!(*in_spike)))
		{
			*in_spike = 1;
		}
		else if (((*filtered_recording_data_chan_buff)[idx] >  amplitude_thres) && (*in_spike))
		{
			*in_spike = 0;
			*in_spike_sample_cntr = 0;
			min = (*filtered_recording_data_chan_buff)[idx];
			min_idx = idx;
			for (i=0; i<SPIKE_MIN_END_SAMP_NUM; i++)	// cannot be larger than SPIKE_MIN_END_SAMP_NUM.. Otherwise handle_spike_end_handling_buffer will fail. since spike end idx might be smaller than filtered_recording_data->buff_idx_prev[mwa][mwa_chan]
			{
				if (idx < i)   // if (idx - i <0) 
				{
					handle_for_peak_idx = idx-i+RECORDING_DATA_BUFF_SIZE;
					if ((*filtered_recording_data_chan_buff)[handle_for_peak_idx] < min)
					{
						min =  (*filtered_recording_data_chan_buff)[handle_for_peak_idx];
						min_idx = handle_for_peak_idx;
						previous_acquisition_time_cntr--;
					}	
				}
				else
				{
					handle_for_peak_idx = idx-i;
					if ((*filtered_recording_data_chan_buff)[handle_for_peak_idx] < min)
					{
						min =  (*filtered_recording_data_chan_buff)[handle_for_peak_idx];
						min_idx = handle_for_peak_idx;
						previous_acquisition_time_cntr--;
					}		
				}
			}
			if ((min_idx+SPIKE_MIN_END_SAMP_NUM) >= RECORDING_DATA_BUFF_SIZE)
				spike_end_idx = min_idx+SPIKE_MIN_END_SAMP_NUM - RECORDING_DATA_BUFF_SIZE;
			else		
				spike_end_idx = min_idx+SPIKE_MIN_END_SAMP_NUM;
			
			peak_time = previous_time_ns + (previous_acquisition_time_cntr * SAMPLING_INTERVAL);  // SAMPLING_INTERVAL = 25000 nanoseconds;
				
			if (is_index_between_indexes(filtered_recording_data->buff_idx_prev[mwa][mwa_chan], filtered_recording_data->buff_idx_write[mwa][mwa_chan], spike_end_idx))
			{
				run_template_matching(filtered_recording_data, mwa, mwa_chan, spike_end_idx, peak_time);  // SAMPLING_INTERVAL = 25000 nanoseconds;
			}			
			else 	//   Write spike end into shared_memory->spike_end_handing
			{
				(*spike_end_handling_buff)[spike_end_handling.buff_write_idx].mwa = mwa;		// push to the end off buffer to be handled later on
				(*spike_end_handling_buff)[spike_end_handling.buff_write_idx].chan = mwa_chan;
				(*spike_end_handling_buff)[spike_end_handling.buff_write_idx].end_of_spike_in_filtered_recording_data_buff = spike_end_idx;
				(*spike_end_handling_buff)[spike_end_handling.buff_write_idx].peak_time = peak_time;
				if ((spike_end_handling.buff_write_idx + 1) == spike_end_handling_buff_size)
					spike_end_handling.buff_write_idx = 0;
				else
					spike_end_handling.buff_write_idx++;		
				spike_end_handling_buff_control_cntr++;	
			}
		}
		else if (*in_spike)
		{
			(*in_spike_sample_cntr)++; 
			if ((*in_spike_sample_cntr) > 20)	// not a spike
			{
				*in_spike_sample_cntr = 0;	
				*in_spike = 0;				// discard this noisy thing
			}
		}
		idx ++;
		if (idx ==	RECORDING_DATA_BUFF_SIZE)
			idx = 0;
	}											
}
void handle_spike_end_handling_buffer(void)
{
	RecordingData		*filtered_recording_data;
	SpikeEndHandlingBuff 	*spike_end_handling_buff;			

	int mwa, chan, filtered_recording_data_buff_idx;	
	TimeStamp	peak_time;
	
	int idx, start_idx, end_idx;	
	
	filtered_recording_data = &blue_spike_data->filtered_recording_data;	
	spike_end_handling_buff = &(spike_end_handling.spike_end_handling_buff);

	start_idx = spike_end_handling.buff_start_idx;
	end_idx = spike_end_handling.buff_write_idx;
	
	idx = start_idx;
	while (idx != end_idx)
	{	
		mwa = (*spike_end_handling_buff)[idx].mwa;
		chan = (*spike_end_handling_buff)[idx].chan;
		filtered_recording_data_buff_idx = (*spike_end_handling_buff)[idx].end_of_spike_in_filtered_recording_data_buff;
		peak_time = (*spike_end_handling_buff)[idx].peak_time;
		
		if (is_index_between_indexes(filtered_recording_data->buff_idx_prev[mwa][chan], filtered_recording_data->buff_idx_write[mwa][chan], filtered_recording_data_buff_idx))
		{
			run_template_matching(filtered_recording_data, mwa, chan, filtered_recording_data_buff_idx, peak_time);
		}	
		else
		{
			(*spike_end_handling_buff)[spike_end_handling.buff_write_idx].mwa = mwa;		// push to the end off buffer to be handled later on
			(*spike_end_handling_buff)[spike_end_handling.buff_write_idx].chan = chan;
			(*spike_end_handling_buff)[spike_end_handling.buff_write_idx].end_of_spike_in_filtered_recording_data_buff = filtered_recording_data_buff_idx;
			(*spike_end_handling_buff)[spike_end_handling.buff_write_idx].peak_time = peak_time;
			if ((spike_end_handling.buff_write_idx + 1) == spike_end_handling_buff_size)
				spike_end_handling.buff_write_idx = 0;
			else
				spike_end_handling.buff_write_idx++;
			spike_end_handling_buff_control_cntr++;
		}
		idx ++;
		if (idx ==	spike_end_handling_buff_size)
			idx = 0;
	}
	spike_end_handling.buff_start_idx = end_idx;  
}


void run_template_matching(RecordingData *filtered_recording_data, int mwa, int chan, int filtered_recording_data_buff_idx, TimeStamp peak_time)
{

	TemplateMatchingData	*template_matching_data;
	TemplateMatchingUnitData *unit_template_data;	
	RecordingDataChanBuff	*filtered_recording_data_chan_buff;
	BlueSpikeTimeStamp 	*blue_spike_time_stamp;
	SpikeTimeStamp 		*spike_time_stamp;
		
	double greatest;	
	int i, j, unit, greatest_idx;
	int blue_spike_time_stamp_buff_idx_write;
	int include_unit;
	int spike_time_stamp_buff_idx_write;
	
	filtered_recording_data_chan_buff = &(filtered_recording_data->recording_data_buff[mwa][chan]);
	template_matching_data = &blue_spike_data->template_matching_data;
	blue_spike_time_stamp = &blue_spike_data->blue_spike_time_stamp;
	spike_time_stamp = &blue_spike_data->spike_time_stamp;

	greatest = -DBL_MAX;
	greatest_idx = MAX_NUM_OF_UNIT_PER_CHAN;   // If doesnt match any one it will be classified as unsorted (MAX_NUM_OF_UNIT_PER_CHAN)

	for (unit = 0; unit <MAX_NUM_OF_UNIT_PER_CHAN; unit++)
	{
		unit_template_data =  &(*template_matching_data)[mwa][chan][unit];
		if (unit_template_data->sorting_on)
		{
			template_matching_g_x[unit] = 0.0;
			for (i=0; i<NUM_OF_SAMP_PER_SPIKE; i++)
			{
				{
					if (filtered_recording_data_buff_idx < i)   //  spike_end_idx_in_filtered_recording - i  < 0
						template_matching_diff[unit][i] = (*filtered_recording_data_chan_buff)[filtered_recording_data_buff_idx-i+RECORDING_DATA_BUFF_SIZE] - unit_template_data->template[NUM_OF_SAMP_PER_SPIKE-i-1];
					else
						template_matching_diff[unit][i] = (*filtered_recording_data_chan_buff)[filtered_recording_data_buff_idx-i] - unit_template_data->template[NUM_OF_SAMP_PER_SPIKE-i-1];
				}
			}
			for (i=0; i <NUM_OF_SAMP_PER_SPIKE;i++)
			{
				template_matching_diff_temporary[unit][i] = 0;
			}
			for (i=0; i<NUM_OF_SAMP_PER_SPIKE; i++)
			{
				for (j=0; j<NUM_OF_SAMP_PER_SPIKE; j++)
				{
					template_matching_diff_temporary[unit][i] = template_matching_diff_temporary[unit][i] + (template_matching_diff[unit][j] * unit_template_data->inv_S[i][j]);
				}
			}
			for (i=0; i<NUM_OF_SAMP_PER_SPIKE; i++)
			{
				template_matching_g_x[unit] = template_matching_g_x[unit] + (template_matching_diff_temporary[unit][i] * template_matching_diff[unit][i]);
			}
			template_matching_exponent[unit] = exp((-0.5)*template_matching_g_x[unit]);
			template_matching_probabl[unit] = (1.06488319787324016356e-12/unit_template_data->sqrt_det_S)*template_matching_exponent[unit];       //   ( 1/ (   ((2*pi)^(d/2)) * (det_S^(1/2)) ) * exp( (-1/2) * (x-u)' * (S^ (-1)) - (x-u) )   d= 30

			template_matching_g_x[unit] = 0 - (unit_template_data->log_det_S) - (template_matching_g_x[unit]);	
			
			if ((template_matching_g_x[unit] > greatest) && (template_matching_probabl[unit] > unit_template_data->probability_thres))    // assign spike to a unit.
			{
				greatest = template_matching_g_x[unit];
				greatest_idx = unit;
			}	
		}
	}
	

	//   Write spike time stamp into shared_memory->spike_time_stamp
	blue_spike_time_stamp_buff_idx_write = blue_spike_time_stamp->buff_idx_write;
	include_unit = (*template_matching_data)[mwa][chan][greatest_idx].include_unit;
	blue_spike_time_stamp->blue_spike_time_stamp_buff[blue_spike_time_stamp_buff_idx_write].peak_time = peak_time;
	blue_spike_time_stamp->blue_spike_time_stamp_buff[blue_spike_time_stamp_buff_idx_write].mwa = mwa;
	blue_spike_time_stamp->blue_spike_time_stamp_buff[blue_spike_time_stamp_buff_idx_write].channel = chan;
	blue_spike_time_stamp->blue_spike_time_stamp_buff[blue_spike_time_stamp_buff_idx_write].unit = greatest_idx;
	blue_spike_time_stamp->blue_spike_time_stamp_buff[blue_spike_time_stamp_buff_idx_write].recording_data_buff_idx = filtered_recording_data_buff_idx;
	blue_spike_time_stamp->blue_spike_time_stamp_buff[blue_spike_time_stamp_buff_idx_write].include_unit = include_unit;
	if ((blue_spike_time_stamp_buff_idx_write +1) ==  blue_spike_time_stamp_buff_size )	   // first check then increment. if first increment and check end of buffer might lead to problem during reading.
		blue_spike_time_stamp->buff_idx_write = 0;
	else
		blue_spike_time_stamp->buff_idx_write++;	
	blue_spike_time_stamp_buff_control_cntr++;		// to check if the buffer gets full in one rt task period
	
	if (include_unit)	// fill in spike time stamp buff
	{
		spike_time_stamp_buff_idx_write = spike_time_stamp->buff_idx_write;
		spike_time_stamp->spike_time_stamp_buff[spike_time_stamp_buff_idx_write].peak_time = peak_time;
		spike_time_stamp->spike_time_stamp_buff[spike_time_stamp_buff_idx_write].mwa_or_layer = mwa;
		spike_time_stamp->spike_time_stamp_buff[spike_time_stamp_buff_idx_write].channel_or_neuron_group = chan;
		spike_time_stamp->spike_time_stamp_buff[spike_time_stamp_buff_idx_write].unit_or_neuron = greatest_idx;	
		if ((spike_time_stamp_buff_idx_write +1) ==  spike_time_stamp_buff_size )	   // first check then increment. if first increment and check end of buffer might lead to problem during reading.
			spike_time_stamp->buff_idx_write = 0;
		else
			spike_time_stamp->buff_idx_write++;			
	}
}

void print_warning_and_errors(void)
{
	if (current_time_ns > KERNELSPIKE_RUN_TIME_LIMIT)	//  Maximum time stamp value is 18446744073709551615 //8 bytes makes 18 446 744 073 709 551 615 nanoseconds 5124095.57603043100417 hours
	{
		printk("KernelSpike: CRITICAL ERROR: KernelSpike reaches maximum time capacity limit\n");
		printk("KernelSpike: CRITICAL ERROR: Current time is: %llu\n", ((long long unsigned int) current_time_ns));
		rt_tasks_data->kernel_task_ctrl.kill_all_rt_tasks = 1;
		return;
	}
	if ((spike_end_handling_buff_size - spike_end_handling_buff_control_cntr) < 100)
	{
		printk("KernelSpike: ------------------------------------------------------\n");
		printk("KernelSpike: ------------   WARNING  !!!  -----------------\n");
		printk("KernelSpike: ---- Spike End Buffer is getting full ----\n");
		printk("KernelSpike: ---- Spike End Buffer is getting full ----\n");				
		printk("KernelSpike: --Latest # of detected spikes is %d---\n", spike_end_handling_buff_control_cntr);	
		printk("KernelSpike: -------Spike End buffer size  is %d------\n", spike_end_handling_buff_size);
		printk("KernelSpike: ------------------------------------------------------\n");					
	}  
	else if (spike_end_handling_buff_size <= spike_end_handling_buff_control_cntr)
	{
		printk("KernelSpike: ------------------------------------------------------\n");
		printk("KernelSpike: ----------------   ERROR !!!  -----------------\n");
		printk("KernelSpike: ---- Spike End Buffer is full ---------------\n");
		printk("KernelSpike: ---- Spike End Buffer is full ---------------\n");				
		printk("KernelSpike: --Latest # of detected spikes is %d ---\n", spike_end_handling_buff_control_cntr);	
		printk("KernelSpike: -------Spike End buffer size  is %d------\n", spike_end_handling_buff_size);
		printk("KernelSpike: ------------------------------------------------------\n");
		rt_tasks_data->kernel_task_ctrl.kill_all_rt_tasks = 1;
		return;		
	}  
	if ((blue_spike_time_stamp_buff_size - blue_spike_time_stamp_buff_control_cntr) < 100)
	{
		printk("KernelSpike: --------------------------------------------------------\n");
		printk("KernelSpike: ------------   WARNING  !!!  -------------------\n");
		printk("KernelSpike: ---- BlueSpikeTimestamp Buffer is getting full ------\n");
		printk("KernelSpike: ---- BlueSpikeTimestamp Buffer is getting full ------\n");				
		printk("KernelSpike: --Latest # of Spike Timestamp is %d----\n", blue_spike_time_stamp_buff_control_cntr);	
		printk("KernelSpike: ---Spike Timestamp buffer size  is %d--\n", blue_spike_time_stamp_buff_size);
		printk("KernelSpike: --------------------------------------------------------\n");					
	}  
	else if (blue_spike_time_stamp_buff_size <= blue_spike_time_stamp_buff_control_cntr)
	{
		printk("KernelSpike: ---------------------------------------------------------\n");
		printk("KernelSpike: ----------------   ERROR  !!!  --------------------\n");
		printk("KernelSpike: ---- BlueSpikeTimestamp Buffer is full -------\n");
		printk("KernelSpike: ---- BlueSpikeTimestamp Buffer is full -------\n");				
		printk("KernelSpike: --Latest # of Spike Timestamp is %d----\n", blue_spike_time_stamp_buff_control_cntr);	
		printk("KernelSpike: ---Spike Timestamp buffer size  is %d--\n", blue_spike_time_stamp_buff_size);
		printk("KernelSpike: --------------------------------------------------------\n");
		rt_tasks_data->kernel_task_ctrl.kill_all_rt_tasks = 1;
		return;		
	}	
	spike_end_handling_buff_control_cntr = 0;
	blue_spike_time_stamp_buff_control_cntr = 0;
}
	
	
bool is_index_between_indexes(int start_idx, int end_idx, int this_idx)
{
	if (
		(
			(start_idx < end_idx) && (start_idx <= this_idx) && (this_idx < end_idx )
		)
		||
		(
			( start_idx > end_idx) &&
								(
									(start_idx <= this_idx ) || (this_idx < end_idx )
																		)
		)
	)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int open_daq_cards(void)
{
	char path_comedi[100], temp[10];	
	int i, j, ret;
	for (i = 0; i<MAX_NUM_OF_DAQ_CARD; i++)
	{
		strcpy(path_comedi, "/dev/comedi");	
		sprintf(temp, "%d" , i);
		strcat(path_comedi, temp);
		ni6070_comedi_dev[i] = comedi_open(path_comedi);
		if (ni6070_comedi_dev[i] == NULL)
		{
			printk("KernelSpike: ERROR: Couldn' t comedi_open %dth device at %s\n", i, path_comedi);
			break;
		}
	}
	if (i != MAX_NUM_OF_DAQ_CARD)   // Couldn' t open all daq cards. Close the open ones & cancel out openning process. 
	{
		for (j = 0; j < i ; j++)
		{
			comedi_cancel(ni6070_comedi_dev[j], COMEDI_SUBDEVICE_AI);
			comedi_close(ni6070_comedi_dev[j]);			
		}
		rt_tasks_data->kernel_task_ctrl.kill_all_rt_tasks = 1;
		return 0;
	} 
	
	for (i = 0; i<MAX_NUM_OF_DAQ_CARD; i++)
	{	
		ret = comedi_map(ni6070_comedi_dev[i], COMEDI_SUBDEVICE_AI, &(comedi_map_ptr[i]));
		printk("KernelSpike: %d th device comedi_map return: %d, ptr: %lu\n", i, ret, (unsigned long int)(comedi_map_ptr[i]));
		if (ret != 0)
		{
			break;
		}
		comedi_buff_size[i] = comedi_get_buffer_size(ni6070_comedi_dev[i], COMEDI_SUBDEVICE_AI);
		printk("KernelSpike: Buffer size of %dth device is %d\n", i, comedi_buff_size[i]);
		ni6070_comedi_configure(i);
	}
	
	if (i != MAX_NUM_OF_DAQ_CARD)   // Couldn' t comedi_map all daq cards. Close all daq cards & cancel out process. 
	{
		for (j = 0; j < MAX_NUM_OF_DAQ_CARD ; j++)
		{
			comedi_cancel(ni6070_comedi_dev[j], COMEDI_SUBDEVICE_AI);
			comedi_close(ni6070_comedi_dev[j]);			
		}
		return 0;
		rt_tasks_data->kernel_task_ctrl.kill_all_rt_tasks = 1;
	}
	return 1; 		
}

void close_daq_cards(void)
{
	int i;
	for (i = 0; i<MAX_NUM_OF_DAQ_CARD; i++)
	{
		comedi_cancel(ni6070_comedi_dev[i], COMEDI_SUBDEVICE_AI);
		comedi_close(ni6070_comedi_dev[i]);
	}
}

int handle_daq_cards(void)
{
	if ((blue_spike_data->blue_spike_ctrl.turn_daq_card_on) && (daq_cards_on))
	{
		return 0;
	}
	else if ((!(blue_spike_data->blue_spike_ctrl.turn_daq_card_on)) && (!daq_cards_on))
	{
		return 1;
	}
	else if ((blue_spike_data->blue_spike_ctrl.turn_daq_card_on) && (!daq_cards_on))
	{
		if (open_daq_cards()) 
			daq_cards_on = 1;
		return 1;
	}
	else if ((!(blue_spike_data->blue_spike_ctrl.turn_daq_card_on)) && (daq_cards_on))
	{
		close_daq_cards();
		daq_cards_on = 0;
		return 1;
	}
	return 1;		// to get rid of warning, unnecessary	
}
void evaluate_period_run_time(unsigned int curr_time)
{
	static unsigned int max_period_run_time = 0;		
	unsigned int period_run_time, period_end_time;

	period_end_time = rt_get_cpu_time_ns();
	period_run_time = period_end_time - curr_time;
	if (period_run_time > max_period_run_time)
	{
		max_period_run_time = period_run_time;
		rt_tasks_data->cpu_rt_task_data[KERNELSPIKE_CPU_ID].cpu_thread_rt_task_data[KERNELSPIKE_CPU_THREAD_ID].max_period_run_time = max_period_run_time;
	}	
}
void evaluate_jitter(unsigned int period_occured)
{
	static unsigned int max_positive_jitter = 0;
	static unsigned int max_negative_jitter = 0;
	unsigned int jitter;	
	if (period_occured > KERNELSPIKE_PERIOD)
	{
		jitter = period_occured - KERNELSPIKE_PERIOD;
		if (jitter > max_positive_jitter)
		{
			max_positive_jitter = jitter;
			rt_tasks_data->cpu_rt_task_data[KERNELSPIKE_CPU_ID].cpu_thread_rt_task_data[KERNELSPIKE_CPU_THREAD_ID].max_positive_jitter = max_positive_jitter;
		}
		if (jitter > rt_tasks_data->cpu_rt_task_data[KERNELSPIKE_CPU_ID].cpu_thread_rt_task_data[KERNELSPIKE_CPU_THREAD_ID].positive_jitter_threshold)
			rt_tasks_data->cpu_rt_task_data[KERNELSPIKE_CPU_ID].cpu_thread_rt_task_data[KERNELSPIKE_CPU_THREAD_ID].num_of_positive_jitter_exceeding_threshold++;
	}
	else
	{
		jitter = KERNELSPIKE_PERIOD - period_occured;
		if (jitter > max_negative_jitter)
		{
			max_negative_jitter = jitter;
			rt_tasks_data->cpu_rt_task_data[KERNELSPIKE_CPU_ID].cpu_thread_rt_task_data[KERNELSPIKE_CPU_THREAD_ID].max_negative_jitter = max_negative_jitter;
		}	
		if (jitter > rt_tasks_data->cpu_rt_task_data[KERNELSPIKE_CPU_ID].cpu_thread_rt_task_data[KERNELSPIKE_CPU_THREAD_ID].negative_jitter_threshold)
			rt_tasks_data->cpu_rt_task_data[KERNELSPIKE_CPU_ID].cpu_thread_rt_task_data[KERNELSPIKE_CPU_THREAD_ID].num_of_negative_jitter_exceeding_threshold++;		
	} 
}
