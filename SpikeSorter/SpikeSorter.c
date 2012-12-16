/***************************************************************************
                          SpikeSorter.c  -  description
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

#include "SpikeSorter.h"

static RecordingData			*filtered_recording_data = NULL;
static TemplateMatchingData	*template_matching_data = NULL;
static BlueSpikeTimeStamp 		*blue_spike_time_stamp = NULL;
static TempMat2KrnlSpkMsg	*template_matching_2_kernel_spike_msgs = NULL;
static int blue_spike_time_stamp_buff_size = BLUE_SPIKE_TIME_STAMP_BUFF_SIZE;

int main( int argc, char *argv[])
{
	filtered_recording_data = (RecordingData*)rtai_malloc(SHM_NUM_KERNEL_SPIKE_FILTERED_RECORDING_DATA, 0);
	if (filtered_recording_data == NULL) 
		return print_message(ERROR_MSG ,"SpikeSorter", "SpikeSorter", "main", "filtered_recording_data == NULL.");
	template_matching_data = (TemplateMatchingData*)rtai_malloc(SHM_NUM_KERNEL_SPIKE_TEMPLATE_MATCHING, 0);
	if (template_matching_data == NULL) 
		return print_message(ERROR_MSG ,"SpikeSorter", "SpikeSorter", "main", "template_matching_data == NULL.");
	blue_spike_time_stamp = (BlueSpikeTimeStamp*)rtai_malloc(SHM_NUM_KERNEL_SPIKE_BLUE_SPIKE_TIME_STAMP, 0);
	if (blue_spike_time_stamp == NULL) 
		return print_message(ERROR_MSG ,"SpikeSorter", "SpikeSorter", "main", "blue_spike_time_stamp == NULL.");
	template_matching_2_kernel_spike_msgs = allocate_shm_client_template_matching_2_kernel_spike_msg_buffer(template_matching_2_kernel_spike_msgs);
	if (template_matching_2_kernel_spike_msgs == NULL) 
		return print_message(ERROR_MSG ,"SpikeSorter", "SpikeSorter", "main", "template_matching_2_kernel_spike_msgs == NULL.");

	gtk_init(&argc, &argv);
	create_gui(); 	
	gtk_main();

	return 0;
}

void create_gui(void)
{
	initialize_data_read_write_handlers();

	color_bg.red = 0;
	color_bg.green = 0;
	color_bg.blue = 0;

	color_non_sorted_all_spike.red = 65535;
	color_non_sorted_all_spike.green = 65535;
	color_non_sorted_all_spike.blue = 65535;
	
	color_spike_template.red = 65535;
	color_spike_template.green = 65535;
	color_spike_template.blue = 65535;	
	
	color_spike[0].red = 0;			// light blue
	color_spike[0].green = 32768;
	color_spike[0].blue = 65535;

	color_spike[1].red = 65535;		// red
	color_spike[1].green = 0;
	color_spike[1].blue = 0;

	color_spike[2].red = 65535;		// yellow
	color_spike[2].green = 65535;
	color_spike[2].blue = 0;
	
	color_spike[3].red = 0;
	color_spike[3].green = 65535;		// green
	color_spike[3].blue = 0;	

/*	color_spike[4].red = 65535;
	color_spike[4].green = 32768;		// orange
	color_spike[4].blue = 0;
	
	color_spike[5].red = 65535;      // purple
	color_spike[5].green = 0;
	color_spike[5].blue = 32768;

*/	color_spike_non_sorted.red = 65535;      // non-sorted
	color_spike_non_sorted.green = 65535;
	color_spike_non_sorted.blue = 65535;
	
	int i, j;
	X_axis = g_new0 (float, NUM_OF_SAMP_PER_SPIKE);
	for (i=0; i<NUM_OF_SAMP_PER_SPIKE; i++)
	{
		X_axis[i] = (float)i;
	}	
	
	float *f_temp;
	Y_non_sorted_all_spikes_last_g_ptr_array_idx = 0;	
	Y_non_sorted_all_spikes = g_ptr_array_new();
	for (i=0; i<SPIKE_MEM_TO_DISPLAY_ALL_NONSORTED_SPIKE; i++)
	{		
		f_temp = g_new0 (float, NUM_OF_SAMP_PER_SPIKE);
		g_ptr_array_add  (Y_non_sorted_all_spikes, f_temp);
	}	
	
	double *dbl_temp;
	dbl_Y_non_sorted_all_spikes_last_g_ptr_array_idx = 0;	
	dbl_Y_non_sorted_all_spikes = g_ptr_array_new();
	for (i=0; i<SPIKE_MEM_TO_DISPLAY_ALL_NONSORTED_SPIKE; i++)
	{		
		dbl_temp = g_new0 (double, NUM_OF_SAMP_PER_SPIKE);
		g_ptr_array_add  (dbl_Y_non_sorted_all_spikes, dbl_temp);
	}		

	for (i=0; i<MAX_NUM_OF_UNIT_PER_CHAN; i++)  // including non-sorted
	{
		Y_sorted_spikes_arr[i] = g_ptr_array_new();
		Y_sorted_spikes_last_g_ptr_array_idx[i] = 0;
		for (j=0; j<SPIKE_MEM_TO_DISPLAY_UNIT; j++)
		{		
			f_temp = g_new0 (float, NUM_OF_SAMP_PER_SPIKE);
			g_ptr_array_add  (Y_sorted_spikes_arr[i], f_temp);
		}
	}	

	for (i=0; i<MAX_NUM_OF_UNIT_PER_CHAN; i++)
	{	
		Y_templates[i] = g_new0 (float, NUM_OF_SAMP_PER_SPIKE);
	}	
	
	Y_non_sorted_spike_last_g_ptr_array_idx = 0;
	Y_non_sorted_spike = g_ptr_array_new();
	for (i=0; i<SPIKE_MEM_TO_DISPLAY_UNIT; i++)
	{		
		f_temp = g_new0 (float, NUM_OF_SAMP_PER_SPIKE);
		g_ptr_array_add  (Y_non_sorted_spike, f_temp);
	}

	disp_mwa = 0;
	disp_chan = 0;
	disp_unit = 0;
	spike_filter_mode_on = 0;
	rect_switch = 0;			
	x_upper_1 = 0; x_lower_1 = 0; y_upper_1 = 0; y_lower_1 = 0; 
	x_upper_2 = 0; x_lower_2 = 0; y_upper_2 = 0; y_lower_2 = 0; 	
	disp_paused = 0;
	plotting_in_progress = 0;
	
	GtkWidget *window;
	GtkWidget *hbox, *vbox, *lbl;

 	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  	gtk_window_set_default_size(GTK_WINDOW(window), 1600, 900);
  	gtk_window_set_title(GTK_WINDOW(window), "SpikeSorter");
  	gtk_container_set_border_width(GTK_CONTAINER(window), 1);
  			
	main_table = gtk_table_new(6, 6, TRUE);     // 6 rows   6 columns
	gtk_container_add(GTK_CONTAINER(window), main_table);	

	// Non Sorted All Spike Shape Plot
	
	gtk_databox_create_box_with_scrollbars_and_rulers (&box_nonsorted_all_spike, &databox_nonsorted_all_spike,TRUE, TRUE, TRUE, TRUE);
  	gtk_widget_modify_bg (box_nonsorted_all_spike, GTK_STATE_NORMAL, &color_bg);
	for (i=0;i<SPIKE_MEM_TO_DISPLAY_ALL_NONSORTED_SPIKE;i++)
	{
		f_temp = g_ptr_array_index(Y_non_sorted_all_spikes, i);
		gtk_databox_graph_add (GTK_DATABOX (box_nonsorted_all_spike), gtk_databox_lines_new (NUM_OF_SAMP_PER_SPIKE, X_axis, f_temp, &color_non_sorted_all_spike, 0));
	}
   	gtk_table_attach_defaults(GTK_TABLE(main_table), databox_nonsorted_all_spike, 2,4,2,6);      
	gtk_databox_set_total_limits (GTK_DATABOX (box_nonsorted_all_spike), 0, NUM_OF_SAMP_PER_SPIKE-1, HIGHEST_VOLTAGE_MV , LOWEST_VOLTAGE_MV);
		
	// Sorted All Spike Shape Plot	
	gtk_databox_create_box_with_scrollbars_and_rulers (&box_sorted_all_spike, &databox_sorted_all_spike,TRUE, TRUE, TRUE, TRUE);
  	gtk_widget_modify_bg (box_sorted_all_spike, GTK_STATE_NORMAL, &color_bg);

	for (i=0; i<MAX_NUM_OF_UNIT_PER_CHAN; i++)  // including non-sorted
	{
		for (j=0; j<SPIKE_MEM_TO_DISPLAY_UNIT; j++)
		{		
			f_temp = g_ptr_array_index(Y_sorted_spikes_arr[i], j);
			gtk_databox_graph_add (GTK_DATABOX (box_sorted_all_spike), gtk_databox_lines_new (NUM_OF_SAMP_PER_SPIKE, X_axis, f_temp, &color_spike[i], 0));
		}
	}
    	gtk_table_attach_defaults(GTK_TABLE(main_table), databox_sorted_all_spike, 0,2,2,6);      
	gtk_databox_set_total_limits (GTK_DATABOX (box_sorted_all_spike), 0, NUM_OF_SAMP_PER_SPIKE-1, HIGHEST_VOLTAGE_MV , LOWEST_VOLTAGE_MV);		

	// Sorted Units Shape Plot		
	for (i=0; i<MAX_NUM_OF_UNIT_PER_CHAN; i++)  // including non-sorted
	{
		gtk_databox_create_box_with_scrollbars_and_rulers (&box_units[i], &databox_units[i],TRUE, TRUE, TRUE, TRUE);
  		gtk_widget_modify_bg (box_units[i], GTK_STATE_NORMAL, &color_bg);	

		if (i < MAX_NUM_OF_UNIT_PER_CHAN)    // skip non sorted screen
		{  		
			for (j=0; j<NUM_OF_SAMP_PER_SPIKE; j++)
			{	
				Y_templates[i][j] = (*template_matching_data)[disp_mwa][disp_chan][i].template[j];
			}
			gtk_databox_graph_add (GTK_DATABOX (box_units[i]), gtk_databox_lines_new (NUM_OF_SAMP_PER_SPIKE, X_axis, Y_templates[i], &color_spike_template, 0));	
		}
		
		for (j=0; j<SPIKE_MEM_TO_DISPLAY_UNIT; j++)
		{		
			f_temp = g_ptr_array_index(Y_sorted_spikes_arr[i], j);
			gtk_databox_graph_add (GTK_DATABOX (box_units[i]), gtk_databox_lines_new (NUM_OF_SAMP_PER_SPIKE, X_axis, f_temp, &color_spike[i], 0));
		}
    		gtk_table_attach_defaults(GTK_TABLE(main_table), databox_units[i], i,i+1,0,2);
 		gtk_databox_set_total_limits (GTK_DATABOX (box_units[i]), 0, NUM_OF_SAMP_PER_SPIKE-1, HIGHEST_VOLTAGE_MV , LOWEST_VOLTAGE_MV);		   	      		
	}	
	
	// Non sorted spike	
	gtk_databox_create_box_with_scrollbars_and_rulers (&box_non_sorted_spike, &databox_non_sorted_spike,TRUE, TRUE, TRUE, TRUE);	
  	gtk_widget_modify_bg (box_non_sorted_spike, GTK_STATE_NORMAL, &color_bg);		
	for (i=0;i<SPIKE_MEM_TO_DISPLAY_UNIT;i++)
	{
		f_temp = g_ptr_array_index(Y_non_sorted_spike, i);
		gtk_databox_graph_add (GTK_DATABOX (box_non_sorted_spike), gtk_databox_lines_new (NUM_OF_SAMP_PER_SPIKE, X_axis, f_temp, &color_spike_non_sorted, 0));
	}
   	gtk_table_attach_defaults(GTK_TABLE(main_table), databox_non_sorted_spike, 5,6,4,6);      
	gtk_databox_set_total_limits (GTK_DATABOX (box_non_sorted_spike), 0, NUM_OF_SAMP_PER_SPIKE-1, HIGHEST_VOLTAGE_MV , LOWEST_VOLTAGE_MV);	
  				
	// Buttons
	
	vbox = gtk_vbox_new(FALSE, 0);
   	gtk_table_attach_defaults(GTK_TABLE(main_table), vbox, 4,5, 2, 6);      // Put adjustments the rightmost

  	hbox = gtk_hbox_new(FALSE, 0);
  	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);  	
   	
  	btn_pause = gtk_button_new_with_label("Pause");
	gtk_box_pack_start (GTK_BOX (hbox), btn_pause, TRUE, TRUE, 0);	
 
  	hbox = gtk_hbox_new(FALSE, 0);
  	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 10); 
  	
  	hbox = gtk_hbox_new(FALSE, 0);
  	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);   	
  	
  	lbl = gtk_label_new("MWA / Channel / Unit : ");
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
 	
 	combo_unit = gtk_combo_box_new_text();
	for (i=0; i<MAX_NUM_OF_UNIT_PER_CHAN; i++)
	{
		sprintf(temp, "%d" , i);
	 	gtk_combo_box_append_text(GTK_COMBO_BOX(combo_unit), temp);
	} 
	gtk_box_pack_start (GTK_BOX (hbox), combo_unit, TRUE, TRUE, 0);
 	gtk_combo_box_set_active(GTK_COMBO_BOX(combo_unit), 0);
 	
  	hbox = gtk_hbox_new(FALSE, 0);
  	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 5); 
  	
  	hbox = gtk_hbox_new(FALSE, 0);
  	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);   
  	
  	btn_unit_sorting_on_off = gtk_button_new_with_label("Unit Sorting: OFF");
	gtk_box_pack_start (GTK_BOX (hbox), btn_unit_sorting_on_off, TRUE, TRUE, 0);

	if ((*template_matching_data)[disp_mwa][disp_chan][disp_unit].sorting_on)
	{
		gtk_button_set_label (GTK_BUTTON(btn_unit_sorting_on_off),"Unit Sorting: ON");
	}	
	
  	btn_include_unit_on_off = gtk_button_new_with_label("Include Unit: OFF");
	gtk_box_pack_start (GTK_BOX (hbox), btn_include_unit_on_off, TRUE, TRUE, 0);	
	
	if ((*template_matching_data)[disp_mwa][disp_chan][disp_unit].include_unit)
	{
		gtk_button_set_label (GTK_BUTTON(btn_include_unit_on_off),"Include Unit: ON");
	}		
	
  	hbox = gtk_hbox_new(FALSE, 0);
  	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 10); 
  	
  	hbox = gtk_hbox_new(FALSE, 0);
  	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);   
  	
  	btn_clear_unit_template = gtk_button_new_with_label("Clear Unit Template");
	gtk_box_pack_start (GTK_BOX (hbox), btn_clear_unit_template, TRUE, TRUE, 0);	
	
  	hbox = gtk_hbox_new(FALSE, 0);
  	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 5); 
  	
  	hbox = gtk_hbox_new(FALSE, 0);
  	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);   
  	
  	btn_spike_filter_on_off = gtk_button_new_with_label("Spike Filtering Mode: OFF");
	gtk_box_pack_start (GTK_BOX (hbox), btn_spike_filter_on_off, TRUE, TRUE, 0);
	
  	hbox = gtk_hbox_new(FALSE, 0);
  	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 5); 	
	
  	hbox = gtk_hbox_new(FALSE, 0);
  	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);  	
   	
        entry_probability_thres  = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox),entry_probability_thres , FALSE,FALSE,0);

	char thres[40];
	sprintf(thres, "%E" , (*template_matching_data)[disp_mwa][disp_chan][disp_unit].probability_thres);
	gtk_entry_set_text (GTK_ENTRY(entry_probability_thres), thres); 	
 	
  	btn_submit_probability_thres = gtk_button_new_with_label("Submit Probabilty");
	gtk_box_pack_start (GTK_BOX (hbox), btn_submit_probability_thres, TRUE, TRUE, 0);			

  	hbox = gtk_hbox_new(FALSE, 0);
  	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 20); 

  	hbox = gtk_hbox_new(FALSE, 0);
  	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 10); 
  	  	
  	hbox = gtk_hbox_new(FALSE, 0);
  	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);   
  	
  	btn_clear_spike_selection_screen = gtk_button_new_with_label("Clear Spike Selection Screen");
	gtk_box_pack_start (GTK_BOX (hbox), btn_clear_spike_selection_screen, TRUE, TRUE, 0);
	
  	hbox = gtk_hbox_new(FALSE, 0);
  	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);   
  	
  	btn_clear_unit_screen = gtk_button_new_with_label("Clear Unit Screen");
	gtk_box_pack_start (GTK_BOX (hbox), btn_clear_unit_screen, TRUE, TRUE, 0);	
	
  	hbox = gtk_hbox_new(FALSE, 0);
  	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);   
  	
  	btn_clear_nonsorted_unit_screen = gtk_button_new_with_label("Clear Non-Sorted Unit Screen");
	gtk_box_pack_start (GTK_BOX (hbox), btn_clear_nonsorted_unit_screen, TRUE, TRUE, 0);	

	hbox = gtk_hbox_new(FALSE, 0);
  	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 30); 	
	
	hbox = gtk_hbox_new(FALSE, 0);
  	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0); 	
	
	btn_select_template_matching_file_to_load = gtk_file_chooser_button_new ("Select Template File", GTK_FILE_CHOOSER_ACTION_OPEN);
        gtk_box_pack_start(GTK_BOX(hbox),btn_select_template_matching_file_to_load, TRUE,TRUE,0);
	gtk_widget_set_size_request(btn_select_template_matching_file_to_load, 130, 25) ;
	set_directory_btn_select_directory_to_load();
	
	btn_load_template_matching_file = gtk_button_new_with_label("Load Template File");
        gtk_box_pack_start(GTK_BOX(hbox),btn_load_template_matching_file,TRUE,TRUE, 0);

	hbox = gtk_hbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE,FALSE,0);	

  	btn_select_folder_to_save_template_matching_file = gtk_file_chooser_button_new ("Select Directory", GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER);
        gtk_box_pack_start(GTK_BOX(hbox), btn_select_folder_to_save_template_matching_file, FALSE,FALSE,0);
	set_directory_btn_select_directory_to_save();

	btn_save_template_matching_file = gtk_button_new_with_label("Save Template File");
        gtk_box_pack_start(GTK_BOX(hbox),btn_save_template_matching_file,TRUE,TRUE, 0);	
        
	gtk_widget_show_all(window);
	
  	g_signal_connect (G_OBJECT (window), "destroy", G_CALLBACK (gtk_main_quit), NULL);	
	g_signal_connect(G_OBJECT(combo_mwa), "changed", G_CALLBACK(combo_mwa_func), NULL);
	g_signal_connect(G_OBJECT(combo_chan), "changed", G_CALLBACK(combo_chan_func), NULL);
	g_signal_connect(G_OBJECT(combo_unit), "changed", G_CALLBACK(combo_unit_func), NULL);
	g_signal_connect(G_OBJECT(btn_clear_spike_selection_screen), "clicked", G_CALLBACK(clear_spike_selection_screen_button_func), NULL);
	g_signal_connect(G_OBJECT(btn_clear_unit_screen), "clicked", G_CALLBACK(clear_unit_screen_button_func), NULL);	
	g_signal_connect(G_OBJECT(btn_clear_nonsorted_unit_screen), "clicked", G_CALLBACK(clear_nonsorted_unit_screen_button_func), NULL);
	g_signal_connect(G_OBJECT(btn_clear_unit_template), "clicked", G_CALLBACK(clear_unit_template_button_func), NULL);
	g_signal_connect(G_OBJECT(btn_unit_sorting_on_off), "clicked", G_CALLBACK(unit_sorting_on_off_button_func), NULL);	
	g_signal_connect(G_OBJECT(btn_include_unit_on_off), "clicked", G_CALLBACK(include_unit_on_off_button_func), NULL);
	g_signal_connect(G_OBJECT(btn_spike_filter_on_off), "clicked", G_CALLBACK(spike_filter_on_off_button_func), NULL);
	g_signal_connect(G_OBJECT(btn_submit_probability_thres), "clicked", G_CALLBACK(submit_probability_thres_button_func), NULL);
	g_signal_connect(G_OBJECT(btn_pause), "clicked", G_CALLBACK(pause_button_func), NULL);	
	g_signal_connect(G_OBJECT(btn_load_template_matching_file ), "clicked", G_CALLBACK(load_template_matching_file_button_func), NULL);
	g_signal_connect(G_OBJECT(btn_save_template_matching_file ), "clicked", G_CALLBACK(save_template_matching_file_button_func), NULL);
	g_signal_connect(G_OBJECT(box_nonsorted_all_spike), "selection-finalized", G_CALLBACK(spike_selection_rectangle_func), NULL);
	
	blue_spike_time_stamp_buff_read_idx = blue_spike_time_stamp->buff_idx_write;
	
	g_timeout_add(50, timeout_callback, NULL);
	
}

void combo_mwa_func (void)
{
	int idx;
	char thres[20];
	idx = gtk_combo_box_get_active (GTK_COMBO_BOX(combo_mwa));
	if ((idx < 0) || (idx >= MAX_NUM_OF_MWA))
	{
		printf("SpikeSorter:\n");	
		printf ("BUG: Selected combobox index from MWA Drop-Down list is %d\n", idx);
		printf ("BUG: 0th Microwire Array selected automatically\n");
		idx = 0;
	}
	disp_mwa = idx;

	sprintf(thres, "%E" , (*template_matching_data)[disp_mwa][disp_chan][disp_unit].probability_thres);
	gtk_entry_set_text (GTK_ENTRY(entry_probability_thres), thres);	
	if ((*template_matching_data)[disp_mwa][disp_chan][disp_unit].sorting_on)
		gtk_button_set_label (GTK_BUTTON(btn_unit_sorting_on_off),"Unit Sorting: ON");
	else
		gtk_button_set_label (GTK_BUTTON(btn_unit_sorting_on_off),"Unit Sorting: OFF");
	 if ((*template_matching_data)[disp_mwa][disp_chan][disp_unit].include_unit)
		gtk_button_set_label (GTK_BUTTON(btn_include_unit_on_off),"Include Unit: ON");
	else
		gtk_button_set_label (GTK_BUTTON(btn_include_unit_on_off),"Include Unit: OFF");
	clear_spikes_screen();
	return;
}

void combo_chan_func (void)
{
	int idx;
	char thres[20];	
	idx = gtk_combo_box_get_active (GTK_COMBO_BOX(combo_chan));
	if ((idx < 0) || (idx >= MAX_NUM_OF_CHAN_PER_MWA))
	{
		printf("SpikeSorter:\n");	
		printf ("BUG: Selected combobox index from MWA Channel Drop-Down list is %d\n", idx);
		printf ("BUG: 0th Microwire Array Channel selected automatically\n");
		idx = 0;
	}
	disp_chan = idx;	
	
	sprintf(thres, "%E" , (*template_matching_data)[disp_mwa][disp_chan][disp_unit].probability_thres);
	gtk_entry_set_text (GTK_ENTRY(entry_probability_thres), thres);	
	if ((*template_matching_data)[disp_mwa][disp_chan][disp_unit].sorting_on)
		gtk_button_set_label (GTK_BUTTON(btn_unit_sorting_on_off),"Unit Sorting: ON");
	else
		gtk_button_set_label (GTK_BUTTON(btn_unit_sorting_on_off),"Unit Sorting: OFF");
	 if ((*template_matching_data)[disp_mwa][disp_chan][disp_unit].include_unit)
		gtk_button_set_label (GTK_BUTTON(btn_include_unit_on_off),"Include Unit: ON");
	else
		gtk_button_set_label (GTK_BUTTON(btn_include_unit_on_off),"Include Unit: OFF");	
	clear_spikes_screen();		
	return;	
}

void combo_unit_func (void)
{
	int idx;
	char thres[20];	
	idx = gtk_combo_box_get_active (GTK_COMBO_BOX(combo_unit));
	if ((idx < 0) || (idx >= MAX_NUM_OF_UNIT_PER_CHAN))
	{
		printf("SpikeSorter:\n");	
		printf ("BUG: Selected combobox index from MWA Channel Unit Drop-Down list is %d\n", idx);
		printf ("BUG: 0th Microwire Array Channel Unit selected automatically\n");
		idx = 0;
	}
	disp_unit = idx;	
	
	sprintf(thres, "%E" , (*template_matching_data)[disp_mwa][disp_chan][disp_unit].probability_thres);
	gtk_entry_set_text (GTK_ENTRY(entry_probability_thres), thres);	
	if ((*template_matching_data)[disp_mwa][disp_chan][disp_unit].sorting_on)
		gtk_button_set_label (GTK_BUTTON(btn_unit_sorting_on_off),"Unit Sorting: ON");
	else
		gtk_button_set_label (GTK_BUTTON(btn_unit_sorting_on_off),"Unit Sorting: OFF");
	 if ((*template_matching_data)[disp_mwa][disp_chan][disp_unit].include_unit)
		gtk_button_set_label (GTK_BUTTON(btn_include_unit_on_off),"Include Unit: ON");
	else
		gtk_button_set_label (GTK_BUTTON(btn_include_unit_on_off),"Include Unit: OFF");	
	return;	
}

void clear_spike_selection_screen_button_func(void)
{
	float *Y_local;
	double *dbl_Y_local;
	int i,j;

	for (i=0;i<SPIKE_MEM_TO_DISPLAY_ALL_NONSORTED_SPIKE;i++)
	{
		Y_local = g_ptr_array_index(Y_non_sorted_all_spikes,i);
		for (j=0; j<NUM_OF_SAMP_PER_SPIKE; j++)
		{
			Y_local[j] = 0;
		}
	}
	
	for (i=0;i<SPIKE_MEM_TO_DISPLAY_ALL_NONSORTED_SPIKE;i++)
	{
		dbl_Y_local = g_ptr_array_index(dbl_Y_non_sorted_all_spikes,i);
		for (j=0; j<NUM_OF_SAMP_PER_SPIKE; j++)
		{
			dbl_Y_local[j] = 0;
		}
	}	
	gtk_databox_set_total_limits (GTK_DATABOX (box_nonsorted_all_spike), 0, NUM_OF_SAMP_PER_SPIKE-1, HIGHEST_VOLTAGE_MV , LOWEST_VOLTAGE_MV);	
}

void clear_unit_screen_button_func(void)
{
	float *Y_local;
	int i,j;

	for (i = 0; i <SPIKE_MEM_TO_DISPLAY_UNIT; i++)
	{
		Y_local = g_ptr_array_index(Y_sorted_spikes_arr[disp_unit],i);
		for (j=0; j<NUM_OF_SAMP_PER_SPIKE; j++)
		{
			Y_local[j] = 0;
		}
		gtk_databox_set_total_limits (GTK_DATABOX (box_units[disp_unit]), 0, NUM_OF_SAMP_PER_SPIKE-1, HIGHEST_VOLTAGE_MV , LOWEST_VOLTAGE_MV);			
	}
	gtk_databox_set_total_limits (GTK_DATABOX (box_sorted_all_spike), 0, NUM_OF_SAMP_PER_SPIKE-1, HIGHEST_VOLTAGE_MV , LOWEST_VOLTAGE_MV);
}

void clear_nonsorted_unit_screen_button_func(void)
{
	float *Y_local;
	int i,j;
	
	for (i=0;i<SPIKE_MEM_TO_DISPLAY_UNIT;i++)
	{
		Y_local = g_ptr_array_index(Y_non_sorted_spike,i);
		for (j=0; j<NUM_OF_SAMP_PER_SPIKE; j++)
		{
			Y_local[j] = 0;
		}
	}
	gtk_databox_set_total_limits (GTK_DATABOX (box_non_sorted_spike), 0, NUM_OF_SAMP_PER_SPIKE-1, HIGHEST_VOLTAGE_MV , LOWEST_VOLTAGE_MV);	
}

void clear_unit_template_button_func(void)
{
	int i;

	for (i=0; i<NUM_OF_SAMP_PER_SPIKE; i++)
	{	
		Y_templates[disp_unit][i] = 0;
		(*template_matching_data)[disp_mwa][disp_chan][disp_unit].template[i] = 0;
	}
}

void unit_sorting_on_off_button_func(void)
{
	if ((*template_matching_data)[disp_mwa][disp_chan][disp_unit].sorting_on)
	{
		if (! write_to_template_matching_2_kernel_spike_msg_buffer(template_matching_2_kernel_spike_msgs, TEMPLATE_MATCHING_2_KERNEL_SPIKE_MSG_SET_UNIT_SORTING_OFF, disp_mwa, disp_chan, disp_unit, TEMPLATE_PROBAB_THRES_NULL, TEMPLATE_MATCHING_2_KERNEL_SPIKE_MSG_ADDITIONAL_NULL))
			return (void)print_message(ERROR_MSG ,"SpikeSorter", "SpikeSorter", "unit_sorting_on_off_button_func", "! write_to_template_matching_2_kernel_spike_msg_buffer().");		
		gtk_button_set_label (GTK_BUTTON(btn_unit_sorting_on_off),"Unit Sorting: OFF");
	}
	else
	{
		if (! write_to_template_matching_2_kernel_spike_msg_buffer(template_matching_2_kernel_spike_msgs, TEMPLATE_MATCHING_2_KERNEL_SPIKE_MSG_SET_UNIT_SORTING_ON, disp_mwa, disp_chan, disp_unit, TEMPLATE_PROBAB_THRES_NULL, TEMPLATE_MATCHING_2_KERNEL_SPIKE_MSG_ADDITIONAL_NULL))
			return (void)print_message(ERROR_MSG ,"SpikeSorter", "SpikeSorter", "unit_sorting_on_off_button_func", "! write_to_template_matching_2_kernel_spike_msg_buffer().");		
		gtk_button_set_label (GTK_BUTTON(btn_unit_sorting_on_off),"Unit Sorting: ON");
	}
	return;
}

void include_unit_on_off_button_func(void)
{
 	if ((*template_matching_data)[disp_mwa][disp_chan][disp_unit].include_unit)
	{
		if (! write_to_template_matching_2_kernel_spike_msg_buffer(template_matching_2_kernel_spike_msgs, TEMPLATE_MATCHING_2_KERNEL_SPIKE_MSG_EXCLUDE_UNIT, disp_mwa, disp_chan, disp_unit, TEMPLATE_PROBAB_THRES_NULL, TEMPLATE_MATCHING_2_KERNEL_SPIKE_MSG_ADDITIONAL_NULL))
			return (void)print_message(ERROR_MSG ,"SpikeSorter", "SpikeSorter", "include_unit_on_off_button_func", "! write_to_template_matching_2_kernel_spike_msg_buffer().");		
		gtk_button_set_label (GTK_BUTTON(btn_include_unit_on_off),"Include Unit: OFF");
	}
	else
	{
		if (! write_to_template_matching_2_kernel_spike_msg_buffer(template_matching_2_kernel_spike_msgs, TEMPLATE_MATCHING_2_KERNEL_SPIKE_MSG_INCLUDE_UNIT, disp_mwa, disp_chan, disp_unit, TEMPLATE_PROBAB_THRES_NULL, TEMPLATE_MATCHING_2_KERNEL_SPIKE_MSG_ADDITIONAL_NULL))
			return (void)print_message(ERROR_MSG ,"SpikeSorter", "SpikeSorter", "include_unit_on_off_button_func", "! write_to_template_matching_2_kernel_spike_msg_buffer().");	
		gtk_button_set_label (GTK_BUTTON(btn_include_unit_on_off),"Include Unit: ON");
	}
	return;
}

void spike_filter_on_off_button_func(void)
{
	if (spike_filter_mode_on)
	{
		gtk_button_set_label (GTK_BUTTON(btn_spike_filter_on_off),"Spike Filtering Mode: OFF");	
		spike_filter_mode_on = 0;
	}
	else
	{
		gtk_button_set_label (GTK_BUTTON(btn_spike_filter_on_off),"Spike Filtering Mode: ON");	
		spike_filter_mode_on = 1;
	}	
}

void submit_probability_thres_button_func(void)
{
	TemplateProbabilityThres threshold = atof(gtk_entry_get_text(GTK_ENTRY(entry_probability_thres)));
	if (threshold >= 0.0)
	{
		if (! write_to_template_matching_2_kernel_spike_msg_buffer(template_matching_2_kernel_spike_msgs, TEMPLATE_MATCHING_2_KERNEL_SPIKE_MSG_SET_PROBAB_THRES, disp_mwa, disp_chan, disp_unit, threshold, TEMPLATE_MATCHING_2_KERNEL_SPIKE_MSG_ADDITIONAL_NULL))
			return (void)print_message(ERROR_MSG ,"SpikeSorter", "SpikeSorter", "submit_probability_thres_button_func", "! write_to_template_matching_2_kernel_spike_msg_buffer().");		
		gtk_button_set_label (GTK_BUTTON(btn_include_unit_on_off),"Include Unit: OFF");
	}
	else
	{
		printf("WARNING: Threshold cannot be less than 0.0\n");
		printf("WARNING: Submit probability threshold cancelled\n");		
	}
}

void pause_button_func(void)
{
	if (disp_paused)
	{
		gtk_button_set_label (GTK_BUTTON(btn_pause),"Pause");	
		disp_paused = 0;
	}
	else
	{
		gtk_button_set_label (GTK_BUTTON(btn_pause),"Resume");	
		disp_paused = 1;
	}	
}

void load_template_matching_file_button_func(void)
{
	char *path = NULL;

	int version;
	path = gtk_file_chooser_get_uri (GTK_FILE_CHOOSER (btn_select_template_matching_file_to_load));
	path = &(path[7]);                  ///     file:///path

	if (!get_format_version(&version, path))
		return (void)print_message(ERROR_MSG ,"SpikeSorter", "SpikeSorter", "load_template_matching_file_button_func", "! get_format_version()."); 
	
	if (!((*read_template_matching_data[version])(3, path, template_matching_2_kernel_spike_msgs, template_matching_data)))
		return (void)print_message(ERROR_MSG ,"SpikeSorter", "SpikeSorter", "load_template_matching_file_button_func", "! *read_spike_thresholds_data[version]()."); 

	return (void)print_message(INFO_MSG ,"SpikeSorter", "SpikeSorter", "load_template_matching_file_button_func", "Succesuflly loaded TemplateMatching data file.");

}

void clear_spikes_screen(void)
{
	float *Y_local;
	double *dbl_Y_local;
	int i,j, k;

	for (i=0;i<SPIKE_MEM_TO_DISPLAY_ALL_NONSORTED_SPIKE;i++)
	{
		dbl_Y_local = g_ptr_array_index(dbl_Y_non_sorted_all_spikes,i);
		for (j=0; j<NUM_OF_SAMP_PER_SPIKE; j++)
		{
			dbl_Y_local[j] = 0;
		}
	}
	for (i=0;i<SPIKE_MEM_TO_DISPLAY_ALL_NONSORTED_SPIKE;i++)
	{
		Y_local = g_ptr_array_index(Y_non_sorted_all_spikes,i);
		for (j=0; j<NUM_OF_SAMP_PER_SPIKE; j++)
		{
			Y_local[j] = 0;
		}
	}
	gtk_databox_set_total_limits (GTK_DATABOX (box_nonsorted_all_spike), 0, NUM_OF_SAMP_PER_SPIKE-1, HIGHEST_VOLTAGE_MV , LOWEST_VOLTAGE_MV);	


	for (i=0; i<MAX_NUM_OF_UNIT_PER_CHAN; i++)  //not  including non-sorted
	{
		for (j=0; j<NUM_OF_SAMP_PER_SPIKE; j++)
		{	
			Y_templates[i][j] = (*template_matching_data)[disp_mwa][disp_chan][i].template[j];
		}
	}	

	for (i=0; i<MAX_NUM_OF_UNIT_PER_CHAN; i++)  // including non-sorted
	{
		for (j = 0; j <SPIKE_MEM_TO_DISPLAY_UNIT; j++)
		{
			Y_local = g_ptr_array_index(Y_sorted_spikes_arr[i],j);
			for (k=0; k<NUM_OF_SAMP_PER_SPIKE; k++)
			{
				Y_local[k] = 0;
			}
		}
		gtk_databox_set_total_limits (GTK_DATABOX (box_units[i]), 0, NUM_OF_SAMP_PER_SPIKE-1, HIGHEST_VOLTAGE_MV , LOWEST_VOLTAGE_MV);			
	}
	gtk_databox_set_total_limits (GTK_DATABOX (box_sorted_all_spike), 0, NUM_OF_SAMP_PER_SPIKE-1, HIGHEST_VOLTAGE_MV , LOWEST_VOLTAGE_MV);		
	
	for (i=0;i<SPIKE_MEM_TO_DISPLAY_UNIT;i++)
	{
		Y_local = g_ptr_array_index(Y_non_sorted_spike,i);
		for (j=0; j<NUM_OF_SAMP_PER_SPIKE; j++)
		{
			Y_local[j] = 0;
		}
	}
	gtk_databox_set_total_limits (GTK_DATABOX (box_non_sorted_spike), 0, NUM_OF_SAMP_PER_SPIKE-1, HIGHEST_VOLTAGE_MV , LOWEST_VOLTAGE_MV);		
	
	return;		
}


gboolean timeout_callback(gpointer user_data) 
{
	RecordingDataChanBuff	*filtered_recording_data_chan_buff;
	int idx, blue_spike_time_stamp_buff_end_idx;
	int blue_spike_time_stamp_buff_mwa, blue_spike_time_stamp_buff_chan, blue_spike_time_stamp_buff_unit, blue_spike_time_stamp_buff_recording_data_idx;	
	int spike_idx;	
	float *Y_temp;
	double *dbl_Y_temp;
	int i, j;
	bool spike_in_range;
	
	if (disp_paused)
	{
		blue_spike_time_stamp_buff_read_idx = blue_spike_time_stamp->buff_idx_write;
		return TRUE;
	}

	plotting_in_progress = 1;

/*	REMOVED to remove THE NECESSITY FOR  mwa_daq_data shared memory allocation. It will not display anything is channel not mapped because template matching in KernelSpike will not find any spike for this chan.
	if ((shared_memory->mwa_daq_map[disp_mwa][disp_chan].daq_card == MAX_NUM_OF_DAQ_CARD) || (shared_memory->mwa_daq_map[disp_mwa][disp_chan].daq_chan == MAX_NUM_OF_CHANNEL_PER_DAQ_CARD))  // non-cinfigured channel. Do not plot
		return TRUE; 
*/ 	
	filtered_recording_data_chan_buff = &filtered_recording_data->recording_data_buff[disp_mwa][disp_chan];
		
	idx = blue_spike_time_stamp_buff_read_idx;			// spike_time_stamp_buff_read_idx first initialized in create_gui() to be shared_memory->spike_time_stamp.buff_idx_write
	blue_spike_time_stamp_buff_end_idx = blue_spike_time_stamp->buff_idx_write;
	while (idx != blue_spike_time_stamp_buff_end_idx)
	{
		blue_spike_time_stamp_buff_recording_data_idx = blue_spike_time_stamp->blue_spike_time_stamp_buff[idx].recording_data_buff_idx;
		blue_spike_time_stamp_buff_mwa = blue_spike_time_stamp->blue_spike_time_stamp_buff[idx].mwa;
		blue_spike_time_stamp_buff_chan = blue_spike_time_stamp->blue_spike_time_stamp_buff[idx].channel;
		blue_spike_time_stamp_buff_unit = blue_spike_time_stamp->blue_spike_time_stamp_buff[idx].unit;		
		spike_idx = blue_spike_time_stamp_buff_recording_data_idx;
		if ((blue_spike_time_stamp_buff_mwa == disp_mwa) && (blue_spike_time_stamp_buff_chan == disp_chan))
		{
			if (blue_spike_time_stamp_buff_unit == MAX_NUM_OF_UNIT_PER_CHAN)    // not sorted spike
			{
				Y_temp = g_ptr_array_index(Y_non_sorted_spike,Y_non_sorted_spike_last_g_ptr_array_idx);
				Y_non_sorted_spike_last_g_ptr_array_idx ++;
				if (Y_non_sorted_spike_last_g_ptr_array_idx == SPIKE_MEM_TO_DISPLAY_UNIT)
					Y_non_sorted_spike_last_g_ptr_array_idx = 0;
				for (i = NUM_OF_SAMP_PER_SPIKE -1; i >= 0; i--)
				{
					Y_temp[i] = (*filtered_recording_data_chan_buff)[spike_idx];
					spike_idx--;
					if (spike_idx < 0)
						spike_idx	= RECORDING_DATA_BUFF_SIZE - 1;
				}
			}
			else
			{
				Y_temp = g_ptr_array_index(Y_sorted_spikes_arr[blue_spike_time_stamp_buff_unit],Y_sorted_spikes_last_g_ptr_array_idx[blue_spike_time_stamp_buff_unit]);
				Y_sorted_spikes_last_g_ptr_array_idx[blue_spike_time_stamp_buff_unit] ++;
				if (Y_sorted_spikes_last_g_ptr_array_idx[blue_spike_time_stamp_buff_unit] == SPIKE_MEM_TO_DISPLAY_UNIT)
					Y_sorted_spikes_last_g_ptr_array_idx[blue_spike_time_stamp_buff_unit] = 0;
				for (i = NUM_OF_SAMP_PER_SPIKE -1; i >= 0; i--)
				{
					Y_temp[i] = (*filtered_recording_data_chan_buff)[spike_idx];
					spike_idx--;
					if (spike_idx < 0)
						spike_idx	= RECORDING_DATA_BUFF_SIZE - 1;
				}
			}
			//    Plot all non sorted spikes
			spike_idx = blue_spike_time_stamp_buff_recording_data_idx;			
			Y_temp = g_ptr_array_index(Y_non_sorted_all_spikes,Y_non_sorted_all_spikes_last_g_ptr_array_idx);
			dbl_Y_temp = g_ptr_array_index(dbl_Y_non_sorted_all_spikes,dbl_Y_non_sorted_all_spikes_last_g_ptr_array_idx);
			for (i = NUM_OF_SAMP_PER_SPIKE -1; i >= 0; i--)
			{
				Y_temp[i] = (*filtered_recording_data_chan_buff)[spike_idx];
				dbl_Y_temp[i] = (*filtered_recording_data_chan_buff)[spike_idx];
				spike_idx--;
				if (spike_idx < 0)
					spike_idx	= RECORDING_DATA_BUFF_SIZE - 1;
			}
			if (spike_filter_mode_on)
			{
				spike_in_range = 0;
				for (i = 0; i < NUM_OF_SAMP_PER_SPIKE; i++)
				{
					if ((Y_temp[i]  >=  y_lower_1) && (Y_temp[i] <=  y_upper_1) && (i >= x_lower_1) && (i <= x_upper_1)) 
					{
						for (j = 0; j < NUM_OF_SAMP_PER_SPIKE; j++)
						{
							if ((Y_temp[j]  >=  y_lower_2) && (Y_temp[j] <=  y_upper_2) && (j >= x_lower_2) && (j <= x_upper_2))
							{
								spike_in_range = 1;
								break;
							} 							
						}
						if (spike_in_range)
						{
							Y_non_sorted_all_spikes_last_g_ptr_array_idx ++;
							if (Y_non_sorted_all_spikes_last_g_ptr_array_idx == SPIKE_MEM_TO_DISPLAY_ALL_NONSORTED_SPIKE)
								Y_non_sorted_all_spikes_last_g_ptr_array_idx = 0;
							dbl_Y_non_sorted_all_spikes_last_g_ptr_array_idx ++;
							if (dbl_Y_non_sorted_all_spikes_last_g_ptr_array_idx == SPIKE_MEM_TO_DISPLAY_ALL_NONSORTED_SPIKE)
								dbl_Y_non_sorted_all_spikes_last_g_ptr_array_idx = 0;									
							break;					
						}				
					}
				}
				if (!spike_in_range)
				{
					for (i = 0; i < NUM_OF_SAMP_PER_SPIKE; i++)
					{
						Y_temp[i] = 0;
					}						
				}							
			}
			else
			{
				Y_non_sorted_all_spikes_last_g_ptr_array_idx ++;
				if (Y_non_sorted_all_spikes_last_g_ptr_array_idx == SPIKE_MEM_TO_DISPLAY_ALL_NONSORTED_SPIKE)
					Y_non_sorted_all_spikes_last_g_ptr_array_idx = 0;
				dbl_Y_non_sorted_all_spikes_last_g_ptr_array_idx ++;
				if (dbl_Y_non_sorted_all_spikes_last_g_ptr_array_idx == SPIKE_MEM_TO_DISPLAY_ALL_NONSORTED_SPIKE)
					dbl_Y_non_sorted_all_spikes_last_g_ptr_array_idx = 0;					
			}			
			
		}
		idx++;	
		if (idx ==	blue_spike_time_stamp_buff_size)
			idx = 0;	
	}
	blue_spike_time_stamp_buff_read_idx = blue_spike_time_stamp_buff_end_idx;
	gtk_databox_set_total_limits (GTK_DATABOX (box_non_sorted_spike), 0, NUM_OF_SAMP_PER_SPIKE-1, HIGHEST_VOLTAGE_MV , LOWEST_VOLTAGE_MV);		
	gtk_databox_set_total_limits (GTK_DATABOX (box_nonsorted_all_spike), 0, NUM_OF_SAMP_PER_SPIKE-1, HIGHEST_VOLTAGE_MV , LOWEST_VOLTAGE_MV);
	gtk_databox_set_total_limits (GTK_DATABOX (box_sorted_all_spike), 0, NUM_OF_SAMP_PER_SPIKE-1, HIGHEST_VOLTAGE_MV , LOWEST_VOLTAGE_MV);	
	for (i=0;i<MAX_NUM_OF_UNIT_PER_CHAN; i++)
	{	
		gtk_databox_set_total_limits (GTK_DATABOX (box_units[i]), 0, NUM_OF_SAMP_PER_SPIKE-1, HIGHEST_VOLTAGE_MV , LOWEST_VOLTAGE_MV);
	}				
	plotting_in_progress = 0;
	return TRUE; 	
}

void spike_selection_rectangle_func(GtkDatabox * box, GtkDataboxValueRectangle * selectionValues)
{
	printf ("SpikeSorter:\n");
	printf ("%f %f %f %f\n", selectionValues->x1, selectionValues->x2, selectionValues->y1, selectionValues->y2);

	if (spike_filter_mode_on)
	{
		if (rect_switch)
		{
			rect_switch = 0;		
			if (selectionValues->x1 > selectionValues->x2)
			{
				x_upper_2 = selectionValues->x1;
				x_lower_2 = selectionValues->x2;
			}
			else 
			{
				x_lower_2 = selectionValues->x1;
				x_upper_2 = selectionValues->x2;
			}
			if (selectionValues->y1 > selectionValues->y2)
			{
				y_upper_2 = selectionValues->y1;
				y_lower_2 = selectionValues->y2;
			}
			else 
			{
				y_lower_2 = selectionValues->y1;
				y_upper_2 = selectionValues->y2;
			}
		}
		else
		{
			rect_switch = 1;		
			if (selectionValues->x1 > selectionValues->x2)
			{
				x_upper_1 = selectionValues->x1;
				x_lower_1 = selectionValues->x2;
			}
			else 
			{
				x_lower_1 = selectionValues->x1;
				x_upper_1 = selectionValues->x2;
			}
			if (selectionValues->y1 > selectionValues->y2)
			{
				y_upper_1 = selectionValues->y1;
				y_lower_1 = selectionValues->y2;
			}
			else 
			{
				y_lower_1 = selectionValues->y1;
				y_upper_1 = selectionValues->y2;
			}
		}		
		return;
	}
	
	float x_upper, x_lower, y_upper, y_lower;  
	if (selectionValues->x1 > selectionValues->x2)
	{
		x_upper = selectionValues->x1;
		x_lower = selectionValues->x2;
	}
	else 
	{
		x_lower = selectionValues->x1;
		x_upper = selectionValues->x2;
	}

	if (selectionValues->y1 > selectionValues->y2)
	{
		y_upper = selectionValues->y1;
		y_lower = selectionValues->y2;
	}
	else 
	{
		y_lower = selectionValues->y1;
		y_upper = selectionValues->y2;
	}
	
	double *Y_analyze, *Y_temp, Y_sum = 0;
	double *Y_mean;
	int i ,j, idx;
	idx= 0;
	GPtrArray *Y_spikes_in_range_array;
	Y_spikes_in_range_array = g_ptr_array_new();
	for (i=0;i<SPIKE_MEM_TO_DISPLAY_ALL_NONSORTED_SPIKE;i++)
	{
		Y_analyze = g_ptr_array_index(dbl_Y_non_sorted_all_spikes,i);
		for (j=0; j<NUM_OF_SAMP_PER_SPIKE; j++)
		{
			if ((Y_analyze[j] >=  y_lower) && (Y_analyze[j] <=  y_upper)) 
			{
				if ((j >= x_lower) && (j <= x_upper))
				{
					g_ptr_array_add  (Y_spikes_in_range_array, Y_analyze);
					idx++;
					break;
				}
			}
		}		
	}
	printf ("SpikeSorter: \n");	
	printf("Selected number of spikes: %d\n", idx);
	printf("Minimum number to be selected for spike sorting template is: %d\n", MIN_SPIKE_NUM_FOR_TEMPLATE);
	
	for (i=0;i<idx; i++)
	{
		Y_sum = 0;
		Y_temp = g_ptr_array_index(Y_spikes_in_range_array,i);
		for (j=0; j<NUM_OF_SAMP_PER_SPIKE; j++)
		{
			Y_sum = Y_sum + fabs(Y_temp[j]);
		}				
		if (Y_sum == 0)
		{
			printf ("SpikeSorter: \n");		
			printf ("ERROR: At least one of the selected spikes is consisting of all zeros.\n");
			break;
		}
	}	
	
	if ((Y_sum == 0) || (idx < MIN_SPIKE_NUM_FOR_TEMPLATE))	
	{
		printf ("SpikeSorter: \n");		
		printf("Too few or inconvenient spikes selected\n");
		g_ptr_array_free(Y_spikes_in_range_array,FALSE);
		return;
	}
	
	Y_mean = g_new0 (double, NUM_OF_SAMP_PER_SPIKE);	
	for (i=0; i<NUM_OF_SAMP_PER_SPIKE; i++)
	{
		for (j=0; j<MIN_SPIKE_NUM_FOR_TEMPLATE; j++)
		{
			Y_temp = g_ptr_array_index(Y_spikes_in_range_array,(int)(j*(((float)idx)/MIN_SPIKE_NUM_FOR_TEMPLATE)));    // to select more distributed spikes in time.
			Y_mean[i] = Y_mean[i]+Y_temp[i];
		}
	}
	
	TemplateMatchingUnitData *template_matching_unit_data = &((*template_matching_data)[disp_mwa][disp_chan][disp_unit]);

	for (i=0; i<NUM_OF_SAMP_PER_SPIKE; i++)
	{
		template_matching_unit_data->template[i] = Y_mean[i]/MIN_SPIKE_NUM_FOR_TEMPLATE;
		Y_templates[disp_unit][i] = template_matching_unit_data->template[i];
	}
	gtk_databox_set_total_limits (GTK_DATABOX (box_units[disp_unit]), 0, NUM_OF_SAMP_PER_SPIKE-1, HIGHEST_VOLTAGE_MV , LOWEST_VOLTAGE_MV);

	MAT *diff_mtx, *trans_diff_mtx;
	MAT *S, *S_inv; 
	S=m_get(NUM_OF_SAMP_PER_SPIKE,NUM_OF_SAMP_PER_SPIKE);
	S_inv=m_get(NUM_OF_SAMP_PER_SPIKE,NUM_OF_SAMP_PER_SPIKE);	
	diff_mtx = m_get(MIN_SPIKE_NUM_FOR_TEMPLATE, NUM_OF_SAMP_PER_SPIKE);
	trans_diff_mtx = m_get(NUM_OF_SAMP_PER_SPIKE, MIN_SPIKE_NUM_FOR_TEMPLATE);

	for (i=0; i<MIN_SPIKE_NUM_FOR_TEMPLATE; i++)
	{
		Y_temp = g_ptr_array_index(Y_spikes_in_range_array,(int)(i*(((float)idx)/MIN_SPIKE_NUM_FOR_TEMPLATE)));
		for (j=0; j<NUM_OF_SAMP_PER_SPIKE; j++)
		{
			diff_mtx->me[i][j] =  Y_temp[j] - template_matching_unit_data->template[j];  
		}
		if (((int)(j*(((float)idx)/MIN_SPIKE_NUM_FOR_TEMPLATE))) >= idx)
		{
			printf("---------------------------------------------------------------------------\n");		
			printf("-----------------------------BUG---------------------------------------\n");
			printf("BUG: index reaches to a value larger than idx = %d\n", idx);
			printf("-----------------------------BUG---------------------------------------\n");
			printf("---------------------------------------------------------------------------\n");						
		}
		else
		{
			printf("%d\t", (int)(i*(((float)idx)/MIN_SPIKE_NUM_FOR_TEMPLATE)));
		}		
	}
	
	m_transp(diff_mtx, trans_diff_mtx);	
	m_mlt(trans_diff_mtx, diff_mtx, S);

	for (i=0; i<NUM_OF_SAMP_PER_SPIKE; i++)
	{
		for (j=0; j<NUM_OF_SAMP_PER_SPIKE; j++)
		{
			S->me[i][j] = S->me[i][j] / (MIN_SPIKE_NUM_FOR_TEMPLATE-1);
			S->me[i][j] = floor ( ( S->me[i][j] * (1.0E+10) ) + 0.5) /  (1.0E+10);					// to round at 10^(-10) th to get a more symmetrical inverted matrix (S_inv)
		}
	}	
		
	MAT *LU; 
	PERM *pivot; 
	LU = m_get(S->m,S->n);
	m_zero(LU);
	for (i=0; i<NUM_OF_SAMP_PER_SPIKE; i++)
	{
		for (j=0; j<NUM_OF_SAMP_PER_SPIKE; j++)
		{
			LU->me[i][j] = S->me[i][j]; 
		}
	}
	pivot = px_get(S->m);
  	LU = LUfactor(LU,pivot);
	double determinant = 1.0;
	for (i=0; i<NUM_OF_SAMP_PER_SPIKE; i++)
	{
		determinant = determinant * (LU->me[i][i]);
	}
		
	m_inverse(S,S_inv);
	
	if (determinant < 0)
		determinant = determinant *(-1.0);	
		
	printf ("log(determinant) = %.10f\n", log(determinant)); 		

	template_matching_unit_data->sqrt_det_S = sqrt(determinant); 
	template_matching_unit_data->log_det_S = log(determinant);
	for (i=0; i<NUM_OF_SAMP_PER_SPIKE; i++)
	{
		for (j=0; j<NUM_OF_SAMP_PER_SPIKE; j++)
		{	
			template_matching_unit_data->inv_S[i][j] = S_inv->me[i][j];
		}
	}
	
	m_free(S);
	m_free(S_inv);
	m_free(diff_mtx);
	m_free(trans_diff_mtx);	

	g_ptr_array_free(Y_spikes_in_range_array,FALSE);
	g_free(Y_mean);
	return;		
}

bool set_directory_btn_select_directory_to_load(void)
{
	char line[600];
	FILE *fp = NULL;
	GFile *gfile_path; 
       	if ((fp = fopen("./path_initial_directory", "r")) == NULL)  
       	{ 
       		printf ("ERROR: SpikeSorter: Couldn't find file: ./path_initial_directory\n"); 
       		printf ("ERROR: SpikeSorter: /home is loaded as initial directory.\n");
		gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (btn_select_template_matching_file_to_load),"/home");
		return FALSE;
       	}
       	else
       	{
		if (fgets(line, sizeof line, fp ) == NULL) 
		{ 
			printf("ERROR: SpikeSorter: Couldn' t read ./path_initial_directory\n"); 
       			printf ("ERROR: SpikeSorter: /home is loaded as initial directory.\n");
			gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (btn_select_template_matching_file_to_load),"/home");
			fclose(fp); 		
			return FALSE;
		}
		else
		{
			gfile_path = g_file_new_for_path (line); 
			gtk_file_chooser_set_file (GTK_FILE_CHOOSER (btn_select_template_matching_file_to_load), gfile_path, NULL);
			g_object_unref(gfile_path);
			fclose(fp); 		
			return TRUE;
		}
	}  	 
}

void set_directory_btn_select_directory_to_save(void)
{
	char line[600];
	FILE *fp = NULL;
       	if ((fp = fopen("./path_initial_directory", "r")) == NULL)  
       	{ 
       		printf ("ERROR: SpikeSorter: Couldn't find file: ./path_initial_directory\n"); 
       		printf ("ERROR: SpikeSorter: /home is loaded as initial direcoty to create data folder\n");
		gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (btn_select_folder_to_save_template_matching_file),"/home");
       	}
       	else
       	{
		if (fgets(line, sizeof line, fp ) == NULL) 
		{ 
			printf("ERROR: SpikeSorter: Couldn' t read ./path_initial_directory\n"); 
       			printf ("ERROR: SpikeSorter: /home is loaded as initial direcoty to create data folder\n");
			gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (btn_select_folder_to_save_template_matching_file),"/home");
		}
		else
		{
			line[strlen(line)-16] = 0;   /// to get the folder instead of ../../DaqConfig file
			gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (btn_select_folder_to_save_template_matching_file),line);
		}
		fclose(fp); 		
	}  	 
}

void save_template_matching_file_button_func (void)
{
	char *path_temp = NULL, *path = NULL;
	path_temp = gtk_file_chooser_get_uri (GTK_FILE_CHOOSER (btn_select_folder_to_save_template_matching_file));
	path = &path_temp[7];   // since     uri returns file:///home/....	
	if (! (*write_template_matching_data[DATA_FORMAT_VERSION])(2, path, template_matching_data))
		return (void)print_message(ERROR_MSG ,"SpikeSorter", "SpikeSorter", "save_template_matching_file_button_func", "! *write_template_matching_data()."); 		
	return (void)print_message(INFO_MSG ,"SpikeSorter", "SpikeSorter", "save_template_matching_file_button_func", "Succesuflly saved TemplateMatching data file."); 
}
