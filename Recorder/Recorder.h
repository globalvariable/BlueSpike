
#include "../SharedMemory.h"

#include <rtai_shm.h>
#include <rtai_nam2num.h>

#include <gtk/gtk.h>
#include <gtkdatabox.h>
#include <gtkdatabox_ruler.h>
#include <gtkdatabox_lines.h>
#include <gtkdatabox_grid.h>


GtkWidget *main_table;

GtkWidget *btn_select_directory_to_save;
GtkWidget *entry_data_name;

GtkWidget *btn_start_stop_recording;

GtkWidget *txv_mwa_daq_map_additional;
GtkWidget *txv_recording_additional;
GtkWidget *txv_spike_time_stamp_additional;
GtkWidget *txv_exp_envi_event_additional;
GtkWidget *txv_exp_envi_command_additional;
GtkWidget *txv_mov_obj_event_additional;
GtkWidget *txv_mov_obj_command_additional;

bool recording_enabled;

// Functions
void create_gui(void);
void start_stop_recording_button_func (GtkDatabox * box);
