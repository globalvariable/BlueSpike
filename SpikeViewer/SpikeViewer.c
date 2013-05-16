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

static RtTasksData			*rt_tasks_data = NULL;
static RecordingData			*recording_data = NULL;
static RecordingData			*filtered_recording_data = NULL;
static SpikeThresholding		*spike_thresholding = NULL;
static BlueSpikeTimeStamp 		*blue_spike_time_stamp = NULL;
static SpikeTimeStamp 			*spike_time_stamps = NULL;
static FilterCtrl				*filter_ctrl = NULL;
static FiltCtrl2KrnlSpkMsg		*filter_ctrl_2_kernel_spike_msgs = NULL;
static SpkThres2KrnlSpkMsg	*spike_thres_2_kernel_spike_msgs = NULL;

static int blue_spike_time_stamp_buff_size = BLUE_SPIKE_TIME_STAMP_BUFF_SIZE;

int main( int argc, char *argv[])
{
	cpu_set_t  mask;
	CPU_ZERO(&mask);
	CPU_SET(BLUE_SPIKE_USER_SPACE_CPU_ID*MAX_NUM_OF_CPU_THREADS_PER_CPU+BLUE_SPIKE_USER_SPACE_CPU_THREAD_ID, &mask);
	printf ("sched_getcpu() = %d (Before sched_setaffinity)\n", sched_getcpu());
	if (sched_setaffinity(0, sizeof(mask), &mask))
		return print_message(ERROR_MSG ,"SpikeViewer", "Gui", "main","! sched_setaffinity(0, sizeof(mask), &mask).");		
	printf ("sched_getcpu() = %d (After sched_setaffinity)\n", sched_getcpu());

   	rt_tasks_data = rtai_malloc(SHM_NUM_RT_TASKS_DATA, 0);
	if (rt_tasks_data == NULL) 
		return print_message(ERROR_MSG ,"SpikeViewer", "SpikeViewer", "main", "rt_tasks_data == NULL.");
	recording_data = (RecordingData*)rtai_malloc(SHM_NUM_KERNEL_SPIKE_RECORDING_DATA, 0);
	if (recording_data == NULL) 
		return print_message(ERROR_MSG ,"SpikeViewer", "SpikeViewer", "main", "recording_data == NULL.");
	filtered_recording_data = (RecordingData*)rtai_malloc(SHM_NUM_KERNEL_SPIKE_FILTERED_RECORDING_DATA, 0);
	if (filtered_recording_data == NULL) 
		return print_message(ERROR_MSG ,"SpikeViewer", "SpikeViewer", "main", "filtered_recording_data == NULL.");
	spike_thresholding = (SpikeThresholding*)rtai_malloc(SHM_NUM_KERNEL_SPIKE_SPIKE_THRESHOLDING, 0);
	if (spike_thresholding == NULL) 
		return print_message(ERROR_MSG ,"SpikeViewer", "SpikeViewer", "main", "spike_thresholding == NULL.");
	blue_spike_time_stamp = (BlueSpikeTimeStamp*)rtai_malloc(SHM_NUM_KERNEL_SPIKE_BLUE_SPIKE_TIME_STAMP, 0);
	if (blue_spike_time_stamp == NULL) 
		return print_message(ERROR_MSG ,"SpikeViewer", "SpikeViewer", "main", "blue_spike_time_stamp == NULL.");
   	spike_time_stamps = rtai_malloc(SHM_NUM_KERNEL_SPIKE_SPIKE_TIME_STAMP, 0);
	if (spike_time_stamps == NULL) 
		return print_message(ERROR_MSG ,"SpikeViewer", "SpikeViewer", "main", "spike_time_stamps == NULL.");
	filter_ctrl = (FilterCtrl*)rtai_malloc(SHM_NUM_KERNEL_SPIKE_FILTER_CTRL, 0);
	if (filter_ctrl == NULL) 
		return print_message(ERROR_MSG ,"SpikeViewer", "SpikeViewer", "main", "filter_ctrl == NULL.");
	filter_ctrl_2_kernel_spike_msgs = allocate_shm_client_filter_ctrl_2_kernel_spike_msg_buffer(filter_ctrl_2_kernel_spike_msgs);
	if (filter_ctrl_2_kernel_spike_msgs == NULL) 
		return print_message(ERROR_MSG ,"SpikeViewer", "SpikeViewer", "main", "filter_ctrl_2_kernel_spike_msgs == NULL.");
	spike_thres_2_kernel_spike_msgs = allocate_shm_client_spike_thres_2_kernel_spike_msg_buffer(spike_thres_2_kernel_spike_msgs);
	if (spike_thres_2_kernel_spike_msgs == NULL) 
		return print_message(ERROR_MSG ,"SpikeViewer", "SpikeViewer", "main", "spike_thres_2_kernel_spike_msgs == NULL.");


	initialize_data_read_write_handlers_for_recording();
	initialize_data_read_write_handlers();

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
  	gtk_window_set_default_size(GTK_WINDOW(window), 1920, 1020);
  	gtk_window_set_title(GTK_WINDOW(window), "SpikeViewer");
  	gtk_container_set_border_width(GTK_CONTAINER(window), 10);


	main_table= gtk_table_new(9, 16, TRUE);     // 9 rows   16 columns
	gtk_container_add(GTK_CONTAINER(window), main_table);

	//   Buttons and Combos
	
	vbox = gtk_vbox_new(FALSE, 0);
   	gtk_table_attach_defaults(GTK_TABLE(main_table), vbox, 12,15, 6, 9);      // Put adjustments the rightmost

  	hbox = gtk_hbox_new(FALSE, 0);
  	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 5);

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
 	gtk_combo_box_set_active(GTK_COMBO_BOX(combo_mwa), 0);
 	
 	combo_chan = gtk_combo_box_new_text();
	for (i=0; i<MAX_NUM_OF_CHAN_PER_MWA; i++)
	{
		sprintf(temp, "%d" , i);
	 	gtk_combo_box_append_text(GTK_COMBO_BOX(combo_chan), temp);
	} 
	gtk_box_pack_start (GTK_BOX (hbox), combo_chan, TRUE, TRUE, 0);
 	gtk_combo_box_set_active(GTK_COMBO_BOX(combo_chan), 0);
 	 	
        lbl = gtk_label_new(": Channel");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, TRUE,TRUE, 0);
 
 
    	hbox = gtk_hbox_new(FALSE, 0);
  	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 5);
  	
    	hbox = gtk_hbox_new(FALSE, 0);
  	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE,0);
  	
        lbl = gtk_label_new("Threshold (mV): ");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE, FALSE, 0);

        entryThreshold = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox),entryThreshold, FALSE,FALSE,0);
	gtk_widget_set_size_request(entryThreshold, 80, 25);

	char thres[20];
	sprintf(thres, "%.2f" , spike_thresholding->amplitude_thres[display_mwa][display_mwa_chan]);
	gtk_entry_set_text (GTK_ENTRY(entryThreshold), thres);


	threshold_button = gtk_button_new_with_label("Submit");
	gtk_box_pack_start (GTK_BOX (hbox), threshold_button, TRUE, TRUE, 0);	

	hbox = gtk_hbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE,FALSE,0);	
	
	btn_select_spike_thresholds_file_to_load = gtk_file_chooser_button_new ("Select Spike Thresholds File", GTK_FILE_CHOOSER_ACTION_OPEN);
        gtk_box_pack_start(GTK_BOX(hbox),btn_select_spike_thresholds_file_to_load, TRUE,TRUE,0);
 	gtk_widget_set_size_request(btn_select_spike_thresholds_file_to_load, 120, 25);       
	set_directory_btn_select_directory_to_load();
	        
        btn_load_spike_thresholds_file = gtk_button_new_with_label("Load Thresholds");
        gtk_box_pack_start(GTK_BOX(hbox),btn_load_spike_thresholds_file,TRUE,TRUE, 0);	

	hbox = gtk_hbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE,FALSE,0);	

  	btn_select_folder_to_save_spike_thresholds_file = gtk_file_chooser_button_new ("Select Directory", GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER);
        gtk_box_pack_start(GTK_BOX(hbox), btn_select_folder_to_save_spike_thresholds_file, FALSE,FALSE,0);
	set_directory_btn_select_directory_to_save();

	btn_save_spike_thresholds_file = gtk_button_new_with_label("Save Thresholds File");
        gtk_box_pack_start(GTK_BOX(hbox),btn_save_spike_thresholds_file,TRUE,TRUE, 0);
          	
   	hbox = gtk_hbox_new(FALSE, 0);
  	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 5);
  	
   	hbox = gtk_hbox_new(FALSE, 0);
  	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE,0);
        
	pause_button = gtk_button_new_with_label("Pause Display");
	gtk_box_pack_start (GTK_BOX (hbox), pause_button, TRUE, TRUE, 0);

	clear_button = gtk_button_new_with_label("Clear Spike Screen");
	gtk_box_pack_start (GTK_BOX (hbox), clear_button, TRUE, TRUE, 0);

   	hbox = gtk_hbox_new(FALSE, 0);
  	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);
  	
    	hbox = gtk_hbox_new(FALSE, 0);
  	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0); 
  	
 	btn_print_spike_end_buff = gtk_button_new_with_label("Enable Spikes Log Printing");
	gtk_box_pack_start (GTK_BOX (hbox), btn_print_spike_end_buff, TRUE, TRUE, 0); 		

  	hbox = gtk_hbox_new(FALSE, 0);
  	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 5);

   	hbox = gtk_hbox_new(FALSE, 0);
  	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);
  		
	btn_filter_highpass_150Hz = gtk_button_new_with_label("HP 150Hz : OFF");
	gtk_box_pack_start (GTK_BOX (hbox), btn_filter_highpass_150Hz, TRUE, TRUE, 0);

	btn_filter_highpass_400Hz = gtk_button_new_with_label("HP 400Hz LP : OFF");
	gtk_box_pack_start (GTK_BOX (hbox), btn_filter_highpass_400Hz, TRUE, TRUE, 0);	
	
 	btn_filter_lowpass_8KHz = gtk_button_new_with_label("LP 8KHz : OFF");
	gtk_box_pack_start (GTK_BOX (hbox), btn_filter_lowpass_8KHz, TRUE, TRUE, 0); 	
	
	if (filter_ctrl->highpass_150Hz_on)
	{
		gtk_button_set_label (GTK_BUTTON(btn_filter_highpass_150Hz),"HP 150Hz : ON");
	}
	if (filter_ctrl->highpass_400Hz_on)
	{
		gtk_button_set_label (GTK_BUTTON(btn_filter_highpass_400Hz),"HP 400Hz : ON");
	}
	if (filter_ctrl->lowpass_8KHz_on)
	{
		gtk_button_set_label (GTK_BUTTON(btn_filter_lowpass_8KHz),"LP 8KHz : OFF");
	}	

  	hbox = gtk_hbox_new(FALSE, 0);
  	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 5);

	hbox = gtk_hbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE,FALSE,0);	

  	btn_select_folder_to_record_data = gtk_file_chooser_button_new ("Select Directory", GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER);
        gtk_box_pack_start(GTK_BOX(hbox), btn_select_folder_to_record_data, FALSE,FALSE,0);

	btn_create_recording_folder = gtk_button_new_with_label("Create Folder");
	gtk_box_pack_start (GTK_BOX (hbox), btn_create_recording_folder, TRUE, TRUE, 0);

	hbox = gtk_hbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE,FALSE,0);	

	btn_record_data = gtk_button_new_with_label("Record");
        gtk_box_pack_start(GTK_BOX(hbox),btn_record_data, TRUE,TRUE, 0);
  	
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
 	
	blue_spike_time_stamp_buff_read_idx = blue_spike_time_stamp->buff_idx_write;

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
	g_signal_connect(G_OBJECT(btn_load_spike_thresholds_file), "clicked", G_CALLBACK(load_spike_thresholds_file_button_func), NULL);	
	g_signal_connect(G_OBJECT(btn_save_spike_thresholds_file), "clicked", G_CALLBACK(save_spike_thresholds_file_button_func), NULL);	
	g_signal_connect(G_OBJECT(btn_create_recording_folder), "clicked", G_CALLBACK(create_recording_folder_button_func), NULL);	
	g_signal_connect(G_OBJECT(btn_record_data), "clicked", G_CALLBACK(record_data_button_func), NULL);	

	g_timeout_add(50, timeout_callback, box_signal);
	g_timeout_add(50, recording_timeout_callback, box_signal);

	return;
}

gboolean timeout_callback(gpointer user_data) 
{
	int start_idx, i;
	int blue_spike_time_stamp_buff_end_idx, idx;
	int blue_spike_time_stamp_buff_mwa, blue_spike_time_stamp_buff_chan, blue_spike_time_stamp_buff_recording_data_idx; 
	TimeStamp blue_spike_time_stamp_buff_peak_time;
	int spike_idx;
	float *Y_temp_spike;
	RecordingData	*handling_data;
	RecordingDataChanBuff	*handling_data_chan_buff;

	if (filter_ctrl->highpass_150Hz_on || filter_ctrl->highpass_400Hz_on)
	{
		handling_data = filtered_recording_data;
	}
	else
	{
		handling_data = recording_data;
	}
	handling_data_chan_buff = &(handling_data->recording_data_buff[display_mwa][display_mwa_chan]);
		
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
	
	idx = blue_spike_time_stamp_buff_read_idx;
	blue_spike_time_stamp_buff_end_idx = blue_spike_time_stamp->buff_idx_write;
	if (!disp_paused)
	{
		while (idx != blue_spike_time_stamp_buff_end_idx)
		{
			blue_spike_time_stamp_buff_recording_data_idx = blue_spike_time_stamp->blue_spike_time_stamp_buff[idx].recording_data_buff_idx;
			blue_spike_time_stamp_buff_mwa = blue_spike_time_stamp->blue_spike_time_stamp_buff[idx].mwa;
			blue_spike_time_stamp_buff_chan = blue_spike_time_stamp->blue_spike_time_stamp_buff[idx].channel;
			blue_spike_time_stamp_buff_peak_time = blue_spike_time_stamp->blue_spike_time_stamp_buff[idx].peak_time;			
			spike_idx = blue_spike_time_stamp_buff_recording_data_idx;
			if ((blue_spike_time_stamp_buff_mwa == display_mwa) && (blue_spike_time_stamp_buff_chan == display_mwa_chan))
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
			}
			if (print_spike_end_buff)
				printf ("%d %d %llu %d\n", blue_spike_time_stamp_buff_mwa, blue_spike_time_stamp_buff_chan, blue_spike_time_stamp_buff_peak_time, blue_spike_time_stamp_buff_recording_data_idx);	
			idx++;	
			if (idx == blue_spike_time_stamp_buff_size)
				idx = 0;	
		}
		gtk_databox_set_total_limits (GTK_DATABOX (box_spike_shape), 0, NUM_OF_SAMP_PER_SPIKE-1, HIGHEST_VOLTAGE_MV , LOWEST_VOLTAGE_MV);
	}
	blue_spike_time_stamp_buff_read_idx = blue_spike_time_stamp_buff_end_idx;
	return TRUE;  

}

gboolean filter_highpass_150Hz_button_func (GtkDatabox * box)
{
	if (filter_ctrl->highpass_150Hz_on)
	{
		if (! write_to_filter_ctrl_2_kernel_spike_msg_buffer(filter_ctrl_2_kernel_spike_msgs, FILTER_CTRL_2_KERNEL_SPIKE_MSG_TURN_HIGH_PASS_150HZ_OFF, FILTER_CTRL_2_KERNEL_SPIKE_MSG_ADDITIONAL_NULL))
			return print_message(ERROR_MSG ,"SpikeViewer", "SpikeViewer", "filter_highpass_150Hz_button_func", "! write_to_filter_ctrl_2_kernel_spike_msg_buffer().");
		gtk_button_set_label (GTK_BUTTON(btn_filter_highpass_150Hz),"HP 150Hz : OFF");
	}
	else
	{
		if (! write_to_filter_ctrl_2_kernel_spike_msg_buffer(filter_ctrl_2_kernel_spike_msgs, FILTER_CTRL_2_KERNEL_SPIKE_MSG_TURN_HIGH_PASS_150HZ_ON, FILTER_CTRL_2_KERNEL_SPIKE_MSG_ADDITIONAL_NULL))
			return print_message(ERROR_MSG ,"SpikeViewer", "SpikeViewer", "filter_highpass_150Hz_button_func", "! write_to_filter_ctrl_2_kernel_spike_msg_buffer().");
		gtk_button_set_label (GTK_BUTTON(btn_filter_highpass_150Hz),"HP 150Hz : ON");
		gtk_button_set_label (GTK_BUTTON(btn_filter_highpass_400Hz),"HP 400Hz : OFF");		
	}
	return TRUE;	
}

gboolean filter_highpass_400Hz_button_func (GtkDatabox * box)
{
	if (filter_ctrl->highpass_400Hz_on)
	{
		if (! write_to_filter_ctrl_2_kernel_spike_msg_buffer(filter_ctrl_2_kernel_spike_msgs, FILTER_CTRL_2_KERNEL_SPIKE_MSG_TURN_HIGH_PASS_400HZ_OFF, FILTER_CTRL_2_KERNEL_SPIKE_MSG_ADDITIONAL_NULL))
			return print_message(ERROR_MSG ,"SpikeViewer", "SpikeViewer", "filter_highpass_400Hz_button_func", "! write_to_filter_ctrl_2_kernel_spike_msg_buffer().");
		gtk_button_set_label (GTK_BUTTON(btn_filter_highpass_400Hz),"HP 400Hz : OFF");
	}
	else
	{
		if (! write_to_filter_ctrl_2_kernel_spike_msg_buffer(filter_ctrl_2_kernel_spike_msgs, FILTER_CTRL_2_KERNEL_SPIKE_MSG_TURN_HIGH_PASS_400HZ_ON, FILTER_CTRL_2_KERNEL_SPIKE_MSG_ADDITIONAL_NULL))
			return print_message(ERROR_MSG ,"SpikeViewer", "SpikeViewer", "filter_highpass_400Hz_button_func", "! write_to_filter_ctrl_2_kernel_spike_msg_buffer().");	
		gtk_button_set_label (GTK_BUTTON(btn_filter_highpass_400Hz),"HP 400Hz : ON");		
		gtk_button_set_label (GTK_BUTTON(btn_filter_highpass_150Hz),"HP 150Hz : OFF");
	}
	return TRUE;		
}

gboolean filter_lowpass_8KHz_button_func (GtkDatabox * box)
{
	if (filter_ctrl->lowpass_8KHz_on)
	{
		if (! write_to_filter_ctrl_2_kernel_spike_msg_buffer(filter_ctrl_2_kernel_spike_msgs, FILTER_CTRL_2_KERNEL_SPIKE_MSG_TURN_LOW_PASS_8KHZ_OFF, FILTER_CTRL_2_KERNEL_SPIKE_MSG_ADDITIONAL_NULL))
			return print_message(ERROR_MSG ,"SpikeViewer", "SpikeViewer", "filter_lowpass_8KHz_button_func", "! write_to_filter_ctrl_2_kernel_spike_msg_buffer().");	
		gtk_button_set_label (GTK_BUTTON(btn_filter_lowpass_8KHz),"LP 8KHz : OFF");
	}
	else
	{
		if ((filter_ctrl->highpass_150Hz_on) || (filter_ctrl->highpass_400Hz_on))
		{
			if (! write_to_filter_ctrl_2_kernel_spike_msg_buffer(filter_ctrl_2_kernel_spike_msgs, FILTER_CTRL_2_KERNEL_SPIKE_MSG_TURN_LOW_PASS_8KHZ_ON, FILTER_CTRL_2_KERNEL_SPIKE_MSG_ADDITIONAL_NULL))
				return print_message(ERROR_MSG ,"SpikeViewer", "SpikeViewer", "filter_lowpass_8KHz_button_func", "! write_to_filter_ctrl_2_kernel_spike_msg_buffer().");	
			gtk_button_set_label (GTK_BUTTON(btn_filter_lowpass_8KHz),"LP 8KHz : ON");
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

	sprintf(thres, "%.2f" , spike_thresholding->amplitude_thres[display_mwa][display_mwa_chan]);
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

	sprintf(thres, "%.2f" , spike_thresholding->amplitude_thres[display_mwa][display_mwa_chan]);
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
		if (! write_to_spike_thres_2_kernel_spike_msg_buffer(spike_thres_2_kernel_spike_msgs, SPIKE_THRES_2_KERNEL_SPIKE_MSG_SET_THRESHOLD, display_mwa, display_mwa_chan, threshold, SPIKE_THRES_2_KERNEL_SPIKE_MSG_ADDITIONAL_NULL))
			return print_message(ERROR_MSG ,"SpikeViewer", "SpikeViewer", "threshold_but_func", "! write_to_spike_thres_2_kernel_spike_msg_buffer().");		
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

bool set_directory_btn_select_directory_to_load(void)
{
	char line[600];
	FILE *fp = NULL;
	GFile *gfile_path; 
       	if ((fp = fopen("./path_initial_directory", "r")) == NULL)  
       	{ 
       		printf ("ERROR: SpikeViewer: Couldn't find file: ./path_initial_directory\n"); 
       		printf ("ERROR: SpikeViewer: /home is loaded as initial directory.\n");
		gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (btn_select_spike_thresholds_file_to_load),"/home");
		return FALSE;
       	}
       	else
       	{
		if (fgets(line, sizeof line, fp ) == NULL) 
		{ 
			printf("ERROR: SpikeViewer: Couldn' t read ./path_initial_directory\n"); 
       			printf ("ERROR: SpikeViewer: /home is loaded as initial directory.\n");
			gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (btn_select_spike_thresholds_file_to_load),"/home");
			fclose(fp); 		
			return FALSE;
		}
		else
		{
			gfile_path = g_file_new_for_path (line); 
			gtk_file_chooser_set_file (GTK_FILE_CHOOSER (btn_select_spike_thresholds_file_to_load), gfile_path, NULL);
			g_object_unref(gfile_path);
			fclose(fp); 		
			return TRUE;
		}
	}  	 
}

gboolean load_spike_thresholds_file_button_func (GtkDatabox * box)
{

	char *path = NULL;

	int version;
	path = gtk_file_chooser_get_uri (GTK_FILE_CHOOSER (btn_select_spike_thresholds_file_to_load));
	path = &(path[7]);                  ///     file:///path

	if (!get_format_version(&version, path))
		return print_message(ERROR_MSG ,"SpikeViewer", "SpikeViewer", "load_spike_thresholds_file_button_func", "! get_format_version()."); 
	
	if (!((*read_spike_thresholds_data[version])(2, path, spike_thres_2_kernel_spike_msgs)))
		return print_message(ERROR_MSG ,"SpikeViewer", "SpikeViewer", "load_spike_thresholds_file_button_func", "! *read_spike_thresholds_data[version]()."); 

	return print_message(INFO_MSG ,"SpikeViewer", "SpikeViewer", "load_spike_thresholds_file_button_func", "Succesuflly loaded SpikeThresholds data file.");

}

void set_directory_btn_select_directory_to_save(void)
{
	char line[600];
	FILE *fp = NULL;
       	if ((fp = fopen("./path_initial_directory", "r")) == NULL)  
       	{ 
       		printf ("ERROR: SpikeViewer: Couldn't find file: ./path_initial_directory\n"); 
       		printf ("ERROR: SpikeViewer: /home is loaded as initial direcoty to create data folder\n");
		gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (btn_select_folder_to_save_spike_thresholds_file),"/home");
       	}
       	else
       	{
		if (fgets(line, sizeof line, fp ) == NULL) 
		{ 
			printf("ERROR: SpikeViewer: Couldn' t read ./path_initial_directory\n"); 
       			printf ("ERROR: SpikeViewer: /home is loaded as initial direcoty to create data folder\n");
			gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (btn_select_folder_to_save_spike_thresholds_file),"/home");
		}
		else
		{
			line[strlen(line)-16] = 0;   /// to get the folder instead of ../../DaqConfig file
			gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (btn_select_folder_to_save_spike_thresholds_file),line);
		}
		fclose(fp); 		
	}  	 
}

gboolean save_spike_thresholds_file_button_func (GtkDatabox * box)
{
	char *path_temp = NULL, *path = NULL;
	path_temp = gtk_file_chooser_get_uri (GTK_FILE_CHOOSER (btn_select_folder_to_save_spike_thresholds_file));
	path = &path_temp[7];   // since     uri returns file:///home/....	
	if (! (*write_spike_thresholds_data[DATA_FORMAT_VERSION])(2, path, spike_thresholding))
		return print_message(ERROR_MSG ,"SpikeViewer", "SpikeViewer", "save_spike_thresholds_file_button_func", "! *write_spike_thresholds_data()."); 		
	return print_message(INFO_MSG ,"SpikeViewer", "SpikeViewer", "save_spike_thresholds_file_button_func", "Succesuflly saved SpikeThresholds data file.");
}

gboolean create_recording_folder_button_func (GtkDatabox * box)
{
	char *path_temp = NULL, *path = NULL;
	path_temp = gtk_file_chooser_get_uri (GTK_FILE_CHOOSER (btn_select_folder_to_record_data));
	path = &path_temp[7];   // since     uri returns file:///home/....	
		
	if ((*create_main_directory[MAX_NUMBER_OF_RECORDING_DATA_FORMAT_VER-1])(1, path))		// record in last format version
	{
		
	}
	else
		print_message(ERROR_MSG ,"SpikeViewer", "Gui", "create_recording_folder_button_func", " *create_main_directory().");
	return TRUE;
}

gboolean record_data_button_func (GtkDatabox * box)
{
	char *path_temp, *path; 
	path_temp = NULL; path = NULL;
	if (recording)
	{
		if (! (*fclose_all_data_files[MAX_NUMBER_OF_RECORDING_DATA_FORMAT_VER-1])(3, &rt_tasks_data->current_system_time, recording_data, spike_time_stamps))	
		{
			print_message(ERROR_MSG ,"SpikeViewer", "Gui", "timeout_callback", " *fclose_all_data_file().");		
			exit(1);
		}
		recording = FALSE;	

		gtk_button_set_label (GTK_BUTTON(btn_record_data),"Record");
	}
	else
	{
		path_temp = gtk_file_chooser_get_uri (GTK_FILE_CHOOSER (btn_select_folder_to_record_data));
		path = &path_temp[7];   // since     uri returns file:///home/....	
		if (!(*create_data_directory[MAX_NUMBER_OF_RECORDING_DATA_FORMAT_VER-1])(5, path, &rt_tasks_data->current_system_time, recording_number, recording_data, spike_time_stamps))	
		{
			print_message(ERROR_MSG ,"SpikeViewer", "Gui", "timeout_callback", " *create_data_directory().");		
			exit(1);
		}
		recording = TRUE;	
		recording_number++;
		gtk_button_set_label (GTK_BUTTON(btn_record_data),"Stop");
	}
	return TRUE;
}

gboolean recording_timeout_callback(gpointer user_data) 
{
	if (recording)
	{
		if (!(*write_to_data_files[MAX_NUMBER_OF_RECORDING_DATA_FORMAT_VER-1])(2, recording_data, spike_time_stamps))	
		{
			print_message(ERROR_MSG ,"SpikeViewer", "Gui", "recording_timeout_callback", " *write_to_data_files().");		
			exit(1);
		}	
	}
	return TRUE;
}
