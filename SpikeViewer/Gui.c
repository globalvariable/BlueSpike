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

	color_bg_signal.red = 65535;
	color_bg_signal.green = 65535;
	color_bg_signal.blue = 65535;

	color_signal.red = 0;
	color_signal.green = 0;
	color_signal.blue = 0;

	color_bg_spike_shape.red = 0;
	color_bg_spike_shape.green = 0;
	color_bg_spike_shape.blue = 0;

	color_grid_signal.red = 50000;
	color_grid_signal.green = 50000;
	color_grid_signal.blue = 50000;

	color_spike_shape.red = 65535;
	color_spike_shape.green = 65535;
	color_spike_shape.blue = 65535;
	strAddFileName = g_new0 (char, 70);
	strAddFileName = "none";

	int i;
	X_spike = g_new0 (float, NUM_OF_SAMP_PER_SPIKE);
	for (i=0; i<NUM_OF_SAMP_PER_SPIKE; i++)
	{
		X_spike[i] = (float)i;
	}
	Y_spikes_ptr = g_ptr_array_new();

	for (i=0;i<SPIKE_MEM_TO_DISPLAY;i++)
	{
		Y_spike = g_new0 (float, NUM_OF_SAMP_PER_SPIKE);
		g_ptr_array_add  (Y_spikes_ptr, Y_spike);
	}
	Y_spikes_idx = 0;

	

	GtkWidget *window;
	GtkWidget *hbox, *hbox1, *vbox, *vbox1, *vbox2, *button, *lbl;
	GtkDataboxGraph *graph;

 	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  	gtk_window_set_default_size(GTK_WINDOW(window), 1200, 900);
  	gtk_window_set_title(GTK_WINDOW(window), "SpikeViewer");
  	gtk_container_set_border_width(GTK_CONTAINER(window), 10);


  	vbox = gtk_vbox_new(FALSE, 0);
	gtk_container_add(GTK_CONTAINER(window), vbox);

  	hbox = gtk_hbox_new(FALSE, 0);
  	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 10);

 	combo = gtk_combo_box_new_text();

	char chan_name[5];	
	for (i=1; i<=NUM_OF_CHAN; i++)
	{
		sprintf(chan_name, "%d" , i);
	 	gtk_combo_box_append_text(GTK_COMBO_BOX(combo), chan_name);
	} 

	gtk_box_pack_start (GTK_BOX (hbox), combo, FALSE, FALSE, 10);

	ch_slct_button = gtk_button_new_with_label("Select Ch");
	record_button = gtk_button_new_with_label("Record");

	gtk_widget_set_size_request(ch_slct_button, 70, 30);
	gtk_widget_set_size_request(record_button, 100, 30);
	gtk_box_pack_start (GTK_BOX (hbox), ch_slct_button, FALSE, FALSE, 50);
	gtk_box_pack_start (GTK_BOX (hbox), record_button, FALSE, FALSE, 0);

        lbl = gtk_label_new("Name Data File :");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE, 50);

        entryAddFileName = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox), entryAddFileName, FALSE,FALSE,0);

	name_file_button = gtk_button_new_with_label("Submit");
	gtk_widget_set_size_request(name_file_button, 70, 30);
	gtk_box_pack_start (GTK_BOX (hbox), name_file_button, FALSE, FALSE, 0);


	hbox = gtk_hbox_new(FALSE, 0);
  	gtk_box_pack_start (GTK_BOX (vbox), hbox, TRUE, TRUE, 0);

	vbox1 = gtk_vbox_new(FALSE, 0);
  	gtk_box_pack_start (GTK_BOX (hbox), vbox1, TRUE, TRUE, 0);

	pause_button = gtk_button_new_with_label("Pause");
	gtk_widget_set_size_request(pause_button, 100, 30);
	gtk_box_pack_start (GTK_BOX (vbox1), pause_button, FALSE, FALSE, 5);

  	gtk_databox_create_box_with_scrollbars_and_rulers (&box_signal, &databox_signal,TRUE, TRUE, TRUE, TRUE);
  	gtk_box_pack_start (GTK_BOX (vbox1), databox_signal, TRUE, TRUE, 0);
  	gtk_widget_modify_bg (GTK_DATABOX (box_signal), GTK_STATE_NORMAL, &color_bg_signal);

	vbox1 = gtk_vbox_new(FALSE, 20);
  	gtk_box_pack_start (GTK_BOX (hbox), vbox1, FALSE, FALSE, 0);

	gtk_databox_create_box_with_scrollbars_and_rulers (&box_spike_shape, &databox_spike_shape,TRUE, TRUE, TRUE, TRUE);
  	gtk_box_pack_start (GTK_BOX (vbox1), databox_spike_shape, TRUE, TRUE, 20);
  	gtk_widget_modify_bg (GTK_DATABOX (box_spike_shape), GTK_STATE_NORMAL, &color_bg_spike_shape);

	clear_button = gtk_button_new_with_label("Clear Screen");
	gtk_widget_set_size_request(clear_button, 100, 30);
	gtk_box_pack_start (GTK_BOX (vbox1), clear_button, FALSE, FALSE, 5);

	hbox1 = gtk_hbox_new(TRUE, 0);
  	gtk_box_pack_start (GTK_BOX (vbox1), hbox1, FALSE, FALSE, 0);

	filter_button = gtk_button_new_with_label("Turn Filter ON");
	gtk_widget_set_size_request(filter_button, 80, 30);
	gtk_box_pack_start (GTK_BOX (hbox1), filter_button, TRUE, TRUE, 0);

	highpass_4th_button = gtk_button_new_with_label("Turn 4th Order ON");
	gtk_widget_set_size_request(highpass_4th_button, 80, 30);
	gtk_box_pack_start (GTK_BOX (hbox1), highpass_4th_button, TRUE, TRUE, 0);

	if (buff->filter_on)
		gtk_button_set_label (filter_button,"Turn Filter OFF");
	else
		gtk_button_set_label (filter_button,"Turn Filter ON");

	if (buff->highpass_4th_on)
		gtk_button_set_label (highpass_4th_button,"Turn 4th Order OFF");
	else
		gtk_button_set_label (highpass_4th_button,"Turn 4th Order ON");


	hbox1 = gtk_hbox_new(FALSE, 0);
  	gtk_box_pack_start (GTK_BOX (vbox1), hbox1, FALSE, FALSE, 10);


        lbl = gtk_label_new("Threshold (ADC Code): ");
        gtk_box_pack_start(GTK_BOX(hbox1),lbl, FALSE,FALSE, 50);

        entryThreshold = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox1),entryThreshold, FALSE,FALSE,0);
 	gtk_entry_set_editable(entryThreshold,TRUE);
	hbox1 = gtk_hbox_new(FALSE, 0);
  	gtk_box_pack_start (GTK_BOX (vbox1), hbox1, FALSE, FALSE, 0);

	threshold_button = gtk_button_new_with_label("Submit Threshold");
	gtk_widget_set_size_request(threshold_button, 300, 30);
	gtk_box_pack_start (GTK_BOX (hbox1), threshold_button, FALSE, FALSE, 50);

	char thres[20];
	strThreshold = thres;
	sprintf(strThreshold, "%.0f" , buff->Threshold[0]);
	gtk_entry_set_text (GTK_ENTRY(entryThreshold), strThreshold);

	vbox2 = gtk_vbox_new(FALSE, 20);
  	gtk_box_pack_start (GTK_BOX (vbox1), vbox2, FALSE, FALSE, 50);
	
	hbox1 = gtk_hbox_new(TRUE, 0);
  	gtk_box_pack_start (GTK_BOX (vbox2), hbox1, FALSE, FALSE, 0);

        lbl = gtk_label_new("Jitter(ns): ");
        gtk_box_pack_start(GTK_BOX(hbox1),lbl, FALSE,FALSE, 0);

        lbl_jitter = gtk_label_new("0");
        gtk_box_pack_start(GTK_BOX(hbox1),lbl_jitter, FALSE,FALSE, 0);

	hbox1 = gtk_hbox_new(TRUE, 0);
  	gtk_box_pack_start (GTK_BOX (vbox2), hbox1, FALSE, FALSE, 0);

        lbl = gtk_label_new("Jitter( >20 us): ");
        gtk_box_pack_start(GTK_BOX(hbox1),lbl, FALSE,FALSE, 0);

        lbl_jitter_20_us = gtk_label_new("0");
        gtk_box_pack_start(GTK_BOX(hbox1),lbl_jitter_20_us, FALSE,FALSE, 0);

	hbox1 = gtk_hbox_new(TRUE, 0);
  	gtk_box_pack_start (GTK_BOX (vbox2), hbox1, FALSE, FALSE, 0);

        lbl = gtk_label_new("Jitter( >50 us): ");
        gtk_box_pack_start(GTK_BOX(hbox1),lbl, FALSE,FALSE, 0);

        lbl_jitter_50_us = gtk_label_new("0");
        gtk_box_pack_start(GTK_BOX(hbox1),lbl_jitter_50_us, FALSE,FALSE, 0);

	hbox1 = gtk_hbox_new(TRUE, 0);
  	gtk_box_pack_start (GTK_BOX (vbox2), hbox1, FALSE, FALSE, 0);

        lbl = gtk_label_new("Jitter( >100 us): ");
        gtk_box_pack_start(GTK_BOX(hbox1),lbl, FALSE,FALSE, 0);

        lbl_jitter_100_us = gtk_label_new("0");
        gtk_box_pack_start(GTK_BOX(hbox1),lbl_jitter_100_us, FALSE,FALSE, 0);



   	graph = gtk_databox_grid_new (21, 21, &color_grid_signal, 0);
   	gtk_databox_graph_add (GTK_DATABOX (box_signal), graph);


	for (i=0;i<SPIKE_MEM_TO_DISPLAY;i++)
	{
		Y_spike = g_ptr_array_index(Y_spikes_ptr,i);
		graph_spike_shape = GTK_DATABOX_GRAPH(gtk_databox_lines_new (NUM_OF_SAMP_PER_SPIKE, X_spike, Y_spike, &color_spike_shape, 0));
		gtk_databox_graph_add (GTK_DATABOX (box_spike_shape), graph_spike_shape);
	}

	gtk_databox_set_total_limits (GTK_DATABOX (box_spike_shape), 0, NUM_OF_SAMP_PER_SPIKE, +2200, -2200);	


  	g_signal_connect (GTK_OBJECT (window), "destroy", G_CALLBACK (gtk_main_quit), NULL);


	g_signal_connect_swapped(G_OBJECT(filter_button), "clicked", G_CALLBACK(filter_button_func), G_OBJECT(box_signal));
	g_signal_connect_swapped(G_OBJECT(highpass_4th_button), "clicked", G_CALLBACK(highpass_4th_button_func), G_OBJECT(box_signal));
	g_signal_connect_swapped(G_OBJECT(ch_slct_button), "clicked", G_CALLBACK(ch_slct_func), G_OBJECT(box_signal));
	g_signal_connect_swapped(G_OBJECT(pause_button), "clicked", G_CALLBACK(pause_button_func), G_OBJECT(box_signal));
	g_signal_connect_swapped(G_OBJECT(record_button), "clicked", G_CALLBACK(record_but_func), G_OBJECT(box_signal));
	g_signal_connect_swapped(G_OBJECT(name_file_button), "clicked", G_CALLBACK(name_file_but_func), G_OBJECT(box_signal));
	g_signal_connect_swapped(G_OBJECT(threshold_button), "clicked", G_CALLBACK(threshold_but_func), G_OBJECT(box_signal));
	g_signal_connect_swapped(G_OBJECT(clear_button), "clicked", G_CALLBACK(clear_screen_but_func), G_OBJECT(box_signal));

 	gtk_widget_show_all(window);

	g_timeout_add(100, timeout_callback, box_signal);

	return;
}

gboolean timeout_callback(gpointer user_data) 
{
	
	GtkDatabox* databox = GTK_DATABOX (box_signal);

   	gint i,j,spike_view_cntr;

	front = buff->scan_number_read;
	if (front < back)
		size = front + NUM_OF_SAMP_IN_BUFF - back;
	else
		size = front - back;	


	if (size == 0)
		return TRUE;  
		


	if (!disp_paused)
	{
		g_free(X);
		g_free(Y);

		X = g_new0 (float, size);
		Y = g_new0 (float, size);
		float *Y_temp;

		if (graph != NULL)
		{
			gtk_databox_graph_remove (GTK_DATABOX (databox), graph);
			g_object_unref(graph);
		}

		if (buff->filter_on)
		{
			for (i = 0; i < size; i++)
			{
				X[i] = (float)i;
				
				if ((back+i) >= NUM_OF_SAMP_IN_BUFF)
				{
					Y[i] = buff->filtered_scan[back+i-NUM_OF_SAMP_IN_BUFF].data[disp_chan];  
					if (buff->spike_end[back+i-NUM_OF_SAMP_IN_BUFF].data[disp_chan] > 0)
					{  
						Y_temp = g_ptr_array_index(Y_spikes_ptr,Y_spikes_idx);
						for (spike_view_cntr=-INIT_POINT_OF_SPIKE; spike_view_cntr<1; spike_view_cntr++)
						{
							if ((back+i-NUM_OF_SAMP_IN_BUFF+spike_view_cntr) < 0)
							{
								Y_temp[spike_view_cntr+INIT_POINT_OF_SPIKE]= buff->filtered_scan[back+i+spike_view_cntr].data[disp_chan];
							}
							else 
							{
								Y_temp[spike_view_cntr+INIT_POINT_OF_SPIKE]= buff->filtered_scan[back+i-NUM_OF_SAMP_IN_BUFF+spike_view_cntr].data[disp_chan];
							}  
						}

						gtk_databox_set_total_limits (GTK_DATABOX (box_spike_shape), 0, NUM_OF_SAMP_PER_SPIKE, +2200, -2200);	
						Y_spikes_idx++;
						if (Y_spikes_idx == SPIKE_MEM_TO_DISPLAY)
							Y_spikes_idx = 0;
					}  
				}
				else
				{
					Y[i] = buff->filtered_scan[back+i].data[disp_chan];
					if (buff->spike_end[back+i].data[disp_chan] > 0)
					{   
						Y_temp = g_ptr_array_index(Y_spikes_ptr,Y_spikes_idx);
						for (spike_view_cntr=-INIT_POINT_OF_SPIKE; spike_view_cntr<1; spike_view_cntr++)
						{
							if ((back+i+spike_view_cntr) < 0)
							{
								Y_temp[spike_view_cntr+INIT_POINT_OF_SPIKE]= buff->filtered_scan[back+i+spike_view_cntr+NUM_OF_SAMP_IN_BUFF].data[disp_chan];
							}
							else 
							{
								Y_temp[spike_view_cntr+INIT_POINT_OF_SPIKE]= buff->filtered_scan[back+i+spike_view_cntr].data[disp_chan];
							} 
						}

						gtk_databox_set_total_limits (GTK_DATABOX (box_spike_shape), -0, NUM_OF_SAMP_PER_SPIKE, +2200, -2200);	
						Y_spikes_idx++;
						if (Y_spikes_idx == SPIKE_MEM_TO_DISPLAY)
							Y_spikes_idx = 0;  
					}  
				} 
			}
		}
		else
		{
			
			for (i = 0; i < size; i++)
			{
				X[i] = (float)i;
				if ((back+i) >= NUM_OF_SAMP_IN_BUFF)
				{
					Y[i] = buff->scan[back+i-NUM_OF_SAMP_IN_BUFF].data[disp_chan];
				}  
				else
				{
					Y[i] = buff->scan[back+i].data[disp_chan];
				}
			}
		}
		graph = GTK_DATABOX_GRAPH(gtk_databox_lines_new (size, X, Y, &color_signal, 0));
		gtk_databox_graph_add (GTK_DATABOX (databox), graph);
		gtk_databox_set_total_limits (GTK_DATABOX (databox), -10., 4500., +2200, -2200);


		update_jitter_monitor();

	}




	if (rec_data)
	{
		for (i = 0; i < size; i++)
		{
			if ((back+i) >= NUM_OF_SAMP_IN_BUFF)
			{
				for (j=0; j<NUM_OF_CHAN; j++)
					fprintf(fp, " %.0f\t", buff->scan[back+i-NUM_OF_SAMP_IN_BUFF].data[j]);
				fprintf(fp, " %d\t", buff->Environment[back+i-NUM_OF_SAMP_IN_BUFF].Status.AllStatus);	
				fprintf(fp, " %d\t", buff->Environment[back+i-NUM_OF_SAMP_IN_BUFF].ShortInt_Status0);
				fprintf(fp, " %d\n", buff->RTStatusFlags[back+i-NUM_OF_SAMP_IN_BUFF].StatusFlags.AllFlags);
				if ((buff->sorting_on) && (buff->filter_on))
				{
					for (j=0; j<NUM_OF_CHAN; j++)
						fprintf(fp_sorted, " %d\t", buff->sorted_spike_data[back+i-NUM_OF_SAMP_IN_BUFF].data[j]);
					fprintf(fp_sorted, " %d\t", buff->Environment[back+i-NUM_OF_SAMP_IN_BUFF].Status.AllStatus);	
					fprintf(fp_sorted, " %d\t", buff->Environment[back+i-NUM_OF_SAMP_IN_BUFF].ShortInt_Status0);	
					fprintf(fp_sorted, " %d\n", buff->RTStatusFlags[back+i-NUM_OF_SAMP_IN_BUFF].StatusFlags.AllFlags);			
				}								
			}
			else
			{
				for (j=0; j<NUM_OF_CHAN; j++)
					fprintf(fp, " %.0f\t", buff->scan[back+i].data[j]);
				fprintf(fp, " %d\t", buff->Environment[back+i].Status.AllStatus);
				fprintf(fp, " %d\t", buff->Environment[back+i].ShortInt_Status0);
				fprintf(fp, " %d\n", buff->RTStatusFlags[back+i].StatusFlags.AllFlags);
				if ((buff->sorting_on) && (buff->filter_on))
				{
					for (j=0; j<NUM_OF_CHAN; j++)
						fprintf(fp_sorted, " %d\t", buff->sorted_spike_data[back+i].data[j]);
					fprintf(fp_sorted, " %d\t", buff->Environment[back+i].Status.AllStatus);
					fprintf(fp_sorted, " %d\t", buff->Environment[back+i].ShortInt_Status0);
					fprintf(fp_sorted, " %d\n", buff->RTStatusFlags[back+i].StatusFlags.AllFlags);				
				}														
			}
		}	
	}
	back = front;

	return TRUE;  

}

gboolean filter_button_func (GtkDatabox * box)
{
	if (buff->filter_on)
	{
		buff->filter_on = 0;
		gtk_button_set_label (filter_button,"Turn Filter ON");
	}
	else
	{
		buff->filter_on = 1;
		gtk_button_set_label (filter_button,"Turn Filter OFF");
	}
		
}

gboolean highpass_4th_button_func (GtkDatabox * box)
{
	if (buff->highpass_4th_on)
	{
		gtk_button_set_label (highpass_4th_button,"Turn 4th Order ON");
		buff->highpass_4th_on = 0;
	}
	else
	{
		gtk_button_set_label (highpass_4th_button,"Turn 4th Order OFF");
		buff->highpass_4th_on = 1;
	}
		
}

gboolean ch_slct_func (GtkDatabox * box)
{

	float *Y_local;
	int i,j;

	for (i=0;i<SPIKE_MEM_TO_DISPLAY;i++)
	{
		Y_local = g_ptr_array_index(Y_spikes_ptr,i);
		for (j=0; j<NUM_OF_SAMP_PER_SPIKE; j++)
		{
			Y_local[j] = 0;
		}
	}

	gtk_databox_set_total_limits (GTK_DATABOX (box_spike_shape), 0, NUM_OF_SAMP_PER_SPIKE, +2200, -2200);	


	if (gtk_combo_box_get_active (GTK_COMBO_BOX(combo)) >= 0)
	{
		char thres[20];
		strThreshold = thres;
		disp_chan = gtk_combo_box_get_active (GTK_COMBO_BOX(combo));
		sprintf(strThreshold, "%.0f" , buff->Threshold[disp_chan]);
		gtk_entry_set_text (GTK_ENTRY(entryThreshold), strThreshold);
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
}

gboolean record_but_func (GtkDatabox * box)
{
	time_t rawtime;
	struct tm * timeinfo;
	time ( &rawtime );
	timeinfo = localtime (&rawtime);
	char * time_str = asctime (timeinfo);
	time_str[7]='_';
	time_str[10]='_';
	time_str[13]='_';
	time_str[16]='_';
	time_str[19]='_';
	time_str[24]='_';
	time_str[25]='_';

	char strFileName[100];
	
	if (!rec_data)
	{
		strcpy(strFileName, "/home/kocaturk/SPIKE_DATA/");	
		strcat(strFileName, time_str+4);
		strcat(strFileName, strAddFileName);
		fp = fopen(strFileName, "w");
		
		strcpy(strFileName, "/home/kocaturk/SPIKE_DATA/Sorted/");	
		strcat(strFileName, time_str+4);
		strcat(strFileName, strAddFileName);
		fp_sorted = fopen(strFileName, "w");

		gtk_button_set_label (record_button,"Release");
		rec_data = 1;
	}
	else if (rec_data)
	{
		rec_data = 0;	
		if (fp != NULL)
			fclose(fp);
		fp = NULL;	
		if (fp_sorted != NULL)
			fclose(fp_sorted);
		fp_sorted = NULL;
		gtk_button_set_label (record_button,"Record");		
	}
	return TRUE;
}

gboolean name_file_but_func (GtkDatabox * box)
{
	strAddFileName = gtk_entry_get_text(GTK_ENTRY(entryAddFileName));
	return TRUE;	
}

gboolean threshold_but_func (GtkDatabox * box)
{
	strThreshold=gtk_entry_get_text(GTK_ENTRY(entryThreshold));
	buff->Threshold[disp_chan]=atof(strThreshold);
	return TRUE;	
}

gboolean clear_screen_but_func (GtkDatabox * box)
{
	float *Y_local;
	int i,j;

	for (i=0;i<SPIKE_MEM_TO_DISPLAY;i++)
	{
		Y_local = g_ptr_array_index(Y_spikes_ptr,i);
		for (j=0; j<NUM_OF_SAMP_PER_SPIKE; j++)
		{
			Y_local[j] = 0;
		}
	}

	gtk_databox_set_total_limits (GTK_DATABOX (box_spike_shape), 0, NUM_OF_SAMP_PER_SPIKE, +2200, -2200);	

	return TRUE;	
}

void update_jitter_monitor (void)
{
	char str [20];
	int i, size_jit;

	jitter_buff_front = buff->jitter_idx;

	if (jitter_buff_front < jitter_buff_back)
		size_jit = jitter_buff_front + 1000 - jitter_buff_back;
	else
		size_jit = jitter_buff_front - jitter_buff_back;	


	if (size_jit == 0)
		return;  
		
	for (i = 0; i < size_jit; i++)
	{			
		if ((jitter_buff_back+i) >= 1000)
		{
			if (abs(buff->jitter[jitter_buff_back+i-1000]-1000000) > 100000)
				  num_jitter_100_us++;
			else if (abs(buff->jitter[jitter_buff_back+i-1000]-1000000) > 50000)
				num_jitter_50_us++;
			else if (abs(buff->jitter[jitter_buff_back+i-1000]-1000000) > 20000)
				num_jitter_20_us++;
		}
		else
		{
			if (abs(buff->jitter[jitter_buff_back+i]-1000000) > 100000)
				  num_jitter_100_us++;
			else if (abs(buff->jitter[jitter_buff_back+i]-1000000) > 50000)
				num_jitter_50_us++;
			else if (abs(buff->jitter[jitter_buff_back+i]-1000000) > 20000)
				num_jitter_20_us++;

		}
	}
	sprintf(str, "%d" , (buff->jitter[jitter_buff_back] - 1000000));
	gtk_label_set_text(lbl_jitter, str);
	
	jitter_buff_back = jitter_buff_front;

	sprintf(str, "%d" , num_jitter_100_us);
	gtk_label_set_text( lbl_jitter_100_us,str);
	sprintf(str, "%d" , num_jitter_50_us);
	gtk_label_set_text( lbl_jitter_50_us,str);
	sprintf(str, "%d" , num_jitter_20_us);
	gtk_label_set_text( lbl_jitter_20_us,str);

	return;
}
