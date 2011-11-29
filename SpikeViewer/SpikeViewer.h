/***************************************************************************
                          SpikeViewer.h  -  description
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

#define SPIKE_MEM_TO_DISPLAY 30 

#define NUM_OF_RAW_SAMPLE_TO_DISPLAY 4000		// Adjusted for 40000KHz sampling rate 
#define RAW_DATA_DISP_DURATION_MS 100.0	


// Variables

GtkWidget *main_table;

GtkWidget *databox_signal;
GtkWidget *databox_spike_shape;

GtkWidget *box_signal;
GtkWidget *box_spike_shape;

GtkWidget *combo_mwa;
GtkWidget *combo_chan;

GtkWidget *clear_button;
GtkWidget *pause_button;
GtkWidget *threshold_button;
GtkWidget *btn_filter_highpass_150Hz;
GtkWidget *btn_filter_highpass_400Hz;
GtkWidget *btn_filter_lowpass_8KHz;
GtkWidget *btn_print_spike_end_buff;

int spike_end_buff_read_idx;

GtkWidget *entryThreshold;

int display_mwa;
int display_mwa_chan;

float *X_raw;
float *Y_raw;
int previous_start_idx_to_plot;

GPtrArray* Y_spikes_ptr;
int Y_spikes_idx;

int disp_paused;
int print_spike_end_buff;

GdkColor color_bg_signal;
GdkColor color_signal;
GdkColor color_grid_signal;

GdkColor color_bg_spike_shape;
GdkColor color_spike_shape;


// Functions
void create_gui(void);
gboolean timeout_callback(gpointer user_data) ;
gboolean filter_highpass_150Hz_button_func (GtkDatabox * box);
gboolean filter_highpass_400Hz_button_func (GtkDatabox * box);
gboolean filter_lowpass_8KHz_button_func (GtkDatabox * box);
gboolean combo_mwa_func (GtkDatabox * box);
gboolean combo_chan_func (GtkDatabox * box);
gboolean pause_button_func (GtkDatabox * box);
gboolean print_spike_end_buff_button_func (GtkDatabox * box);
gboolean threshold_but_func (GtkDatabox * box);
gboolean clear_screen_but_func (GtkDatabox * box);
void clear_spike_screen(void);
void clear_raw_data_screen(void);
