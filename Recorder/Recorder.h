
#include "../SharedMemory.h"

#include <rtai_shm.h>
#include <rtai_nam2num.h>

#include <gtk/gtk.h>



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

bool recording_started;

int recording_data_buff_prev_idx;
int spike_timestamp_buff_prev_idx;
int exp_envi_event_buff_prev_idx;
int exp_envi_command_buff_prev_idx;
int mov_obj_event_buff_prev_idx;
int mov_obj_command_buff_prev_idx;
// Functions
void create_gui(void);
void start_stop_recording_button_func (void);
void delete_last_recording_button_func (void);
gboolean timeout_callback(gpointer user_data); 
