
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

	
 	gtk_widget_show_all(window);
 	
   	g_signal_connect (GTK_OBJECT (window), "destroy", G_CALLBACK (gtk_main_quit), NULL);		
   	
	return;
}	
