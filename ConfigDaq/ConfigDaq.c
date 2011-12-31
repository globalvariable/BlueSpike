#include "ConfigDaq.h"


int main( int argc, char *argv[])
{
   	shared_memory = (SharedMemStruct*)rtai_malloc(nam2num(SHARED_MEM_NAME), SHARED_MEM_SIZE);
	if (shared_memory == NULL)
	{
		printf("rtai_malloc() failed (maybe /dev/rtai_shm is missing)!\n");
		return -1;
   	}
        printf("sizeof(SharedMemStruct) : %d SHARED_MEM_SIZE: %d\n", sizeof(SharedMemStruct), SHARED_MEM_SIZE);
	gtk_init(&argc, &argv);
	create_gui(); 	
	gtk_main();

	return 0;
}



void create_gui(void)
{
	GtkWidget *window, *hbox, *hbox1, *vbox, *vbox1, *lbl;
 	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  	gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);
  	gtk_window_set_title(GTK_WINDOW(window), "ConfigDaq");
  	gtk_container_set_border_width(GTK_CONTAINER(window), 10);	
  	
    	vbox = gtk_vbox_new(FALSE, 0);
	gtk_container_add(GTK_CONTAINER(window), vbox);	

    	hbox = gtk_hbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE,FALSE,20);
	
    	vbox1 = gtk_vbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(hbox), vbox1, FALSE,FALSE,0);

	hbox1 = gtk_hbox_new(FALSE, 0);
  	gtk_box_pack_start (GTK_BOX (vbox1), hbox1, FALSE, FALSE, 0);	

	lbl = gtk_label_new("DAQ Card");
        gtk_box_pack_start(GTK_BOX(hbox1),lbl,FALSE,FALSE, 0);
	
	hbox1 = gtk_hbox_new(FALSE, 0);
  	gtk_box_pack_start (GTK_BOX (vbox1), hbox1, FALSE, FALSE, 0);	
  	
	lbl = gtk_label_new("Card #			: ");
        gtk_box_pack_start(GTK_BOX(hbox1),lbl,FALSE,FALSE, 0);
  	
        entry_daq_card_num = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox1),entry_daq_card_num, FALSE,FALSE,0);
	gtk_entry_set_text (GTK_ENTRY(entry_daq_card_num), "0");
  	gtk_widget_set_size_request(entry_daq_card_num, 30, 25) ;
  	  	
	hbox1 = gtk_hbox_new(FALSE, 0);
  	gtk_box_pack_start (GTK_BOX (vbox1), hbox1, FALSE, FALSE, 0);	
  	
	lbl = gtk_label_new("Channel # (Lower)	: ");
        gtk_box_pack_start(GTK_BOX(hbox1),lbl,FALSE,FALSE, 0);
  	
        entry_daq_card_chan_range_lower = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox1),entry_daq_card_chan_range_lower, FALSE,FALSE,0);
	gtk_entry_set_text (GTK_ENTRY(entry_daq_card_chan_range_lower), "0");
  	gtk_widget_set_size_request(entry_daq_card_chan_range_lower, 30, 25) ;
  	
	hbox1 = gtk_hbox_new(FALSE, 0);
  	gtk_box_pack_start (GTK_BOX (vbox1), hbox1, FALSE, FALSE, 0);	
  	
	lbl = gtk_label_new("Channel # (Upper)	: ");
        gtk_box_pack_start(GTK_BOX(hbox1),lbl,FALSE,FALSE, 0);
  	
        entry_daq_card_chan_range_upper = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox1),entry_daq_card_chan_range_upper, FALSE,FALSE,0);
	gtk_entry_set_text (GTK_ENTRY(entry_daq_card_chan_range_upper), "0");
  	gtk_widget_set_size_request(entry_daq_card_chan_range_upper, 30, 25) ;
  
      	vbox1 = gtk_vbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(hbox), vbox1, FALSE,FALSE,0);
	
	lbl = gtk_label_new("");
        gtk_box_pack_start(GTK_BOX(vbox1),lbl,FALSE,FALSE, 0);	
        	
	lbl = gtk_label_new("  =>  ");
        gtk_box_pack_start(GTK_BOX(vbox1),lbl,TRUE,TRUE, 0);	
        
        lbl = gtk_label_new("  =>  ");
        gtk_box_pack_start(GTK_BOX(vbox1),lbl,TRUE,TRUE, 0);
        
        lbl = gtk_label_new("  =>  ");
        gtk_box_pack_start(GTK_BOX(vbox1),lbl,TRUE,TRUE, 0);
  	
    	vbox1 = gtk_vbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(hbox), vbox1, FALSE,FALSE, 0);
	
	hbox1 = gtk_hbox_new(FALSE, 0);
  	gtk_box_pack_start (GTK_BOX (vbox1), hbox1, FALSE, FALSE, 0);	

	lbl = gtk_label_new("MEA");
        gtk_box_pack_start(GTK_BOX(hbox1),lbl,FALSE,FALSE, 0);
	
	hbox1 = gtk_hbox_new(FALSE, 0);
  	gtk_box_pack_start (GTK_BOX (vbox1), hbox1, FALSE, FALSE, 0);	
  	
	lbl = gtk_label_new("Array #			: ");
        gtk_box_pack_start(GTK_BOX(hbox1),lbl,FALSE,FALSE, 0);
  	
        entry_mwa_num = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox1),entry_mwa_num, FALSE,FALSE,0);
	gtk_entry_set_text (GTK_ENTRY(entry_mwa_num), "0");
  	gtk_widget_set_size_request(entry_mwa_num, 30, 25) ;
  		
	hbox1 = gtk_hbox_new(FALSE, 0);
  	gtk_box_pack_start (GTK_BOX (vbox1), hbox1, FALSE, FALSE, 0);	
  	
	lbl = gtk_label_new("Channel # (Lower)	: ");
        gtk_box_pack_start(GTK_BOX(hbox1),lbl,FALSE,FALSE, 0);
  	
        entry_mwa_chan_range_lower = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox1),entry_mwa_chan_range_lower, FALSE,FALSE,0);
	gtk_entry_set_text (GTK_ENTRY(entry_mwa_chan_range_lower), "0");
	gtk_widget_set_size_request(entry_mwa_chan_range_lower, 30, 25) ;
	  	
	hbox1 = gtk_hbox_new(FALSE, 0);
  	gtk_box_pack_start (GTK_BOX (vbox1), hbox1, FALSE, FALSE, 0);	
  	
	lbl = gtk_label_new("Channel # (Upper)	: ");
        gtk_box_pack_start(GTK_BOX(hbox1),lbl,FALSE,FALSE, 0);
  	
        entry_mwa_chan_range_upper = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox1),entry_mwa_chan_range_upper, FALSE,FALSE,0);
	gtk_entry_set_text (GTK_ENTRY(entry_mwa_chan_range_upper), "0");	
	gtk_widget_set_size_request(entry_mwa_chan_range_upper, 30, 25) ;
		
    	hbox = gtk_hbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE,FALSE,10);		
	
	btn_map_channels = gtk_button_new_with_label("Map Channels");
	gtk_box_pack_start (GTK_BOX (hbox), btn_map_channels, TRUE, FALSE, 0);
	
	btn_interrogate_mapping = gtk_button_new_with_label("Interrogate Mapping");
	gtk_box_pack_start (GTK_BOX (hbox), btn_interrogate_mapping, TRUE, FALSE, 0);	
	
	btn_cancel_all_mapping = gtk_button_new_with_label("Cancel All Mapping");
	gtk_box_pack_start (GTK_BOX (hbox), btn_cancel_all_mapping, TRUE, FALSE, 0);		
	
    	hbox = gtk_hbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE,FALSE,10);		
	
	btn_turn_daq_on_off = gtk_button_new_with_label("DAQ Card : ON");
	gtk_box_pack_start (GTK_BOX (hbox), btn_turn_daq_on_off, TRUE, FALSE, 0);
	if (shared_memory->kernel_task_ctrl.turn_daq_card_on)
	{	
		gtk_button_set_label (GTK_BUTTON(btn_turn_daq_on_off),"DAQ Card : ON");
		gtk_widget_set_sensitive(btn_turn_daq_on_off, TRUE);
		gtk_widget_set_sensitive(btn_map_channels, FALSE);
		gtk_widget_set_sensitive(btn_load_maps_file, FALSE);	
		if (!shared_memory->kernel_task_ctrl.daq_card_mapped)
			gtk_widget_set_sensitive(btn_cancel_all_mapping, FALSE);			
	}
	else
	{
		gtk_button_set_label (GTK_BUTTON(btn_turn_daq_on_off),"DAQ Card : OFF");	
		if (!shared_memory->kernel_task_ctrl.daq_card_mapped)
			gtk_widget_set_sensitive(btn_turn_daq_on_off, FALSE);
	}
	hbox = gtk_hbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE,FALSE,20);	
	
	btn_select_maps_file_to_load = gtk_file_chooser_button_new ("Select Maps File", GTK_FILE_CHOOSER_ACTION_OPEN);
        gtk_box_pack_start(GTK_BOX(hbox),btn_select_maps_file_to_load, TRUE,TRUE,0);
	set_directory_btn_select_directory_to_load();        

	btn_load_maps_file = gtk_button_new_with_label("Load Maps File");
        gtk_box_pack_start(GTK_BOX(hbox),btn_load_maps_file,TRUE,TRUE, 0);		

  	gtk_widget_show_all(window);

	initialize_data_read_write_handlers();

        g_signal_connect (GTK_OBJECT (window), "destroy", G_CALLBACK (gtk_main_quit), NULL);
	g_signal_connect(G_OBJECT(btn_turn_daq_on_off), "clicked", G_CALLBACK(turn_daq_on_off_button_func), NULL);       
        g_signal_connect(G_OBJECT(btn_map_channels), "clicked", G_CALLBACK(map_channels_button_func), NULL);
        g_signal_connect(G_OBJECT(btn_interrogate_mapping), "clicked", G_CALLBACK(interrogate_mapping_button_func), NULL);      
        g_signal_connect(G_OBJECT(btn_cancel_all_mapping), "clicked", G_CALLBACK(cancel_all_mapping_button_func), NULL);              
        g_signal_connect(G_OBJECT(btn_load_maps_file), "clicked", G_CALLBACK(load_maps_file_button_func), NULL);
        
	return;
}

void turn_daq_on_off_button_func(void)
{
	if (shared_memory->kernel_task_ctrl.turn_daq_card_on)
	{		
		while (!(shared_memory->kernel_task_ctrl.kernel_task_idle)) { usleep(1); }   // wait until rt_task_wait_period starts
		shared_memory->kernel_task_ctrl.turn_daq_card_on = 0;
		gtk_button_set_label (GTK_BUTTON(btn_turn_daq_on_off),"DAQ Card : OFF");
		gtk_widget_set_sensitive(btn_map_channels, TRUE);
		gtk_widget_set_sensitive(btn_load_maps_file, TRUE);	
		gtk_widget_set_sensitive(btn_cancel_all_mapping, TRUE);						
	}
	else
	{
		while (!(shared_memory->kernel_task_ctrl.kernel_task_idle)) { usleep(1); }   // wait until rt_task_wait_period starts
		shared_memory->kernel_task_ctrl.turn_daq_card_on = 1;
		gtk_button_set_label (GTK_BUTTON(btn_turn_daq_on_off),"DAQ Card : ON");
		gtk_widget_set_sensitive(btn_map_channels, FALSE);
		gtk_widget_set_sensitive(btn_load_maps_file, FALSE);		
		gtk_widget_set_sensitive(btn_cancel_all_mapping, FALSE);						
	}		
}

void map_channels_button_func(void)
{
	int daq_num, daq_chan_start, daq_chan_end;
	int mwa_num, mwa_chan_start, mwa_chan_end;
	int i, j;

	daq_num = (int)atof(gtk_entry_get_text(GTK_ENTRY(entry_daq_card_num)));
	daq_chan_start = (int)atof(gtk_entry_get_text(GTK_ENTRY(entry_daq_card_chan_range_lower)));	
	daq_chan_end = (int)atof(gtk_entry_get_text(GTK_ENTRY(entry_daq_card_chan_range_upper)));	
	
	mwa_num = (int)atof(gtk_entry_get_text(GTK_ENTRY(entry_mwa_num)));
	mwa_chan_start = (int)atof(gtk_entry_get_text(GTK_ENTRY(entry_mwa_chan_range_lower)));	
	mwa_chan_end = (int)atof(gtk_entry_get_text(GTK_ENTRY(entry_mwa_chan_range_upper)));		
	
	if (daq_num >= MAX_NUM_OF_DAQ_CARD)
	{
		printf ("ERROR: Invalid DAQ Card Number\n");
		printf ("ERROR: It should be less than %d\n", MAX_NUM_OF_DAQ_CARD);
		return;
	}
	
	if (mwa_num >= MAX_NUM_OF_MWA)
	{
		printf ("ERROR: Invalid Microwire Array Number\n");
		printf ("ERROR: It should be less than %d\n", MAX_NUM_OF_MWA);
		return;
	}	
	
	if (daq_chan_start >= MAX_NUM_OF_CHANNEL_PER_DAQ_CARD)
	{
		printf ("ERROR: Invalid DAQ Card Channel Number\n");
		printf ("ERROR: It should be less than %d\n",  MAX_NUM_OF_CHANNEL_PER_DAQ_CARD);
		return;
	}
	
	if (daq_chan_end >= MAX_NUM_OF_CHANNEL_PER_DAQ_CARD)
	{
		printf ("ERROR: Invalid DAQ Card Channel Number\n");
		printf ("ERROR: It should be less than %d\n", MAX_NUM_OF_CHANNEL_PER_DAQ_CARD);
		return;
	}	
		
	if (mwa_chan_start >= MAX_NUM_OF_CHAN_PER_MWA)
	{
		printf ("ERROR: Invalid Microwire Array Channel Number\n");
		printf ("ERROR: It should be less than %d\n",  MAX_NUM_OF_CHAN_PER_MWA);
		return;
	}
	
	if (mwa_chan_end >= MAX_NUM_OF_CHAN_PER_MWA)
	{
		printf ("ERROR: Invalid Microwire Array Channel Number\n");
		printf ("ERROR: It should be less than %d\n", MAX_NUM_OF_CHAN_PER_MWA);
		return;
	}	
	
	if (daq_chan_start > daq_chan_end)
	{
		printf ("ERROR: Invalid DAQ Card Channel Number\n");
		printf ("ERROR: DAQ Card Channel Configuration  Start = %d\n", daq_chan_start);
		printf ("ERROR: DAQ Card Channel Configuration End = %d\n", daq_chan_end);	
		printf ("ERROR: Start cannot be greater than End\n");			
		return;
	}			
		
	if (mwa_chan_start > mwa_chan_end)
	{
		printf ("ERROR: Invalid Microwire Array Channel Number\n");
		printf ("ERROR: Microwire Array Channel Configuration Start = %d\n", mwa_chan_start);
		printf ("ERROR: Microwire Array Channel Configuration End = %d\n", mwa_chan_end);	
		printf ("ERROR: Start cannot be greater than End\n");			
		return;
	}			
	
	if ((mwa_chan_end - mwa_chan_start) != (daq_chan_end - daq_chan_start))
	{
		printf ("ERROR: MWA Channel Range and DAQ Card Channel Range should be equal\n");
		printf ("ERROR: DAQ Card Channel Range = %d - %d\n", daq_chan_start, daq_chan_end);
		printf ("ERROR: Microwire Array Channel Range = %d - %d\n", mwa_chan_start, mwa_chan_end);	
		return;
	}

	if ((daq_num < 0) || (daq_chan_start < 0) || (daq_chan_end < 0) || (mwa_num < 0) || (mwa_chan_start < 0) || (mwa_chan_end < 0))
	{
		printf ("ERROR: No DAQ Card, Microwire Array or Channel Number can be smaller than zero\n");
		return;
	}

	j = mwa_chan_start;
	for (i = daq_chan_start; i<= daq_chan_end; i++)
	{	
		if ((shared_memory->daq_mwa_map[daq_num][i].mwa != MAX_NUM_OF_MWA) || (shared_memory->daq_mwa_map[daq_num][i].channel != MAX_NUM_OF_CHAN_PER_MWA))
		{
			printf ("WARNING: Daq Card %d, Channel %d has been configured previously\n", daq_num, i);
			printf ("WARNING: Configured as DAQ: %d   Channel: %d  ----> MWA: %d   Channel: %d\n", daq_num, i, shared_memory->daq_mwa_map[daq_num][i].mwa, shared_memory->daq_mwa_map[daq_num][i].channel);
		}
		
		while (!(shared_memory->kernel_task_ctrl.kernel_task_idle)) { usleep(1); }   // wait until rt_task_wait_period starts
		// First delete ex mwa_daq_map
		shared_memory->mwa_daq_map[shared_memory->daq_mwa_map[daq_num][i].mwa][shared_memory->daq_mwa_map[daq_num][i].channel].daq_card = MAX_NUM_OF_DAQ_CARD;
		shared_memory->mwa_daq_map[shared_memory->daq_mwa_map[daq_num][i].mwa][shared_memory->daq_mwa_map[daq_num][i].channel].daq_chan = MAX_NUM_OF_CHANNEL_PER_DAQ_CARD;	
		// Now map daq to mwa	
		shared_memory->daq_mwa_map[daq_num][i].mwa = mwa_num;
		shared_memory->daq_mwa_map[daq_num][i].channel = j;
		// Now map mwa to daq
		shared_memory->mwa_daq_map[mwa_num][j].daq_card = daq_num;
		shared_memory->mwa_daq_map[mwa_num][j].daq_chan = i;	

		j++;  
	}

	printf ("DAQ Card -> Microwire Array mapping completed\n");
	printf ("Microwire Array -> DAQ Card mapping completed\n");
	interrogate_mapping();
	shared_memory->kernel_task_ctrl.daq_card_mapped = 1;
	gtk_widget_set_sensitive(btn_turn_daq_on_off, TRUE);
	return;
}

void interrogate_mapping_button_func(void)
{
	printf("Interrogating mapping\n");
	interrogate_mapping();
	printf("Interrogating mapping...complete\n");	
	return;
}

void cancel_all_mapping_button_func(void)
{
	int i, j;
	for (i=0; i < MAX_NUM_OF_DAQ_CARD; i++)
	{
		for (j=0; j<MAX_NUM_OF_CHANNEL_PER_DAQ_CARD; j++)
		{
			while (!(shared_memory->kernel_task_ctrl.kernel_task_idle)) { usleep(1); }   // wait until rt_task_wait_period starts			
			shared_memory->daq_mwa_map[i][j].mwa = MAX_NUM_OF_MWA;
			shared_memory->daq_mwa_map[i][j].channel = MAX_NUM_OF_CHAN_PER_MWA;
		}
	}
	
	for (i=0; i < MAX_NUM_OF_MWA; i++)
	{
		for (j=0; j<MAX_NUM_OF_CHAN_PER_MWA; j++)
		{
			while (!(shared_memory->kernel_task_ctrl.kernel_task_idle)) { usleep(1); }   // wait until rt_task_wait_period starts		
			shared_memory->mwa_daq_map[i][j].daq_card = MAX_NUM_OF_DAQ_CARD;
			shared_memory->mwa_daq_map[i][j].daq_chan = MAX_NUM_OF_CHANNEL_PER_DAQ_CARD;
		}
	}
	gtk_widget_set_sensitive(btn_turn_daq_on_off, FALSE);
}

void load_maps_file_button_func(void)
{
	char *path_maps = NULL, path_temp[600];

	int version;
	path_maps = gtk_file_chooser_get_uri (GTK_FILE_CHOOSER (btn_select_maps_file_to_load));
	path_maps = &path_maps[7];                  ///     file:///path
	strcpy(path_temp, path_maps);
	path_temp[(strlen(path_maps)-5)] = 0;    // to get the main BlueSpikeData directory path    (BlueSpikeData/maps)

	if (!get_format_version(&version, path_temp))
	{
		printf("Couldn't retrieve mapping.\n");		
		return;
	}
	
	if (!((*read_mapping[version])(1, path_maps)))
	{
		printf("Couldn't retrieve mapping.\n");	
		return;
	}
	gtk_widget_set_sensitive(btn_turn_daq_on_off, TRUE);		
}

void set_directory_btn_select_directory_to_load(void)
{
	char line[600];
	FILE *fp = NULL;
	GFile *gfile_path; 
       	if ((fp = fopen("./path_initial_directory", "r")) == NULL)  
       	{ 
       		printf ("ERROR: Recorder: Couldn't find file: ./path_initial_directory\n"); 
       		printf ("ERROR: Recorder: /home is loaded as initial directory.\n");
		gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (btn_select_maps_file_to_load),"/home");
       	}
       	else
       	{
		if (fgets(line, sizeof line, fp ) == NULL) 
		{ 
			printf("ERROR: Recorder: Couldn' t read ./path_initial_directory\n"); 
       			printf ("ERROR: Recorder: /home is loaded as initial directory.\n");
			gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (btn_select_maps_file_to_load),"/home");
		}
		else
		{
			gfile_path = g_file_new_for_path (line); 
			gtk_file_chooser_set_file (GTK_FILE_CHOOSER (btn_select_maps_file_to_load), gfile_path, NULL);
			g_object_unref(gfile_path);
		}
		fclose(fp); 		
	}  	 
}


