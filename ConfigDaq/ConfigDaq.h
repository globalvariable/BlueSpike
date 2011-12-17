#ifndef CONFIGDAQ_H
#define CONFIGDAQ_H
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

GtkWidget *btn_turn_daq_on_off;
GtkWidget *btn_map_channels;
GtkWidget *btn_interrogate_mapping;
GtkWidget *btn_cancel_all_mapping;

GtkWidget *btn_select_maps_file_to_load;
GtkWidget *btn_load_maps_file; 

void create_gui(void);
void turn_daq_on_off_button_func(void); 
void map_channels_button_func(void); 
void interrogate_mapping_button_func(void); 
void cancel_all_mapping_button_func(void); 
void load_maps_file_button_func(void); 
bool interrogate_mapping(void);
void set_directory_btn_select_directory_to_load(void);
#endif
