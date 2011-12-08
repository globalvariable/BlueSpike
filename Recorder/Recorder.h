
#include "../SharedMemory.h"

#include <rtai_shm.h>
#include <rtai_nam2num.h>

#include <gtk/gtk.h>

#include "pthread.h"


GtkWidget *main_table;

GtkWidget *btn_select_directory_to_save;
GtkWidget *entry_data_name;

GtkWidget *btn_start_stop_recording;
GtkWidget *btn_delete_last_recording;

GtkWidget *txv_mwa_daq_map_additional;
GtkWidget *txv_recording_additional;
GtkWidget *txv_spike_time_stamp_additional;
GtkWidget *txv_exp_envi_event_additional;
GtkWidget *txv_exp_envi_command_additional;
GtkWidget *txv_mov_obj_event_additional;
GtkWidget *txv_mov_obj_command_additional;

bool start_recording_request;
bool stop_recording_request;
bool recording_ongoing;

TimeStamp recording_start_time_ns;
TimeStamp recording_end_time_ns;

pthread_t recording_thread;

int recording_data_buff_start_idx[MAX_NUM_OF_MWA][MAX_NUM_OF_CHAN_PER_MWA];
int spike_timestamp_buff_start_idx;
int exp_envi_event_buff_start_idx[MAX_NUM_OF_EXP_ENVI_ITEMS];
int exp_envi_command_buff_start_idx[MAX_NUM_OF_EXP_ENVI_ITEMS];
int mov_obj_event_buff_start_idx[MAX_NUM_OF_MOVING_OBJECTS];
int mov_obj_command_buff_start_idx[MAX_NUM_OF_MOVING_OBJECTS];

int recording_data_buff_prev_idx[MAX_NUM_OF_MWA][MAX_NUM_OF_CHAN_PER_MWA];
int spike_timestamp_buff_prev_idx;
int exp_envi_event_buff_prev_idx[MAX_NUM_OF_EXP_ENVI_ITEMS];
int exp_envi_command_buff_prev_idx[MAX_NUM_OF_EXP_ENVI_ITEMS];
int mov_obj_event_buff_prev_idx[MAX_NUM_OF_MOVING_OBJECTS];
int mov_obj_command_buff_prev_idx[MAX_NUM_OF_MOVING_OBJECTS];

int recording_data_buff_end_idx[MAX_NUM_OF_MWA][MAX_NUM_OF_CHAN_PER_MWA];
int spike_timestamp_buff_end_idx;
int exp_envi_event_buff_end_idx[MAX_NUM_OF_EXP_ENVI_ITEMS];
int exp_envi_command_buff_end_idx[MAX_NUM_OF_EXP_ENVI_ITEMS];
int mov_obj_event_buff_end_idx[MAX_NUM_OF_MOVING_OBJECTS];
int mov_obj_command_buff_end_idx[MAX_NUM_OF_MOVING_OBJECTS];

DaqMwaMap				*daq_mwa_map;
MwaDaqMap				*mwa_daq_map;
RecordingData				*recording_data;
SpikeThresholding			*spike_thresholding;
SpikeTimeStamp 			*spike_time_stamp; 
ExpEnviEventTimeStamp		*exp_envi_event_time_stamp;
ExpEnviCommandTimeStamp 	*exp_envi_command_time_stamp;		
MovingObjEventTimeStamp	*mov_obj_event_time_stamp;
MovingObjCommandTimeStamp	*mov_obj_command_time_stamp;
TemplateMatchingData		*template_matching_data;

// Functions
void create_gui(void);
void start_stop_recording_button_func (void);
void delete_last_recording_button_func (void);
void initialize_buffer_reading_start_indexes_and_time_for_recording(void);
void get_buffer_reading_range_indexes_for_recording(void);
void get_buffer_reading_end_indexes_and_time_for_recording(void);
void *recording_handler(void *ptr);

