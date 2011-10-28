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

#define KERNELSPIKE

#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <rtai.h>
#include <rtai_sched.h>
#include <rtai_comedi.h>
#include <rtai_shm.h>
#include <rtai_nam2num.h>
#include <rtai_math.h>

#include "../SharedMemory.h"


#define TICK_PERIOD 1000000
#define TASK_PRIORITY 1
#define STACK_SIZE 10000

static RT_TASK rt_task0;

static void rt_handler(int t);




static void rt_handler(int t)
{
	int i, j, return_value;
	int while_ctrl = 1;
	int front[MAX_NUM_OF_DAQ_CARD], back[MAX_NUM_OF_DAQ_CARD], num_byte[MAX_NUM_OF_DAQ_CARD], chan_num[MAX_NUM_OF_DAQ_CARD] ;
	RecordingData			*recording_data;
	int *recording_data_write_idx;
	int mwa, mwa_chan;
	DaqMwaMap			*daq_mwa_map;
	recording_data = &shared_memory->recording_data;
	daq_mwa_map = &shared_memory->daq_mwa_map;
	for (i=0; i < MAX_NUM_OF_DAQ_CARD; i++)
	{
		front[i] = 0;
		back[i] = 0;
		chan_num[i] = 0;
	}
	while (while_ctrl) 
	{
		rt_task_wait_period();
		for (i=0; i < MAX_NUM_OF_DAQ_CARD; i++)
		{
			comedi_poll(ni6070_comedi_dev[i], COMEDI_SUBDEVICE_AI);
			num_byte[i] = comedi_get_buffer_contents(ni6070_comedi_dev[i], COMEDI_SUBDEVICE_AI);
			front[i] = front[i] + num_byte[i];
			if (front[i] >= comedi_buff_size[i])
				front[i] = front[i] - comedi_buff_size[i];
			if(num_byte[i] == 0)
			{
				printk("num_byte[%d] = 0\n", i);
				continue;
			}
			
			for(j = 0; j < num_byte[i]; j += sizeof(sampl_t))
			{

					  
				mwa = (*daq_mwa_map)[i][chan_num[i]].mwa;
				mwa_chan = (*daq_mwa_map)[i][chan_num[i]].channel;	
								
				recording_data_write_idx = &(recording_data->buff_idx_write[mwa][mwa_chan]);

				if ((comedi_map_ptr[i]+back[i]+j) >= (comedi_map_ptr[i]+comedi_buff_size[i]))
				{
					recording_data->recording_data_buff[mwa][mwa_chan][*recording_data_write_idx] = ((*(sampl_t *)(comedi_map_ptr[i] + back[i] + j - comedi_buff_size[i])) - 2048.0) ;
				}
				else
				{
					recording_data->recording_data_buff[mwa][mwa_chan][*recording_data_write_idx] = ((*(sampl_t *)(comedi_map_ptr[i] + back[i] + j)) - 2048.0);
				}
				(chan_num[i])++;
				if (chan_num[i] == MAX_NUM_OF_CHANNEL_PER_DAQ_CARD)
					chan_num[i] = 0;
				(*recording_data_write_idx)++;
				if ((*recording_data_write_idx) == RECORDING_DATA_BUFF_SIZE)
					(*recording_data_write_idx) = 0;
			}
			
			return_value = comedi_mark_buffer_read(ni6070_comedi_dev[i], COMEDI_SUBDEVICE_AI, num_byte[i]);
			if(return_value < 0)
			{
				printk("ERROR: comedi_mark_buffer_read");
				while_ctrl = 0;
			}
			back[i] = front[i];
		}
	}
}



int __init xinit_module(void)
{
	int ret;
	int i;
	char path_comedi[100], temp[10];
	RTIME tick_period;

	shared_memory = (SharedMemStruct*)rtai_kmalloc(nam2num(SHARED_MEM_NAME), SHARED_MEM_SIZE);
	if (shared_memory == NULL)
		return -ENOMEM;
	memset(shared_memory, 0, SHARED_MEM_SIZE);
        printk("sizeof(SharedMemStruct) : %d\n", SHARED_MEM_SIZE);


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
		ret = comedi_map(ni6070_comedi_dev[i], COMEDI_SUBDEVICE_AI, &comedi_map_ptr[i]);
		printk("%d th device comedi_map return: %d, ptr: %u\n", i, ret, (unsigned int)comedi_map_ptr[i]);
		comedi_buff_size[i] = comedi_get_buffer_size(ni6070_comedi_dev[i], COMEDI_SUBDEVICE_AI);
		printk("buffer size of %dth device is %d\n", i, comedi_buff_size[i]);
		ni6070_comedi_configure(i);
	}

	rt_set_periodic_mode();
	rt_task_init_cpuid(&rt_task0, rt_handler, 1, STACK_SIZE, TASK_PRIORITY, 1, 0,7);
	tick_period = start_rt_timer(nano2count(TICK_PERIOD));
	rt_task_make_periodic(&rt_task0, rt_get_time() + tick_period, tick_period);

	return 0;
}

void __exit xcleanup_module(void)
{
	int i;
	for (i = 0; i<MAX_NUM_OF_DAQ_CARD; i++)
	{
		comedi_cancel(ni6070_comedi_dev[i], COMEDI_SUBDEVICE_AI);
		comedi_close(ni6070_comedi_dev[i]);
	}
	stop_rt_timer();
	rt_task_delete(&rt_task0);
    	rtai_kfree(nam2num(SHARED_MEM_NAME));
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

	ni6070_comedi_cmd[card_number].chanlist = ni6070_comedi_chanlist[MAX_NUM_OF_DAQ_CARD];
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
