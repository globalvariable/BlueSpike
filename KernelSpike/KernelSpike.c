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


void rt_handler(int t)
{
	int i, j, k, m, return_value;
	int front[MAX_NUM_OF_DAQ_CARD], back[MAX_NUM_OF_DAQ_CARD], num_byte[MAX_NUM_OF_DAQ_CARD], daq_chan_num[MAX_NUM_OF_DAQ_CARD] ;
	DaqMwaMap			*daq_mwa_map;
	RecordingData			*recording_data;
	RecordingData			*filtered_recording_data;
	KernelTaskCtrl			*kernel_task_ctrl;
	SpikeEnd				*spike_end;
	
	int *recording_data_write_idx;
	int mwa, mwa_chan;
	bool *highpass_150Hz_on, *highpass_400Hz_on, *lowpass_8KHz_on, *kernel_task_idle; 
	
	int prev_time= rt_get_cpu_time_ns(); // local_time  unsigned int
	int curr_time ;		// local_time  unsigned int
	 
	current_time_ns = 0;		// global time  long long unsigned int  // TimeStamp	
	previous_time_ns = 0;
	spike_end_buff_control_cntr = 0; 
	spike_timestamp_buff_control_cntr = 0;	
	daq_mwa_map = &shared_memory->daq_mwa_map;
	recording_data = &shared_memory->recording_data;
	filtered_recording_data = &shared_memory->filtered_recording_data;
	spike_end = &shared_memory->spike_end;	
	kernel_task_ctrl = &shared_memory->kernel_task_ctrl;
		
	highpass_150Hz_on = &kernel_task_ctrl->highpass_150Hz_on; 
	highpass_400Hz_on = &kernel_task_ctrl->highpass_400Hz_on;
	lowpass_8KHz_on = &kernel_task_ctrl->lowpass_8KHz_on;
	kernel_task_idle = &kernel_task_ctrl->kernel_task_idle;	
	
	for (i=0; i < MAX_NUM_OF_DAQ_CARD; i++)
	{
		front[i] = 0;
		back[i] = 0;
		daq_chan_num[i] = 0;
	}
	
	rt_task_stay_alive = 1;
	     
	while (rt_task_stay_alive) 
	{
		rt_task_wait_period();
		
		curr_time = rt_get_cpu_time_ns();
		current_time_ns += (curr_time - prev_time);
		prev_time = curr_time;
		
		*kernel_task_idle = 0;
		for (i=0; i < MAX_NUM_OF_DAQ_CARD; i++)
		{	
			comedi_poll(ni6070_comedi_dev[i], COMEDI_SUBDEVICE_AI);
			num_byte[i] = comedi_get_buffer_contents(ni6070_comedi_dev[i], COMEDI_SUBDEVICE_AI);
			front[i] = front[i] + num_byte[i];
			if (front[i] >= comedi_buff_size[i])
				front[i] = front[i] - comedi_buff_size[i];
			if(num_byte[i] == 0)
			{
				printk("CRITICAL ERROR: Must be exceeding rt_task_wait_period, num_byte[%d] = 0\n", i);
				printk("CRITICAL ERROR: Task lsted too long > TICK_PERIOD.\n");
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

					(*recording_data_write_idx)++;
					if ((*recording_data_write_idx) == RECORDING_DATA_BUFF_SIZE)
						(*recording_data_write_idx) = 0;
				}
				else if (((mwa == MAX_NUM_OF_MWA) && (mwa_chan != MAX_NUM_OF_CHAN_PER_MWA)) || ((mwa != MAX_NUM_OF_MWA) && (mwa_chan == MAX_NUM_OF_CHAN_PER_MWA)))
				{
					printk("BUG: mwa or mwa_chan is problematic\n");
				}
				
				(daq_chan_num[i])++;
				if (daq_chan_num[i] == MAX_NUM_OF_CHANNEL_PER_DAQ_CARD)
					daq_chan_num[i] = 0;
			}
				
			return_value = comedi_mark_buffer_read(ni6070_comedi_dev[i], COMEDI_SUBDEVICE_AI, num_byte[i]);
			if(return_value < 0)
			{
				printk("ERROR: comedi_mark_buffer_read");
				rt_task_stay_alive = 0;
			}
			back[i] = front[i];
		}
			
		if ((*highpass_150Hz_on) || (*highpass_400Hz_on))
		{
			spike_end_buff_control_cntr = 0;
			spike_timestamp_buff_control_cntr = 0;	
			for (k=0; k<MAX_NUM_OF_MWA; k++)
			{
				for (m=0; m<MAX_NUM_OF_CHAN_PER_MWA; m++)
				{
					filter_recording_data(recording_data, filtered_recording_data, k, m, *highpass_150Hz_on, *highpass_400Hz_on, *lowpass_8KHz_on);
					find_spike_end(spike_end, filtered_recording_data, k, m);
				}
			}
			template_matching();
		}
		else
		{
			//   shouldn' t lose recording data write index when filtering is turned off.  
			for (k=0; k<MAX_NUM_OF_MWA; k++)
			{
				for (m=0; m<MAX_NUM_OF_CHAN_PER_MWA; m++)
				{
					filtered_recording_data->buff_idx_write[k][m] = recording_data->buff_idx_write[k][m];
				//	spike_end->search_idx_start[k][m] = filtered_recording_data->buff_idx_write[k][m];             //redundant
				}
			} 	
			// spike_time_stamp->spike_end_buff_read_idx = spike_end->buff_idx_write;	// redundant
		} 
		print_buffer_warning_and_errors();		

		*kernel_task_idle = 1;
		previous_time_ns = current_time_ns;
				
	}

	for (i = 0; i<MAX_NUM_OF_DAQ_CARD; i++)
	{
		comedi_cancel(ni6070_comedi_dev[i], COMEDI_SUBDEVICE_AI);
		comedi_close(ni6070_comedi_dev[i]);
	}
	stop_rt_timer();
    	rtai_kfree(nam2num(SHARED_MEM_NAME));	
}



int __init xinit_module(void)
{
	int ret;
	int i, j;
	char path_comedi[100], temp[10];
	RTIME tick_period;

	shared_memory = (SharedMemStruct*)rtai_kmalloc(nam2num(SHARED_MEM_NAME), SHARED_MEM_SIZE);
	if (shared_memory == NULL)
		return -ENOMEM;
	memset(shared_memory, 0, SHARED_MEM_SIZE);
        printk("sizeof(SharedMemStruct) : %d\n", SHARED_MEM_SIZE);
        
	for (i=0; i < MAX_NUM_OF_DAQ_CARD; i++)
	{
		for (j=0; j<MAX_NUM_OF_CHANNEL_PER_DAQ_CARD; j++)
		{
			shared_memory->daq_mwa_map[i][j].mwa = MAX_NUM_OF_MWA;
			shared_memory->daq_mwa_map[i][j].channel = MAX_NUM_OF_CHAN_PER_MWA;
		}
	}
	
	for (i=0; i < MAX_NUM_OF_MWA; i++)
	{
		for (j=0; j<MAX_NUM_OF_CHAN_PER_MWA; j++)
		{
			shared_memory->mwa_daq_map[i][j].daq_card = MAX_NUM_OF_DAQ_CARD;
			shared_memory->mwa_daq_map[i][j].daq_chan = MAX_NUM_OF_CHANNEL_PER_DAQ_CARD;
		}
	}

	shared_memory->kernel_task_ctrl.kernel_task_idle = 1;
	for (i = 0; i<MAX_NUM_OF_DAQ_CARD; i++)
	{
		strcpy(path_comedi, "/dev/comedi");	
		sprintf(temp, "%d" , i);
		strcat(path_comedi, temp);
		ni6070_comedi_dev[i] = comedi_open(path_comedi);
		if (ni6070_comedi_dev[i] == NULL)
		{
			printk("ERROR: Couldn' t comedi_open %dth device at %s\n", i, path_comedi);
			return 0;
		}
		ret = comedi_map(ni6070_comedi_dev[i], COMEDI_SUBDEVICE_AI, &(comedi_map_ptr[i]));
		printk("%d th device comedi_map return: %d, ptr: %d\n", i, ret, (int)(comedi_map_ptr[i]));
		comedi_buff_size[i] = comedi_get_buffer_size(ni6070_comedi_dev[i], COMEDI_SUBDEVICE_AI);
		printk("buffer size of %dth device is %d\n", i, comedi_buff_size[i]);
		ni6070_comedi_configure(i);
	}

	rt_set_periodic_mode();
	rt_task_init_cpuid(&rt_task0, rt_handler, 1, STACK_SIZE, TASK_PRIORITY, 1, 0, KERNELSPIKE_CPUID);
	tick_period = start_rt_timer(nano2count(TICK_PERIOD));
	rt_task_make_periodic(&rt_task0, rt_get_time() + tick_period, tick_period);
	return 0;
}

void __exit xcleanup_module(void)
{
	int i;
	stop_rt_timer();
	rt_task_delete(&rt_task0);	
	for (i = 0; i<MAX_NUM_OF_DAQ_CARD; i++)
	{
		comedi_cancel(ni6070_comedi_dev[i], COMEDI_SUBDEVICE_AI);
		comedi_close(ni6070_comedi_dev[i]);
	}
    	rtai_kfree(nam2num(SHARED_MEM_NAME));
    	printk("rmmod KernelSpike\n");
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
	printk("test 1: %i\n", comedi_command_test(ni6070_comedi_dev[card_number], &ni6070_comedi_cmd[card_number]));
	print_cmd(card_number);
	printk("test 2: %i\n", comedi_command_test(ni6070_comedi_dev[card_number],&ni6070_comedi_cmd[card_number]));
	print_cmd(card_number);

	return comedi_command(ni6070_comedi_dev[card_number], &ni6070_comedi_cmd[card_number]);
}

void print_cmd(int card_number)
{
	printk("comedi_cmd[%d].subdev = %i\n", card_number, ni6070_comedi_cmd[card_number].subdev);
	printk("comedi_cmd[%d].flags = %i\n", card_number, ni6070_comedi_cmd[card_number].flags);

	printk("comedi_cmd[%d].start_src = %i\n", card_number, ni6070_comedi_cmd[card_number].start_src);
	printk("comedi_cmd[%d].start_arg = %i\n", card_number, ni6070_comedi_cmd[card_number].start_arg);

	printk("comedi_cmd[%d].scan_begin_src = %i\n", card_number, ni6070_comedi_cmd[card_number].scan_begin_src);
	printk("comedi_cmd[%d].scan_begin_arg = %i\n", card_number, ni6070_comedi_cmd[card_number].scan_begin_arg);
	
	printk("comedi_cmd[%d].convert_src = %i\n", card_number, ni6070_comedi_cmd[card_number].convert_src);
	printk("comedi_cmd[%d].convert_arg = %i\n", card_number, ni6070_comedi_cmd[card_number].convert_arg);

	printk("comedi_cmd[%d].scan_end_src = %i\n", card_number, ni6070_comedi_cmd[card_number].scan_end_src);
	printk("comedi_cmd[%d].scan_end_arg = %i\n", card_number, ni6070_comedi_cmd[card_number].scan_end_arg);

	printk("comedi_cmd[%d].stop_src = %i\n", card_number, ni6070_comedi_cmd[card_number].stop_src);
	printk("comedi_cmd[%d].stop_arg = %i\n", card_number, ni6070_comedi_cmd[card_number].stop_arg);

	printk("comedi_cmd[%d].chanlist_len = %i\n", card_number, ni6070_comedi_cmd[card_number].chanlist_len);
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
}

void find_spike_end(SpikeEnd *spike_end, RecordingData *filtered_recording_data, int mwa, int mwa_chan)
{
	RecordingDataChanBuff	*filtered_recording_data_chan_buff;
	int previous_acquisition_time_cntr =0; 
	int idx, start_idx, end_idx, min_idx, i, spike_end_idx, handle_for_peak_idx;
	float amplitude_thres, min;
	bool *in_spike;

	amplitude_thres = spike_end->amplitude_thres[mwa][mwa_chan];
	if (amplitude_thres == 0.0)
	{
		spike_end->search_idx_start[mwa][mwa_chan] = filtered_recording_data->buff_idx_write[mwa][mwa_chan];
		return;
	}
	
	filtered_recording_data_chan_buff = &(filtered_recording_data->recording_data_buff[mwa][mwa_chan]);
	start_idx = spike_end->search_idx_start[mwa][mwa_chan];
	end_idx = filtered_recording_data->buff_idx_write[mwa][mwa_chan];
	in_spike = &(spike_end->in_spike[mwa][mwa_chan]);

	idx = start_idx;

	while (idx != end_idx)
	{	
		previous_acquisition_time_cntr++;
		
		if (((*filtered_recording_data_chan_buff)[idx] <  amplitude_thres) && (!(*in_spike)))
		{
			*in_spike = 1;
		}
		if (((*filtered_recording_data_chan_buff)[idx] >  amplitude_thres) && (*in_spike))
		{
			*in_spike = 0;
			min = (*filtered_recording_data_chan_buff)[idx];
			min_idx = idx;
			for (i=0; i<20; i++)
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
				
			
	
			//   Write spike end into shared_memory->spike_end
			spike_end->spike_end_buff[spike_end->buff_idx_write].recording_data_buff_idx = spike_end_idx;
			spike_end->spike_end_buff[spike_end->buff_idx_write].mwa = mwa;
			spike_end->spike_end_buff[spike_end->buff_idx_write].chan = mwa_chan;
			spike_end->spike_end_buff[spike_end->buff_idx_write].peak_time = previous_time_ns + (previous_acquisition_time_cntr * SAMPLING_INTERVAL);  // SAMPLING_INTERVAL = 25000 nanoseconds
			(spike_end->buff_idx_write)++;
			if (spike_end->buff_idx_write == SPIKE_END_DATA_BUFF_SIZE)
				spike_end->buff_idx_write = 0;
			spike_end_buff_control_cntr++;		// to check if the buffer gets full in one rt task period
		}
		idx ++;
		if (idx ==	RECORDING_DATA_BUFF_SIZE)
			idx = 0;
	}											
	spike_end->search_idx_start[mwa][mwa_chan] = end_idx;	
}


void template_matching(void)
{
	RecordingData		*filtered_recording_data;
	SpikeEndHandling 	*spike_end_handling;	
	SpikeEnd			*spike_end;
	
	SpikeEndHandlingBuff 	*spike_end_handling_buff;	
	SpikeEndBuff			*spike_end_buff;	

	int i, j;	
	int idx;
	int spike_end_handling_buff_start_idx, spike_end_handling_buff_end_idx;
	int spike_end_buff_start_idx, spike_end_buff_end_idx;

	int mwa, chan, filtered_recording_data_buff_idx;
	TimeStamp	peak_time;
	
	filtered_recording_data = &shared_memory->filtered_recording_data;
	spike_end_handling = &shared_memory->spike_end_handling;
	spike_end_handling_buff = &(spike_end_handling->spike_end_handling_buff);

	// Handle spike_end_handling_buffer
	
 	spike_end_handling_buff_start_idx = spike_end_handling->buff_start_idx;
	spike_end_handling_buff_end_idx = spike_end_handling->buff_write_idx;

	idx = spike_end_handling_buff_start_idx;

	while (idx != spike_end_handling_buff_end_idx)
	{
		mwa = (*spike_end_handling_buff)[idx].mwa;
		chan = (*spike_end_handling_buff)[idx].chan;
		filtered_recording_data_buff_idx = (*spike_end_handling_buff)[idx].recording_data_buff_idx;
		peak_time = (*spike_end_handling_buff)[idx].peak_time;		
		
		if (is_index_between_indexes(spike_end_handling->filtered_recording_data_prev_idx[mwa][chan], filtered_recording_data->buff_idx_write[mwa][chan], filtered_recording_data_buff_idx))
		{
			run_template_matching(filtered_recording_data, mwa, chan, filtered_recording_data_buff_idx, peak_time);
			spike_end_handling->buff_start_idx++;
			if (spike_end_handling->buff_start_idx == SPIKE_END_HANDLING_DATA_BUFF_SIZE)
				spike_end_handling->buff_start_idx = 0;
		}
		else
		{
			(*spike_end_handling_buff)[spike_end_handling->buff_write_idx].mwa = mwa;		// push to the end off buffer to be handled later on
			(*spike_end_handling_buff)[spike_end_handling->buff_write_idx].chan = chan;
			(*spike_end_handling_buff)[spike_end_handling->buff_write_idx].recording_data_buff_idx = filtered_recording_data_buff_idx;
			(*spike_end_handling_buff)[spike_end_handling->buff_write_idx].peak_time = peak_time;
			spike_end_handling->buff_write_idx++;
			if (spike_end_handling->buff_write_idx == SPIKE_END_HANDLING_DATA_BUFF_SIZE)
				spike_end_handling->buff_write_idx = 0;
			spike_end_handling->buff_start_idx++;
			if (spike_end_handling->buff_start_idx == SPIKE_END_HANDLING_DATA_BUFF_SIZE)
				spike_end_handling->buff_start_idx = 0;				
 		}
		idx ++;
		if (idx ==	SPIKE_END_HANDLING_DATA_BUFF_SIZE)
			idx = 0;		
	}


	// Handle spike_end_buffer

	spike_end = &shared_memory->spike_end;
	spike_end_buff = &(spike_end->spike_end_buff);
	
	spike_end_buff_start_idx = spike_end_handling->spike_end_buff_prev_idx;
	spike_end_buff_end_idx = spike_end->buff_idx_write;

	idx = spike_end_buff_start_idx;
	while (idx != spike_end_buff_end_idx)
	{	
		mwa = (*spike_end_buff)[idx].mwa;
		chan = (*spike_end_buff)[idx].chan;
		filtered_recording_data_buff_idx = (*spike_end_buff)[idx].recording_data_buff_idx;
		peak_time = (*spike_end_buff)[idx].peak_time;
			
		if (is_index_between_indexes(spike_end_handling->filtered_recording_data_prev_idx[mwa][chan], filtered_recording_data->buff_idx_write[mwa][chan], filtered_recording_data_buff_idx))
		{
			run_template_matching(filtered_recording_data, mwa, chan, filtered_recording_data_buff_idx, peak_time);
		}
		else
		{
			(*spike_end_handling_buff)[spike_end_handling->buff_write_idx].mwa = mwa;		// push to the end off buffer to be handled later on
			(*spike_end_handling_buff)[spike_end_handling->buff_write_idx].chan = chan;
			(*spike_end_handling_buff)[spike_end_handling->buff_write_idx].recording_data_buff_idx = filtered_recording_data_buff_idx;
			(*spike_end_handling_buff)[spike_end_handling->buff_write_idx].peak_time = peak_time;
			spike_end_handling->buff_write_idx++;
			if (spike_end_handling->buff_write_idx == SPIKE_END_HANDLING_DATA_BUFF_SIZE)
				spike_end_handling->buff_write_idx = 0;
		}
		idx ++;
		if (idx ==	SPIKE_END_DATA_BUFF_SIZE)
			idx = 0;
	}
	for (i=0; i<MAX_NUM_OF_MWA; i++)
	{
		for (j=0; j<MAX_NUM_OF_CHAN_PER_MWA; j++)
		{
			spike_end_handling->filtered_recording_data_prev_idx[i][j] =  filtered_recording_data->buff_idx_write[i][j];
		}
	}	
	spike_end_handling->spike_end_buff_prev_idx = spike_end_buff_end_idx;   // set read index even though spike sorting is off. otherwise template mathing might work through all spike ends saved before enabling spike sorting. 
}


void run_template_matching(RecordingData *filtered_recording_data, int mwa, int chan, int filtered_recording_data_buff_idx, TimeStamp peak_time)
{

	TemplateMatchingData	*template_matching_data;
	TemplateMatchingUnitData *unit_template_data;	
	RecordingDataChanBuff	*filtered_recording_data_chan_buff;
	SpikeTimeStamp 		*spike_time_stamp;

		
	double g_x[MAX_NUM_OF_UNIT_PER_CHAN];
	double diff[MAX_NUM_OF_UNIT_PER_CHAN][NUM_OF_SAMP_PER_SPIKE];
	double diff_temporary[MAX_NUM_OF_UNIT_PER_CHAN][NUM_OF_SAMP_PER_SPIKE];
	double exponent[MAX_NUM_OF_UNIT_PER_CHAN];	
	double probabl[MAX_NUM_OF_UNIT_PER_CHAN];
	double greatest;	
	int i, j, unit, greatest_idx;

	filtered_recording_data_chan_buff = &(filtered_recording_data->recording_data_buff[mwa][chan]);
	template_matching_data = &shared_memory->template_matching_data;
	spike_time_stamp = &shared_memory->spike_time_stamp;

	greatest = -DBL_MAX;
	greatest_idx = MAX_NUM_OF_UNIT_PER_CHAN;   // If doesnt match any one it will be classified as unsorted (MAX_NUM_OF_UNIT_PER_CHAN)

	for (unit = 0; unit <MAX_NUM_OF_UNIT_PER_CHAN; unit++)
	{
		unit_template_data =  &(*template_matching_data)[mwa][chan][unit];
		if (unit_template_data->sorting_on)
		{
			g_x[unit] = 0.0;
			for (i=0; i<NUM_OF_SAMP_PER_SPIKE; i++)
			{
				{
					if (filtered_recording_data_buff_idx < i)   //  spike_end_idx_in_filtered_recording - i  < 0
						diff[unit][i] = (*filtered_recording_data_chan_buff)[filtered_recording_data_buff_idx-i+RECORDING_DATA_BUFF_SIZE] - unit_template_data->template[NUM_OF_SAMP_PER_SPIKE-i-1];
					else
						diff[unit][i] = (*filtered_recording_data_chan_buff)[filtered_recording_data_buff_idx-i] - unit_template_data->template[NUM_OF_SAMP_PER_SPIKE-i-1];
				}
			}
			for (i=0; i <NUM_OF_SAMP_PER_SPIKE;i++)
			{
				diff_temporary[unit][i] = 0;
			}
			for (i=0; i<NUM_OF_SAMP_PER_SPIKE; i++)
			{
				for (j=0; j<NUM_OF_SAMP_PER_SPIKE; j++)
				{
					diff_temporary[unit][i] = diff_temporary[unit][i] + (diff[unit][j] * unit_template_data->inv_S[i][j]);
				}
			}
			for (i=0; i<NUM_OF_SAMP_PER_SPIKE; i++)
			{
				g_x[unit] = g_x[unit] + (diff_temporary[unit][i] * diff[unit][i]);
			}
			exponent[unit] = exp((-0.5)*g_x[unit]);
			probabl[unit] = (1.06488319787324016356e-12/unit_template_data->sqrt_det_S)*exponent[unit];       //   ( 1/ (   ((2*pi)^(d/2)) * (det_S^(1/2)) ) * exp( (-1/2) * (x-u)' * (S^ (-1)) - (x-u) )   d= 30
			probabl[unit] = (probabl[unit] + 1) - 1;    // to check DBL_EPSILON 
			g_x[unit] = 0 - (unit_template_data->log_det_S) - (g_x[unit]);	
			
			if ((g_x[unit] > greatest) && (probabl[unit] > unit_template_data->probability_thres))    // assign spike to a unit.
			{
				greatest = g_x[unit];
				greatest_idx = unit;
			}	
		}
	}
	

	//   Write spike time stamp into shared_memory->spike_time_stamp
	spike_time_stamp->spike_timestamp_buff[spike_time_stamp->buff_idx_write].peak_time = peak_time;
	spike_time_stamp->spike_timestamp_buff[spike_time_stamp->buff_idx_write].mwa = mwa;
	spike_time_stamp->spike_timestamp_buff[spike_time_stamp->buff_idx_write].channel = chan;
	spike_time_stamp->spike_timestamp_buff[spike_time_stamp->buff_idx_write].unit = greatest_idx;
	spike_time_stamp->spike_timestamp_buff[spike_time_stamp->buff_idx_write].recording_data_buff_idx = filtered_recording_data_buff_idx;
	spike_time_stamp->buff_idx_write++;
	if (spike_time_stamp->buff_idx_write == SPIKE_TIMESTAMP_BUFF_SIZE)
		spike_time_stamp->buff_idx_write  = 0;	
	spike_timestamp_buff_control_cntr++;		// to check if the buffer gets full in one rt task period
		
}

void print_buffer_warning_and_errors(void)
{
	if ((SPIKE_END_DATA_BUFF_SIZE - spike_end_buff_control_cntr) < 100)
	{
		printk("------------------------------------------------------\n");
		printk("------------   WARNING  !!!  -----------------\n");
		printk("---- Spike End Buffer is getting full ----\n");
		printk("---- Spike End Buffer is getting full ----\n");				
		printk("--Latest # of detected spikes is %d---\n", spike_end_buff_control_cntr);	
		printk("-------Spike End buffer size  is %d------\n", SPIKE_END_DATA_BUFF_SIZE);
		printk("------------------------------------------------------\n");					
	}  
	else if (SPIKE_END_DATA_BUFF_SIZE <= spike_end_buff_control_cntr)
	{
		printk("------------------------------------------------------\n");
		printk("----------------   ERROR !!!  -----------------\n");
		printk("---- Spike End Buffer is full ---------------\n");
		printk("---- Spike End Buffer is full ---------------\n");				
		printk("--Latest # of detected spikes is %d ---\n", spike_end_buff_control_cntr);	
		printk("-------Spike End buffer size  is %d------\n", SPIKE_END_DATA_BUFF_SIZE);
		printk("------------------------------------------------------\n");
		rt_task_stay_alive = 0;	// kill task			
	}  
	if ((SPIKE_TIMESTAMP_BUFF_SIZE - spike_end_buff_control_cntr) < 100)
	{
		printk("--------------------------------------------------------\n");
		printk("------------   WARNING  !!!  -------------------\n");
		printk("---- Spike Timestamp Buffer is getting full ------\n");
		printk("---- Spike Timestamp Buffer is getting full ------\n");				
		printk("--Latest # of Spike Timestamp is %d----\n", spike_timestamp_buff_control_cntr);	
		printk("---Spike Timestamp buffer size  is %d--\n", SPIKE_TIMESTAMP_BUFF_SIZE);
		printk("--------------------------------------------------------\n");					
	}  
	else if (SPIKE_TIMESTAMP_BUFF_SIZE <= spike_timestamp_buff_control_cntr)
	{
		printk("---------------------------------------------------------\n");
		printk("----------------   ERROR  !!!  --------------------\n");
		printk("---- Spike Timestamp Buffer is full -------\n");
		printk("---- Spike Timestamp Buffer is full -------\n");				
		printk("--Latest # of Spike Timestamp is %d----\n", spike_timestamp_buff_control_cntr);	
		printk("---Spike Timestamp buffer size  is %d--\n", SPIKE_TIMESTAMP_BUFF_SIZE);
		printk("--------------------------------------------------------\n");
		rt_task_stay_alive = 0;	// kill task			
	}	
	spike_end_buff_control_cntr = 0;
	spike_timestamp_buff_control_cntr = 0;
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
