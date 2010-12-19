/***************************************************************************
                          KernelSpike.c  -  description
                             -------------------
    copyright            : (C) 2010 by Mehmet Kocaturk
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


#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <rtai.h>
#include <rtai_sched.h>
#include <rtai_comedi.h>
#include <rtai_shm.h>
#include <rtai_nam2num.h>
#include <rtai_math.h>

#include <linux/comedi.h>
#include <linux/comedilib.h>

#define TICK_PERIOD 1000000
#define TASK_PRIORITY 1
#define STACK_SIZE 10000
#define COMEDI_SUBDEVICE_AI 0
#define NUM_OF_CHAN 16 
#define SAMPLING_INTERVAL 25000
#define NUM_OF_SAMP_IN_BUFF 40000  
#define SHMSIZ  sizeof(buff_data_struct)
#define BUFFNAME  "SHBUFF"

#define SPIKE_MIN_END_SAMP_NUM   40
#define NUM_OF_SAMP_PER_SPIKE 60
#define NUM_OF_TEMP_PER_CHAN 3

static RT_TASK rt_task0;
static RTIME tick_period;
static comedi_cmd ni6070_comedi_cmd;
static comedi_t* ni6070_comedi_dev;
static unsigned ni6070_comedi_chanlist[NUM_OF_CHAN];
static int ni6070_comedi_configure(void);
static int ni6070_comedi_init(void);
static void dsp(void);
static void threshold_spikes(void);
static void template_matching(void);
void *ptr = NULL;
static int size, chan_number;
static int front, back;


typedef struct{
	union
	{
		struct{
			unsigned Bit0:1;
			unsigned Bit1:1;
			unsigned Bit2:1;
			unsigned Bit3:1;
			unsigned Bit4:1;
			unsigned Bit5:1;
			unsigned Bit6:1;						
			unsigned Bit7:1;
		};
		unsigned AllCommand;
	} Command;
	union
	{
		struct{
			unsigned Bit0:1;
			unsigned Bit1:1;
			unsigned Bit2:1;
			unsigned Bit3:1;
			unsigned Bit4:1;
			unsigned Bit5:1;
			unsigned Bit6:1;						
			unsigned Bit7:1;
		};
		unsigned AllStatus;
	} Status;	 	
} ExpEnv; 


typedef struct sample_data
{
	float data[NUM_OF_CHAN];
} scan_data;

typedef struct spk_data
{
	int data[NUM_OF_CHAN];
} spike_data;

typedef struct templ_data
{
	float template[NUM_OF_CHAN][NUM_OF_TEMP_PER_CHAN][NUM_OF_SAMP_PER_SPIKE];
	float diff_thres[NUM_OF_CHAN][NUM_OF_TEMP_PER_CHAN];
} template_matching_data;

typedef struct buff_data 
{
	int scan_number_write;
	int scan_number_read;
	scan_data scan[NUM_OF_SAMP_IN_BUFF];
	scan_data filtered_scan[NUM_OF_SAMP_IN_BUFF];
	spike_data spike_peak[NUM_OF_SAMP_IN_BUFF];
	spike_data spike_end[NUM_OF_SAMP_IN_BUFF];	
	float Threshold[NUM_OF_CHAN];
	bool in_spike[NUM_OF_CHAN];	
	bool filter_on;
	ExpEnv Environment[NUM_OF_SAMP_IN_BUFF];
	ExpEnv Curr_Environment;
	template_matching_data spike_template; 	
	bool sorting_on;
	spike_data sorted_spike_data[NUM_OF_SAMP_IN_BUFF];
} buff_data_struct;

static buff_data_struct *buff;

static void fun(int t)
{
	while (1) 
	{
		int ret,i, chan_iter;
		int num_byte;
		rt_task_wait_period();
		comedi_poll(ni6070_comedi_dev, COMEDI_SUBDEVICE_AI);
		num_byte= comedi_get_buffer_contents(ni6070_comedi_dev, COMEDI_SUBDEVICE_AI);
		front = front + num_byte;
		if (front >= size)
			front = front - size;

		if(num_byte == 0)
		{
			printk("num_byte = 0\n");
			continue;
		}

		for(i = 0; i < num_byte; i += sizeof(sampl_t))
		{			
			if ((ptr+back+i) >= (ptr+size))
			{
				buff->scan[buff->scan_number_write].data[chan_number] = ((*(sampl_t *)(ptr + back + i - size)) - 2048.0) ;
			}	
			else
			{
				buff->scan[buff->scan_number_write].data[chan_number] = ((*(sampl_t *)(ptr + back + i)) - 2048.0);
			}
			chan_number++;
			if(chan_number==NUM_OF_CHAN)
			{
				buff->Environment[buff->scan_number_write] = buff->Curr_Environment;
				if (buff->filter_on)
				{
					dsp();
					threshold_spikes();
					if(buff->sorting_on)
					{
						template_matching();
					}
					else
					{
						for (chan_iter = 0; chan_iter < NUM_OF_CHAN; chan_iter++)
						{
							buff->sorted_spike_data[buff->scan_number_write].data[chan_iter]=0;
						}
					}
				}
				else
				{
					for (chan_iter = 0; chan_iter < NUM_OF_CHAN; chan_iter++)
					{
						buff->spike_peak[buff->scan_number_write].data[chan_iter]=0;
						if ((buff->scan_number_write+SPIKE_MIN_END_SAMP_NUM) >= NUM_OF_SAMP_IN_BUFF)
							buff->spike_end[buff->scan_number_write+SPIKE_MIN_END_SAMP_NUM-NUM_OF_SAMP_IN_BUFF].data[chan_iter] = 0;
						else
							buff->spike_end[buff->scan_number_write+SPIKE_MIN_END_SAMP_NUM].data[chan_iter] = 0;						
					}
				}
				chan_number=0;
				(buff->scan_number_write)++;
				buff->scan_number_read = buff->scan_number_write;
				if ((buff->scan_number_write) == NUM_OF_SAMP_IN_BUFF) 
				{
					(buff->scan_number_write) = 0;
				}
			}
			
		}


		ret = comedi_mark_buffer_read(ni6070_comedi_dev, COMEDI_SUBDEVICE_AI, num_byte);
		if(ret < 0){
			printk("comedi_mark_buffer_read");
			break;
		}
		back = front;
	}

}



int __init xinit_module(void)
{
	int ret;

	buff = (buff_data_struct*)rtai_kmalloc(nam2num(BUFFNAME), SHMSIZ);
	if (buff == NULL)
		return -ENOMEM;
	memset(buff, 0, SHMSIZ);
	ni6070_comedi_init();
	ret = comedi_map(ni6070_comedi_dev, COMEDI_SUBDEVICE_AI, &ptr);
	printk("comedi_map ret: %d, ptr: %d\n", ret, (int)ptr);
	size = comedi_get_buffer_size(ni6070_comedi_dev, COMEDI_SUBDEVICE_AI);
	printk("buffer size is %d\n", size);
        printk("sizeof(buff_data_struct) : % d\n", sizeof(buff_data_struct));
	ni6070_comedi_configure();
	rt_set_periodic_mode();
	rt_task_init_cpuid(&rt_task0, fun, 1, STACK_SIZE, TASK_PRIORITY, 1, 0,7);
	tick_period = start_rt_timer(nano2count(TICK_PERIOD));
	rt_task_make_periodic(&rt_task0, rt_get_time() + tick_period, tick_period);

	return 0;
}

void __exit xcleanup_module(void)
{
	comedi_cancel(ni6070_comedi_dev, COMEDI_SUBDEVICE_AI);
	comedi_close(ni6070_comedi_dev);
	stop_rt_timer();
	rt_task_delete(&rt_task0);
    	rtai_kfree(nam2num(BUFFNAME));
	return;
}

module_init(xinit_module);
module_exit(xcleanup_module);
MODULE_LICENSE("GPL");

static void print_cmd(void)
{
	printk("comedi_cmd.subdev = %i\n", ni6070_comedi_cmd.subdev);
	printk("comedi_cmd.flags = %i\n", ni6070_comedi_cmd.flags);

	printk("comedi_cmd.start_src = %i\n", ni6070_comedi_cmd.start_src);
	printk("comedi_cmd.start_arg = %i\n", ni6070_comedi_cmd.start_arg);

	printk("comedi_cmd.scan_begin_src = %i\n", ni6070_comedi_cmd.scan_begin_src);
	printk("comedi_cmd.scan_begin_arg = %i\n", ni6070_comedi_cmd.scan_begin_arg);
	
	printk("comedi_cmd.convert_src = %i\n", ni6070_comedi_cmd.convert_src);
	printk("comedi_cmd.convert_arg = %i\n", ni6070_comedi_cmd.convert_arg);

	printk("comedi_cmd.scan_end_src = %i\n", ni6070_comedi_cmd.scan_end_src);
	printk("comedi_cmd.scan_end_arg = %i\n", ni6070_comedi_cmd.scan_end_arg);

	printk("comedi_cmd.stop_src = %i\n", ni6070_comedi_cmd.stop_src);
	printk("comedi_cmd.stop_arg = %i\n", ni6070_comedi_cmd.stop_arg);

	printk("comedi_cmd.chanlist_len = %i\n", ni6070_comedi_cmd.chanlist_len);
}


static int ni6070_comedi_configure(void)
{
	int i;
	memset(&ni6070_comedi_cmd, 0, sizeof(comedi_cmd));
	memset(&ni6070_comedi_chanlist, 0, sizeof(unsigned)*NUM_OF_CHAN);

	ni6070_comedi_cmd.subdev = COMEDI_SUBDEVICE_AI;
	ni6070_comedi_cmd.flags = 0;

	ni6070_comedi_cmd.start_src = TRIG_NOW;
	ni6070_comedi_cmd.start_arg = 0;

	ni6070_comedi_cmd.scan_begin_src = TRIG_TIMER;
	ni6070_comedi_cmd.scan_begin_arg = SAMPLING_INTERVAL;
	
	ni6070_comedi_cmd.convert_src = TRIG_TIMER;
	ni6070_comedi_cmd.convert_arg = 1000;

	ni6070_comedi_cmd.scan_end_src = TRIG_COUNT;
	ni6070_comedi_cmd.scan_end_arg = NUM_OF_CHAN;

	ni6070_comedi_cmd.stop_src = TRIG_NONE;
	ni6070_comedi_cmd.stop_arg = 0;

	ni6070_comedi_cmd.chanlist = ni6070_comedi_chanlist;
	ni6070_comedi_cmd.chanlist_len = NUM_OF_CHAN;

	for (i = 0 ; i < NUM_OF_CHAN ; i++)
	{
		ni6070_comedi_chanlist[i] = CR_PACK(i, 4, AREF_GROUND);
	}

	ni6070_comedi_cmd.data = NULL;
	ni6070_comedi_cmd.data_len = 0;

	print_cmd();
	printk("test 1: %i\n", comedi_command_test(ni6070_comedi_dev,&ni6070_comedi_cmd));
	print_cmd();
	printk("test 2: %i\n", comedi_command_test(ni6070_comedi_dev,&ni6070_comedi_cmd));
	print_cmd();

	return comedi_command(ni6070_comedi_dev,&ni6070_comedi_cmd);
}


static int ni6070_comedi_init(void)
{
	ni6070_comedi_dev = comedi_open("/dev/comedi0");
	return 0;
}

static void dsp(void)
{
	int i;

	if (buff->scan_number_write == 0)
	{
		for (i=0;i<NUM_OF_CHAN; i++)
		{
			buff->filtered_scan[buff->scan_number_write].data[i]=   (0.9835 * buff->scan[0].data[i]) +
										(-1.967 * buff->scan[39999].data[i]) +
										(0.9835 * buff->scan[39998].data[i]) - 
										(-1.9667 * buff->filtered_scan[39999].data[i])-
										(0.9672 * buff->filtered_scan[39998].data[i]) ;
 		}		
	}

	else if (buff->scan_number_write == 1)
	{
		for (i=0;i<NUM_OF_CHAN; i++)
		{
			buff->filtered_scan[buff->scan_number_write].data[i]=   (0.9835 * buff->scan[1].data[i]) +
										(-1.967 * buff->scan[0].data[i]) +
										(0.9835 * buff->scan[39999].data[i]) - 
										(-1.9667 * buff->filtered_scan[0].data[i])-
										(0.9672 * buff->filtered_scan[39999].data[i]) ;
 		}
	}
	else 
	{
		for (i=0;i<NUM_OF_CHAN; i++)
		{

			buff->filtered_scan[buff->scan_number_write].data[i]=   (0.9835 * buff->scan[buff->scan_number_write].data[i]) +
										(-1.967 * buff->scan[buff->scan_number_write-1].data[i]) +
										(0.9835 * buff->scan[buff->scan_number_write-2].data[i]) - 
										(-1.9667 * buff->filtered_scan[buff->scan_number_write-1].data[i]) -
										(0.9672 * buff->filtered_scan[buff->scan_number_write-2].data[i]) ;

		}		
	}
}

static void threshold_spikes(void)
{
	int i, idx;
	int min_idx, min;
	for (i=0;i<NUM_OF_CHAN; i++)
	{
		buff->spike_peak[buff->scan_number_write].data[i] = 0;
		if ((buff->scan_number_write+SPIKE_MIN_END_SAMP_NUM) >= NUM_OF_SAMP_IN_BUFF)
			buff->spike_end[buff->scan_number_write+SPIKE_MIN_END_SAMP_NUM-NUM_OF_SAMP_IN_BUFF].data[i] = 0;
		else
			buff->spike_end[buff->scan_number_write+SPIKE_MIN_END_SAMP_NUM].data[i] = 0;
		if (buff->Threshold[i] == 0.0)
			continue;
		if ((buff->filtered_scan[buff->scan_number_write].data[i] <  buff->Threshold[i]) && (!(buff->in_spike[i])))
		{

			buff->in_spike[i] = 1;
		}
		if ((buff->filtered_scan[buff->scan_number_write].data[i] >  buff->Threshold[i]) && (buff->in_spike[i]))
		{
			buff->in_spike[i] = 0;
			min = buff->filtered_scan[buff->scan_number_write].data[i];
			for (idx = 0; idx < 20; idx++)
			{
				if (buff->scan_number_write < idx)
				{
					if (buff->filtered_scan[buff->scan_number_write-idx+NUM_OF_SAMP_IN_BUFF].data[i] < min)
					{
						min =  buff->filtered_scan[buff->scan_number_write-idx+NUM_OF_SAMP_IN_BUFF].data[i];
						min_idx = buff->scan_number_write-idx+NUM_OF_SAMP_IN_BUFF;
					}									
				}
				else
				{					
					if (buff->filtered_scan[buff->scan_number_write-idx].data[i] < min)
					{
						min =  buff->filtered_scan[buff->scan_number_write-idx].data[i];
						min_idx = buff->scan_number_write-idx;
					}
				}
			}
			buff->spike_peak[min_idx].data[i] = 1;
		if ((min_idx+SPIKE_MIN_END_SAMP_NUM) >= NUM_OF_SAMP_IN_BUFF)
			buff->spike_end[min_idx+SPIKE_MIN_END_SAMP_NUM-NUM_OF_SAMP_IN_BUFF].data[i] = 1;
		else		
			buff->spike_end[min_idx+SPIKE_MIN_END_SAMP_NUM].data[i] = 1;

		}
 	}
}

static void template_matching(void)
{
	int chan,chan_temp_num,idx;
	float diff[NUM_OF_TEMP_PER_CHAN];
	for (chan=0;chan<NUM_OF_CHAN; chan++)
	{

		if (buff->spike_end[buff->scan_number_write].data[chan] == 1)
		{
			for (chan_temp_num=0;chan_temp_num<NUM_OF_TEMP_PER_CHAN; chan_temp_num++)
			{	
				diff[chan_temp_num] = 0;		
				for (idx=0;idx<NUM_OF_SAMP_PER_SPIKE;idx++)
				{
					if (buff->scan_number_write-idx < 0)
						diff[chan_temp_num] = diff[chan_temp_num] + abs(buff->filtered_scan[buff->scan_number_write-idx+NUM_OF_SAMP_IN_BUFF].data[chan]-buff->spike_template.template[chan][chan_temp_num][NUM_OF_SAMP_PER_SPIKE-idx-1]);
					else
						diff[chan_temp_num] = diff[chan_temp_num] + abs(buff->filtered_scan[buff->scan_number_write-idx].data[chan]-buff->spike_template.template[chan][chan_temp_num][NUM_OF_SAMP_PER_SPIKE-idx-1]);
				}
			}
			if ((diff[0]<buff->spike_template.diff_thres[chan][0]) && (diff[0] < diff [1]) && (diff[0] < diff[2]))
			{
				buff->sorted_spike_data[buff->scan_number_write].data[chan]=1;
			}
			else if ((diff[1]<buff->spike_template.diff_thres[chan][1]) && (diff[1] < diff [0]) && (diff[1] < diff[2]))
			{
				buff->sorted_spike_data[buff->scan_number_write].data[chan]=2;
			}
			else if ((diff[2]<buff->spike_template.diff_thres[chan][2]) && (diff[2] < diff [0]) && (diff[2] < diff[1]))
			{
				buff->sorted_spike_data[buff->scan_number_write].data[chan]=3;
			}
			else
				buff->sorted_spike_data[buff->scan_number_write].data[chan]=0;			
		}
	}
}
