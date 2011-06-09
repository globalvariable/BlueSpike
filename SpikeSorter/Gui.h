/***************************************************************************
                          Gui.h  -  description
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

#include "Global.h"

#include <gtk/gtk.h>
#include <gtkdatabox.h>
#include <gtkdatabox_ruler.h>
#include <gtkdatabox_lines.h>


#define INIT_POINT_OF_SPIKE 39
#define SPIKE_MEM_TO_DISPLAY 150
#define SPIKE_MEM_TO_DISPLAY_UNIT 30
#define MIN_SPIKE_NUM_FOR_TEMPLATE 80


// Variables
GtkWidget *databox_all_spike;
GtkWidget *databox_sorted_all_spike;
GtkWidget *databox_spike_0;
GtkWidget *databox_spike_1;
GtkWidget *databox_spike_2;
GtkWidget *databox_spike_nonsorted;

GtkWidget *box_all_spike;
GtkWidget *box_sorted_all_spike;
GtkWidget *box_spike_0;
GtkWidget *box_spike_1;
GtkWidget *box_spike_2;
GtkWidget *box_spike_nonsorted;

GtkWidget *combo;
GtkWidget *ch_slct_button;

GtkWidget *combo_spike;
GtkWidget *unit_slct_button;

GtkWidget *lbl_selected_chan;
GtkWidget *lbl_selected_unit;

char *str_selected_chan;
char *str_selected_unit;

GtkWidget *clear_button;
GtkWidget *clear_unit_button;
GtkWidget *clear_nonsorted_button;
GtkWidget *clear_unit_template_button;
GtkWidget *pause_button;
GtkWidget *diff_button;
GtkWidget *sorting_onoff_button;
GtkWidget *unit_sorting_onoff_button;
GtkWidget *include_unit_onoff_button;
GtkWidget *spike_filter_mode_button;
GtkWidget *save_template_button;
GtkWidget *load_template_button;

char *strDiff;
GtkWidget *entryDiff;

char *strAddFileName;
GtkWidget *entryAddFileName;

char *strLoadFileName;
GtkWidget *entryLoadFileName;

FILE *fp;

GtkDataboxGraph *graph_all_spike;
GtkDataboxGraph *graph_sorted_all_spike;
GtkDataboxGraph *graph_spike_0;
GtkDataboxGraph *graph_spike_template_0;
GtkDataboxGraph *graph_spike_1;
GtkDataboxGraph *graph_spike_template_1;
GtkDataboxGraph *graph_spike_2;
GtkDataboxGraph *graph_spike_template_2;
GtkDataboxGraph *graph_spike_nonsorted;
	
int GraphIdx;	

float Y_ex;

int disp_chan;
int selected_spike_unit;
int front, back, size;
float *X;
float *Y;

float *X_axis;
float *Y_all_spike;
float *Y_spike_0;
float *Y_spike_1;
float *Y_spike_2;
float *Y_spike_nonsorted;

float *Y_spike_0_template;
float *Y_spike_1_template;
float *Y_spike_2_template;

GPtrArray* Y_all_spikes_ptr;
GPtrArray* Y_spikes_0_ptr;
GPtrArray* Y_spikes_1_ptr;
GPtrArray* Y_spikes_2_ptr;
GPtrArray* Y_spikes_nonsorted_ptr;

int Y_all_spikes_idx;
int Y_spikes_0_idx;
int Y_spikes_1_idx;
int Y_spikes_2_idx;
int Y_spikes_nonsorted_idx;

int disp_paused;
int spike_filter_mode;
float x_upper, x_lower, y_upper, y_lower;   // for rectangle to select spikes
float x_upper_1, x_lower_1, y_upper_1, y_lower_1; 
float x_upper_2, x_lower_2, y_upper_2, y_lower_2; 
int rect_switch;

GdkColor color_bg;
GdkColor color_all_spike;
GdkColor color_spike_0;
GdkColor color_spike_1;
GdkColor color_spike_2;
GdkColor color_spike_nonsorted;
GdkColor color_spike_template;

// Functions
void create_gui(void);
gboolean timeout_callback(gpointer user_data) ;
gboolean ch_slct_func (GtkDatabox * box);
gboolean spk_unit_slct_func (GtkDatabox * box);
gboolean pause_button_func (GtkDatabox * box);
gboolean threshold_but_func (GtkDatabox * box);
gboolean clear_screen_but_func (GtkDatabox * box);
gboolean clear_nonsorted_but_func (GtkDatabox * box);
gboolean clear_unit_but_func (GtkDatabox * box);
gboolean clear_unit_template_but_func (GtkDatabox * box);
gboolean diff_but_func (GtkDatabox * box);
gboolean rect_selection_func (GtkDatabox * box, GtkDataboxValueRectangle * selectionValues);
gboolean sorting_onoff_but_func (GtkDatabox * box);
gboolean unit_sorting_onoff_but_func (GtkDatabox * box);
gboolean include_unit_onoff_but_func (GtkDatabox * box);
gboolean spike_filter_mode_but_func (GtkDatabox * box);
gboolean save_template_but_func (GtkDatabox * box);
gboolean load_template_but_func (GtkDatabox * box);
