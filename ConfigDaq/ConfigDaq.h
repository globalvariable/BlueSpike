#include "../SharedMemory.h"


#include <stdlib.h>
#include <stdbool.h>
#include <rtai_shm.h>
#include <rtai_nam2num.h>

#include <rtai_comedi.h>

#include <gtk/gtk.h>
#include <gtkdatabox.h>
#include <gtkdatabox_ruler.h>
#include <gtkdatabox_lines.h>


GtkWidget *entry_daq_card_num;
GtkWidget *entry_daq_card_chan_range_lower;
GtkWidget *entry_daq_card_chan_range_upper;

GtkWidget *entry_mwa_num;
GtkWidget *entry_mwa_chan_range_lower;
GtkWidget *entry_mwa_chan_range_upper;

GtkWidget *btn_map_channels;
GtkWidget *btn_interrogate_mapping;

GtkWidget *btn_select_config_file_to_load;
GtkWidget *btn_load_config_file; 

GtkWidget *btn_select_config_file_directory_to_save;
GtkWidget *entry_config_file_name;
GtkWidget *btn_save_config_file;

void create_gui(void);
void map_channels_button_func(void); 
void interrogate_mapping_button_func(void); 
void load_config_file_button_func(void); 
void save_config_file_button_func(void); 
bool interrogate_mapping(void);
