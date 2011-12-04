/***************************************************************************
                          SpikeViewer.c  -  description
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

#include "SpikeViewer.h"

int main( int argc, char *argv[])
{
   	shared_memory = (SharedMemStruct*)rtai_malloc(nam2num(SHARED_MEM_NAME), SHARED_MEM_SIZE);
	if (shared_memory == NULL)
	{
		printf("rtai_malloc() failed (maybe /dev/rtai_shm is missing)!\n");
		return -1;
   	}
	gtk_init(&argc, &argv);
	create_gui(); 	
	gtk_main();

	return 0;
}

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
	
	display_mwa = 0;
	display_mwa_chan = 0;
	disp_paused = 0;
	print_spike_end_buff = 0;
	
	int i;
	X_raw = g_new0 (float, NUM_OF_RAW_SAMPLE_TO_DISPLAY);
	Y_raw = g_new0 (float, NUM_OF_RAW_SAMPLE_TO_DISPLAY);
	for (i=0; i<NUM_OF_RAW_SAMPLE_TO_DISPLAY; i++)
	{
		X_raw[i] = ((float)i)*(RAW_DATA_DISP_DURATION_MS/NUM_OF_RAW_SAMPLE_TO_DISPLAY);
	}	
	
	float *X_spike;
	float *Y_spike;
	X_spike = g_new0 (float, NUM_OF_SAMP_PER_SPIKE);
	for (i=0; i<NUM_OF_SAMP_PER_SPIKE; i++)
	{
		X_spike[i] = (float)i;
	}
	Y_spikes_idx = 0;
	Y_spikes_ptr = g_ptr_array_new();
	for (i=0;i<SPIKE_MEM_TO_DISPLAY;i++)
	{
		Y_spike = g_new0 (float, NUM_OF_SAMP_PER_SPIKE);
		g_ptr_array_add  (Y_spikes_ptr, Y_spike);
	}

	GtkWidget *window;
	GtkWidget *hbox, *vbox, *lbl;

 	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  	gtk_window_set_default_size(GTK_WINDOW(window), 1600, 900);
  	gtk_window_set_title(GTK_WINDOW(window), "SpikeViewer");
  	gtk_container_set_border_width(GTK_CONTAINER(window), 10);


	main_table= gtk_table_new(9, 16, TRUE);     // 9 rows   16 columns
	gtk_container_add(GTK_CONTAINER(window), main_table);

	//   Buttons and Combos
	
	vbox = gtk_vbox_new(FALSE, 0);
   	gtk_table_attach_defaults(GTK_TABLE(main_table), vbox, 12,15, 6, 9);      // Put adjustments the rightmost

  	hbox = gtk_hbox_new(FALSE, 0);
  	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 10);
  	
  	hbox = gtk_hbox_new(FALSE, 0);
  	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);

        lbl = gtk_label_new("Microwire Array : ");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, TRUE,TRUE, 0);

 	combo_mwa = gtk_combo_box_new_text();
 	char temp[5];	
	for (i=0; i<MAX_NUM_OF_MWA; i++)
	{
		sprintf(temp, "%d" , i);
	 	gtk_combo_box_append_text(GTK_COMBO_BOX(combo_mwa), temp);
	} 		
 	gtk_box_pack_start (GTK_BOX (hbox), combo_mwa, TRUE, TRUE, 0);
 
 	combo_chan = gtk_combo_box_new_text();
	for (i=0; i<MAX_NUM_OF_CHAN_PER_MWA; i++)
	{
		sprintf(temp, "%d" , i);
	 	gtk_combo_box_append_text(GTK_COMBO_BOX(combo_chan), temp);
	} 
	gtk_box_pack_start (GTK_BOX (hbox), combo_chan, TRUE, TRUE, 0);
 	
        lbl = gtk_label_new(": Channel");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, TRUE,TRUE, 0);
 
 
    	hbox = gtk_hbox_new(FALSE, 0);
  	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 10);
  	
    	hbox = gtk_hbox_new(FALSE, 0);
  	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE,0);
  	
        lbl = gtk_label_new("Threshold (mV): ");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE, FALSE, 0);

        entryThreshold = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox),entryThreshold, FALSE,FALSE,0);

	char thres[20];
	sprintf(thres, "%.2f" , shared_memory->spike_thresholding.amplitude_thres[display_mwa][display_mwa_chan]);
	gtk_entry_set_text (GTK_ENTRY(entryThreshold), thres);

    	hbox = gtk_hbox_new(FALSE, 0);
  	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE,0);
  	
	threshold_button = gtk_button_new_with_label("Submit Threshold");
	gtk_box_pack_start (GTK_BOX (hbox), threshold_button, TRUE, TRUE, 0);	
  	
   	hbox = gtk_hbox_new(FALSE, 0);
  	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 10);
  	
   	hbox = gtk_hbox_new(FALSE, 0);
  	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE,0);
        
	pause_button = gtk_button_new_with_label("Pause Display");
	gtk_box_pack_start (GTK_BOX (hbox), pause_button, TRUE, TRUE, 0);

	clear_button = gtk_button_new_with_label("Clear Spike Screen");
	gtk_box_pack_start (GTK_BOX (hbox), clear_button, TRUE, TRUE, 0);

   	hbox = gtk_hbox_new(FALSE, 0);
  	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 10);
  	
    	hbox = gtk_hbox_new(FALSE, 0);
  	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0); 
  	
 	btn_print_spike_end_buff = gtk_button_new_with_label("Enable Spikes Log Printing");
	gtk_box_pack_start (GTK_BOX (hbox), btn_print_spike_end_buff, TRUE, TRUE, 0); 		

  	hbox = gtk_hbox_new(FALSE, 0);
  	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 10);

   	hbox = gtk_hbox_new(FALSE, 0);
  	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);
  		
	btn_filter_highpass_150Hz = gtk_button_new_with_label("Turn HP 150Hz ON");
	gtk_box_pack_start (GTK_BOX (hbox), btn_filter_highpass_150Hz, TRUE, TRUE, 0);

	btn_filter_highpass_400Hz = gtk_button_new_with_label("Turn HP 400Hz LP ON");
	gtk_box_pack_start (GTK_BOX (hbox), btn_filter_highpass_400Hz, TRUE, TRUE, 0);	
	
  	hbox = gtk_hbox_new(FALSE, 0);
  	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);	
  	
 	btn_filter_lowpass_8KHz = gtk_button_new_with_label("Turn LP 8KHz ON");
	gtk_box_pack_start (GTK_BOX (hbox), btn_filter_lowpass_8KHz, TRUE, TRUE, 0); 	
	
	if (shared_memory->kernel_task_ctrl.highpass_150Hz_on)
	{
		gtk_button_set_label (GTK_BUTTON(btn_filter_highpass_150Hz),"Turn HP 150Hz OFF");
	}
	if (shared_memory->kernel_task_ctrl.highpass_400Hz_on)
	{
		gtk_button_set_label (GTK_BUTTON(btn_filter_highpass_400Hz),"Turn HP 400Hz OFF");
	}
	if (shared_memory->kernel_task_ctrl.lowpass_8KHz_on)
	{
		gtk_button_set_label (GTK_BUTTON(btn_filter_lowpass_8KHz),"Turn LP 8KHz OFF");
	}	
  	
	// Raw Data Plot 
	
  	gtk_databox_create_box_with_scrollbars_and_rulers (&box_signal, &databox_signal,TRUE, TRUE, TRUE, TRUE);
  	gtk_widget_modify_bg (box_signal, GTK_STATE_NORMAL, &color_bg_signal);	
   	gtk_databox_graph_add (GTK_DATABOX (box_signal), gtk_databox_grid_new (21, 21, &color_grid_signal, 0));
	gtk_databox_graph_add (GTK_DATABOX (box_signal), gtk_databox_lines_new (NUM_OF_RAW_SAMPLE_TO_DISPLAY, X_raw, Y_raw, &color_signal, 0));  	
   	gtk_table_attach_defaults(GTK_TABLE(main_table), databox_signal, 0,11,0,9);      
	gtk_databox_set_total_limits (GTK_DATABOX (box_signal), 0, RAW_DATA_DISP_DURATION_MS, HIGHEST_VOLTAGE_MV, LOWEST_VOLTAGE_MV);	

	// Spike Shape Plot
	
	gtk_databox_create_box_with_scrollbars_and_rulers (&box_spike_shape, &databox_spike_shape,TRUE, TRUE, TRUE, TRUE);
  	gtk_widget_modify_bg (box_spike_shape, GTK_STATE_NORMAL, &color_bg_spike_shape);
	for (i=0;i<SPIKE_MEM_TO_DISPLAY;i++)
	{
		Y_spike = g_ptr_array_index(Y_spikes_ptr,i);
		gtk_databox_graph_add (GTK_DATABOX (box_spike_shape), gtk_databox_lines_new (NUM_OF_SAMP_PER_SPIKE, X_spike, Y_spike, &color_spike_shape, 0));
	}
   	gtk_table_attach_defaults(GTK_TABLE(main_table), databox_spike_shape, 11,16,0,6);      
	gtk_databox_set_total_limits (GTK_DATABOX (box_spike_shape), 0, NUM_OF_SAMP_PER_SPIKE-1, HIGHEST_VOLTAGE_MV , LOWEST_VOLTAGE_MV);	


 	gtk_widget_show_all(window);
 	
	spike_time_stamp_buff_read_idx = shared_memory->spike_time_stamp.buff_idx_write;

  	g_signal_connect (G_OBJECT (window), "destroy", G_CALLBACK (gtk_main_quit), NULL);
	g_signal_connect(G_OBJECT(btn_filter_highpass_150Hz), "clicked", G_CALLBACK(filter_highpass_150Hz_button_func), NULL);
	g_signal_connect(G_OBJECT(btn_filter_highpass_400Hz), "clicked", G_CALLBACK(filter_highpass_400Hz_button_func), NULL);
	g_signal_connect(G_OBJECT(btn_filter_lowpass_8KHz), "clicked", G_CALLBACK(filter_lowpass_8KHz_button_func), NULL);
	g_signal_connect(G_OBJECT(combo_mwa), "changed", G_CALLBACK(combo_mwa_func), NULL);
	g_signal_connect(G_OBJECT(combo_chan), "changed", G_CALLBACK(combo_chan_func), NULL);
	g_signal_connect(G_OBJECT(pause_button), "clicked", G_CALLBACK(pause_button_func), NULL);
	g_signal_connect(G_OBJECT(threshold_button), "clicked", G_CALLBACK(threshold_but_func), NULL);
	g_signal_connect(G_OBJECT(clear_button), "clicked", G_CALLBACK(clear_screen_but_func), NULL);
	g_signal_connect(G_OBJECT(btn_print_spike_end_buff), "clicked", G_CALLBACK(print_spike_end_buff_button_func), NULL);	

	g_timeout_add(50, timeout_callback, box_signal);


	return;
}

gboolean timeout_callback(gpointer user_data) 
{
	int start_idx, i;
	int spike_time_stamp_buff_end_idx, idx;
	int spike_time_stamp_buff_mwa, spike_time_stamp_buff_chan, spike_time_stamp_buff_recording_data_idx; 
	long long unsigned int spike_time_stamp_buff_peak_time;
	int spike_idx;
	float *Y_temp_spike;
	RecordingData	*handling_data;
	RecordingDataChanBuff	*handling_data_chan_buff;
	SpikeTimeStamp *spike_time_stamp;
	
	if ((shared_memory->mwa_daq_map[display_mwa][display_mwa_chan].daq_card == MAX_NUM_OF_DAQ_CARD) || (shared_memory->mwa_daq_map[display_mwa][display_mwa_chan].daq_chan == MAX_NUM_OF_CHANNEL_PER_DAQ_CARD))  // non-cinfigured channel. Do not plot
		return TRUE;
	
	if (shared_memory->kernel_task_ctrl.highpass_150Hz_on || shared_memory->kernel_task_ctrl.highpass_400Hz_on)
	{
		handling_data = &shared_memory->filtered_recording_data;
	}
	else
	{
		handling_data = &shared_memory->recording_data;
	}
	handling_data_chan_buff = &(handling_data->recording_data_buff[display_mwa][display_mwa_chan]);
	spike_time_stamp = &shared_memory->spike_time_stamp;	
			
	start_idx = handling_data->buff_idx_write[display_mwa][display_mwa_chan] / NUM_OF_RAW_SAMPLE_TO_DISPLAY;   // Handle previous NUM_OF_RAW_SAMPLE_TO_DISPLAY

	if (start_idx == 0)
		 start_idx = RECORDING_DATA_BUFF_SIZE - NUM_OF_RAW_SAMPLE_TO_DISPLAY;    // read previous section
	else
		start_idx = (start_idx*NUM_OF_RAW_SAMPLE_TO_DISPLAY) - NUM_OF_RAW_SAMPLE_TO_DISPLAY;  // read previous section
			
	if ((start_idx != previous_start_idx_to_plot) && (!disp_paused))  // Do not plot the same section if it is the same as the previous one due to high refresh rate (high timeout_callback frequency)
	{	
		previous_start_idx_to_plot = start_idx;
		for (i = 0; i < NUM_OF_RAW_SAMPLE_TO_DISPLAY; i++)
		{
			Y_raw[i] = (*handling_data_chan_buff)[i+start_idx];
		}		
		gtk_databox_set_total_limits (GTK_DATABOX (box_signal), 0, RAW_DATA_DISP_DURATION_MS, HIGHEST_VOLTAGE_MV , LOWEST_VOLTAGE_MV);	
	}
	
	idx = spike_time_stamp_buff_read_idx;
	spike_time_stamp_buff_end_idx = shared_memory->spike_time_stamp.buff_idx_write;
	while (idx != spike_time_stamp_buff_end_idx)
	{
		spike_time_stamp_buff_recording_data_idx = spike_time_stamp->spike_timestamp_buff[idx].recording_data_buff_idx;
		spike_time_stamp_buff_mwa = spike_time_stamp->spike_timestamp_buff[idx].mwa;
		spike_time_stamp_buff_chan = spike_time_stamp->spike_timestamp_buff[idx].channel;
		spike_time_stamp_buff_peak_time = spike_time_stamp->spike_timestamp_buff[idx].peak_time;			
		spike_idx = spike_time_stamp_buff_recording_data_idx;
		if ((spike_time_stamp_buff_mwa == display_mwa) && (spike_time_stamp_buff_chan == display_mwa_chan))
		{
			Y_temp_spike = g_ptr_array_index(Y_spikes_ptr,Y_spikes_idx);
			Y_spikes_idx ++;
			if (Y_spikes_idx == SPIKE_MEM_TO_DISPLAY)
				Y_spikes_idx = 0;
			for (i = NUM_OF_SAMP_PER_SPIKE -1; i >= 0; i--)
			{
				Y_temp_spike[i] = (*handling_data_chan_buff)[spike_idx];
				spike_idx--;
				if (spike_idx < 0)
					spike_idx	= RECORDING_DATA_BUFF_SIZE - 1;
			}
			gtk_databox_set_total_limits (GTK_DATABOX (box_spike_shape), 0, NUM_OF_SAMP_PER_SPIKE-1, HIGHEST_VOLTAGE_MV , LOWEST_VOLTAGE_MV);
		}
		if (print_spike_end_buff)
			printf ("%d %d %llu %d\n", spike_time_stamp_buff_mwa, spike_time_stamp_buff_chan, spike_time_stamp_buff_peak_time, spike_time_stamp_buff_recording_data_idx);	
		idx++;	
		if (idx ==	SPIKE_END_DATA_BUFF_SIZE)
			idx = 0;	
	}
	spike_time_stamp_buff_read_idx = spike_time_stamp_buff_end_idx;
	return TRUE;  

}

gboolean filter_highpass_150Hz_button_func (GtkDatabox * box)
{
	if (shared_memory->kernel_task_ctrl.highpass_150Hz_on)
	{
		while (!(shared_memory->kernel_task_ctrl.kernel_task_idle)) { printf("in while\n"); }
		shared_memory->kernel_task_ctrl.highpass_150Hz_on = 0;	
		gtk_button_set_label (GTK_BUTTON(btn_filter_highpass_150Hz),"Turn HP 150Hz ON");
	}
	else
	{
		while (!(shared_memory->kernel_task_ctrl.kernel_task_idle)) { printf("in while\n"); }								
		shared_memory->kernel_task_ctrl.highpass_150Hz_on = 1;	
		shared_memory->kernel_task_ctrl.highpass_400Hz_on = 0;		
		gtk_button_set_label (GTK_BUTTON(btn_filter_highpass_150Hz),"Turn HP 150Hz OFF");
		gtk_button_set_label (GTK_BUTTON(btn_filter_highpass_400Hz),"Turn HP 400Hz ON");		
	}
	return TRUE;	
}

gboolean filter_highpass_400Hz_button_func (GtkDatabox * box)
{
	if (shared_memory->kernel_task_ctrl.highpass_400Hz_on)
	{
		while (!(shared_memory->kernel_task_ctrl.kernel_task_idle)) { printf("in while\n"); }										
		shared_memory->kernel_task_ctrl.highpass_400Hz_on = 0;		
		gtk_button_set_label (GTK_BUTTON(btn_filter_highpass_400Hz),"Turn HP 400Hz ON");
	}
	else
	{
		while (!(shared_memory->kernel_task_ctrl.kernel_task_idle)) { printf("in while\n"); }																					
		shared_memory->kernel_task_ctrl.highpass_400Hz_on = 1;		
		shared_memory->kernel_task_ctrl.highpass_150Hz_on = 0;	
		gtk_button_set_label (GTK_BUTTON(btn_filter_highpass_400Hz),"Turn HP 400Hz OFF");		
		gtk_button_set_label (GTK_BUTTON(btn_filter_highpass_150Hz),"Turn HP 150Hz ON");
	}
	return TRUE;		
}

gboolean filter_lowpass_8KHz_button_func (GtkDatabox * box)
{
	if (shared_memory->kernel_task_ctrl.lowpass_8KHz_on)
	{
		while (!(shared_memory->kernel_task_ctrl.kernel_task_idle)) { printf("in while\n"); }										
		shared_memory->kernel_task_ctrl.lowpass_8KHz_on = 0;
		gtk_button_set_label (GTK_BUTTON(btn_filter_lowpass_8KHz),"Turn LP 8KHz ON");
	}
	else
	{
		if ((shared_memory->kernel_task_ctrl.highpass_150Hz_on) || (shared_memory->kernel_task_ctrl.highpass_400Hz_on))
		{
			while (!(shared_memory->kernel_task_ctrl.kernel_task_idle)) { printf("in while\n"); }															
			shared_memory->kernel_task_ctrl.lowpass_8KHz_on = 1;
			gtk_button_set_label (GTK_BUTTON(btn_filter_lowpass_8KHz),"Turn LP 8KHz OFF");
		}
		else
		{
			printf("SpikeViewer:\n");	
			printf("WARNING: Cannot turn 8 KHz Low-pass filter on\n");
			printf("WARNNG: First turn one High-pass filter on\n");			
		}
	}	
	return TRUE;		
}

gboolean combo_mwa_func (GtkDatabox * box)
{
	int idx;
	char thres[20];
	idx = gtk_combo_box_get_active (GTK_COMBO_BOX(combo_mwa));
	if ((idx < 0) || (idx >= MAX_NUM_OF_MWA))
	{
		printf("SpikeViewer:\n");	
		printf ("BUG: Selected combobox index from MWA Drop-Down list is %d\n", idx);
		printf ("BUG: 0th Microwire Array selected automatically\n");
		idx = 0;
	}
	display_mwa = idx;
	if ((shared_memory->mwa_daq_map[display_mwa][display_mwa_chan].daq_card == MAX_NUM_OF_DAQ_CARD) || (shared_memory->mwa_daq_map[display_mwa][display_mwa_chan].daq_chan == MAX_NUM_OF_CHANNEL_PER_DAQ_CARD))  // non-cinfigured channel.
	{
		printf("SpikeViewer:\n");	
		printf("ERROR: The selected mwa-channel was not mapped to any DAQ Card Channel\n");
		printf("ERROR: No data will be plotted.\n");
	}

	sprintf(thres, "%.2f" , shared_memory->spike_thresholding.amplitude_thres[display_mwa][display_mwa_chan]);
	gtk_entry_set_text (GTK_ENTRY(entryThreshold), thres);	
	clear_spike_screen();
	clear_raw_data_screen();		
	return TRUE;	
}
gboolean combo_chan_func (GtkDatabox * box)
{
	int idx;
	char thres[20];	
	idx = gtk_combo_box_get_active (GTK_COMBO_BOX(combo_chan));
	if ((idx < 0) || (idx >= MAX_NUM_OF_CHAN_PER_MWA))
	{
		printf("SpikeViewer:\n");	
		printf ("BUG: Selected combobox index from MWA Channel Drop-Down list is %d\n", idx);
		printf ("BUG: 0th Microwire Array selected automatically\n");
		idx = 0;
	}
	display_mwa_chan = idx;	
	if ((shared_memory->mwa_daq_map[display_mwa][display_mwa_chan].daq_card == MAX_NUM_OF_DAQ_CARD) || (shared_memory->mwa_daq_map[display_mwa][display_mwa_chan].daq_chan == MAX_NUM_OF_CHANNEL_PER_DAQ_CARD))  // non-cinfigured channel.
	{
		printf("SpikeViewer:\n");	
		printf("ERROR: The selected mwa-channel was not mapped to any DAQ Card Channel\n");
		printf("ERROR: No data will be plotted.\n");
	}	
	sprintf(thres, "%.2f" , shared_memory->spike_thresholding.amplitude_thres[display_mwa][display_mwa_chan]);
	gtk_entry_set_text (GTK_ENTRY(entryThreshold), thres);	
	clear_spike_screen();
	clear_raw_data_screen();		
	return TRUE;	
}

gboolean pause_button_func (GtkDatabox * box)
{
	if (disp_paused)
	{
		disp_paused = 0;
		gtk_button_set_label (GTK_BUTTON(pause_button),"Pause");
	}
	else
	{
		disp_paused = 1;
		gtk_button_set_label (GTK_BUTTON(pause_button),"Resume");
	}	
	return TRUE;	
}

gboolean print_spike_end_buff_button_func (GtkDatabox * box)
{
	if (print_spike_end_buff)
	{
		print_spike_end_buff = 0;
		gtk_button_set_label (GTK_BUTTON(btn_print_spike_end_buff),"Enable Spikes Log Printing");
		printf("SpikeViewer: Spike End Buffer log printing disabled\n");
	}
	else
	{
		print_spike_end_buff = 1;
		gtk_button_set_label (GTK_BUTTON(btn_print_spike_end_buff),"Disable Spikes Log Printing");
		printf("SpikeViewer: Spike End Buffer log printing enabled\n");
		printf ("Data Format:  [mwa] [channel] [spike_peak_time] [spike_end_index_in_recording_data_buffer]");	
	}	
	return TRUE;	
}

gboolean threshold_but_func (GtkDatabox * box)
{
	float threshold = atof(gtk_entry_get_text(GTK_ENTRY(entryThreshold)));
	if (threshold <= 0.0)
	{
		while (!(shared_memory->kernel_task_ctrl.kernel_task_idle)) { printf("in while\n"); }
		shared_memory->spike_thresholding.amplitude_thres[display_mwa][display_mwa_chan]=threshold;
		if (threshold == 0.0)
		{
			printf("Spike detection is disable for this channel by applying 0.0 Volts as threshold\n");		
		}
	}
	else
	{
		printf("WARNING: Threshold cannot be greater than 0.0\n");
		printf("WARNING: Submit Threshold cancelled\n");		
	}
	return TRUE;
}

gboolean clear_screen_but_func (GtkDatabox * box)
{
	clear_spike_screen();
	clear_raw_data_screen();
	return TRUE;	
}

void clear_spike_screen(void)
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
	gtk_databox_set_total_limits (GTK_DATABOX (box_spike_shape), 0, NUM_OF_SAMP_PER_SPIKE-1, HIGHEST_VOLTAGE_MV , LOWEST_VOLTAGE_MV);	
}

void clear_raw_data_screen(void)
{
	int i;

	for (i = 0; i < NUM_OF_RAW_SAMPLE_TO_DISPLAY; i++)
	{
		Y_raw[i] = 0;
	}		
	gtk_databox_set_total_limits (GTK_DATABOX (box_signal), 0, RAW_DATA_DISP_DURATION_MS, HIGHEST_VOLTAGE_MV , LOWEST_VOLTAGE_MV);
}
