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


#define SPIKE_MEM_TO_DISPLAY_UNIT 30
#define SPIKE_MEM_TO_DISPLAY_ALL_NONSORTED_SPIKE 150

GdkColor color_bg;
GdkColor color_non_sorted_all_spike;
GdkColor color_spike[MAX_NUM_OF_UNIT_PER_CHAN+1];
GdkColor color_spike_template;

float *X_axis;
GPtrArray *Y_non_sorted_all_spikes;
GPtrArray *Y_spikes_arr[MAX_NUM_OF_UNIT_PER_CHAN+1];

GtkWidget *main_table;

GtkWidget *databox_nonsorted_all_spike;
GtkWidget *box_nonsorted_all_spike;

GtkWidget *databox_sorted_all_spike;
GtkWidget *box_sorted_all_spike;

GtkWidget *databox_units[MAX_NUM_OF_UNIT_PER_CHAN+1];
GtkWidget *box_units[MAX_NUM_OF_UNIT_PER_CHAN+1];

void create_gui(void);
