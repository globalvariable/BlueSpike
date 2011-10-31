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

#define TICK_PERIOD 1000000
#define TASK_PRIORITY 1
#define STACK_SIZE 10000
#define KERNELSPIKE_CPUID 2

//	Variables

RT_TASK rt_task0;
TimeStamp current_time_ns;
TimeStamp previous_time_ns;

comedi_t* ni6070_comedi_dev[MAX_NUM_OF_DAQ_CARD];
comedi_cmd ni6070_comedi_cmd[MAX_NUM_OF_DAQ_CARD];
unsigned ni6070_comedi_chanlist[MAX_NUM_OF_DAQ_CARD][MAX_NUM_OF_CHANNEL_PER_DAQ_CARD];
void *comedi_map_ptr[MAX_NUM_OF_DAQ_CARD];
int comedi_buff_size[MAX_NUM_OF_DAQ_CARD];

//	Functions
void rt_handler(int t);
int ni6070_comedi_configure(int card_number);
void print_cmd(int card_number);
void filter_recording_data( RecordingData *recording_data, RecordingData *highpass_filtered_recording_data, RecordingData *filtered_recording_data, int end_idx, int mwa, int mwa_chan, bool *highpass_150Hz_on, bool *highpass_400Hz_on, bool *lowpass_8KHz_on);
void find_spike_end(SpikeEnd *spike_end, RecordingData *filtered_recording_data, int mwa, int mwa_chan);
void template_matching(RecordingData *filtered_recording_data, SpikeEnd *spike_end, SpikeTimeStamp *spike_time_stamp, TemplateMatchingData *template_matching_data);
bool is_index_between_indexes(int start_idx, int end_idx, int this_idx);
void run_template_matching(RecordingData *filtered_recording_data, SpikeEnd *spike_end, SpikeTimeStamp *spike_time_stamp, TemplateMatchingData *template_matching_data, int spike_end_buffer_index_to_read);
