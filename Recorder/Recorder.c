
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
  	gtk_window_set_default_size(GTK_WINDOW(window), 500, 700);
  	gtk_window_set_title(GTK_WINDOW(window), "Recorder");
  	gtk_container_set_border_width(GTK_CONTAINER(window), 10);

	initialize_data_read_write_handlers();	
	
	main_table= gtk_table_new(9, 4, TRUE);   // 9 rows   4 columns
	gtk_container_add(GTK_CONTAINER(window), main_table);
	gtk_widget_set_size_request(main_table, 500, 700);

  	vbox = gtk_vbox_new(FALSE, 0);
    	gtk_table_attach_defaults(GTK_TABLE(main_table), vbox, 0,4, 0, 1);       

   	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE, 0);  	      

   	lbl = gtk_label_new("Select Directory : ");
        gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE, 0);
 
  	btn_select_directory_to_save = gtk_file_chooser_button_new ("Select Directory", GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER);
        gtk_box_pack_start(GTK_BOX(hbox), btn_select_directory_to_save, FALSE,FALSE,0);
	set_directory_btn_select_directory_to_save();
	gtk_widget_set_size_request(btn_select_directory_to_save, 200, 30);	

	btn_create_bluespikedata_folder = gtk_button_new_with_label("Create Main Folder");
	gtk_box_pack_start (GTK_BOX (hbox), btn_create_bluespikedata_folder, FALSE, FALSE, 0);
	gtk_widget_set_size_request(btn_create_bluespikedata_folder, 150, 30);

	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, TRUE,FALSE, 10);  
        	
	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, TRUE,FALSE, 0);  	 

	btn_save_maps_templates = gtk_button_new_with_label("Save Maps & Templates");
	gtk_box_pack_start (GTK_BOX (hbox), btn_save_maps_templates, TRUE, FALSE, 0);
	gtk_widget_set_size_request(btn_save_maps_templates, 165, 30);
	gtk_widget_set_sensitive(btn_save_maps_templates, FALSE);				
	
  	vbox = gtk_vbox_new(FALSE, 0);
    	gtk_table_attach_defaults(GTK_TABLE(main_table), vbox, 0,4, 1, 2);       

   	hbox = gtk_hbox_new(TRUE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, TRUE ,FALSE, 0);  
        
        btn_start_stop_recording = gtk_button_new_with_label("Start");
	gtk_box_pack_start (GTK_BOX (hbox), btn_start_stop_recording, FALSE, FALSE, 0);
	gtk_widget_set_size_request(btn_start_stop_recording, 200, 50);
	gtk_widget_set_sensitive(btn_start_stop_recording, FALSE);		
	
        btn_delete_last_recording = gtk_button_new_with_label("Delete Last");
	gtk_box_pack_start (GTK_BOX (hbox), btn_delete_last_recording, FALSE, FALSE, 0);
	gtk_widget_set_size_request(btn_delete_last_recording, 90, 50);	
	gtk_widget_set_sensitive( btn_delete_last_recording, FALSE);			
 
   	vbox = gtk_vbox_new(FALSE, 10);      
     	gtk_table_attach_defaults(GTK_TABLE(main_table), vbox, 0,4, 2, 5);   
     	
  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE, 0);       	
     	  	
  	lbl = gtk_label_new("Notes:");
	gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE, 0);
	 	
  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE, 0);  	      
 	gtk_widget_set_size_request(hbox, 50, 250);
 	
	txv_notes = gtk_text_view_new();
	gtk_box_pack_start(GTK_BOX(hbox),txv_notes, TRUE,TRUE, 0);
	
	fill_notes_text_view();

  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE, 0); 
	
        btn_submit_notes = gtk_button_new_with_label("   Submit Notes   ");
	gtk_box_pack_start (GTK_BOX (hbox), btn_submit_notes, TRUE, FALSE, 0);	
	gtk_widget_set_sensitive(btn_submit_notes, FALSE);		
		
   	vbox = gtk_vbox_new(FALSE, 10);      
     	gtk_table_attach_defaults(GTK_TABLE(main_table), vbox, 0,4, 6, 8);   
     	
  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE, 0);       	
     	  	
  	lbl = gtk_label_new("\nAdditonal Notes:");
	gtk_box_pack_start(GTK_BOX(hbox),lbl, FALSE,FALSE, 0);
	 	
  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE, 0);  	      
 	gtk_widget_set_size_request(hbox, 30, 150);
 	
	txv_additional_notes = gtk_text_view_new();
	gtk_box_pack_start(GTK_BOX(hbox),txv_additional_notes, TRUE,TRUE, 0);

  	hbox = gtk_hbox_new(FALSE, 0);
        gtk_box_pack_start(GTK_BOX(vbox),hbox, FALSE,FALSE, 0);   
	
	btn_submit_additional_notes = gtk_button_new_with_label("Submit Addional Notes");
	gtk_box_pack_start (GTK_BOX (hbox), btn_submit_additional_notes, TRUE, FALSE, 0);	
	gtk_widget_set_sensitive(btn_submit_additional_notes, FALSE);		
	
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
	
	data_directory_cntr = 0;
 	
 	gtk_widget_show_all(window);
 	
   	g_signal_connect (GTK_OBJECT (window), "destroy", G_CALLBACK (gtk_main_quit), NULL);	
	g_signal_connect(G_OBJECT(btn_create_bluespikedata_folder), "clicked", G_CALLBACK(create_bluespikedata_folder_button_func), NULL);      
	g_signal_connect(G_OBJECT(btn_start_stop_recording), "clicked", G_CALLBACK(start_stop_recording_button_func), NULL);   
	g_signal_connect(G_OBJECT(btn_delete_last_recording), "clicked", G_CALLBACK(delete_last_recording_button_func), NULL);   
	g_signal_connect(G_OBJECT(btn_save_maps_templates), "clicked", G_CALLBACK(save_maps_templates_button_func), NULL);   
	g_signal_connect(G_OBJECT(btn_submit_notes), "clicked", G_CALLBACK(submit_notes_button_func), NULL);   
	g_signal_connect(G_OBJECT(btn_submit_additional_notes), "clicked", G_CALLBACK(submit_additional_notes_button_func), NULL);   	
	return;
}	

void create_bluespikedata_folder_button_func (void)
{
	char *path_temp = NULL, *path = NULL;
	path_temp = gtk_file_chooser_get_uri (GTK_FILE_CHOOSER (btn_select_directory_to_save));
	path = &path_temp[7];   // since     uri returns file:///home/....	
	if ((*create_main_directory[MAX_NUMBER_OF_DATA_FORMAT_VER-1])(1, path))		// record in last format version
	{
		gtk_widget_set_sensitive(btn_save_maps_templates, TRUE);
		gtk_widget_set_sensitive(btn_submit_notes, TRUE);
		gtk_widget_set_sensitive(btn_submit_additional_notes, FALSE);
		gtk_widget_set_sensitive(btn_start_stop_recording, FALSE);
		gtk_widget_set_sensitive(btn_delete_last_recording, FALSE);		
	}
}

void save_maps_templates_button_func (void)
{
	if (!(*write_maps_templates_to_files[DATA_FORMAT_VERSION])(0))
		return;
}   

void submit_notes_button_func (void)
{
	if (!((*write_notes_to_files[DATA_FORMAT_VERSION])(1, txv_notes)))	
		return;
	gtk_widget_set_sensitive(btn_submit_additional_notes, TRUE);			// Reset of notes is available until starting of first recording
	gtk_widget_set_sensitive(btn_start_stop_recording, TRUE);			// Reset of notes is available until starting of first recording	
}   

void submit_additional_notes_button_func (void)
{
	(*write_additional_notes_to_files[DATA_FORMAT_VERSION])(1, txv_additional_notes);	
}

void start_stop_recording_button_func (void)
{
	int ret_val;
	if ((recording_ongoing) && (!stop_recording_request))
	{
		stop_recording_request = 1;
		pthread_join (recording_thread, NULL);
		recording_ongoing = 0;
		gtk_widget_set_sensitive(btn_save_maps_templates, FALSE);
		gtk_widget_set_sensitive(btn_submit_notes, FALSE);
		gtk_widget_set_sensitive(btn_submit_additional_notes, TRUE);
		gtk_widget_set_sensitive(btn_start_stop_recording, TRUE);
		gtk_widget_set_sensitive(btn_delete_last_recording, TRUE);				
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
			return;
		}
		gtk_widget_set_sensitive(btn_save_maps_templates, FALSE);	// After first recording do not let any change in notes and maps/templates files
		gtk_widget_set_sensitive(btn_submit_notes, FALSE);
		gtk_widget_set_sensitive(btn_submit_additional_notes, TRUE);
		gtk_widget_set_sensitive(btn_start_stop_recording, TRUE);
		gtk_widget_set_sensitive(btn_delete_last_recording, FALSE);	// do not let any deletion during recording			
	}
	return;
	
}

void delete_last_recording_button_func (void)
{
	(*delete_last_recording[DATA_FORMAT_VERSION])(0);			
	gtk_widget_set_sensitive(btn_delete_last_recording, FALSE);
						
}

void *recording_handler(void *ptr)
{
	int part_num = 0;
	TimeStamp recording_start_time_ns;
	TimeStamp recording_end_time_ns;	
	ptr = NULL;
	
	while (1)
	{
		if (start_recording_request)
		{
			start_recording_request = 0;
			recording_start_time_ns = (*initialize_buffer_reading_start_indexes_and_time_for_recording[DATA_FORMAT_VERSION])(0);
			if (!((*create_data_directory[DATA_FORMAT_VERSION])(1,recording_start_time_ns)))		
			{
				(*fclose_all_data_files[DATA_FORMAT_VERSION])(0);
				break;
			}
			gtk_button_set_label (GTK_BUTTON(btn_start_stop_recording),"Stop");
			gtk_widget_set_sensitive( btn_delete_last_recording, FALSE);						
		}
		else if ((recording_ongoing) && (!stop_recording_request))
		{
			usleep(100000);		
			(*get_buffer_reading_range_indexes_for_recording[DATA_FORMAT_VERSION])(0);
			if ((*write_data_in_buffer[DATA_FORMAT_VERSION])(1, part_num))		
				continue;
			(*fclose_all_data_files[DATA_FORMAT_VERSION])(0);	
			gtk_widget_set_sensitive( btn_delete_last_recording, TRUE);							
			break;
		}
		else if (stop_recording_request)
		{
			stop_recording_request = 0;
			recording_end_time_ns = (*get_buffer_reading_end_indexes_and_time_for_recording[DATA_FORMAT_VERSION])(0);				
			if ((*write_data_in_buffer[DATA_FORMAT_VERSION])(2, part_num, recording_end_time_ns))		
				gtk_widget_set_sensitive( btn_delete_last_recording, TRUE);
			(*fclose_all_data_files[DATA_FORMAT_VERSION])(0);
			gtk_button_set_label (GTK_BUTTON(btn_start_stop_recording),"Start");			
			break;														
		}
		else
		{
			printf("Recorder: BUG: Inconvenient recording request and status condition\n");
			printf("Recorder: BUG: Inconvenient recording request and status condition\n");		
			printf("Recorder: BUG: Inconvenient recording request and status condition\n");
			break;
		}
		part_num++;
	}
	return ptr;
}


void set_directory_btn_select_directory_to_save(void)
{
	char line[600];
	FILE *fp = NULL;
       	if ((fp = fopen("./path_initial_directory", "r")) == NULL)  
       	{ 
       		printf ("ERROR: Recorder: Couldn't find file: ./path_initial_directory\n"); 
       		printf ("ERROR: Recorder: /home is loaded as initial direcoty to create data folder\n");
		gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (btn_select_directory_to_save),"/home");
       	}
       	else
       	{
		if (fgets(line, sizeof line, fp ) == NULL) 
		{ 
			printf("ERROR: Recorder: Couldn' t read ./path_initial_directory\n"); 
       			printf ("ERROR: Recorder: /home is loaded as initial direcoty to create data folder\n");
			gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (btn_select_directory_to_save),"/home");
		}
		else
		{
			gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (btn_select_directory_to_save),line);
		}
		fclose(fp); 		
	}  	 
}

void fill_notes_text_view(void)
{
	char line[1000];
	FILE *fp = NULL;
	int i = 0;
	GtkTextIter start, end;
	GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(txv_notes));	
       	gtk_text_buffer_get_start_iter ( buffer, &start);
        gtk_text_buffer_get_end_iter ( buffer, &end);
        
        if ((fp = fopen("./initial_note", "r")) == NULL)  
       	{ 
       		printf ("ERROR: Recorder: Couldn't find file ./initial_note\n"); 
       		printf ("ERROR: Recorder: N/A is loaded as initial note\n");
		gtk_text_buffer_set_text (gtk_text_view_get_buffer(GTK_TEXT_VIEW(txv_notes)), "N/A\n", -1);       		 
       	}
       	else
       	{
   		
		while (fgets(line, sizeof line, fp ) != NULL) 
		{ 
        		gtk_text_buffer_get_end_iter ( buffer, &end);   				
			gtk_text_buffer_insert(buffer, &end, line, -1);
			i++;
			if (i == 100)
			{
       				printf ("ERROR: Recorder: ./initial_note is longer than 200 lines.\n");
       				printf ("ERROR: Recorder: Only initial 200 lines were read.\n\n");      
       				break; 				 				
			}
		}
		fclose(fp);       		
       	}
}


