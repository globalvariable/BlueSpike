#define KERNELSPIKE_H

#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <rtai.h>
#include <rtai_sched.h>
#include <rtai_comedi.h>
#include <rtai_shm.h>
#include <rtai_nam2num.h>
#include <rtai_math.h>
#include <float.h>

#include <linux/comedi.h>
#include <linux/comedilib.h>

#include "../SharedMemory.h"
#include "SpikeEnd.h"
#include "SpikeEndHandling.h"

#define TICK_PERIOD 1000000
#define TASK_PRIORITY 1
#define STACK_SIZE 10000
#define KERNELSPIKE_CPUID 2
#define KERNELSPIKE_RUN_TIME_LIMIT 18446744000000000000ULL

//	Variables

RT_TASK rt_task0;

TimeStamp current_time_ns;
TimeStamp previous_time_ns;

int spike_end_buff_control_cntr;     // to check if the buffer gets full in one rt task period
int spike_timestamp_buff_control_cntr;    // to check if the buffer gets full in one rt task period

int rt_task_stay_alive;

comedi_t* ni6070_comedi_dev[MAX_NUM_OF_DAQ_CARD];
comedi_cmd ni6070_comedi_cmd[MAX_NUM_OF_DAQ_CARD];
unsigned ni6070_comedi_chanlist[MAX_NUM_OF_DAQ_CARD][MAX_NUM_OF_CHANNEL_PER_DAQ_CARD];
void *comedi_map_ptr[MAX_NUM_OF_DAQ_CARD];
int comedi_buff_size[MAX_NUM_OF_DAQ_CARD];

RecordingData			highpass_filtered_recording_data;
SpikeEnd				spike_end;
SpikeEndHandling		spike_end_handling;	
	
//	Functions
void rt_handler(int t);
int ni6070_comedi_configure(int card_number);
void print_cmd(int card_number);
void filter_recording_data( RecordingData *recording_data, RecordingData *filtered_recording_data, int mwa, int mwa_chan, bool highpass_150Hz_on, bool highpass_400Hz_on, bool lowpass_8KHz_on);
void find_spike_end(RecordingData *filtered_recording_data, int mwa, int mwa_chan);
void template_matching(void);
void run_template_matching(RecordingData *filtered_recording_data, int mwa, int chan, int filtered_recording_data_buff_idx, TimeStamp peak_time);
void print_warning_and_errors(void);
bool is_index_between_indexes(int start_idx, int end_idx, int this_idx);
