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


#define NUM_OF_SAMP_PER_TRAIN 1500
#define NUM_OF_CHAN_TO_DISP 8
// Variables

GPtrArray *databoxes_ptr_all;
GPtrArray *graphs_ptr_all;

GtkWidget *combo;
GtkWidget *ch_slct_button;
GtkWidget *pause_button;

int disp_page;
int front, back, size;

float *X_train;
GPtrArray *trains_ptr_all;

int disp_paused;

GdkColor color_bg_train;
GdkColor color_train[NUM_OF_TEMP_PER_CHAN];
GdkColor color_train_nonsorted;

// Functions
void create_gui(void);
gboolean timeout_callback(gpointer user_data) ;
gboolean ch_slct_func (GtkDatabox * box);
gboolean pause_button_func (GtkDatabox * box);
gboolean clear_screen_but_func (GtkDatabox * box);
