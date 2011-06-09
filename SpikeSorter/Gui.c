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

	color_bg.red = 0;
	color_bg.green = 0;
	color_bg.blue = 0;

	color_all_spike.red = 65535;
	color_all_spike.green = 65535;
	color_all_spike.blue = 65535;

	color_spike_0.red = 0;
	color_spike_0.green = 0;
	color_spike_0.blue = 65535;

	color_spike_1.red = 65535;
	color_spike_1.green = 0;
	color_spike_1.blue = 0;

	color_spike_2.red = 65535;
	color_spike_2.green = 65535;
	color_spike_2.blue = 0;

	color_spike_nonsorted.red = 65535;
	color_spike_nonsorted.green = 65535;
	color_spike_nonsorted.blue = 65535;

	color_spike_template.red = 65535;
	color_spike_template.green = 65535;
	color_spike_template.blue = 65535;

	disp_paused = 0;
	spike_filter_mode = 0;

	int i;
	X_axis = g_new0 (float, NUM_OF_SAMP_PER_SPIKE);
	for (i=0; i<NUM_OF_SAMP_PER_SPIKE; i++)
	{
		X_axis[i] = (float)i;
	}

	Y_all_spikes_ptr = g_ptr_array_new();
	for (i=0;i<SPIKE_MEM_TO_DISPLAY;i++)
	{
		Y_all_spike = g_new0 (float, NUM_OF_SAMP_PER_SPIKE);
		g_ptr_array_add  (Y_all_spikes_ptr, Y_all_spike);
	}
	Y_all_spikes_idx = 0;

	
	Y_spikes_0_ptr = g_ptr_array_new();
	for (i=0;i<SPIKE_MEM_TO_DISPLAY_UNIT;i++)
	{
		Y_spike_0 = g_new0 (float, NUM_OF_SAMP_PER_SPIKE);
		g_ptr_array_add  (Y_spikes_0_ptr, Y_spike_0);
	}
	Y_spikes_0_idx = 0;

	Y_spikes_1_ptr = g_ptr_array_new();
	for (i=0;i<SPIKE_MEM_TO_DISPLAY_UNIT;i++)
	{
		Y_spike_1 = g_new0 (float, NUM_OF_SAMP_PER_SPIKE);
		g_ptr_array_add  (Y_spikes_1_ptr, Y_spike_1);
	}
	Y_spikes_1_idx = 0;

	Y_spikes_2_ptr = g_ptr_array_new();
	for (i=0;i<SPIKE_MEM_TO_DISPLAY_UNIT;i++)
	{
		Y_spike_2 = g_new0 (float, NUM_OF_SAMP_PER_SPIKE);
		g_ptr_array_add  (Y_spikes_2_ptr, Y_spike_2);
	}
	Y_spikes_2_idx = 0;

	Y_spikes_nonsorted_ptr = g_ptr_array_new();
	for (i=0;i<SPIKE_MEM_TO_DISPLAY_UNIT;i++)
	{
		Y_spike_nonsorted = g_new0 (float, NUM_OF_SAMP_PER_SPIKE);
		g_ptr_array_add  (Y_spikes_nonsorted_ptr, Y_spike_nonsorted);
	}
	Y_spikes_nonsorted_idx = 0;

	GtkWidget *window;
	GtkWidget *hbox, *hbox1, *vbox, *vbox1, *vbox2, *lbl;

 	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  	gtk_window_set_default_size(GTK_WINDOW(window), 1200, 900);
  	gtk_window_set_title(GTK_WINDOW(window), "SpikeSorter");
  	gtk_container_set_border_width(GTK_CONTAINER(window), 10);


  	vbox = gtk_vbox_new(FALSE, 0);
	gtk_container_add(GTK_CONTAINER(window), vbox);

	hbox = gtk_hbox_new(TRUE, 0);
  	gtk_box_pack_start (GTK_BOX (vbox), hbox, TRUE, TRUE, 0);

	vbox1 = gtk_vbox_new(TRUE, 0);
  	gtk_box_pack_start (GTK_BOX (hbox), vbox1, TRUE, TRUE, 0);

	hbox1 = gtk_hbox_new(TRUE, 0);
  	gtk_box_pack_start (GTK_BOX (vbox1), hbox1, TRUE, TRUE, 0);

	vbox2 = gtk_vbox_new(TRUE, 0);
  	gtk_box_pack_start (GTK_BOX (hbox1), vbox2, TRUE, TRUE, 20);

  	gtk_databox_create_box_with_scrollbars_and_rulers (&box_all_spike, &databox_all_spike,TRUE, TRUE, TRUE, TRUE);
  	gtk_box_pack_start (GTK_BOX (vbox2), databox_all_spike, TRUE, TRUE, 0);
  	gtk_widget_modify_bg (GTK_DATABOX (box_all_spike), GTK_STATE_NORMAL, &color_bg);

  	gtk_databox_create_box_with_scrollbars_and_rulers (&box_sorted_all_spike, &databox_sorted_all_spike,TRUE, TRUE, TRUE, TRUE);
  	gtk_box_pack_start (GTK_BOX (vbox2), databox_sorted_all_spike, TRUE, TRUE, 0);
  	gtk_widget_modify_bg (GTK_DATABOX (box_sorted_all_spike), GTK_STATE_NORMAL, &color_bg);

	hbox1 = gtk_hbox_new(TRUE, 0);
  	gtk_box_pack_start (GTK_BOX (hbox), hbox1, TRUE, TRUE, 0);

	vbox1 = gtk_vbox_new(TRUE, 0);
  	gtk_box_pack_start (GTK_BOX (hbox1), vbox1, TRUE, TRUE, 0);

	gtk_databox_create_box_with_scrollbars_and_rulers (&box_spike_0, &databox_spike_0,TRUE, TRUE, TRUE, TRUE);
  	gtk_box_pack_start (GTK_BOX (vbox1), databox_spike_0, TRUE, TRUE, 0);
  	gtk_widget_modify_bg (GTK_DATABOX (box_spike_0), GTK_STATE_NORMAL, &color_bg);

	gtk_databox_create_box_with_scrollbars_and_rulers (&box_spike_1, &databox_spike_1,TRUE, TRUE, TRUE, TRUE);
  	gtk_box_pack_start (GTK_BOX (vbox1), databox_spike_1, TRUE, TRUE, 0);
  	gtk_widget_modify_bg (GTK_DATABOX (box_spike_1), GTK_STATE_NORMAL, &color_bg);

	gtk_databox_create_box_with_scrollbars_and_rulers (&box_spike_2, &databox_spike_2,TRUE, TRUE, TRUE, TRUE);
  	gtk_box_pack_start (GTK_BOX (vbox1), databox_spike_2, TRUE, TRUE, 0);
  	gtk_widget_modify_bg (GTK_DATABOX (box_spike_2), GTK_STATE_NORMAL, &color_bg);


	vbox1 = gtk_vbox_new(TRUE, 0);
  	gtk_box_pack_start (GTK_BOX (hbox1), vbox1, TRUE, TRUE, 0);

	vbox2 = gtk_vbox_new(TRUE, 0);
  	gtk_box_pack_start (GTK_BOX (vbox1), vbox2, TRUE, TRUE, 0);

	gtk_databox_create_box_with_scrollbars_and_rulers (&box_spike_nonsorted, &databox_spike_nonsorted,TRUE, TRUE, TRUE, TRUE);
  	gtk_box_pack_start (GTK_BOX (vbox2), databox_spike_nonsorted, TRUE, TRUE, 0);
  	gtk_widget_modify_bg (GTK_DATABOX (box_spike_nonsorted), GTK_STATE_NORMAL, &color_bg);

	vbox2 = gtk_vbox_new(TRUE, 0);
  	gtk_box_pack_start (GTK_BOX (vbox1), vbox2, TRUE, TRUE, 0);


	if (buff->sorting_on)
		sorting_onoff_button = gtk_button_new_with_label("Turn Sorting-OFF");	
	else
		sorting_onoff_button = gtk_button_new_with_label("Turn Sorting-ON");			
	gtk_box_pack_start (GTK_BOX (vbox2), sorting_onoff_button, TRUE, TRUE, 0);

	hbox1 = gtk_hbox_new(TRUE, 0);
  	gtk_box_pack_start (GTK_BOX (vbox2), hbox1, TRUE, TRUE, 0);

 	combo = gtk_combo_box_new_text();

	char chan_name[5];	
	for (i=1; i<=NUM_OF_CHAN; i++)
	{
		sprintf(chan_name, "%d" , i);
	 	gtk_combo_box_append_text(GTK_COMBO_BOX(combo), chan_name);
	} 
	gtk_box_pack_start (GTK_BOX (hbox1), combo, TRUE, TRUE, 0);

 	combo_spike = gtk_combo_box_new_text();

	ch_slct_button = gtk_button_new_with_label("Select Channel");

	gtk_widget_set_size_request(ch_slct_button, 50, 30);
	gtk_box_pack_start (GTK_BOX (hbox1), ch_slct_button, TRUE, TRUE, 0);

	hbox1 = gtk_hbox_new(TRUE, 0);
  	gtk_box_pack_start (GTK_BOX (vbox2), hbox1, TRUE, TRUE, 0);

	char spike_unit_name[5];	
	for (i=1; i<=NUM_OF_TEMP_PER_CHAN; i++)
	{
		sprintf(spike_unit_name, "%d" , i);
	 	gtk_combo_box_append_text(GTK_COMBO_BOX(combo_spike), spike_unit_name);
	} 
	gtk_box_pack_start (GTK_BOX (hbox1), combo_spike, TRUE, TRUE, 0);

	unit_slct_button = gtk_button_new_with_label("Select Spike Unit");
	gtk_widget_set_size_request(unit_slct_button, 50, 30);
	gtk_box_pack_start (GTK_BOX (hbox1), unit_slct_button, TRUE, TRUE, 0);

	hbox1 = gtk_hbox_new(TRUE, 0);
  	gtk_box_pack_start (GTK_BOX (vbox2), hbox1, TRUE, TRUE, 0);

        lbl = gtk_label_new("Selected Channel: ");
        gtk_box_pack_start(GTK_BOX(hbox1),lbl,TRUE,TRUE, 0);	

        lbl_selected_chan = gtk_label_new("1");
        gtk_box_pack_start(GTK_BOX(hbox1),lbl_selected_chan,TRUE,TRUE, 0);

	str_selected_chan = g_new0 (char, 20);

	hbox1 = gtk_hbox_new(TRUE, 0);
  	gtk_box_pack_start (GTK_BOX (vbox2), hbox1, TRUE, TRUE, 0);

        lbl = gtk_label_new("Selected Unit: ");
        gtk_box_pack_start(GTK_BOX(hbox1),lbl,TRUE,TRUE, 0);

        lbl_selected_unit = gtk_label_new("1");
        gtk_box_pack_start(GTK_BOX(hbox1),lbl_selected_unit,TRUE,TRUE, 0);

	str_selected_unit = g_new0 (char, 20);

	if (buff->spike_template.sorting_on[0][0])
		unit_sorting_onoff_button = gtk_button_new_with_label("Turn Unit Sorting-OFF");	
	else
		unit_sorting_onoff_button  = gtk_button_new_with_label("Turn Unit Sorting-ON");			
	gtk_box_pack_start (GTK_BOX (vbox2), unit_sorting_onoff_button, TRUE, TRUE, 0);

	if (buff->spike_template.include_unit[0][0])
		include_unit_onoff_button = gtk_button_new_with_label("Include Unit-OFF");	
	else
		include_unit_onoff_button  = gtk_button_new_with_label("Include Unit-ON");			
	gtk_box_pack_start (GTK_BOX (vbox2), include_unit_onoff_button, TRUE, TRUE, 0);

	spike_filter_mode_button = gtk_button_new_with_label("Turn Spike Filter ON");
	gtk_box_pack_start (GTK_BOX (vbox2), spike_filter_mode_button, TRUE, TRUE, 0);

	hbox1 = gtk_hbox_new(TRUE, 0);
  	gtk_box_pack_start (GTK_BOX (vbox2), hbox1, TRUE, TRUE, 0);

        entryDiff = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox1),entryDiff, TRUE,TRUE,0);
 	gtk_entry_set_editable(entryDiff,TRUE);


	strDiff = g_new0 (char, 20);
	sprintf(strDiff, "%E" , buff->spike_template.diff_thres[0][0]);
	gtk_entry_set_text (GTK_ENTRY(entryDiff), strDiff);

	diff_button = gtk_button_new_with_label("Submit Diff");
	gtk_widget_set_size_request(diff_button, 30, 30);
	gtk_box_pack_start (GTK_BOX (hbox1), diff_button, TRUE, TRUE, 0);

	pause_button = gtk_button_new_with_label("Pause");
	gtk_widget_set_size_request(pause_button, 50, 30);
	gtk_box_pack_start (GTK_BOX (vbox2), pause_button, TRUE, TRUE, 0);



	vbox2 = gtk_vbox_new(TRUE, 0);
  	gtk_box_pack_start (GTK_BOX (vbox1), vbox2, TRUE, TRUE, 0);

	clear_button = gtk_button_new_with_label("Clear Screen");
	gtk_widget_set_size_request(clear_button, 50, 30);
	gtk_box_pack_start (GTK_BOX (vbox2), clear_button, TRUE, TRUE, 0);

	clear_unit_button = gtk_button_new_with_label("Clear Unit Screen");
	gtk_widget_set_size_request(clear_unit_button, 30, 30);
	gtk_box_pack_start (GTK_BOX (vbox2), clear_unit_button, TRUE, TRUE, 0);	

	clear_unit_template_button = gtk_button_new_with_label("Clear Unit Template");
	gtk_widget_set_size_request(clear_unit_template_button, 30, 30);
	gtk_box_pack_start (GTK_BOX (vbox2), clear_unit_template_button, TRUE, TRUE, 0);	

	clear_nonsorted_button = gtk_button_new_with_label("Clear Non-Sorted Screen");
	gtk_widget_set_size_request(clear_nonsorted_button, 30, 30);
	gtk_box_pack_start (GTK_BOX (vbox2), clear_nonsorted_button, TRUE, TRUE, 0);	

	hbox1 = gtk_hbox_new(TRUE, 0);
  	gtk_box_pack_start (GTK_BOX (vbox2), hbox1, FALSE, FALSE, 0);

	strAddFileName = g_new0 (char, 20);
	strAddFileName = "none";
	save_template_button = gtk_button_new_with_label("Save Template");
	gtk_widget_set_size_request(save_template_button, 30, 30);	
  	gtk_box_pack_start (GTK_BOX (hbox1), save_template_button, TRUE, TRUE, 0);

        entryAddFileName = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox1),entryAddFileName, TRUE,TRUE,0);
 	gtk_entry_set_editable(entryAddFileName,TRUE);

	hbox1 = gtk_hbox_new(TRUE, 0);
  	gtk_box_pack_start (GTK_BOX (vbox2), hbox1, FALSE, FALSE, 0);

	strLoadFileName = g_new0 (char, 100);;
	load_template_button = gtk_button_new_with_label("Load Template");
	gtk_widget_set_size_request(save_template_button, 30, 30);	
  	gtk_box_pack_start (GTK_BOX (hbox1), load_template_button, TRUE, TRUE, 0);

        entryLoadFileName = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox1),entryLoadFileName, TRUE,TRUE,0);
 	gtk_entry_set_editable(entryLoadFileName,TRUE);



	for (i=0;i<SPIKE_MEM_TO_DISPLAY;i++)
	{
		Y_all_spike = g_ptr_array_index(Y_all_spikes_ptr,i);
		graph_all_spike = GTK_DATABOX_GRAPH(gtk_databox_lines_new (NUM_OF_SAMP_PER_SPIKE, X_axis, Y_all_spike, &color_all_spike, 0));
		gtk_databox_graph_add (GTK_DATABOX (box_all_spike), graph_all_spike);
	}
	gtk_databox_set_total_limits (GTK_DATABOX (box_all_spike), 0, NUM_OF_SAMP_PER_SPIKE, +2200, -2200);	

	Y_spike_0_template = g_new0 (float, NUM_OF_SAMP_PER_SPIKE);
	graph_spike_template_0 = GTK_DATABOX_GRAPH(gtk_databox_lines_new (NUM_OF_SAMP_PER_SPIKE, X_axis, Y_spike_0_template, &color_spike_template, 0));
	gtk_databox_graph_add (GTK_DATABOX (box_spike_0), graph_spike_template_0);
	for (i=0;i<SPIKE_MEM_TO_DISPLAY_UNIT;i++)
	{
		Y_spike_0 = g_ptr_array_index(Y_spikes_0_ptr,i);
		graph_spike_0 = GTK_DATABOX_GRAPH(gtk_databox_lines_new (NUM_OF_SAMP_PER_SPIKE, X_axis, Y_spike_0, &color_spike_0, 0));
		graph_sorted_all_spike = GTK_DATABOX_GRAPH(gtk_databox_lines_new (NUM_OF_SAMP_PER_SPIKE, X_axis, Y_spike_0, &color_spike_0, 0));
		gtk_databox_graph_add (GTK_DATABOX (box_spike_0), graph_spike_0);
		gtk_databox_graph_add (GTK_DATABOX (box_sorted_all_spike), graph_sorted_all_spike);
	}
	for (i=0; i<NUM_OF_SAMP_PER_SPIKE; i++)
	{
		Y_spike_0_template[i] = buff->spike_template.template[0][0][i];
	}	
	
	gtk_databox_set_total_limits (GTK_DATABOX (box_spike_0), 0, NUM_OF_SAMP_PER_SPIKE, +2200, -2200);	


	Y_spike_1_template = g_new0 (float, NUM_OF_SAMP_PER_SPIKE);
	graph_spike_template_1 = GTK_DATABOX_GRAPH(gtk_databox_lines_new (NUM_OF_SAMP_PER_SPIKE, X_axis, Y_spike_1_template, &color_spike_template, 0));
	gtk_databox_graph_add (GTK_DATABOX (box_spike_1), graph_spike_template_1);
	for (i=0;i<SPIKE_MEM_TO_DISPLAY_UNIT;i++)
	{
		Y_spike_1 = g_ptr_array_index(Y_spikes_1_ptr,i);
		graph_spike_1 = GTK_DATABOX_GRAPH(gtk_databox_lines_new (NUM_OF_SAMP_PER_SPIKE, X_axis, Y_spike_1, &color_spike_1, 0));
		graph_sorted_all_spike = GTK_DATABOX_GRAPH(gtk_databox_lines_new (NUM_OF_SAMP_PER_SPIKE, X_axis, Y_spike_1, &color_spike_1, 0));
		gtk_databox_graph_add (GTK_DATABOX (box_spike_1), graph_spike_1);
		gtk_databox_graph_add (GTK_DATABOX (box_sorted_all_spike), graph_sorted_all_spike);
	}
	for (i=0; i<NUM_OF_SAMP_PER_SPIKE; i++)
	{
		Y_spike_1_template[i] = buff->spike_template.template[0][1][i];
	}
	gtk_databox_set_total_limits (GTK_DATABOX (box_spike_1), 0, NUM_OF_SAMP_PER_SPIKE, +2200, -2200);


	Y_spike_2_template = g_new0 (float, NUM_OF_SAMP_PER_SPIKE);
	graph_spike_template_2 = GTK_DATABOX_GRAPH(gtk_databox_lines_new (NUM_OF_SAMP_PER_SPIKE, X_axis, Y_spike_2_template, &color_spike_template, 0));
	gtk_databox_graph_add (GTK_DATABOX (box_spike_2), graph_spike_template_2);
	for (i=0;i<SPIKE_MEM_TO_DISPLAY_UNIT;i++)
	{
		Y_spike_2 = g_ptr_array_index(Y_spikes_2_ptr,i);
		graph_spike_2 = GTK_DATABOX_GRAPH(gtk_databox_lines_new (NUM_OF_SAMP_PER_SPIKE, X_axis, Y_spike_2, &color_spike_2, 0));
		graph_sorted_all_spike = GTK_DATABOX_GRAPH(gtk_databox_lines_new (NUM_OF_SAMP_PER_SPIKE, X_axis, Y_spike_2, &color_spike_2, 0));
		gtk_databox_graph_add (GTK_DATABOX (box_spike_2), graph_spike_2);
		gtk_databox_graph_add (GTK_DATABOX (box_sorted_all_spike), graph_sorted_all_spike);
	}
	for (i=0; i<NUM_OF_SAMP_PER_SPIKE; i++)
	{
		Y_spike_2_template[i] = buff->spike_template.template[0][2][i];
	}
	gtk_databox_set_total_limits (GTK_DATABOX (box_spike_2), 0, NUM_OF_SAMP_PER_SPIKE, +2200, -2200);

	for (i=0;i<SPIKE_MEM_TO_DISPLAY_UNIT;i++)
	{
		Y_spike_nonsorted = g_ptr_array_index(Y_spikes_nonsorted_ptr,i);
		graph_spike_nonsorted = GTK_DATABOX_GRAPH(gtk_databox_lines_new (NUM_OF_SAMP_PER_SPIKE, X_axis, Y_spike_nonsorted, &color_spike_nonsorted, 0));
		graph_sorted_all_spike = GTK_DATABOX_GRAPH(gtk_databox_lines_new (NUM_OF_SAMP_PER_SPIKE, X_axis, Y_spike_nonsorted, &color_spike_nonsorted, 0));
		gtk_databox_graph_add (GTK_DATABOX (box_spike_nonsorted), graph_spike_nonsorted);
		gtk_databox_graph_add (GTK_DATABOX (box_sorted_all_spike), graph_sorted_all_spike);
	}
	gtk_databox_set_total_limits (GTK_DATABOX (box_spike_nonsorted), 0, NUM_OF_SAMP_PER_SPIKE, +2200, -2200);

	gtk_databox_set_total_limits (GTK_DATABOX (box_sorted_all_spike), 0, NUM_OF_SAMP_PER_SPIKE, +2200, -2200);


  	g_signal_connect (GTK_OBJECT (window), "destroy", G_CALLBACK (gtk_main_quit), NULL);

	back = buff->scan_number_read;

	g_signal_connect_swapped(G_OBJECT(ch_slct_button), "clicked", G_CALLBACK(ch_slct_func), G_OBJECT(box_all_spike));
	g_signal_connect_swapped(G_OBJECT(unit_slct_button), "clicked", G_CALLBACK(spk_unit_slct_func), G_OBJECT(box_all_spike));
	g_signal_connect_swapped(G_OBJECT(pause_button), "clicked", G_CALLBACK(pause_button_func), G_OBJECT(box_all_spike));
	g_signal_connect_swapped(G_OBJECT(clear_button), "clicked", G_CALLBACK(clear_screen_but_func), G_OBJECT(box_all_spike));
	g_signal_connect_swapped(G_OBJECT(clear_nonsorted_button), "clicked", G_CALLBACK(clear_nonsorted_but_func), G_OBJECT(box_all_spike));
	g_signal_connect_swapped(G_OBJECT(clear_unit_button), "clicked", G_CALLBACK(clear_unit_but_func), G_OBJECT(box_all_spike));
	g_signal_connect_swapped(G_OBJECT(clear_unit_template_button), "clicked", G_CALLBACK(clear_unit_template_but_func), G_OBJECT(box_all_spike));
	g_signal_connect_swapped(G_OBJECT(diff_button), "clicked", G_CALLBACK(diff_but_func), G_OBJECT(box_all_spike));
	g_signal_connect_swapped(G_OBJECT(sorting_onoff_button), "clicked", G_CALLBACK(sorting_onoff_but_func), G_OBJECT(box_all_spike));
	g_signal_connect_swapped(G_OBJECT(unit_sorting_onoff_button), "clicked", G_CALLBACK(unit_sorting_onoff_but_func), G_OBJECT(box_all_spike));
	g_signal_connect_swapped(G_OBJECT(include_unit_onoff_button), "clicked", G_CALLBACK(include_unit_onoff_but_func), G_OBJECT(box_all_spike));
	g_signal_connect_swapped(G_OBJECT(spike_filter_mode_button), "clicked", G_CALLBACK(spike_filter_mode_but_func), G_OBJECT(box_all_spike));
	g_signal_connect_swapped(G_OBJECT(save_template_button), "clicked", G_CALLBACK(save_template_but_func), G_OBJECT(box_all_spike));
	g_signal_connect_swapped(G_OBJECT(load_template_button), "clicked", G_CALLBACK(load_template_but_func), G_OBJECT(box_all_spike));	
	GtkWidget **nil;
   	g_signal_connect (G_OBJECT (box_all_spike), "selection-finalized", G_CALLBACK (rect_selection_func), nil);
 	gtk_widget_show_all(window);

	g_timeout_add(100, timeout_callback, box_all_spike);

	return;
}

gboolean timeout_callback(gpointer user_data) 
{
	


   	gint i,spike_view_cntr, spike_filter_cntr, spike_filter_cntr_2;
	bool spike_in_range;

	front = buff->scan_number_read;
	if (front < back)
		size = front + NUM_OF_SAMP_IN_BUFF - back;
	else
		size = front - back;	


	if (size == 0)
		return TRUE;  
		


	if (!disp_paused)
	{
		float *Y_temp, *Y_temp_sort;

		if (buff->filter_on)
		{
			for (i = 0; i < size; i++)
			{
				if ((back+i) >= NUM_OF_SAMP_IN_BUFF)
				{
					if (buff->spike_end[back+i-NUM_OF_SAMP_IN_BUFF].data[disp_chan] > 0)
					{  
						Y_temp = g_ptr_array_index(Y_all_spikes_ptr,Y_all_spikes_idx);
						Y_all_spikes_idx++;
						if (Y_all_spikes_idx == SPIKE_MEM_TO_DISPLAY)
							Y_all_spikes_idx = 0;

						if ((buff->sorted_spike_data[back+i-NUM_OF_SAMP_IN_BUFF].data[disp_chan] == 1) || (buff->sorted_spike_data[back+i-NUM_OF_SAMP_IN_BUFF].data[disp_chan] == 4))
						{
							Y_temp_sort = g_ptr_array_index(Y_spikes_0_ptr,Y_spikes_0_idx);
							Y_spikes_0_idx++;
							if (Y_spikes_0_idx == SPIKE_MEM_TO_DISPLAY_UNIT)
								Y_spikes_0_idx = 0;	
						}
						else if ((buff->sorted_spike_data[back+i-NUM_OF_SAMP_IN_BUFF].data[disp_chan] == 2) || (buff->sorted_spike_data[back+i-NUM_OF_SAMP_IN_BUFF].data[disp_chan] == 5))
						{
							Y_temp_sort = g_ptr_array_index(Y_spikes_1_ptr,Y_spikes_1_idx);
							Y_spikes_1_idx++;
							if (Y_spikes_1_idx == SPIKE_MEM_TO_DISPLAY_UNIT)
								Y_spikes_1_idx = 0;
						}
						else if ((buff->sorted_spike_data[back+i-NUM_OF_SAMP_IN_BUFF].data[disp_chan] == 3) || (buff->sorted_spike_data[back+i-NUM_OF_SAMP_IN_BUFF].data[disp_chan] == 6))
						{
							Y_temp_sort = g_ptr_array_index(Y_spikes_2_ptr,Y_spikes_2_idx);
							Y_spikes_2_idx++;
							if (Y_spikes_2_idx == SPIKE_MEM_TO_DISPLAY_UNIT)
								Y_spikes_2_idx = 0;
						}
						else
						{
							Y_temp_sort = g_ptr_array_index(Y_spikes_nonsorted_ptr,Y_spikes_nonsorted_idx);
							Y_spikes_nonsorted_idx++;
							if (Y_spikes_nonsorted_idx == SPIKE_MEM_TO_DISPLAY_UNIT)
								Y_spikes_nonsorted_idx = 0;							
						}
	
						for (spike_view_cntr=-INIT_POINT_OF_SPIKE; spike_view_cntr<1; spike_view_cntr++)
						{
							if ((back+i-NUM_OF_SAMP_IN_BUFF+spike_view_cntr) < 0)
							{
								Y_temp[spike_view_cntr+INIT_POINT_OF_SPIKE]= buff->filtered_scan[back+i+spike_view_cntr].data[disp_chan];
								Y_temp_sort[spike_view_cntr+INIT_POINT_OF_SPIKE]= buff->filtered_scan[back+i+spike_view_cntr].data[disp_chan];
							}
							else 
							{
								Y_temp[spike_view_cntr+INIT_POINT_OF_SPIKE]= buff->filtered_scan[back+i-NUM_OF_SAMP_IN_BUFF+spike_view_cntr].data[disp_chan];
								Y_temp_sort[spike_view_cntr+INIT_POINT_OF_SPIKE]= buff->filtered_scan[back+i-NUM_OF_SAMP_IN_BUFF+spike_view_cntr].data[disp_chan];
							}  
						}

						if (spike_filter_mode)
						{
							spike_in_range = 0;
							for (spike_filter_cntr = 0 ; spike_filter_cntr<NUM_OF_SAMP_PER_SPIKE;spike_filter_cntr++)
							{
								if ((Y_temp[spike_filter_cntr] >=  y_lower_1) && (Y_temp[spike_filter_cntr] <=  y_upper_1)) 
								{
									if ((spike_filter_cntr >= x_lower_1) && (spike_filter_cntr <= x_upper_1))
									{
										for (spike_filter_cntr_2 = 0 ; spike_filter_cntr_2<NUM_OF_SAMP_PER_SPIKE;spike_filter_cntr_2++)
										{
											if ((Y_temp[spike_filter_cntr_2] >=  y_lower_2) && (Y_temp[spike_filter_cntr_2] <=  y_upper_2)) 
											{
												if ((spike_filter_cntr_2 >= x_lower_2) && (spike_filter_cntr_2 <= x_upper_2))
												{
													spike_in_range = 1;
													break;
												}
											}
										}
									}
								}
							}
							if (!spike_in_range)
							{
								for (spike_filter_cntr = 0 ; spike_filter_cntr<NUM_OF_SAMP_PER_SPIKE;spike_filter_cntr++)
								{
									Y_temp[spike_filter_cntr] = 0;
								}
								Y_all_spikes_idx--;
								if (Y_all_spikes_idx < 0)
									Y_all_spikes_idx = SPIKE_MEM_TO_DISPLAY -1;
							}
						}		
						gtk_databox_set_total_limits (GTK_DATABOX (box_all_spike), 0, NUM_OF_SAMP_PER_SPIKE, +2200, -2200);	
						gtk_databox_set_total_limits (GTK_DATABOX (box_spike_0), 0, NUM_OF_SAMP_PER_SPIKE, +2200, -2200);							
						gtk_databox_set_total_limits (GTK_DATABOX (box_spike_1), 0, NUM_OF_SAMP_PER_SPIKE, +2200, -2200);	
						gtk_databox_set_total_limits (GTK_DATABOX (box_spike_2), 0, NUM_OF_SAMP_PER_SPIKE, +2200, -2200);	
						gtk_databox_set_total_limits (GTK_DATABOX (box_spike_nonsorted), 0, NUM_OF_SAMP_PER_SPIKE, +2200, -2200);
						gtk_databox_set_total_limits (GTK_DATABOX (box_sorted_all_spike), 0, NUM_OF_SAMP_PER_SPIKE, +2200, -2200);
					}  
				}
				else
				{
					if (buff->spike_end[back+i].data[disp_chan] > 0)
					{   
						Y_temp = g_ptr_array_index(Y_all_spikes_ptr,Y_all_spikes_idx);
						Y_all_spikes_idx++;
						if (Y_all_spikes_idx == SPIKE_MEM_TO_DISPLAY)
							Y_all_spikes_idx = 0;

						if ((buff->sorted_spike_data[back+i].data[disp_chan] == 1) || (buff->sorted_spike_data[back+i].data[disp_chan] == 4))
						{
							Y_temp_sort = g_ptr_array_index(Y_spikes_0_ptr,Y_spikes_0_idx);
							Y_spikes_0_idx++;
							if (Y_spikes_0_idx == SPIKE_MEM_TO_DISPLAY_UNIT)
								Y_spikes_0_idx = 0;
						}
						else if ((buff->sorted_spike_data[back+i].data[disp_chan] == 2) ||  (buff->sorted_spike_data[back+i].data[disp_chan] == 5))
						{
							Y_temp_sort = g_ptr_array_index(Y_spikes_1_ptr,Y_spikes_1_idx);
							Y_spikes_1_idx++;
							if (Y_spikes_1_idx == SPIKE_MEM_TO_DISPLAY_UNIT)
								Y_spikes_1_idx = 0;	
						}
						else if ((buff->sorted_spike_data[back+i].data[disp_chan] == 3) || (buff->sorted_spike_data[back+i].data[disp_chan] == 6))
						{
							Y_temp_sort = g_ptr_array_index(Y_spikes_2_ptr,Y_spikes_2_idx);
							Y_spikes_2_idx++;
							if (Y_spikes_2_idx == SPIKE_MEM_TO_DISPLAY_UNIT)
								Y_spikes_2_idx = 0;
						}
						else
						{
							Y_temp_sort = g_ptr_array_index(Y_spikes_nonsorted_ptr,Y_spikes_nonsorted_idx);
							Y_spikes_nonsorted_idx++;
							if (Y_spikes_nonsorted_idx == SPIKE_MEM_TO_DISPLAY_UNIT)
								Y_spikes_nonsorted_idx = 0;							
						}

						for (spike_view_cntr=-INIT_POINT_OF_SPIKE; spike_view_cntr<1; spike_view_cntr++)
						{
							if ((back+i+spike_view_cntr) < 0)
							{
								Y_temp[spike_view_cntr+INIT_POINT_OF_SPIKE]= buff->filtered_scan[back+i+spike_view_cntr+NUM_OF_SAMP_IN_BUFF].data[disp_chan];
								Y_temp_sort[spike_view_cntr+INIT_POINT_OF_SPIKE]= buff->filtered_scan[back+i+spike_view_cntr+NUM_OF_SAMP_IN_BUFF].data[disp_chan];
							}
							else 
							{
								Y_temp[spike_view_cntr+INIT_POINT_OF_SPIKE]= buff->filtered_scan[back+i+spike_view_cntr].data[disp_chan];
								Y_temp_sort[spike_view_cntr+INIT_POINT_OF_SPIKE]= buff->filtered_scan[back+i+spike_view_cntr].data[disp_chan];
							} 
						}

						if (spike_filter_mode)
						{
							spike_in_range = 0;
							for (spike_filter_cntr = 0 ; spike_filter_cntr<NUM_OF_SAMP_PER_SPIKE;spike_filter_cntr++)
							{
								if ((Y_temp[spike_filter_cntr] >=  y_lower_1) && (Y_temp[spike_filter_cntr] <=  y_upper_1)) 
								{
									if ((spike_filter_cntr >= x_lower_1) && (spike_filter_cntr <= x_upper_1))
									{
										for (spike_filter_cntr_2 = 0 ; spike_filter_cntr_2<NUM_OF_SAMP_PER_SPIKE;spike_filter_cntr_2++)
										{
											if ((Y_temp[spike_filter_cntr_2] >=  y_lower_2) && (Y_temp[spike_filter_cntr_2] <=  y_upper_2)) 
											{
												if ((spike_filter_cntr_2 >= x_lower_2) && (spike_filter_cntr_2 <= x_upper_2))
												{
													spike_in_range = 1;
													break;
												}
											}
										}
									}
								}
							}
							if (!spike_in_range)
							{
								for (spike_filter_cntr = 0 ; spike_filter_cntr<NUM_OF_SAMP_PER_SPIKE;spike_filter_cntr++)
								{
									Y_temp[spike_filter_cntr] = 0;
								}
								Y_all_spikes_idx--;
								if (Y_all_spikes_idx < 0)
									Y_all_spikes_idx = SPIKE_MEM_TO_DISPLAY -1;
							}
						}		
					
						gtk_databox_set_total_limits (GTK_DATABOX (box_all_spike), -0, NUM_OF_SAMP_PER_SPIKE, +2200, -2200);	
						gtk_databox_set_total_limits (GTK_DATABOX (box_spike_0), 0, NUM_OF_SAMP_PER_SPIKE, +2200, -2200);							
						gtk_databox_set_total_limits (GTK_DATABOX (box_spike_1), 0, NUM_OF_SAMP_PER_SPIKE, +2200, -2200);	
						gtk_databox_set_total_limits (GTK_DATABOX (box_spike_2), 0, NUM_OF_SAMP_PER_SPIKE, +2200, -2200);	
						gtk_databox_set_total_limits (GTK_DATABOX (box_spike_nonsorted), 0, NUM_OF_SAMP_PER_SPIKE, +2200, -2200);
						gtk_databox_set_total_limits (GTK_DATABOX (box_sorted_all_spike), 0, NUM_OF_SAMP_PER_SPIKE, +2200, -2200);
					}  
				} 
			}
		}

	}


	back = front;

	return TRUE;  

}

gboolean ch_slct_func (GtkDatabox * box)
{

	float *Y_local;
	int i,j;

	for (i=0;i<SPIKE_MEM_TO_DISPLAY;i++)
	{
		Y_local = g_ptr_array_index(Y_all_spikes_ptr,i);
		for (j=0; j<NUM_OF_SAMP_PER_SPIKE; j++)
		{
			Y_local[j] = 0;
		}
	}

	for (i=0;i<SPIKE_MEM_TO_DISPLAY_UNIT;i++)
	{
		Y_local = g_ptr_array_index(Y_spikes_0_ptr,i);
		for (j=0; j<NUM_OF_SAMP_PER_SPIKE; j++)
		{
			Y_local[j] = 0;
		}
	}

	for (i=0;i<SPIKE_MEM_TO_DISPLAY_UNIT;i++)
	{
		Y_local = g_ptr_array_index(Y_spikes_1_ptr,i);
		for (j=0; j<NUM_OF_SAMP_PER_SPIKE; j++)
		{
			Y_local[j] = 0;
		}
	}

	for (i=0;i<SPIKE_MEM_TO_DISPLAY_UNIT;i++)
	{
		Y_local = g_ptr_array_index(Y_spikes_2_ptr,i);
		for (j=0; j<NUM_OF_SAMP_PER_SPIKE; j++)
		{
			Y_local[j] = 0;
		}
	}

	for (i=0;i<SPIKE_MEM_TO_DISPLAY_UNIT;i++)
	{
		Y_local = g_ptr_array_index(Y_spikes_nonsorted_ptr,i);
		for (j=0; j<NUM_OF_SAMP_PER_SPIKE; j++)
		{
			Y_local[j] = 0;
		}
	}

	if (gtk_combo_box_get_active (GTK_COMBO_BOX(combo)) >= 0)
	{
		disp_chan = gtk_combo_box_get_active (GTK_COMBO_BOX(combo));
		sprintf(strDiff, "%E" , buff->spike_template.diff_thres[disp_chan][selected_spike_unit]);
		gtk_entry_set_text (GTK_ENTRY(entryDiff), strDiff);
		sprintf(str_selected_chan, "%d" , disp_chan+1);
		gtk_label_set_text( lbl_selected_chan, str_selected_chan );
	}

	for (i=0; i<NUM_OF_SAMP_PER_SPIKE; i++)
	{
		Y_spike_0_template[i] = buff->spike_template.template[disp_chan][0][i];
		Y_spike_1_template[i] = buff->spike_template.template[disp_chan][1][i];
		Y_spike_2_template[i] = buff->spike_template.template[disp_chan][2][i];						
	}	

	gtk_databox_set_total_limits (GTK_DATABOX (box_all_spike), 0, NUM_OF_SAMP_PER_SPIKE, +2200, -2200);	
	gtk_databox_set_total_limits (GTK_DATABOX (box_spike_0), 0, NUM_OF_SAMP_PER_SPIKE, +2200, -2200);
	gtk_databox_set_total_limits (GTK_DATABOX (box_spike_1), 0, NUM_OF_SAMP_PER_SPIKE, +2200, -2200);
	gtk_databox_set_total_limits (GTK_DATABOX (box_spike_2), 0, NUM_OF_SAMP_PER_SPIKE, +2200, -2200);
	gtk_databox_set_total_limits (GTK_DATABOX (box_spike_nonsorted), 0, NUM_OF_SAMP_PER_SPIKE, +2200, -2200);

	if (buff->spike_template.sorting_on[disp_chan][selected_spike_unit] )
	{
		gtk_button_set_label (unit_sorting_onoff_button,"Turn Unit Sorting-OFF");
	}
	else
	{
		gtk_button_set_label (unit_sorting_onoff_button,"Turn Unit Sorting-ON");
	}

	if (buff->spike_template.include_unit[disp_chan][selected_spike_unit] )
	{
		gtk_button_set_label (include_unit_onoff_button,"Include Unit-OFF");
	}
	else
	{
		gtk_button_set_label (include_unit_onoff_button,"Include Unit-ON");
	}

	return TRUE;
}

gboolean spk_unit_slct_func (GtkDatabox * box)
{
	if (gtk_combo_box_get_active (GTK_COMBO_BOX(combo_spike)) >= 0)
	{
		selected_spike_unit = gtk_combo_box_get_active (GTK_COMBO_BOX(combo_spike));
		sprintf(strDiff, "%E" , buff->spike_template.diff_thres[disp_chan][selected_spike_unit]);
		gtk_entry_set_text (GTK_ENTRY(entryDiff), strDiff);
		sprintf(str_selected_unit, "%d" , selected_spike_unit+1);
		gtk_label_set_text( lbl_selected_unit, str_selected_unit );
	}

	if (buff->spike_template.sorting_on[disp_chan][selected_spike_unit] )
	{
		gtk_button_set_label (unit_sorting_onoff_button,"Turn Unit Sorting-OFF");
	}
	else
	{
		gtk_button_set_label (unit_sorting_onoff_button,"Turn Unit Sorting-ON");
	}

	if (buff->spike_template.include_unit[disp_chan][selected_spike_unit] )
	{
		gtk_button_set_label (include_unit_onoff_button,"Include Unit-OFF");
	}
	else
	{
		gtk_button_set_label (include_unit_onoff_button,"Include Unit-ON");
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


gboolean clear_screen_but_func (GtkDatabox * box)
{
	float *Y_local;
	int i,j;

	for (i=0;i<SPIKE_MEM_TO_DISPLAY;i++)
	{
		Y_local = g_ptr_array_index(Y_all_spikes_ptr,i);
		for (j=0; j<NUM_OF_SAMP_PER_SPIKE; j++)
		{
			Y_local[j] = 0;
		}
	}

	gtk_databox_set_total_limits (GTK_DATABOX (box_all_spike), 0, NUM_OF_SAMP_PER_SPIKE, +2200, -2200);	

	return TRUE;	
}

gboolean clear_unit_but_func (GtkDatabox * box)
{

	float *Y_local;
	int i,j;
	if (selected_spike_unit == 0)
	{
		for (i=0;i<SPIKE_MEM_TO_DISPLAY_UNIT;i++)
		{
			Y_local = g_ptr_array_index(Y_spikes_0_ptr,i);
			for (j=0; j<NUM_OF_SAMP_PER_SPIKE; j++)
			{
				Y_local[j] = 0;
			}
		}
		gtk_databox_set_total_limits (GTK_DATABOX (box_spike_0), 0, NUM_OF_SAMP_PER_SPIKE, +2200, -2200);
	}
	else if (selected_spike_unit == 1)
	{
		for (i=0;i<SPIKE_MEM_TO_DISPLAY_UNIT;i++)
		{
			Y_local = g_ptr_array_index(Y_spikes_1_ptr,i);
			for (j=0; j<NUM_OF_SAMP_PER_SPIKE; j++)
			{
				Y_local[j] = 0;
			}
		}	
		gtk_databox_set_total_limits (GTK_DATABOX (box_spike_1), 0, NUM_OF_SAMP_PER_SPIKE, +2200, -2200);
	}	
	else if (selected_spike_unit == 2)	
	{
		for (i=0;i<SPIKE_MEM_TO_DISPLAY_UNIT;i++)
		{
			Y_local = g_ptr_array_index(Y_spikes_2_ptr,i);
			for (j=0; j<NUM_OF_SAMP_PER_SPIKE; j++)
			{
				Y_local[j] = 0;
			}
		}
		gtk_databox_set_total_limits (GTK_DATABOX (box_spike_2), 0, NUM_OF_SAMP_PER_SPIKE, +2200, -2200);	
	}	

	return TRUE;	
}

gboolean clear_unit_template_but_func (GtkDatabox * box)
{
	int i,j;

	gtk_button_set_label (unit_sorting_onoff_button,"Turn Unit Sorting-ON");
	gtk_button_set_label (include_unit_onoff_button,"Include Unit-ON");
	if (selected_spike_unit == 0)
	{
		buff->spike_template.sorting_on[disp_chan][0] = 0;
		buff->spike_template.include_unit[disp_chan][0] = 0;

		for (i=0; i<NUM_OF_SAMP_PER_SPIKE; i++)
		{
			for (j=0; j<NUM_OF_SAMP_PER_SPIKE; j++)
			{
				buff->spike_template.S[disp_chan][0][i][j] = 0;
			}
		}

		buff->spike_template.diff_thres[disp_chan][0] = 0;
		sprintf(strDiff, "%E" , buff->spike_template.diff_thres[disp_chan][0]);
		gtk_entry_set_text (GTK_ENTRY(entryDiff), strDiff);
	
		for (i=0; i<NUM_OF_SAMP_PER_SPIKE; i++)
		{
			buff->spike_template.template[disp_chan][0][i] = 0;
			Y_spike_0_template[i] = buff->spike_template.template[disp_chan][0][i];
		}

		gtk_databox_set_total_limits (GTK_DATABOX (box_spike_0), 0, NUM_OF_SAMP_PER_SPIKE, +2200, -2200);	
	}
	else if (selected_spike_unit == 1)
	{
		buff->spike_template.sorting_on[disp_chan][1] = 0;
		buff->spike_template.include_unit[disp_chan][1] = 0;

		for (i=0; i<NUM_OF_SAMP_PER_SPIKE; i++)
		{
			for (j=0; j<NUM_OF_SAMP_PER_SPIKE; j++)
			{
				buff->spike_template.S[disp_chan][1][i][j] = 0;
			}
		}

		buff->spike_template.diff_thres[disp_chan][1] = 0;
		sprintf(strDiff, "%E" , buff->spike_template.diff_thres[disp_chan][1]);
		gtk_entry_set_text (GTK_ENTRY(entryDiff), strDiff);

		for (i=0; i<NUM_OF_SAMP_PER_SPIKE; i++)
		{
			buff->spike_template.template[disp_chan][1][i] = 0;
			Y_spike_1_template[i] = buff->spike_template.template[disp_chan][1][i];
		}
		gtk_databox_set_total_limits (GTK_DATABOX (box_spike_1), 0, NUM_OF_SAMP_PER_SPIKE, +2200, -2200);
	}
	else if (selected_spike_unit == 2)
	{
		buff->spike_template.sorting_on[disp_chan][2] = 0;
		buff->spike_template.include_unit[disp_chan][2] = 0;

		for (i=0; i<NUM_OF_SAMP_PER_SPIKE; i++)
		{
			for (j=0; j<NUM_OF_SAMP_PER_SPIKE; j++)
			{
				buff->spike_template.S[disp_chan][2][i][j] = 0;
			}
		}

		buff->spike_template.diff_thres[disp_chan][2] = 0;
		sprintf(strDiff, "%E" , buff->spike_template.diff_thres[disp_chan][2]);
		gtk_entry_set_text (GTK_ENTRY(entryDiff), strDiff);

		for (i=0; i<NUM_OF_SAMP_PER_SPIKE; i++)
		{
			buff->spike_template.template[disp_chan][2][i] = 0;
			Y_spike_2_template[i] = buff->spike_template.template[disp_chan][2][i];
		}
		gtk_databox_set_total_limits (GTK_DATABOX (box_spike_2), 0, NUM_OF_SAMP_PER_SPIKE, +2200, -2200);
	}
	return TRUE;	
}


gboolean clear_nonsorted_but_func (GtkDatabox * box)
{
	float *Y_local;
	int i,j;
	for (i=0;i<SPIKE_MEM_TO_DISPLAY_UNIT;i++)
	{
		Y_local = g_ptr_array_index(Y_spikes_nonsorted_ptr,i);
		for (j=0; j<NUM_OF_SAMP_PER_SPIKE; j++)
		{
			Y_local[j] = 0;
		}
	}
	gtk_databox_set_total_limits (GTK_DATABOX (box_spike_nonsorted), 0, NUM_OF_SAMP_PER_SPIKE, +2200, -2200);
	return TRUE;
}

gboolean diff_but_func (GtkDatabox * box)
{
 	g_stpcpy(strDiff, gtk_entry_get_text(GTK_ENTRY(entryDiff)));
//	strDiff = gtk_entry_get_text(GTK_ENTRY(entryDiff));
	buff->spike_template.diff_thres[disp_chan][selected_spike_unit]=(double)atof(strDiff);
	return TRUE;	
}

gboolean sorting_onoff_but_func (GtkDatabox * box)
{
	if (buff->sorting_on)
	{
		buff->sorting_on = 0;
		gtk_button_set_label (sorting_onoff_button,"Turn Sorting-ON");
	}
	else
	{
		buff->sorting_on = 1;
		gtk_button_set_label (sorting_onoff_button,"Turn Sorting-OFF");
	}
}

gboolean unit_sorting_onoff_but_func (GtkDatabox * box)
{
	if (buff->spike_template.sorting_on[disp_chan][selected_spike_unit] )
	{
		buff->spike_template.sorting_on[disp_chan][selected_spike_unit] = 0 ;
		gtk_button_set_label (unit_sorting_onoff_button,"Turn Unit Sorting-ON");
	}
	else
	{
		buff->spike_template.sorting_on[disp_chan][selected_spike_unit] = 1 ;
		gtk_button_set_label (unit_sorting_onoff_button,"Turn Unit Sorting-OFF");
	}
}

gboolean include_unit_onoff_but_func (GtkDatabox * box)
{
	if (buff->spike_template.include_unit[disp_chan][selected_spike_unit] )
	{
		buff->spike_template.include_unit[disp_chan][selected_spike_unit] = 0 ;
		gtk_button_set_label (include_unit_onoff_button,"Include Unit-ON");
	}
	else
	{
		buff->spike_template.include_unit[disp_chan][selected_spike_unit] = 1 ;
		gtk_button_set_label (include_unit_onoff_button,"Include Unit-OFF");
	}
}

gboolean spike_filter_mode_but_func (GtkDatabox * box)
{
	if (spike_filter_mode)
	{
		spike_filter_mode = 0 ;
		gtk_button_set_label (spike_filter_mode_button,"Turn Spike Filter ON");
	}
	else
	{
		spike_filter_mode = 1 ;
		gtk_button_set_label (spike_filter_mode_button,"Turn Spike Filter OFF");
	}
}

gboolean rect_selection_func (GtkDatabox * box, GtkDataboxValueRectangle * selectionValues)
{
	printf ("%f %f %f %f\n", selectionValues->x1, selectionValues->x2, selectionValues->y1, selectionValues->y2);

	if (spike_filter_mode)
	{
		if (rect_switch)
		{
			rect_switch = 0;		
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
		else
		{
			rect_switch = 1;		
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
		return TRUE;
	}
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



	float *Y_analyze, *Y_mean, *Y_temp;
	int i ,j, k, idx;
	idx= 0;
	GPtrArray *Y_spikes_in_range_array;
	Y_spikes_in_range_array = g_ptr_array_new();
	for (i=0;i<SPIKE_MEM_TO_DISPLAY;i++)
	{
		Y_analyze = g_ptr_array_index(Y_all_spikes_ptr,i);
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
	printf("Selected number of spikes: %d\n", idx);

	Y_mean = g_new0 (float, NUM_OF_SAMP_PER_SPIKE);	
	if (idx > MIN_SPIKE_NUM_FOR_TEMPLATE)
	{		
		for (i=0; i<NUM_OF_SAMP_PER_SPIKE; i++)
		{
			for (j=0; j<MIN_SPIKE_NUM_FOR_TEMPLATE; j++)
			{
				Y_temp = g_ptr_array_index(Y_spikes_in_range_array,(int)(j*(((float)idx)/MIN_SPIKE_NUM_FOR_TEMPLATE)));    // to select more distributed spikes in time.
				printf("idx: %d\n", (int)(j*(((float)idx)/MIN_SPIKE_NUM_FOR_TEMPLATE)));
				Y_mean[i] = Y_mean[i]+Y_temp[i];
			}
		}
	}

	float fabs_Y_mean =0;
	for (i=0; i<NUM_OF_SAMP_PER_SPIKE; i++)
	{
		fabs_Y_mean = fabs_Y_mean + fabs(Y_mean[i]);
	}

	if ((fabs_Y_mean != 0) && (idx > MIN_SPIKE_NUM_FOR_TEMPLATE))
	{
		for (i=0; i<NUM_OF_SAMP_PER_SPIKE; i++)
		{
			buff->spike_template.template[disp_chan][selected_spike_unit][i] = Y_mean[i]/MIN_SPIKE_NUM_FOR_TEMPLATE;
			if (selected_spike_unit == 0)
				Y_spike_0_template[i] = buff->spike_template.template[disp_chan][selected_spike_unit][i];
			else if (selected_spike_unit == 1)
				Y_spike_1_template[i] = buff->spike_template.template[disp_chan][selected_spike_unit][i];
			else if (selected_spike_unit == 2)
				Y_spike_2_template[i] = buff->spike_template.template[disp_chan][selected_spike_unit][i];						
		}


		gtk_databox_set_total_limits (GTK_DATABOX (box_spike_0), 0, NUM_OF_SAMP_PER_SPIKE, +2200, -2200);
		gtk_databox_set_total_limits (GTK_DATABOX (box_spike_1), 0, NUM_OF_SAMP_PER_SPIKE, +2200, -2200);
		gtk_databox_set_total_limits (GTK_DATABOX (box_spike_2), 0, NUM_OF_SAMP_PER_SPIKE, +2200, -2200);

		double temp_mtx_1[MIN_SPIKE_NUM_FOR_TEMPLATE][NUM_OF_SAMP_PER_SPIKE];
		double temp_mtx_2[MIN_SPIKE_NUM_FOR_TEMPLATE][NUM_OF_SAMP_PER_SPIKE][NUM_OF_SAMP_PER_SPIKE];

		for (i=0; i<MIN_SPIKE_NUM_FOR_TEMPLATE; i++)
		{
			Y_temp = g_ptr_array_index(Y_spikes_in_range_array,i);
			for (j=0; j<NUM_OF_SAMP_PER_SPIKE; j++)
			{
				temp_mtx_1[i][j] = (double)(Y_temp[j] - buff->spike_template.template[disp_chan][selected_spike_unit][j]);
			}		
		}
		for (i=0; i<MIN_SPIKE_NUM_FOR_TEMPLATE; i++)
		{
			for (j=0; j<NUM_OF_SAMP_PER_SPIKE; j++)
			{
				for (k=0; k<NUM_OF_SAMP_PER_SPIKE; k++)
				{
					temp_mtx_2[i][j][k] = temp_mtx_1[i][j] * temp_mtx_1[i][k] ;
				}
			}	
		}


		MAT *S, *S_inv; 
		S=m_get(NUM_OF_SAMP_PER_SPIKE,NUM_OF_SAMP_PER_SPIKE);
		S_inv=m_get(NUM_OF_SAMP_PER_SPIKE,NUM_OF_SAMP_PER_SPIKE);
		m_zero(S);
		m_zero(S_inv);
		for (i=0; i<NUM_OF_SAMP_PER_SPIKE; i++)
		{
			for (j=0; j<NUM_OF_SAMP_PER_SPIKE; j++)
			{
				for (k=0; k<MIN_SPIKE_NUM_FOR_TEMPLATE; k++)
				{
					S->me[i][j] = S->me[i][j] + temp_mtx_2[k][i][j];
				}
				S->me[i][j] = (double)round((S->me[i][j] / MIN_SPIKE_NUM_FOR_TEMPLATE)*1000000)/1000000.0;
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
			for (j=0; j<NUM_OF_SAMP_PER_SPIKE; j++)
			{
				if (i == j)
					determinant = determinant * (LU->me[i][j]);
			}
		}
		
		m_inverse(S,S_inv);
	
		if (determinant < 0)
			determinant = determinant *(-1.0);	
		printf ("log(determinant) = %lf\n", log(determinant)); 	
		buff->spike_template.sqrt_det_S[disp_chan][selected_spike_unit] = sqrt(determinant); 
		buff->spike_template.log_det_S[disp_chan][selected_spike_unit] = log(determinant);
		for (i=0; i<NUM_OF_SAMP_PER_SPIKE; i++)
		{
			for (j=0; j<NUM_OF_SAMP_PER_SPIKE; j++)
			{	
				buff->spike_template.inv_S[disp_chan][selected_spike_unit][i][j] = S_inv->me[i][j];
				buff->spike_template.S[disp_chan][selected_spike_unit][i][j] = S->me[i][j];
			}
		}
	
		m_free(S);
		m_free(S_inv);
	
	}
	else
	{
		printf("Too few or inconvenient spikes selected\n");
	}
	g_ptr_array_free(Y_spikes_in_range_array,FALSE);
	g_free(Y_mean);
}


gboolean save_template_but_func (GtkDatabox * box)
{
	strAddFileName = gtk_entry_get_text(GTK_ENTRY(entryAddFileName));

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

	strcpy(strFileName, "/home/kocaturk/SPIKE_DATA/templates/");	
	strcat(strFileName, time_str+4);
	strcat(strFileName, strAddFileName);
	fp = fopen(strFileName, "w");

	int i,j,k,l,check_S[NUM_OF_CHAN][NUM_OF_TEMP_PER_CHAN];

	for (i=0; i<NUM_OF_CHAN; i++)
	{
		for (j=0; j<NUM_OF_TEMP_PER_CHAN; j++)
		{
			check_S[i][j] = 0;
			for (k=0; k<NUM_OF_SAMP_PER_SPIKE; k++)
			{
				for (l=0; l<NUM_OF_SAMP_PER_SPIKE; l++)
				{
					check_S[i][j] = check_S[i][j] + abs(buff->spike_template.S[i][j][k][l]);
				}	
			}
		}
	}

	for (i=0; i<NUM_OF_CHAN; i++)
	{
		for (j=0; j<NUM_OF_TEMP_PER_CHAN; j++)
		{			
			for (k=0; k<NUM_OF_SAMP_PER_SPIKE; k++)
			{
				if (check_S[i][j] == 0)
					buff->spike_template.template[i][j][k] = 0;
				fprintf(fp, "%E\n", buff->spike_template.template[i][j][k]);
			}
		}
	}

	for (i=0; i<NUM_OF_CHAN; i++)
	{
		for (j=0; j<NUM_OF_TEMP_PER_CHAN; j++)
		{
			for (k=0; k<NUM_OF_SAMP_PER_SPIKE; k++)
			{
				for (l=0; l<NUM_OF_SAMP_PER_SPIKE; l++)
				{
					fprintf(fp, "%E\n", buff->spike_template.S[i][j][k][l]);
				}
			}
		}
	}

	for (i=0; i<NUM_OF_CHAN; i++)
	{
		for (j=0; j<NUM_OF_TEMP_PER_CHAN; j++)
		{
			if (check_S[i][j] == 0)
				buff->spike_template.diff_thres[i][j] = 0;
			fprintf(fp, "%E\n", buff->spike_template.diff_thres[i][j]);
		}
	}

	for (i=0; i<NUM_OF_CHAN; i++)
	{
		for (j=0; j<NUM_OF_TEMP_PER_CHAN; j++)
		{
			if (check_S[i][j] == 0)
				buff->spike_template.sorting_on[i][j] = 0;
			fprintf(fp, "%d\n", buff->spike_template.sorting_on[i][j]);
		}
	}

	for (i=0; i<NUM_OF_CHAN; i++)
	{
		for (j=0; j<NUM_OF_TEMP_PER_CHAN; j++)
		{
			if (check_S[i][j] == 0)
				buff->spike_template.include_unit[i][j] = 0;
			fprintf(fp, "%d\n", buff->spike_template.include_unit[i][j]);
		}
	}

	for (i=0; i<NUM_OF_CHAN; i++)
	{
		fprintf(fp, "%.0f\n", buff->Threshold[i]);
	}

	fclose(fp);
	return TRUE;	
}

gboolean load_template_but_func (GtkDatabox * box)
{
	char strFileName[100];
	strcpy(strFileName, "/home/kocaturk/SPIKE_DATA/templates/");

	strLoadFileName = gtk_entry_get_text(GTK_ENTRY(entryLoadFileName));
	strcat(strFileName, strLoadFileName);

	int i,j,k,l, check_S[NUM_OF_CHAN][NUM_OF_TEMP_PER_CHAN];

	fp = fopen(strFileName, "r");
	if ( fp != NULL )
	{
		char line [20];  
		for (i=0; i<NUM_OF_CHAN; i++)
		{
			for (j=0; j<NUM_OF_TEMP_PER_CHAN; j++)
			{
				for (k=0; k<NUM_OF_SAMP_PER_SPIKE; k++)
				{
      					fgets(line, sizeof line, fp );
					buff->spike_template.template[i][j][k] = (double)atof(line);
				}
			}
		}
		for (i=0; i<NUM_OF_CHAN; i++)
		{
			for (j=0; j<NUM_OF_TEMP_PER_CHAN; j++)
			{
				for (k=0; k<NUM_OF_SAMP_PER_SPIKE; k++)
				{
					for (l=0; l<NUM_OF_SAMP_PER_SPIKE; l++)
					{
      						fgets(line, sizeof line, fp );
						buff->spike_template.S[i][j][k][l] = (double)atof(line);
					}
				}
			}
		}

		//Calculate inverse S and determinant from saved S
		MAT *S, *S_inv; 
		S=m_get(NUM_OF_SAMP_PER_SPIKE,NUM_OF_SAMP_PER_SPIKE);
		S_inv=m_get(NUM_OF_SAMP_PER_SPIKE,NUM_OF_SAMP_PER_SPIKE);
		MAT *LU; 
		PERM *pivot; 
		LU = m_get(S->m,S->n);
		double determinant;
		for (i=0; i<NUM_OF_CHAN; i++)
		{
			for (j=0; j<NUM_OF_TEMP_PER_CHAN; j++)
			{
				check_S[i][j] = 0;
				for (k=0; k<NUM_OF_SAMP_PER_SPIKE; k++)
				{
					for (l=0; l<NUM_OF_SAMP_PER_SPIKE; l++)
					{
						check_S[i][j] = check_S[i][j] + abs(buff->spike_template.S[i][j][k][l]);
					}	
				}
			}
		}
		for (i=0; i<NUM_OF_CHAN; i++)
		{
			for (j=0; j<NUM_OF_TEMP_PER_CHAN; j++)
			{
				if (check_S[i][j] > 0)
				{
					m_zero(S);
					m_zero(S_inv);
					m_zero(LU);
					for (k=0; k<NUM_OF_SAMP_PER_SPIKE; k++)
					{
						for (l=0; l<NUM_OF_SAMP_PER_SPIKE; l++)
						{
							S->me[k][l] = buff->spike_template.S[i][j][k][l];
						}	
					}
					for (k=0; k<NUM_OF_SAMP_PER_SPIKE; k++)
					{
						for (l=0; l<NUM_OF_SAMP_PER_SPIKE; l++)
						{
							LU->me[k][l] = S->me[k][l]; 
						}
					}

					pivot = px_get(S->m);
					LU = LUfactor(LU,pivot);
					determinant = 1.0;

					for (k=0; k<NUM_OF_SAMP_PER_SPIKE; k++)
					{
						for (l=0; l<NUM_OF_SAMP_PER_SPIKE; l++)
						{
							if (k == l)
								determinant = determinant * (LU->me[k][l]);
						}
					}
		
					m_inverse(S,S_inv);

					if (determinant < 0)
						determinant = determinant *(-1.0);	

					buff->spike_template.sqrt_det_S[i][j] = sqrt(determinant); 
					buff->spike_template.log_det_S[i][j] = log(determinant);

					for (k=0; k<NUM_OF_SAMP_PER_SPIKE; k++)
					{
						for (l=0; l<NUM_OF_SAMP_PER_SPIKE; l++)
						{	
							buff->spike_template.inv_S[i][j][k][l] = S_inv->me[k][l];
						}
					}
				}
			}

		}
		m_free(S);
		m_free(S_inv);
		m_free(LU);

		for (i=0; i<NUM_OF_CHAN; i++)
		{
			for (j=0; j<NUM_OF_TEMP_PER_CHAN; j++)
			{
				if (check_S[i][j] == 0)
					buff->spike_template.template[i][j][k] = 0;
			}
		}

		for (i=0; i<NUM_OF_CHAN; i++)
		{
			for (j=0; j<NUM_OF_TEMP_PER_CHAN; j++)
			{
      				fgets(line, sizeof line, fp );
				if (check_S[i][j] > 0)
					buff->spike_template.diff_thres[i][j] = (double)atof(line);
				else
					buff->spike_template.diff_thres[i][j] = 0;
			}
		}
		for (i=0; i<NUM_OF_CHAN; i++)
		{
			for (j=0; j<NUM_OF_TEMP_PER_CHAN; j++)
			{
      				fgets(line, sizeof line, fp );
				if (check_S[i][j] > 0)
					buff->spike_template.sorting_on[i][j] = (bool)atof(line);
				else
					buff->spike_template.sorting_on[i][j] = 0;
			}
		}
		for (i=0; i<NUM_OF_CHAN; i++)
		{
			for (j=0; j<NUM_OF_TEMP_PER_CHAN; j++)
			{
      				fgets(line, sizeof line, fp );
				if (check_S[i][j] > 0)
					buff->spike_template.include_unit[i][j] = (bool)atof(line);
				else
					buff->spike_template.include_unit[i][j] = 0;
			}
		}
		for (i=0; i<NUM_OF_CHAN; i++)
		{
		      	fgets(line, sizeof line, fp );				
			buff->Threshold[i]= (double)atof(line);
		}	
		fclose (fp);
	}
	else
	{
		perror (strFileName); 
	}

	for (i=0; i<NUM_OF_SAMP_PER_SPIKE; i++)
	{
		Y_spike_0_template[i] = buff->spike_template.template[disp_chan][0][i];
	}
	for (i=0; i<NUM_OF_SAMP_PER_SPIKE; i++)
	{
		Y_spike_1_template[i] = buff->spike_template.template[disp_chan][1][i];
	}
	for (i=0; i<NUM_OF_SAMP_PER_SPIKE; i++)
	{
		Y_spike_2_template[i] = buff->spike_template.template[disp_chan][2][i];
	}

	gtk_databox_set_total_limits (GTK_DATABOX (box_spike_0), 0, NUM_OF_SAMP_PER_SPIKE, +2200, -2200);
	gtk_databox_set_total_limits (GTK_DATABOX (box_spike_1), 0, NUM_OF_SAMP_PER_SPIKE, +2200, -2200);
	gtk_databox_set_total_limits (GTK_DATABOX (box_spike_2), 0, NUM_OF_SAMP_PER_SPIKE, +2200, -2200);

	sprintf(strDiff, "%E" , buff->spike_template.diff_thres[disp_chan][selected_spike_unit]);
	gtk_entry_set_text (GTK_ENTRY(entryDiff), strDiff);

	if (buff->spike_template.sorting_on[disp_chan][selected_spike_unit] )
	{
		gtk_button_set_label (unit_sorting_onoff_button,"Turn Unit Sorting-OFF");
	}
	else
	{
		gtk_button_set_label (unit_sorting_onoff_button,"Turn Unit Sorting-ON");
	}

	if (buff->spike_template.include_unit[disp_chan][selected_spike_unit] )
	{
		gtk_button_set_label (include_unit_onoff_button,"Include Unit-OFF");
	}
	else
	{
		gtk_button_set_label (include_unit_onoff_button,"Include Unit-ON");
	}
}		

