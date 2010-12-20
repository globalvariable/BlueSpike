/***************************************************************************
                          Gui.h  -  description
                             -------------------
    copyright            : (C) 2010 by Mehmet Kocaturk
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

// Variables
GtkWidget *databox_signal;
GtkWidget *databox_spike_shape;

GtkWidget *box_signal;
GtkWidget *box_spike_shape;


GtkWidget *combo;
GtkWidget *ch_slct_button;
GtkWidget *record_button;

GtkWidget *clear_button;
GtkWidget *pause_button;
GtkWidget *threshold_button;
GtkWidget *name_file_button;
GtkWidget *filter_button;

GtkDataboxGraph *graph;	
GtkDataboxGraph *graph_spike_shape;
int GraphIdx;	

char *strThreshold;
GtkWidget *entryThreshold;

const char *strAddFileName;
GtkWidget *entryAddFileName;

int disp_chan;
int front, back, size;
float *X;
float *Y;

float *X_spike;
float *Y_spike;
GPtrArray* Y_spikes_ptr;
int Y_spikes_idx;

FILE *fp;
int rec_data;
int disp_paused;

GdkColor color_bg_signal;
GdkColor color_signal;
GdkColor color_grid_signal;

GdkColor color_bg_spike_shape;
GdkColor color_spike_shape;


// Functions
void create_gui(void);
gboolean timeout_callback(gpointer user_data) ;
gboolean filter_button_func (GtkDatabox * box);
gboolean ch_slct_func (GtkDatabox * box);
gboolean pause_button_func (GtkDatabox * box);
gboolean record_but_func (GtkDatabox * box);
gboolean name_file_but_func (GtkDatabox * box);
gboolean threshold_but_func (GtkDatabox * box);
gboolean clear_screen_but_func (GtkDatabox * box);
