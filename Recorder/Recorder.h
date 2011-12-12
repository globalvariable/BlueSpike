
#include <rtai_shm.h>
#include <rtai_nam2num.h>

#include "./DataHandling/DataHandling.h"


GtkWidget *main_table;

GtkWidget *btn_select_directory_to_save;

GtkWidget *btn_start_stop_recording;
GtkWidget *btn_delete_last_recording;
GtkWidget *btn_create_bluespikedata_folder;

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

bool created_bluespikedata_folder;

TimeStamp recording_start_time_ns;
TimeStamp recording_end_time_ns;

pthread_t recording_thread;

// Functions
void create_gui(void);
void start_stop_recording_button_func (void);
void delete_last_recording_button_func (void);
void create_bluespikedata_folder_button_func (void);
void *recording_handler(void *ptr);

