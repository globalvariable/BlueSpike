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

	color_bg.red = 0;
	color_bg.green = 0;
	color_bg.blue = 0;

	color_non_sorted_all_spike.red = 65535;
	color_non_sorted_all_spike.green = 65535;
	color_non_sorted_all_spike.blue = 65535;
	
	color_spike_template.red = 65535;
	color_spike_template.green = 65535;
	color_spike_template.blue = 65535;	
	
	color_spike[0].red = 0;
	color_spike[0].green = 32768;
	color_spike[0].blue = 65535;

	color_spike[1].red = 65535;
	color_spike[1].green = 0;
	color_spike[1].blue = 0;

	color_spike[2].red = 65535;
	color_spike[2].green = 65535;
	color_spike[2].blue = 0;
	
	color_spike[3].red = 0;
	color_spike[3].green = 65535;
	color_spike[3].blue = 0;	

	color_spike[MAX_NUM_OF_UNIT_PER_CHAN].red = 65535;      // non-sorted
	color_spike[MAX_NUM_OF_UNIT_PER_CHAN].green = 65535;
	color_spike[MAX_NUM_OF_UNIT_PER_CHAN].blue = 65535;
	
	int i, j;
	X_axis = g_new0 (float, NUM_OF_SAMP_PER_SPIKE);
	for (i=0; i<NUM_OF_SAMP_PER_SPIKE; i++)
	{
		X_axis[i] = (float)i;
	}	
	
	float *f_temp;
	Y_non_sorted_all_spikes = g_ptr_array_new();
	for (i=0; i<SPIKE_MEM_TO_DISPLAY_ALL_NONSORTED_SPIKE; i++)
	{		
		f_temp = g_new0 (float, NUM_OF_SAMP_PER_SPIKE);
		g_ptr_array_add  (Y_non_sorted_all_spikes, f_temp);
	}	

	for (i=0; i<MAX_NUM_OF_UNIT_PER_CHAN+1; i++)  // including non-sorted
	{
		Y_spikes_arr[i] = g_ptr_array_new();
		for (j=0; j<SPIKE_MEM_TO_DISPLAY_UNIT; j++)
		{		
			f_temp = g_new0 (float, NUM_OF_SAMP_PER_SPIKE);
			g_ptr_array_add  (Y_spikes_arr[i], f_temp);
		}
	}	


	GtkWidget *window;
	GtkWidget *hbox, *vbox, *lbl;

 	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  	gtk_window_set_default_size(GTK_WINDOW(window), 1600, 900);
  	gtk_window_set_title(GTK_WINDOW(window), "SpikeSorter");
  	gtk_container_set_border_width(GTK_CONTAINER(window), 1);
  			
	main_table = gtk_table_new(6, 5, TRUE);     // 6 rows   5 columns
	gtk_container_add(GTK_CONTAINER(window), main_table);	

	// Non Sorted All Spike Shape Plot
	
	gtk_databox_create_box_with_scrollbars_and_rulers (&box_nonsorted_all_spike, &databox_nonsorted_all_spike,TRUE, TRUE, TRUE, TRUE);
  	gtk_widget_modify_bg (box_nonsorted_all_spike, GTK_STATE_NORMAL, &color_bg);
	for (i=0;i<SPIKE_MEM_TO_DISPLAY_ALL_NONSORTED_SPIKE;i++)
	{
		f_temp = g_ptr_array_index(Y_non_sorted_all_spikes, i);
		gtk_databox_graph_add (GTK_DATABOX (box_nonsorted_all_spike), gtk_databox_lines_new (NUM_OF_SAMP_PER_SPIKE, X_axis, f_temp, &color_non_sorted_all_spike, 0));
	}
   	gtk_table_attach_defaults(GTK_TABLE(main_table), databox_nonsorted_all_spike, 0,2,2,6);      
	gtk_databox_set_total_limits (GTK_DATABOX (box_nonsorted_all_spike), 0, NUM_OF_SAMP_PER_SPIKE-1, HIGHEST_VOLTAGE_MV , LOWEST_VOLTAGE_MV);
		
	// Sorted All Spike Shape Plot	
	gtk_databox_create_box_with_scrollbars_and_rulers (&box_sorted_all_spike, &databox_sorted_all_spike,TRUE, TRUE, TRUE, TRUE);
  	gtk_widget_modify_bg (box_sorted_all_spike, GTK_STATE_NORMAL, &color_bg);

	for (i=0; i<MAX_NUM_OF_UNIT_PER_CHAN+1; i++)  // including non-sorted
	{
		for (j=0; j<SPIKE_MEM_TO_DISPLAY_UNIT; j++)
		{		
			f_temp = g_ptr_array_index(Y_spikes_arr[i], j);
			gtk_databox_graph_add (GTK_DATABOX (box_sorted_all_spike), gtk_databox_lines_new (NUM_OF_SAMP_PER_SPIKE, X_axis, f_temp, &color_spike[i], 0));
		}
	}	
    	gtk_table_attach_defaults(GTK_TABLE(main_table), databox_sorted_all_spike, 2,4,2,6);      
	gtk_databox_set_total_limits (GTK_DATABOX (box_sorted_all_spike), 0, NUM_OF_SAMP_PER_SPIKE-1, HIGHEST_VOLTAGE_MV , LOWEST_VOLTAGE_MV);		
	
	// Sorted Units Shape Plot		
	for (i=0; i<MAX_NUM_OF_UNIT_PER_CHAN+1; i++)  // including non-sorted
	{
		gtk_databox_create_box_with_scrollbars_and_rulers (&box_units[i], &databox_units[i],TRUE, TRUE, TRUE, TRUE);
  		gtk_widget_modify_bg (box_units[i], GTK_STATE_NORMAL, &color_bg);	
	
		for (j=0; j<SPIKE_MEM_TO_DISPLAY_UNIT; j++)
		{		
			f_temp = g_ptr_array_index(Y_spikes_arr[i], j);
			gtk_databox_graph_add (GTK_DATABOX (box_units[i]), gtk_databox_lines_new (NUM_OF_SAMP_PER_SPIKE, X_axis, f_temp, &color_spike[i], 0));
		}
    		gtk_table_attach_defaults(GTK_TABLE(main_table), databox_units[i], i,i+1,0,2);
 		gtk_databox_set_total_limits (GTK_DATABOX (box_units[i]), 0, NUM_OF_SAMP_PER_SPIKE-1, HIGHEST_VOLTAGE_MV , LOWEST_VOLTAGE_MV);		   	      		
	}	
		
		
	// Buttons
	
	vbox = gtk_vbox_new(FALSE, 0);
   	gtk_table_attach_defaults(GTK_TABLE(main_table), vbox, 4,5, 2, 6);      // Put adjustments the rightmost

  	hbox = gtk_hbox_new(FALSE, 0);
  	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);  	
   	
  	btn_pause = gtk_button_new_with_label("Pause");
	gtk_box_pack_start (GTK_BOX (hbox), btn_pause, TRUE, TRUE, 0);	
 
  	hbox = gtk_hbox_new(FALSE, 0);
  	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);   
  	
  	btn_sorting_on_off = gtk_button_new_with_label("Sorting: OFF");
	gtk_box_pack_start (GTK_BOX (hbox), btn_sorting_on_off, TRUE, TRUE, 0);
 
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

 	combo_chan = gtk_combo_box_new_text();
	for (i=0; i<MAX_NUM_OF_CHAN_PER_MWA; i++)
	{
		sprintf(temp, "%d" , i);
	 	gtk_combo_box_append_text(GTK_COMBO_BOX(combo_chan), temp);
	} 
	gtk_box_pack_start (GTK_BOX (hbox), combo_chan, TRUE, TRUE, 0);

 	combo_unit = gtk_combo_box_new_text();
	for (i=0; i<MAX_NUM_OF_UNIT_PER_CHAN; i++)
	{
		sprintf(temp, "%d" , i);
	 	gtk_combo_box_append_text(GTK_COMBO_BOX(combo_unit), temp);
	} 
	gtk_box_pack_start (GTK_BOX (hbox), combo_unit, TRUE, TRUE, 0);

  	hbox = gtk_hbox_new(FALSE, 0);
  	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 5); 
  	
  	hbox = gtk_hbox_new(FALSE, 0);
  	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);   
  	
  	btn_unit_sorting_on_off = gtk_button_new_with_label("Unit Sorting: OFF");
	gtk_box_pack_start (GTK_BOX (hbox), btn_unit_sorting_on_off, TRUE, TRUE, 0);
	
  	btn_include_unit_on_off = gtk_button_new_with_label("Include Unit: OFF");
	gtk_box_pack_start (GTK_BOX (hbox), btn_include_unit_on_off, TRUE, TRUE, 0);	
	
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
  	
  	btn_spike_filter_on_off = gtk_button_new_with_label("Spike Filter: OFF");
	gtk_box_pack_start (GTK_BOX (hbox), btn_spike_filter_on_off, TRUE, TRUE, 0);
	
  	hbox = gtk_hbox_new(FALSE, 0);
  	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 5); 	
	
  	hbox = gtk_hbox_new(FALSE, 0);
  	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);  	
   	
        entry_probability_thres  = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox),entry_probability_thres , FALSE,FALSE,0);

/*	char thres[20];
	sprintf(thres, "%.2f" , shared_memory->spike_end.amplitude_thres[display_mwa][display_mwa_chan]);
	gtk_entry_set_text (GTK_ENTRY(entryThreshold), thres); 	
 */ 	
  	btn_submit_probability_thres = gtk_button_new_with_label("Submit Probabilty Threshold");
	gtk_box_pack_start (GTK_BOX (hbox), btn_submit_probability_thres, TRUE, TRUE, 0);			

  	hbox = gtk_hbox_new(FALSE, 0);
  	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 20); 

  	hbox = gtk_hbox_new(FALSE, 0);
  	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 10); 
  	  	
  	hbox = gtk_hbox_new(FALSE, 0);
  	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);   
  	
  	btn_clear_spike_select_screen = gtk_button_new_with_label("Clear Spike Selection Screen");
	gtk_box_pack_start (GTK_BOX (hbox), btn_clear_spike_select_screen, TRUE, TRUE, 0);
	
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
  	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 10); 	
	
  	hbox = gtk_hbox_new(FALSE, 0);
  	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);  	
   	
	btn_select_template_file_to_load = gtk_file_chooser_button_new ("Select Template File", GTK_FILE_CHOOSER_ACTION_OPEN);
        gtk_box_pack_start(GTK_BOX(hbox),btn_select_template_file_to_load, TRUE,TRUE,0);
	gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (btn_select_template_file_to_load),"/home");	
	gtk_widget_set_size_request(btn_select_template_file_to_load, 218, 25) ;
	
	btn_load_template_file = gtk_button_new_with_label("Load Template File");
        gtk_box_pack_start(GTK_BOX(hbox),btn_load_template_file,TRUE,TRUE, 0);	
        
   	hbox = gtk_hbox_new(FALSE, 0);
  	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 10); 	
	
  	hbox = gtk_hbox_new(FALSE, 0);
  	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);  
  	
 	btn_select_template_file_directory_to_save = gtk_file_chooser_button_new ("Select Template Files Folder", GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER);
        gtk_box_pack_start(GTK_BOX(hbox), btn_select_template_file_directory_to_save, TRUE,TRUE,0);
	gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (btn_select_template_file_directory_to_save),"/home");		
	gtk_widget_set_size_request(btn_select_template_file_directory_to_save, 90, 25) ;
		
        entry_template_file_name = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox), entry_template_file_name, TRUE,TRUE,0);
	gtk_widget_set_size_request(entry_template_file_name, 120, 25) ;
	
	btn_save_template_file = gtk_button_new_with_label("Save Template File");
        gtk_box_pack_start(GTK_BOX(hbox),btn_save_template_file,TRUE,TRUE, 0);	 	       

	gtk_widget_show_all(window);
	
  	g_signal_connect (G_OBJECT (window), "destroy", G_CALLBACK (gtk_main_quit), NULL);	
	g_signal_connect(G_OBJECT(combo_mwa), "changed", G_CALLBACK(combo_mwa_func), NULL);
	g_signal_connect(G_OBJECT(combo_chan), "changed", G_CALLBACK(combo_chan_func), NULL);
	g_signal_connect(G_OBJECT(combo_unit), "changed", G_CALLBACK(combo_unit_func), NULL);
	g_signal_connect(G_OBJECT(btn_clear_spike_select_screen), "clicked", G_CALLBACK(clear_spike_select_screen_button_func), NULL);
	g_signal_connect(G_OBJECT(btn_clear_unit_screen), "clicked", G_CALLBACK(clear_unit_screen_button_func), NULL);	
	g_signal_connect(G_OBJECT(btn_clear_nonsorted_unit_screen), "clicked", G_CALLBACK(clear_nonsorted_unit_screen_button_func), NULL);
	g_signal_connect(G_OBJECT(btn_clear_unit_template), "clicked", G_CALLBACK(clear_unit_template_button_func), NULL);
	g_signal_connect(G_OBJECT(btn_sorting_on_off), "clicked", G_CALLBACK(sorting_on_off_button_func), NULL);
	g_signal_connect(G_OBJECT(btn_include_unit_on_off), "clicked", G_CALLBACK(include_unit_on_off_button_func), NULL);
	g_signal_connect(G_OBJECT(btn_spike_filter_on_off), "clicked", G_CALLBACK(spike_filter_on_off_button_func), NULL);
	g_signal_connect(G_OBJECT(btn_submit_probability_thres), "clicked", G_CALLBACK(submit_probability_thres_button_func), NULL);
	g_signal_connect(G_OBJECT(btn_pause), "clicked", G_CALLBACK(pause_button_func), NULL);	
	g_signal_connect(G_OBJECT(btn_load_template_file ), "clicked", G_CALLBACK(load_template_file_button_func), NULL);
	g_signal_connect(G_OBJECT(btn_save_template_file), "clicked", G_CALLBACK(save_template_file_button_func), NULL);


}

void combo_mwa_func (void)
{

}

void combo_chan_func (void)
{

}

void combo_unit_func (void)
{

}

void clear_spike_select_screen_button_func(void)
{

}

void clear_unit_screen_button_func(void)
{

}

void clear_nonsorted_unit_screen_button_func(void)
{

}

void clear_unit_template_button_func(void)
{
 
}

void sorting_on_off_button_func(void)
{
 
}

void unit_sorting_on_off_button_func(void)
{
 
}

void include_unit_on_off_button_func(void)
{
 
}

void spike_filter_on_off_button_func(void)
{
 
}

void submit_probability_thres_button_func(void)
{
 
}

void pause_button_func(void)
{
 
}

void load_template_file_button_func(void)
{
 
}

void save_template_file_button_func(void)
{
 
}




