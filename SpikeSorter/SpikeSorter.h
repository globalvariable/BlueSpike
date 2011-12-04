/***************************************************************************
                          SpikeSorter.h  -  description
                             -------------------
    copyright            : (C) 2011 by Mehmet Kocaturk
    email                : mehmet.kocaturk@boun.edu.tr
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "../SharedMemory.h"

#include <rtai_shm.h>
#include <rtai_nam2num.h>

#include <gtk/gtk.h>
#include <gtkdatabox.h>
#include <gtkdatabox_ruler.h>
#include <gtkdatabox_lines.h>
#include <gtkdatabox_grid.h>

#include <meschach/matrix.h>
#include <meschach/matrix2.h>

#include <math.h>

#define SPIKE_MEM_TO_DISPLAY_UNIT 30
#define SPIKE_MEM_TO_DISPLAY_ALL_NONSORTED_SPIKE 150

#define MIN_SPIKE_NUM_FOR_TEMPLATE 60

GdkColor color_bg;
GdkColor color_non_sorted_all_spike;
GdkColor color_spike[MAX_NUM_OF_UNIT_PER_CHAN];
GdkColor color_spike_non_sorted;
GdkColor color_spike_template;

float *X_axis;
GPtrArray *Y_non_sorted_all_spikes;
int Y_non_sorted_all_spikes_last_g_ptr_array_idx;

GPtrArray *Y_sorted_spikes_arr[MAX_NUM_OF_UNIT_PER_CHAN];
int Y_sorted_spikes_last_g_ptr_array_idx[MAX_NUM_OF_UNIT_PER_CHAN];

GPtrArray *Y_non_sorted_spike;
int Y_non_sorted_spike_last_g_ptr_array_idx;

float *Y_templates[MAX_NUM_OF_UNIT_PER_CHAN];

GtkWidget *main_table;

GtkWidget *databox_nonsorted_all_spike;
GtkWidget *box_nonsorted_all_spike;

GtkWidget *databox_sorted_all_spike;
GtkWidget *box_sorted_all_spike;

GtkWidget *databox_units[MAX_NUM_OF_UNIT_PER_CHAN];
GtkWidget *box_units[MAX_NUM_OF_UNIT_PER_CHAN];

GtkWidget *databox_non_sorted_spike;
GtkWidget *box_non_sorted_spike;

GtkWidget *combo_mwa;
GtkWidget *combo_chan;
GtkWidget *combo_unit;

GtkWidget *btn_clear_spike_selection_screen;
GtkWidget *btn_clear_unit_screen;
GtkWidget *btn_clear_nonsorted_unit_screen;

GtkWidget *btn_clear_unit_template;

GtkWidget *btn_sorting_on_off;
GtkWidget *btn_unit_sorting_on_off;
GtkWidget *btn_include_unit_on_off;

GtkWidget *btn_spike_filter_on_off;

GtkWidget *entry_probability_thres;
GtkWidget *btn_submit_probability_thres;

GtkWidget *btn_pause;

GtkWidget *btn_select_template_file_to_load;
GtkWidget *btn_load_template_file; 

GtkWidget *btn_select_template_file_directory_to_save;
GtkWidget *entry_template_file_name;
GtkWidget *btn_save_template_file;

int disp_mwa;
int disp_chan;
int disp_unit;

int spike_filter_mode_on;
int rect_switch;
float x_upper_1, x_lower_1, y_upper_1, y_lower_1;   // for rectangle to select areas for spike filtering
float x_upper_2, x_lower_2, y_upper_2, y_lower_2; 

int disp_paused;

int spike_time_stamp_buff_read_idx;

int plotting_in_progress;

void create_gui(void);
void combo_mwa_func (void);
void combo_chan_func (void);
void combo_unit_func (void);
void clear_spike_selection_screen_button_func(void);
void clear_unit_screen_button_func(void);
void clear_nonsorted_unit_screen_button_func(void);
void clear_unit_template_button_func(void);
void unit_sorting_on_off_button_func(void);
void include_unit_on_off_button_func(void);
void spike_filter_on_off_button_func(void);
void submit_probability_thres_button_func(void);
void pause_button_func(void);
void load_template_file_button_func(void);
void save_template_file_button_func(void);
void clear_spikes_screen(void);
void spike_selection_rectangle_func(GtkDatabox * box, GtkDataboxValueRectangle * selectionValues);
gboolean timeout_callback(gpointer user_data);



