
#include "Recorder.h"

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
	GtkWidget *window;
	GtkWidget *hbox, *vbox, *lbl;

 	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  	gtk_window_set_default_size(GTK_WINDOW(window), 600, 800);
  	gtk_window_set_title(GTK_WINDOW(window), "Recorder");
  	gtk_container_set_border_width(GTK_CONTAINER(window), 10);

	initialize_data_read_write_handlers();	
	
	main_table= gtk_table_new(8, 4, TRUE);   // 9 rows   4 columns
	gtk_container_add(GTK_CONTAINER(window), main_table);
	
 	gtk_widget_set_size_request(main_table, 600, 800);
  	vbox = gtk_vbox_new(FALSE, 0);
    	gtk_table_attach_defaults(GTK_TABLE(main_table), vbox, 0,4, 0, 1);       

   	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE, 0);  	      

   	lbl = gtk_label_new("Select Directory :  ");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE, 0);
 
  	btn_select_directory_to_save = gtk_file_chooser_button_new ("Select Directory", GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER);
        gtk_box_pack_start(GTK_BOX(hbox), btn_select_directory_to_save, FALSE,FALSE,0);
	gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (btn_select_directory_to_save),"/home");		

        entry_data_name = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox), entry_data_name, FALSE,FALSE,0);
	gtk_widget_set_size_request(entry_data_name, 270, 25);
	 	
  	lbl = gtk_label_new("  : Data Name");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE, 0);

   	hbox = gtk_hbox_new(TRUE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE, 5);  

   	hbox = gtk_hbox_new(TRUE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE, 0);  
        
        btn_start_stop_recording = gtk_button_new_with_label("Start");
	gtk_box_pack_start (GTK_BOX (hbox), btn_start_stop_recording, FALSE, FALSE, 0);
	gtk_widget_set_size_request(btn_start_stop_recording, 200, 40);
	
        btn_delete_last_recording = gtk_button_new_with_label("Delete");
	gtk_box_pack_start (GTK_BOX (hbox), btn_delete_last_recording, FALSE, FALSE, 0);
	gtk_widget_set_size_request(btn_delete_last_recording, 60, 25);	
 
   	vbox = gtk_vbox_new(FALSE, 10);      
     	gtk_table_attach_defaults(GTK_TABLE(main_table), vbox, 0,1, 1, 2);     	
  	lbl = gtk_label_new("MWA-DAQ Map:");
	gtk_box_pack_start(GTK_BOX(vbox),lbl, FALSE,FALSE, 0);
  	lbl = gtk_label_new("(Additional Info)");
	gtk_box_pack_start(GTK_BOX(vbox),lbl, FALSE,FALSE, 0);
	 	
	vbox = gtk_vbox_new(FALSE, 0);
    	gtk_table_attach_defaults(GTK_TABLE(main_table), vbox, 1,4, 1, 2);   
 	gtk_widget_set_size_request(vbox, 0, 0);
 	
  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE, 0);  	      
 	gtk_widget_set_size_request(hbox, 30, 98);
 	
	txv_mwa_daq_map_additional = gtk_text_view_new();
	gtk_box_pack_start(GTK_BOX(hbox),txv_mwa_daq_map_additional, TRUE,TRUE, 0);

   	vbox = gtk_vbox_new(FALSE, 10);      
     	gtk_table_attach_defaults(GTK_TABLE(main_table), vbox, 0,1, 2, 3);     	
  	lbl = gtk_label_new("Recording Data:");
	gtk_box_pack_start(GTK_BOX(vbox),lbl, FALSE,FALSE, 0);
  	lbl = gtk_label_new("(Additional Info)");
	gtk_box_pack_start(GTK_BOX(vbox),lbl, FALSE,FALSE, 0);
 	
	vbox = gtk_vbox_new(FALSE, 0);
    	gtk_table_attach_defaults(GTK_TABLE(main_table), vbox, 1,4, 2, 3);   
 	gtk_widget_set_size_request(vbox, 0, 0);
 	
  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE, 0);  	      
 	gtk_widget_set_size_request(hbox, 30, 98);
 	
	txv_recording_additional = gtk_text_view_new();
	gtk_box_pack_start(GTK_BOX(hbox),txv_recording_additional, TRUE,TRUE, 0);

   	vbox = gtk_vbox_new(FALSE, 10);      
     	gtk_table_attach_defaults(GTK_TABLE(main_table), vbox, 0,1, 3, 4);     	
  	lbl = gtk_label_new("Spike Time Stamp:");
	gtk_box_pack_start(GTK_BOX(vbox),lbl, FALSE,FALSE, 0);
   	lbl = gtk_label_new("(Additional Info)");
	gtk_box_pack_start(GTK_BOX(vbox),lbl, FALSE,FALSE, 0);
		
	vbox = gtk_vbox_new(FALSE, 0);
    	gtk_table_attach_defaults(GTK_TABLE(main_table), vbox, 1,4, 3, 4);   
 	gtk_widget_set_size_request(vbox, 0, 0);
 	
  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE, 0);  	      
 	gtk_widget_set_size_request(hbox, 30, 98);
 	
	txv_spike_time_stamp_additional = gtk_text_view_new();
	gtk_box_pack_start(GTK_BOX(hbox),txv_spike_time_stamp_additional, TRUE,TRUE, 0);
	
   	vbox = gtk_vbox_new(FALSE, 10);    
     	gtk_table_attach_defaults(GTK_TABLE(main_table), vbox, 0,1, 4, 5);     	
  	lbl = gtk_label_new("Enviro. Events:");
	gtk_box_pack_start(GTK_BOX(vbox),lbl, FALSE,FALSE, 0);
   	lbl = gtk_label_new("(Additional Info)");
	gtk_box_pack_start(GTK_BOX(vbox),lbl, FALSE,FALSE, 0);
 	
	vbox = gtk_vbox_new(FALSE, 0);
    	gtk_table_attach_defaults(GTK_TABLE(main_table), vbox, 1,4, 4, 5);   
 	gtk_widget_set_size_request(vbox, 0, 0);
 	
  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE, 0);  	      
 	gtk_widget_set_size_request(hbox, 30, 98);
 	
	txv_exp_envi_event_additional = gtk_text_view_new();
	gtk_box_pack_start(GTK_BOX(hbox),txv_exp_envi_event_additional, TRUE,TRUE, 0);	
	
   	vbox = gtk_vbox_new(FALSE, 10);    
     	gtk_table_attach_defaults(GTK_TABLE(main_table), vbox, 0,1, 5, 6);     	
  	lbl = gtk_label_new("Enviro. Commands:");
	gtk_box_pack_start(GTK_BOX(vbox),lbl, FALSE,FALSE, 0);
   	lbl = gtk_label_new("(Additional Info)");
	gtk_box_pack_start(GTK_BOX(vbox),lbl, FALSE,FALSE, 0);
 	
	vbox = gtk_vbox_new(FALSE, 0);
    	gtk_table_attach_defaults(GTK_TABLE(main_table), vbox, 1,4, 5, 6);   
 	gtk_widget_set_size_request(vbox, 0, 0);
 	
  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE, 0);  	      
 	gtk_widget_set_size_request(hbox, 30, 98);
 	
	txv_exp_envi_command_additional = gtk_text_view_new();
	gtk_box_pack_start(GTK_BOX(hbox),txv_exp_envi_command_additional, TRUE,TRUE, 0);		
	
   	vbox = gtk_vbox_new(FALSE, 10);    
     	gtk_table_attach_defaults(GTK_TABLE(main_table), vbox, 0,1, 6, 7);     	
  	lbl = gtk_label_new("Object Events:");
	gtk_box_pack_start(GTK_BOX(vbox),lbl, FALSE,FALSE, 0);
   	lbl = gtk_label_new("(Additional Info)");
	gtk_box_pack_start(GTK_BOX(vbox),lbl, FALSE,FALSE, 0);
 	 	
	vbox = gtk_vbox_new(FALSE, 0);
    	gtk_table_attach_defaults(GTK_TABLE(main_table), vbox, 1,4, 6, 7);   
 	gtk_widget_set_size_request(vbox, 0, 0);
 	
  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE, 0);  	      
 	gtk_widget_set_size_request(hbox, 30, 98);
 	
	txv_mov_obj_event_additional = gtk_text_view_new();
	gtk_box_pack_start(GTK_BOX(hbox),txv_mov_obj_event_additional , TRUE,TRUE, 0);	
	
   	vbox = gtk_vbox_new(FALSE, 10);    
     	gtk_table_attach_defaults(GTK_TABLE(main_table), vbox, 0,1, 7, 8);     	
  	lbl = gtk_label_new("Object Commands:");
	gtk_box_pack_start(GTK_BOX(vbox),lbl, FALSE,FALSE, 0);
   	lbl = gtk_label_new("(Additional Info)");
	gtk_box_pack_start(GTK_BOX(vbox),lbl, FALSE,FALSE, 0);
 	
	vbox = gtk_vbox_new(FALSE, 0);
    	gtk_table_attach_defaults(GTK_TABLE(main_table), vbox, 1,4, 7, 8);   
 	gtk_widget_set_size_request(vbox, 0, 0);
 	
  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE, 0);  	      
 	gtk_widget_set_size_request(hbox, 30, 98);
 	
	txv_mov_obj_command_additional = gtk_text_view_new();
	gtk_box_pack_start(GTK_BOX(hbox),txv_mov_obj_command_additional , TRUE,TRUE, 0);		

//  	gtk_text_buffer_set_text (gtk_text_view_get_buffer(GTK_TEXT_VIEW(txv_mwa_daq_map_additional)), "Hello Text View!", -1);
 /* 	
  gtk_text_buffer_get_start_iter (buffer, &start);
  gtk_text_buffer_get_end_iter (buffer, &end);
*/
  /* Get the entire buffer text. */
//  text = gtk_text_buffer_get_text (buffer, &start, &end, FALSE);

	daq_mwa_map = &shared_memory->daq_mwa_map;
	mwa_daq_map = &shared_memory->mwa_daq_map;
	recording_data = &shared_memory->recording_data;
	spike_thresholding = &shared_memory->spike_thresholding;
	spike_time_stamp = &shared_memory->spike_time_stamp;
	exp_envi_event_time_stamp = &shared_memory->exp_envi_event_time_stamp;
	exp_envi_command_time_stamp  = &shared_memory->exp_envi_command_time_stamp;	
	mov_obj_event_time_stamp  = &shared_memory->mov_obj_event_time_stamp;			
	mov_obj_command_time_stamp  = &shared_memory->mov_obj_command_time_stamp;	
	template_matching_data = &shared_memory->template_matching_data;
	
	start_recording_request = 0;	
	stop_recording_request = 0;
	recording_ongoing = 0;
		
 	gtk_widget_show_all(window);
 	
   	g_signal_connect (GTK_OBJECT (window), "destroy", G_CALLBACK (gtk_main_quit), NULL);		
	g_signal_connect(G_OBJECT(btn_start_stop_recording), "clicked", G_CALLBACK(start_stop_recording_button_func), NULL);   
	g_signal_connect(G_OBJECT(btn_delete_last_recording), "clicked", G_CALLBACK(delete_last_recording_button_func), NULL);   
	

	return;
}	

void start_stop_recording_button_func (void)
{
	int ret_val;
	if ((recording_ongoing) && (!stop_recording_request))
	{
		stop_recording_request = 1;
		pthread_join (recording_thread, NULL);
		recording_ongoing = 0;
	}
	else if ((!recording_ongoing) & (!start_recording_request))
	{
		start_recording_request = 1;
		recording_ongoing = 1;
		if( (ret_val = pthread_create( &recording_thread, NULL, &recording_handler, NULL)) )
		{
			start_recording_request = 0;
			recording_ongoing = 0;		
			printf("Thread creation failed: %d\n", ret_val);
		}
	}
	return;
	
}


void *recording_handler(void *ptr)
{
	int time_prev = rt_get_cpu_time_ns();
	int part_num = 0;
	int time_curr;
	ptr = NULL;
	while (1)
	{
		if (start_recording_request)
		{
			start_recording_request = 0;
			initialize_buffer_reading_start_indexes_and_time_for_recording();
			printf ("current time: %u\t %llu\n", (unsigned int)rt_get_cpu_time_ns(), (long long unsigned int)shared_memory->kernel_task_ctrl.current_time_ns);										
			printf ("recording start time: %llu\n", (long long unsigned int)shared_memory->kernel_task_ctrl.current_time_ns);		
			usleep(100000);
		}
		else if ((recording_ongoing) && (!stop_recording_request))
		{
			get_buffer_reading_range_indexes_for_recording();
			printf ("current time: %u\t %llu\n", (unsigned int)rt_get_cpu_time_ns(), (long long unsigned int)shared_memory->kernel_task_ctrl.current_time_ns);
			printf("in_recording|n");				
//			fprint_range
			usleep(100000);
		}
		else if (stop_recording_request)
		{
			stop_recording_request = 0;		
			get_buffer_reading_end_indexes_and_time_for_recording();
			printf ("current time: %u\t %llu\n", (unsigned int)rt_get_cpu_time_ns(), (long long unsigned int)shared_memory->kernel_task_ctrl.current_time_ns);				
			printf ("recording end time: %llu\n", (long long unsigned int)shared_memory->kernel_task_ctrl.current_time_ns);		
			printf ("\n\n\n");					
//			fprint_buffer_range
			break;														
		}
		else
		{
			printf("Recorder: BUG: Inconvenient recording request and status condition\n");
			printf("Recorder: BUG: Inconvenient recording request and status condition\n");		
			printf("Recorder: BUG: Inconvenient recording request and status condition\n");
			break;
		}
		time_curr = rt_get_cpu_time_ns();
		
		if ((time_curr - time_prev) > 50000)		// if writing exceeds x milliseconds, there might be buffer reading error. (buffer might be overwrited before reading it.)
		{								
			printf ("Recorder: ERROR: Recording data files (Part: %d) wirting process lasted longer than 50 msec\n", part_num);
			printf ("Recorder: ERROR: It lasted %d nanoseconds\n", time_curr - time_prev);
			printf("Recorder: Recording interrupted.\n\n");
			// delete_last_recorded_file();
			break;						// Interrupt recording
		}					
		
		time_prev = time_curr;
		part_num++;
	}
	return ptr;
}


void delete_last_recording_button_func (void)
{
}


void initialize_buffer_reading_start_indexes_and_time_for_recording(void)
{
	int i, j;
	
	while (!(shared_memory->kernel_task_ctrl.kernel_task_idle)) { usleep(1); }
	
	for (i=0; i < MAX_NUM_OF_MWA; i++)
	{
		for (j=0; j<MAX_NUM_OF_CHAN_PER_MWA; j++)
		{
			recording_data_buff_start_idx[i][j] = recording_data->buff_idx_write[i][j];
		}
	}		
	spike_timestamp_buff_start_idx = spike_time_stamp->buff_idx_write;
	for (i=0; i <MAX_NUM_OF_EXP_ENVI_ITEMS; i++)
	{
		exp_envi_event_buff_start_idx[i] = exp_envi_event_time_stamp->buff_idx_write[i];
	}		
	for (i=0; i <MAX_NUM_OF_EXP_ENVI_ITEMS; i++)
	{
		exp_envi_command_buff_start_idx[i] = exp_envi_command_time_stamp->buff_idx_write[i];
	}			
	for (i=0; i <MAX_NUM_OF_MOVING_OBJECTS; i++)
	{
		mov_obj_event_buff_start_idx[i] = mov_obj_event_time_stamp->buff_idx_write[i];
	}		
	for (i=0; i <MAX_NUM_OF_MOVING_OBJECTS; i++)
	{
		mov_obj_command_buff_start_idx[i] = mov_obj_command_time_stamp->buff_idx_write[i];
	}	

	recording_start_time_ns = shared_memory->kernel_task_ctrl.current_time_ns;

	for (i=0; i < MAX_NUM_OF_MWA; i++)
	{
		for (j=0; j<MAX_NUM_OF_CHAN_PER_MWA; j++)
		{
			recording_data_buff_prev_idx[i][j] = recording_data_buff_start_idx[i][j];
		}
	}		
	spike_timestamp_buff_prev_idx = spike_timestamp_buff_start_idx;
	for (i=0; i <MAX_NUM_OF_EXP_ENVI_ITEMS; i++)
	{
		exp_envi_event_buff_prev_idx[i] = exp_envi_event_buff_start_idx[i];
	}		
	for (i=0; i <MAX_NUM_OF_EXP_ENVI_ITEMS; i++)
	{
		exp_envi_command_buff_prev_idx[i] = exp_envi_command_buff_start_idx[i];
	}			
	for (i=0; i <MAX_NUM_OF_MOVING_OBJECTS; i++)
	{
		mov_obj_event_buff_prev_idx[i] = mov_obj_event_buff_start_idx[i];
	}		
	for (i=0; i <MAX_NUM_OF_MOVING_OBJECTS; i++)
	{
		mov_obj_command_buff_prev_idx[i] = mov_obj_command_buff_start_idx[i];
	}	
	
	for (i=0; i < MAX_NUM_OF_MWA; i++)
	{
		for (j=0; j<MAX_NUM_OF_CHAN_PER_MWA; j++)
		{
			recording_data_buff_end_idx[i][j] = recording_data_buff_start_idx[i][j];
		}
	}		
	spike_timestamp_buff_end_idx = spike_timestamp_buff_start_idx;
	for (i=0; i <MAX_NUM_OF_EXP_ENVI_ITEMS; i++)
	{
		exp_envi_event_buff_end_idx[i] = exp_envi_event_buff_start_idx[i];
	}		
	for (i=0; i <MAX_NUM_OF_EXP_ENVI_ITEMS; i++)
	{
		exp_envi_command_buff_end_idx[i] = exp_envi_command_buff_start_idx[i];
	}			
	for (i=0; i <MAX_NUM_OF_MOVING_OBJECTS; i++)
	{
		mov_obj_event_buff_end_idx[i] = mov_obj_event_buff_start_idx[i];
	}		
	for (i=0; i <MAX_NUM_OF_MOVING_OBJECTS; i++)
	{
		mov_obj_command_buff_end_idx[i] = mov_obj_command_buff_start_idx[i];
	}	
	return;	
}

void get_buffer_reading_range_indexes_for_recording(void)
{
	int i, j;
	
	for (i=0; i < MAX_NUM_OF_MWA; i++)
	{
		for (j=0; j<MAX_NUM_OF_CHAN_PER_MWA; j++)
		{
			recording_data_buff_prev_idx[i][j] = recording_data_buff_end_idx[i][j];
		}
	}		
	spike_timestamp_buff_prev_idx = spike_timestamp_buff_end_idx;
	for (i=0; i <MAX_NUM_OF_EXP_ENVI_ITEMS; i++)
	{
		exp_envi_event_buff_prev_idx[i] = exp_envi_event_buff_end_idx[i];
	}		
	for (i=0; i <MAX_NUM_OF_EXP_ENVI_ITEMS; i++)
	{
		exp_envi_command_buff_prev_idx[i] = exp_envi_command_buff_end_idx[i];
	}			
	for (i=0; i <MAX_NUM_OF_MOVING_OBJECTS; i++)
	{
		mov_obj_event_buff_prev_idx[i] = mov_obj_event_buff_end_idx[i];
	}		
	for (i=0; i <MAX_NUM_OF_MOVING_OBJECTS; i++)
	{
		mov_obj_command_buff_prev_idx[i] = mov_obj_command_buff_end_idx[i];
	}		
	
	while (!(shared_memory->kernel_task_ctrl.kernel_task_idle)) { usleep(1); }	
	
	for (i=0; i < MAX_NUM_OF_MWA; i++)
	{
		for (j=0; j<MAX_NUM_OF_CHAN_PER_MWA; j++)
		{
			recording_data_buff_end_idx[i][j] = recording_data->buff_idx_write[i][j];
		}
	}		
	spike_timestamp_buff_end_idx = spike_time_stamp->buff_idx_write;
	for (i=0; i <MAX_NUM_OF_EXP_ENVI_ITEMS; i++)
	{
		exp_envi_event_buff_end_idx[i] = exp_envi_event_time_stamp->buff_idx_write[i];
	}		
	for (i=0; i <MAX_NUM_OF_EXP_ENVI_ITEMS; i++)
	{
		exp_envi_command_buff_end_idx[i] = exp_envi_command_time_stamp->buff_idx_write[i];
	}			
	for (i=0; i <MAX_NUM_OF_MOVING_OBJECTS; i++)
	{
		mov_obj_event_buff_end_idx[i] = mov_obj_event_time_stamp->buff_idx_write[i];
	}		
	for (i=0; i <MAX_NUM_OF_MOVING_OBJECTS; i++)
	{
		mov_obj_command_buff_end_idx[i] = mov_obj_command_time_stamp->buff_idx_write[i];
	}	

	return;
}


void get_buffer_reading_end_indexes_and_time_for_recording(void)
{
	int i, j;
	
	for (i=0; i < MAX_NUM_OF_MWA; i++)
	{
		for (j=0; j<MAX_NUM_OF_CHAN_PER_MWA; j++)
		{
			recording_data_buff_prev_idx[i][j] = recording_data_buff_end_idx[i][j];
		}
	}		
	spike_timestamp_buff_prev_idx = spike_timestamp_buff_end_idx;
	for (i=0; i <MAX_NUM_OF_EXP_ENVI_ITEMS; i++)
	{
		exp_envi_event_buff_prev_idx[i] = exp_envi_event_buff_end_idx[i];
	}		
	for (i=0; i <MAX_NUM_OF_EXP_ENVI_ITEMS; i++)
	{
		exp_envi_command_buff_prev_idx[i] = exp_envi_command_buff_end_idx[i];
	}			
	for (i=0; i <MAX_NUM_OF_MOVING_OBJECTS; i++)
	{
		mov_obj_event_buff_prev_idx[i] = mov_obj_event_buff_end_idx[i];
	}		
	for (i=0; i <MAX_NUM_OF_MOVING_OBJECTS; i++)
	{
		mov_obj_command_buff_prev_idx[i] = mov_obj_command_buff_end_idx[i];
	}		
	
	while (!(shared_memory->kernel_task_ctrl.kernel_task_idle)) { usleep(1); }	
	
	for (i=0; i < MAX_NUM_OF_MWA; i++)
	{
		for (j=0; j<MAX_NUM_OF_CHAN_PER_MWA; j++)
		{
			recording_data_buff_end_idx[i][j] = recording_data->buff_idx_write[i][j];
		}
	}		
	spike_timestamp_buff_end_idx = spike_time_stamp->buff_idx_write;
	for (i=0; i <MAX_NUM_OF_EXP_ENVI_ITEMS; i++)
	{
		exp_envi_event_buff_end_idx[i] = exp_envi_event_time_stamp->buff_idx_write[i];
	}		
	for (i=0; i <MAX_NUM_OF_EXP_ENVI_ITEMS; i++)
	{
		exp_envi_command_buff_end_idx[i] = exp_envi_command_time_stamp->buff_idx_write[i];
	}			
	for (i=0; i <MAX_NUM_OF_MOVING_OBJECTS; i++)
	{
		mov_obj_event_buff_end_idx[i] = mov_obj_event_time_stamp->buff_idx_write[i];
	}		
	for (i=0; i <MAX_NUM_OF_MOVING_OBJECTS; i++)
	{
		mov_obj_command_buff_end_idx[i] = mov_obj_command_time_stamp->buff_idx_write[i];
	}	

	recording_end_time_ns = shared_memory->kernel_task_ctrl.current_time_ns;

	return;	
	

}

void initialize_buffer_reading_start_indexes(void)
{
	int i, j;
	for (i=0; i < MAX_NUM_OF_MWA; i++)
	{
		for (j=0; j<MAX_NUM_OF_CHAN_PER_MWA; j++)
		{
			recording_data_buff_prev_idx[i][j] = recording_data->buff_idx_write[i][j];
		}
	}		
	spike_timestamp_buff_prev_idx = spike_time_stamp->buff_idx_write;
	for (i=0; i <MAX_NUM_OF_EXP_ENVI_ITEMS; i++)
	{
		exp_envi_event_buff_prev_idx[i] = exp_envi_event_time_stamp->buff_idx_write[i];
	}		
	for (i=0; i <MAX_NUM_OF_EXP_ENVI_ITEMS; i++)
	{
		exp_envi_command_buff_prev_idx[i] = exp_envi_command_time_stamp->buff_idx_write[i];
	}			
	for (i=0; i <MAX_NUM_OF_MOVING_OBJECTS; i++)
	{
		mov_obj_event_buff_prev_idx[i] = mov_obj_event_time_stamp->buff_idx_write[i];
	}		
	for (i=0; i <MAX_NUM_OF_MOVING_OBJECTS; i++)
	{
		mov_obj_command_buff_prev_idx[i] = mov_obj_command_time_stamp->buff_idx_write[i];
	}	
	return;
}




