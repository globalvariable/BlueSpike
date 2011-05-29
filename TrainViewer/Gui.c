/***************************************************************************
                          Gui.c  -  description
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

#include "Gui.h"


void create_gui(void)
{

	color_bg_train.red = 0;
	color_bg_train.green = 0;
	color_bg_train.blue = 0;

	color_train[0].red = 0;
	color_train[0].green = 65535;
	color_train[0].blue = 0;

	color_train[1].red = 65535;
	color_train[1].green = 65535;
	color_train[1].blue = 0;

	color_train[2].red = 65535;
	color_train[2].green = 0;
	color_train[2].blue = 0;

	color_train_nonsorted.red = 65535;
	color_train_nonsorted.green = 65535;
	color_train_nonsorted.blue = 65535;

	int i,j;
	X_train = g_new0 (float, NUM_OF_SAMP_PER_TRAIN);
	for (i=0; i<NUM_OF_SAMP_PER_TRAIN; i++)
	{
		X_train[i] = (float)i;
	}
	
	GPtrArray *trains_ptr_chan;
	trains_ptr_all = g_ptr_array_new();
	float *Y_temp;
	for (i=0; i < NUM_OF_CHAN; i++)
	{
		trains_ptr_chan = g_ptr_array_new();
		for (j=0;j<(NUM_OF_TEMP_PER_CHAN);j++)
		{
			Y_temp = g_new0 (float, NUM_OF_SAMP_PER_TRAIN);
			g_ptr_array_add  (trains_ptr_chan, Y_temp);
		}
		Y_temp = g_new0 (float, NUM_OF_SAMP_PER_TRAIN);
		g_ptr_array_add  (trains_ptr_chan, Y_temp);
		g_ptr_array_add  (trains_ptr_all, trains_ptr_chan);
	}


	GtkWidget *window;
	GtkWidget *hbox, *hbox1, *vbox, *vbox1, *vbox2, *button, *lbl;

 	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  	gtk_window_set_default_size(GTK_WINDOW(window), 1200, 900);
  	gtk_window_set_title(GTK_WINDOW(window), "TrainViewer");
  	gtk_container_set_border_width(GTK_CONTAINER(window), 10);


  	vbox = gtk_vbox_new(FALSE, 0);
	gtk_container_add(GTK_CONTAINER(window), vbox);

  	hbox = gtk_hbox_new(FALSE, 0);
  	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);

 	combo = gtk_combo_box_new_text();
 	gtk_combo_box_append_text(GTK_COMBO_BOX(combo), "1-8");
 	gtk_combo_box_append_text(GTK_COMBO_BOX(combo), "9-16");

	gtk_box_pack_start (GTK_BOX (hbox), combo, FALSE, FALSE, 10);

	ch_slct_button = gtk_button_new_with_label("Select Ch");
	pause_button = gtk_button_new_with_label("Pause");

	gtk_widget_set_size_request(ch_slct_button, 70, 30);
	gtk_widget_set_size_request(pause_button, 100, 30);
	gtk_box_pack_start (GTK_BOX (hbox), ch_slct_button, FALSE, FALSE, 50);
	gtk_box_pack_start (GTK_BOX (hbox), pause_button, TRUE, TRUE, 5);

	hbox = gtk_hbox_new(FALSE, 0);
  	gtk_box_pack_start (GTK_BOX (vbox), hbox, TRUE, TRUE, 0);

	vbox1 = gtk_vbox_new(TRUE, 0);
  	gtk_box_pack_start (GTK_BOX (hbox), vbox1, TRUE, TRUE, 0);

	GPtrArray *databoxes_ptr_chan;
	GtkWidget *databox;
	databoxes_ptr_all = g_ptr_array_new();
	for (i=0; i < NUM_OF_CHAN_TO_DISP; i++)
	{
		vbox2 = gtk_vbox_new(TRUE, 0);
  		gtk_box_pack_start (GTK_BOX (vbox1), vbox2, TRUE, TRUE, 1);

		databoxes_ptr_chan = g_ptr_array_new();
		for (j=0;j<(NUM_OF_TEMP_PER_CHAN);j++)
		{
			databox = gtk_databox_new ();			
			g_ptr_array_add  (databoxes_ptr_chan,  databox);
			gtk_box_pack_start (GTK_BOX (vbox2), databox, TRUE, TRUE, 0);
			gtk_widget_modify_bg (GTK_DATABOX (databox), GTK_STATE_NORMAL, &color_bg_train);
			gtk_databox_set_total_limits (GTK_DATABOX (databox), NUM_OF_SAMP_PER_TRAIN, 0, +1.10, 0.8);
		}
		databox = gtk_databox_new ();			
		g_ptr_array_add  (databoxes_ptr_chan,  databox);
		gtk_box_pack_start (GTK_BOX (vbox2), databox, TRUE, TRUE, 0);
		gtk_widget_modify_bg (GTK_DATABOX (databox), GTK_STATE_NORMAL, &color_bg_train);
		gtk_databox_set_total_limits (GTK_DATABOX (databox), NUM_OF_SAMP_PER_TRAIN, 0, +1.10, 0.8);
		g_ptr_array_add  (databoxes_ptr_all, databoxes_ptr_chan);
	}

	GtkDataboxGraph *graph;
	GPtrArray *graphs_ptr_chan;
	graphs_ptr_all = g_ptr_array_new();
	for (i=0; i < NUM_OF_CHAN; i++)
	{
		graphs_ptr_chan = g_ptr_array_new();
		trains_ptr_chan = g_ptr_array_index(trains_ptr_all, i);
		for (j=0;j<(NUM_OF_TEMP_PER_CHAN);j++)
		{
			Y_temp = g_ptr_array_index(trains_ptr_chan, j);
			if (buff->spike_template.include_unit[i][j])
				graph = GTK_DATABOX_GRAPH(gtk_databox_lines_new (NUM_OF_SAMP_PER_TRAIN, X_train, Y_temp, &color_train[j], 0));
			else
				graph = GTK_DATABOX_GRAPH(gtk_databox_lines_new (NUM_OF_SAMP_PER_TRAIN, X_train, Y_temp, &color_train_nonsorted, 0));				
			g_ptr_array_add  (graphs_ptr_chan,  graph);		
		}
		Y_temp = g_ptr_array_index(trains_ptr_chan, NUM_OF_TEMP_PER_CHAN);
		graph = GTK_DATABOX_GRAPH(gtk_databox_lines_new (NUM_OF_SAMP_PER_TRAIN, X_train, Y_temp, &color_train_nonsorted, 0));	
		g_ptr_array_add  (graphs_ptr_chan,  graph);		
		g_ptr_array_add  (graphs_ptr_all, graphs_ptr_chan);
	}

	for (i=0; i < NUM_OF_CHAN_TO_DISP; i++)
	{
		trains_ptr_chan = g_ptr_array_index(trains_ptr_all, i);
		databoxes_ptr_chan = g_ptr_array_index(databoxes_ptr_all, i);
		graphs_ptr_chan = g_ptr_array_index(graphs_ptr_all, i);			
		for (j=0;j<(NUM_OF_TEMP_PER_CHAN);j++)
		{
			databox = g_ptr_array_index(databoxes_ptr_chan, j);
			graph = g_ptr_array_index(graphs_ptr_chan, j);
			gtk_databox_graph_add (GTK_DATABOX (databox), graph);
			gtk_databox_set_total_limits (GTK_DATABOX (databox), NUM_OF_SAMP_PER_TRAIN, 0, 1.1, 0.8);	
		}
		databox = g_ptr_array_index(databoxes_ptr_chan, NUM_OF_TEMP_PER_CHAN);
		graph = g_ptr_array_index(graphs_ptr_chan, NUM_OF_TEMP_PER_CHAN);
		gtk_databox_graph_add (GTK_DATABOX (databox), graph);
		gtk_databox_set_total_limits (GTK_DATABOX (databox), NUM_OF_SAMP_PER_TRAIN, 0, 1.1, 0.8);		
	}


  	g_signal_connect (GTK_OBJECT (window), "destroy", G_CALLBACK (gtk_main_quit), NULL);

	g_signal_connect_swapped(G_OBJECT(ch_slct_button), "clicked", G_CALLBACK(ch_slct_func), G_OBJECT(databox));
	g_signal_connect_swapped(G_OBJECT(pause_button), "clicked", G_CALLBACK(pause_button_func), G_OBJECT(databox));

 	gtk_widget_show_all(window);
	back = (buff->scan_number_read/80) * 80;
	g_timeout_add(10, timeout_callback, databox);   //25 fps

	return;
}

gboolean timeout_callback(gpointer user_data) 
{
   	gint i,j,k, size_binned;
	front = (buff->scan_number_read/80) * 80;
	if (front < back)
		size = front + NUM_OF_SAMP_IN_BUFF - back;
	else
		size = front - back;	

	if (size == 0)
		return TRUE;  	

	size_binned = size/80;
	float *Y_temp;
	GPtrArray *trains_ptr_chan;
	for (i=0; i < NUM_OF_CHAN; i++)
	{	
		trains_ptr_chan = g_ptr_array_index(trains_ptr_all, i);
		for (j=0;j<(NUM_OF_TEMP_PER_CHAN);j++)
		{
			Y_temp = g_ptr_array_index(trains_ptr_chan, j);
			for (k=NUM_OF_SAMP_PER_TRAIN-1; k> -1; k--)
			{	
				if (k >= size_binned)
					Y_temp[k] = Y_temp[k-size_binned];
				else
					 Y_temp[k] = 0;
			}
		}
		Y_temp = g_ptr_array_index(trains_ptr_chan, j);
		for (k=NUM_OF_SAMP_PER_TRAIN-1; k> -1; k--)
		{	
			if (k >= size_binned)
				Y_temp[k] = Y_temp[k-size_binned];
			else
				 Y_temp[k] = 0;
		}
	}

	for (i = 0; i < size; i++)
	{
		if ((back+i) >= NUM_OF_SAMP_IN_BUFF)
		{
			for (j=0;j<NUM_OF_CHAN;j++)
			{
				trains_ptr_chan = g_ptr_array_index(trains_ptr_all, j);
				if (buff->sorted_spike_data[back+i-NUM_OF_SAMP_IN_BUFF].data[j] == 0) 
				{ 
					continue;
				}

				if ((buff->sorted_spike_data[back+i-NUM_OF_SAMP_IN_BUFF].data[j] == 1) || (buff->sorted_spike_data[back+i-NUM_OF_SAMP_IN_BUFF].data[j] == 4))
				{
					Y_temp = g_ptr_array_index(trains_ptr_chan, 0);
					Y_temp [size_binned - 1 - (i/80)] = 1;			
				}
				else if  ((buff->sorted_spike_data[back+i-NUM_OF_SAMP_IN_BUFF].data[j] == 2) || (buff->sorted_spike_data[back+i-NUM_OF_SAMP_IN_BUFF].data[j] == 5))
				{
					Y_temp = g_ptr_array_index(trains_ptr_chan, 1);
					Y_temp [size_binned - 1 - (i/80)] = 1;			
				}
				else if  ((buff->sorted_spike_data[back+i-NUM_OF_SAMP_IN_BUFF].data[j] == 3) || (buff->sorted_spike_data[back+i-NUM_OF_SAMP_IN_BUFF].data[j] == 6))
				{
					Y_temp = g_ptr_array_index(trains_ptr_chan, 2);
					Y_temp [size_binned - 1 - (i/80)] = 1;			
				}
				else if  (buff->sorted_spike_data[back+i-NUM_OF_SAMP_IN_BUFF].data[j] == 9) 
				{
					Y_temp = g_ptr_array_index(trains_ptr_chan, 3);
					Y_temp [size_binned - 1 - (i/80)] = 1;			
				}
			}
		}
		else
		{
			for (j=0;j<NUM_OF_CHAN;j++)
			{
				trains_ptr_chan = g_ptr_array_index(trains_ptr_all, j);
				if (buff->sorted_spike_data[back+i].data[j] == 0)
				{ 
					continue;
				}
				if ((buff->sorted_spike_data[back+i].data[j] == 1) || (buff->sorted_spike_data[back+i].data[j] == 4))
				{
					Y_temp = g_ptr_array_index(trains_ptr_chan, 0);
					Y_temp [size_binned - 1 - (i/80)] = 1;			
				}
				else if  ((buff->sorted_spike_data[back+i].data[j] == 2) || (buff->sorted_spike_data[back+i].data[j] == 5))
				{
					Y_temp = g_ptr_array_index(trains_ptr_chan, 1);
					Y_temp [size_binned - 1 - (i/80)] = 1;			
				}
				else if  ((buff->sorted_spike_data[back+i].data[j] == 3) || (buff->sorted_spike_data[back+i].data[j] == 6))
				{
					Y_temp = g_ptr_array_index(trains_ptr_chan, 2);
					Y_temp [size_binned - 1 - (i/80)] = 1;			
				}
				else if  (buff->sorted_spike_data[back+i].data[j] == 9) 
				{
					Y_temp = g_ptr_array_index(trains_ptr_chan, 3);
					Y_temp [size_binned - 1 - (i/80)] = 1;			
				}
			}
		}
	}
	back = front;

	GPtrArray *databoxes_ptr_chan;
	GtkWidget *databox;
	if (!disp_paused)
	{
		for (i=0; i < NUM_OF_CHAN_TO_DISP; i++)
		{
			databoxes_ptr_chan = g_ptr_array_index(databoxes_ptr_all, i);			
			for (j=0;j<(NUM_OF_TEMP_PER_CHAN);j++)
			{
				databox = g_ptr_array_index(databoxes_ptr_chan, j);
				gtk_databox_set_total_limits (GTK_DATABOX (databox), NUM_OF_SAMP_PER_TRAIN, 0, 1.1, 0.8);	
			}
			databox = g_ptr_array_index(databoxes_ptr_chan, NUM_OF_TEMP_PER_CHAN);
			gtk_databox_set_total_limits (GTK_DATABOX (databox), NUM_OF_SAMP_PER_TRAIN, 0, 1.1, 0.8);				
		}	
	}

	return TRUE;  

}

gboolean ch_slct_func (GtkDatabox * box)
{

	int i,j;
	GtkDataboxGraph *graph;
	GPtrArray *graphs_ptr_chan;
	GPtrArray *databoxes_ptr_chan;
	GtkWidget *databox;
	for (i=0; i < NUM_OF_CHAN_TO_DISP; i++)
	{	
		graphs_ptr_chan = g_ptr_array_index(graphs_ptr_all, (i+(disp_page*NUM_OF_CHAN_TO_DISP)));	
		databoxes_ptr_chan = g_ptr_array_index(databoxes_ptr_all, i);			
		for (j=0;j<(NUM_OF_TEMP_PER_CHAN);j++)
		{
			graph = g_ptr_array_index(graphs_ptr_chan, j);
			databox = g_ptr_array_index(databoxes_ptr_chan, j);
			gtk_databox_graph_remove (GTK_DATABOX (databox), graph);
		}
		databox = g_ptr_array_index(databoxes_ptr_chan, NUM_OF_TEMP_PER_CHAN);
		graph = g_ptr_array_index(graphs_ptr_chan, NUM_OF_TEMP_PER_CHAN);
		gtk_databox_graph_remove (GTK_DATABOX (databox), graph);
	}

	if (gtk_combo_box_get_active (GTK_COMBO_BOX(combo)) >= 0)
	{
		disp_page = gtk_combo_box_get_active (GTK_COMBO_BOX(combo));
	}

	for (i=0; i < NUM_OF_CHAN_TO_DISP; i++)
	{
		databoxes_ptr_chan = g_ptr_array_index(databoxes_ptr_all, i);
		graphs_ptr_chan = g_ptr_array_index(graphs_ptr_all, (i+(disp_page*NUM_OF_CHAN_TO_DISP)));				
		for (j=0;j<(NUM_OF_TEMP_PER_CHAN);j++)
		{
			databox = g_ptr_array_index(databoxes_ptr_chan, j);
			graph = g_ptr_array_index(graphs_ptr_chan, j);
			gtk_databox_graph_add (GTK_DATABOX (databox), graph);		
			gtk_databox_set_total_limits (GTK_DATABOX (databox), NUM_OF_SAMP_PER_TRAIN, 0, 1.1, 0.8);	
		}
		databox = g_ptr_array_index(databoxes_ptr_chan, NUM_OF_TEMP_PER_CHAN);
		graph = g_ptr_array_index(graphs_ptr_chan, NUM_OF_TEMP_PER_CHAN);
		gtk_databox_graph_add (GTK_DATABOX (databox), graph);
		gtk_databox_set_total_limits (GTK_DATABOX (databox), NUM_OF_SAMP_PER_TRAIN, 0, 1.1, 0.8);		
	}

	return TRUE;
}

gboolean pause_button_func (GtkDatabox * box)
{
	if (disp_paused)
	{
		disp_paused = 0;
		gtk_button_set_label (pause_button,"Pause");
	}
	else
	{
		disp_paused = 1;
		gtk_button_set_label (pause_button,"Resume");
	}	
	return TRUE;	
}

