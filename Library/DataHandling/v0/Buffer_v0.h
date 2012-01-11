#ifndef BUFFER_V0_H
#define BUFFER_V0_H

#include "./../../../SharedMemory.h"
#include <unistd.h>

int recording_data_buff_prev_idx[MAX_NUM_OF_MWA][MAX_NUM_OF_CHAN_PER_MWA];
int recording_data_buff_end_idx[MAX_NUM_OF_MWA][MAX_NUM_OF_CHAN_PER_MWA];
int blue_spike_time_stamp_buff_prev_idx;
int blue_spike_time_stamp_buff_end_idx;
int spike_time_stamp_buff_prev_idx;
int spike_time_stamp_buff_end_idx;
int exp_envi_event_buff_prev_idx[MAX_NUM_OF_EXP_ENVI_ITEMS];
int exp_envi_event_buff_end_idx[MAX_NUM_OF_EXP_ENVI_ITEMS];
int exp_envi_command_buff_prev_idx[MAX_NUM_OF_EXP_ENVI_ITEMS];
int exp_envi_command_buff_end_idx[MAX_NUM_OF_EXP_ENVI_ITEMS];
int mov_obj_event_buff_prev_idx[MAX_NUM_OF_MOVING_OBJECTS];
int mov_obj_event_buff_end_idx[MAX_NUM_OF_MOVING_OBJECTS];
int mov_obj_command_buff_prev_idx[MAX_NUM_OF_MOVING_OBJECTS];
int mov_obj_command_buff_end_idx[MAX_NUM_OF_MOVING_OBJECTS];

int recording_data_buff_start_idx[MAX_NUM_OF_MWA][MAX_NUM_OF_CHAN_PER_MWA];
int blue_spike_time_stamp_buff_start_idx;
int spike_time_stamp_buff_start_idx;
int exp_envi_event_buff_start_idx[MAX_NUM_OF_EXP_ENVI_ITEMS];
int exp_envi_command_buff_start_idx[MAX_NUM_OF_EXP_ENVI_ITEMS];
int mov_obj_event_buff_start_idx[MAX_NUM_OF_MOVING_OBJECTS];
int mov_obj_command_buff_start_idx[MAX_NUM_OF_MOVING_OBJECTS];

DaqMwaMap				*daq_mwa_map;
MwaDaqMap				*mwa_daq_map;
RecordingData				*recording_data;
SpikeThresholding			*spike_thresholding;
BlueSpikeTimeStamp 		*blue_spike_time_stamp; 
SpikeTimeStamp 			*spike_time_stamp; 
ExpEnviEventTimeStamp		*exp_envi_event_time_stamp;
ExpEnviCommandTimeStamp 	*exp_envi_command_time_stamp;		
MovingObjEventTimeStamp	*mov_obj_event_time_stamp;
MovingObjCommandTimeStamp	*mov_obj_command_time_stamp;
TemplateMatchingData		*template_matching_data;


//Functions
TimeStamp initialize_buffer_reading_start_indexes_and_time_for_recording_v0 (int num, ...);
void get_buffer_reading_range_indexes_for_recording_v0 (int num, ...);
TimeStamp get_buffer_reading_end_indexes_and_time_for_recording_v0 (int num, ...);

#endif
