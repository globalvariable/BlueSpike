#ifndef KERNELSPIKE_H
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

#include "../DaqMwaData.h"
#include "../RecordingData.h"
#include "../SpikeThresholding.h"
#include "../BlueSpikeTimeStamp.h"
#include "../SpikeTimeStamp.h"
#include "../TemplateMatchingData.h"
#include "../FilterCtrl.h"
#include "../RtTasksData.h"
#include "SpikeEndHandling.h"
#include "../Library/Messages/DaqCon2KrnlSpk.h"
#include "../Library/Messages/FiltCtrl2KrnlSpk.h"
#include "../Library/Messages/SpkThres2KrnlSpk.h"
#include "../Library/Messages/TempMat2KrnlSpk.h"
#include "KernelSpikeSharedMem.h"

#define KERNELSPIKE_RUN_TIME_LIMIT 18446744000000000000ULL

//	Variables

RT_TASK rt_task0;

TimeStamp current_time_ns;
TimeStamp previous_time_ns;

int spike_end_handling_buff_control_cntr;     // to check if the buffer gets full in one rt task period
int blue_spike_time_stamp_buff_control_cntr;    // to check if the buffer gets full in one rt task period

int rt_task_stay_alive;

int daq_cards_on;

comedi_t* ni6070_comedi_dev[MAX_NUM_OF_DAQ_CARD];
comedi_cmd ni6070_comedi_cmd[MAX_NUM_OF_DAQ_CARD];
unsigned ni6070_comedi_chanlist[MAX_NUM_OF_DAQ_CARD][MAX_NUM_OF_CHANNEL_PER_DAQ_CARD];
void *comedi_map_ptr[MAX_NUM_OF_DAQ_CARD];
int comedi_buff_size[MAX_NUM_OF_DAQ_CARD];

RecordingData		highpass_filtered_recording_data;
SpikeEndHandling		spike_end_handling;	
	
//	Functions
void rt_handler(long int t);
int ni6070_comedi_configure(int card_number);
void print_cmd(int card_number);
void filter_recording_data(int mwa, int mwa_chan, bool highpass_150Hz_on, bool highpass_400Hz_on, bool lowpass_8KHz_on);
void find_spike_end(int mwa, int mwa_chan);
void handle_spike_end_handling_buffer(void);
void run_template_matching(int mwa, int chan, int filtered_recording_data_buff_idx, TimeStamp peak_time);
void print_warning_and_errors(void);
bool is_index_between_indexes(int start_idx, int end_idx, int this_idx);
int open_daq_cards(void);
void close_daq_cards(void);
int handle_daq_cards(void);
void evaluate_period_run_time(unsigned int curr_time);
void evaluate_jitter(unsigned int period_occured);
#endif
