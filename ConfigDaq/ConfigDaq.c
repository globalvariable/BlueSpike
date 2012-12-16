#include "ConfigDaq.h"

static DaqMwaData *daq_mwa_data = NULL; 
static DaqCon2KrnlSpkMsg *daq_config_2_kernel_spike_msgs = NULL;


static GtkWidget *entry_daq_card_num;
static GtkWidget *entry_daq_card_chan_range_lower;
static GtkWidget *entry_daq_card_chan_range_upper;

static GtkWidget *entry_mwa_num;
static GtkWidget *entry_mwa_chan_range_lower;
static GtkWidget *entry_mwa_chan_range_upper;

static GtkWidget *btn_turn_daq_on_off;
static GtkWidget *btn_map_channels;
static GtkWidget *btn_interrogate_mapping;
static GtkWidget *btn_cancel_all_mapping;

static GtkWidget *btn_select_config_daq_file_to_load;
static GtkWidget *btn_load_config_daq_file; 

static GtkWidget *btn_select_folder_to_save_config_daq_file;
static GtkWidget *btn_save_config_daq_file; 

static void create_gui(void);
static void turn_daq_on_off_button_func(void); 
static void map_channels_button_func(void); 
static void interrogate_mapping_button_func(void); 
static void cancel_all_mapping_button_func(void); 
static void load_config_daq_file_button_func(void); 
static void save_config_daq_file_button_func(void); 

static bool set_directory_btn_select_directory_to_load(void);
static void set_directory_btn_select_directory_to_save(void);

int main( int argc, char *argv[])
{
	daq_mwa_data = (DaqMwaData*)rtai_malloc(SHM_NUM_KERNEL_SPIKE_DAQ_MWA_DATA, 0);
	if (daq_mwa_data == NULL) 
		return print_message(ERROR_MSG ,"ConfigDaq", "ConfigDaq", "main", "daq_mwa_data == NULL.");
	daq_config_2_kernel_spike_msgs = allocate_shm_client_daq_config_2_kernel_spike_msg_buffer(daq_config_2_kernel_spike_msgs);
	if (daq_config_2_kernel_spike_msgs == NULL) 
		return print_message(ERROR_MSG ,"ConfigDaq", "ConfigDaq", "main", "daq_config_2_kernel_spike_msgs == NULL.");
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
  
	initialize_data_read_write_handlers();
	
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
	if (daq_mwa_data->daq_cards_on)
	{	
		gtk_button_set_label (GTK_BUTTON(btn_turn_daq_on_off),"DAQ Card : ON");
		gtk_widget_set_sensitive(btn_turn_daq_on_off, TRUE);
		gtk_widget_set_sensitive(btn_map_channels, FALSE);
		gtk_widget_set_sensitive(btn_load_config_daq_file, FALSE);	
	}
	else
	{
		gtk_button_set_label (GTK_BUTTON(btn_turn_daq_on_off),"DAQ Card : OFF");	
	}

	hbox = gtk_hbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE,FALSE,20);	
	
	btn_select_config_daq_file_to_load = gtk_file_chooser_button_new ("Select Maps File", GTK_FILE_CHOOSER_ACTION_OPEN);
        gtk_box_pack_start(GTK_BOX(hbox),btn_select_config_daq_file_to_load, TRUE,TRUE,0);
	set_directory_btn_select_directory_to_load();

	btn_load_config_daq_file = gtk_button_new_with_label("Load ConfigDaq File");
        gtk_box_pack_start(GTK_BOX(hbox),btn_load_config_daq_file,TRUE,TRUE, 0);		

	hbox = gtk_hbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE,FALSE,20);	

  	btn_select_folder_to_save_config_daq_file = gtk_file_chooser_button_new ("Select Directory", GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER);
        gtk_box_pack_start(GTK_BOX(hbox), btn_select_folder_to_save_config_daq_file, FALSE,FALSE,0);
	set_directory_btn_select_directory_to_save();
	
	btn_save_config_daq_file = gtk_button_new_with_label("Save ConfigDaq File");
        gtk_box_pack_start(GTK_BOX(hbox),btn_save_config_daq_file,TRUE,TRUE, 0);


  	gtk_widget_show_all(window);

        g_signal_connect (GTK_OBJECT (window), "destroy", G_CALLBACK (gtk_main_quit), NULL);
	g_signal_connect(G_OBJECT(btn_turn_daq_on_off), "clicked", G_CALLBACK(turn_daq_on_off_button_func), NULL);       
        g_signal_connect(G_OBJECT(btn_map_channels), "clicked", G_CALLBACK(map_channels_button_func), NULL);
        g_signal_connect(G_OBJECT(btn_interrogate_mapping), "clicked", G_CALLBACK(interrogate_mapping_button_func), NULL);      
        g_signal_connect(G_OBJECT(btn_cancel_all_mapping), "clicked", G_CALLBACK(cancel_all_mapping_button_func), NULL);              
        g_signal_connect(G_OBJECT(btn_load_config_daq_file), "clicked", G_CALLBACK(load_config_daq_file_button_func), NULL);
        g_signal_connect(G_OBJECT(btn_save_config_daq_file), "clicked", G_CALLBACK(save_config_daq_file_button_func), NULL);
	return;
}

void turn_daq_on_off_button_func(void)
{
	if (daq_mwa_data->daq_cards_on)
	{		
		if (! write_to_daq_config_2_kernel_spike_msg_buffer(daq_config_2_kernel_spike_msgs, DAQ_CONFIG_2_KERNEL_SPIKE_MSG_TURN_CARDS_OFF, NUM_OF_MWA_NULL, NUM_OF_CHAN_PER_MWA_NULL, NUM_OF_DAQ_CARD_NULL, NUM_OF_CHANNEL_PER_DAQ_CARD_NULL, DAQ_CONFIG_2_KERNEL_SPIKE_MSG_ADDITIONAL_NULL))
			return (void)print_message(ERROR_MSG ,"ConfigDaq", "ConfigDaq", "turn_daq_on_off_button_func", "! write_to_daq_config_2_kernel_spike_msg_buffer().");
		gtk_button_set_label (GTK_BUTTON(btn_turn_daq_on_off),"DAQ Card : OFF");
		gtk_widget_set_sensitive(btn_map_channels, TRUE);
		gtk_widget_set_sensitive(btn_load_config_daq_file, TRUE);	
		gtk_widget_set_sensitive(btn_cancel_all_mapping, TRUE);						
	}
	else
	{
		if (! write_to_daq_config_2_kernel_spike_msg_buffer(daq_config_2_kernel_spike_msgs, DAQ_CONFIG_2_KERNEL_SPIKE_MSG_TURN_CARDS_ON, NUM_OF_MWA_NULL, NUM_OF_CHAN_PER_MWA_NULL, NUM_OF_DAQ_CARD_NULL, NUM_OF_CHANNEL_PER_DAQ_CARD_NULL, DAQ_CONFIG_2_KERNEL_SPIKE_MSG_ADDITIONAL_NULL))
			return (void)print_message(ERROR_MSG ,"ConfigDaq", "ConfigDaq", "turn_daq_on_off_button_func", "! write_to_daq_config_2_kernel_spike_msg_buffer().");
		gtk_button_set_label (GTK_BUTTON(btn_turn_daq_on_off),"DAQ Card : ON");
		gtk_widget_set_sensitive(btn_map_channels, FALSE);
		gtk_widget_set_sensitive(btn_load_config_daq_file, FALSE);		
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
		if ((daq_mwa_data->daq_mwa_map[daq_num][i].mwa != NUM_OF_MWA_NULL) || (daq_mwa_data->daq_mwa_map[daq_num][i].channel != NUM_OF_CHAN_PER_MWA_NULL))
		{
			printf ("WARNING: Daq Card %d, Channel %d has been configured previously\n", daq_num, i);
			printf ("WARNING: Configured as DAQ: %d   Channel: %d  ----> MWA: %d   Channel: %d\n", daq_num, i, daq_mwa_data->daq_mwa_map[daq_num][i].mwa, daq_mwa_data->daq_mwa_map[daq_num][i].channel);
		}

		if (! write_to_daq_config_2_kernel_spike_msg_buffer(daq_config_2_kernel_spike_msgs, DAQ_CONFIG_2_KERNEL_SPIKE_MSG_MAP_MWA_CHAN, mwa_num, j, daq_num, i, DAQ_CONFIG_2_KERNEL_SPIKE_MSG_ADDITIONAL_NULL))
			return (void)print_message(ERROR_MSG ,"ConfigDaq", "ConfigDaq", "map_channels_button_func", "! write_to_daq_config_2_kernel_spike_msg_buffer().");
		j++;  
	}

	printf ("DAQ Card -> Microwire Array mapping completed\n");
	printf ("Microwire Array -> DAQ Card mapping completed\n");
	gtk_widget_set_sensitive(btn_turn_daq_on_off, TRUE);
	return;
}

void interrogate_mapping_button_func(void)
{
	printf("Interrogating mapping\n");
	interrogate_mapping(daq_mwa_data);
	printf("Interrogating mapping...complete\n");	
	return;
}

void cancel_all_mapping_button_func(void)
{
	if (! write_to_daq_config_2_kernel_spike_msg_buffer(daq_config_2_kernel_spike_msgs, DAQ_CONFIG_2_KERNEL_SPIKE_MSG_CANCEL_ALL_MAPPING, NUM_OF_MWA_NULL, NUM_OF_CHAN_PER_MWA_NULL, NUM_OF_DAQ_CARD_NULL, NUM_OF_CHANNEL_PER_DAQ_CARD_NULL, DAQ_CONFIG_2_KERNEL_SPIKE_MSG_ADDITIONAL_NULL))
		return (void)print_message(ERROR_MSG ,"ConfigDaq", "ConfigDaq", "cancel_all_mapping_button_func", "! write_to_daq_config_2_kernel_spike_msg_buffer().");
}

void load_config_daq_file_button_func(void)
{
	char *path = NULL;

	int version;
	path = gtk_file_chooser_get_uri (GTK_FILE_CHOOSER (btn_select_config_daq_file_to_load));
	path = &(path[7]);                  ///     file:///path

	if (!get_format_version(&version, path))
		return (void)print_message(ERROR_MSG ,"ConfigDaq", "ConfigDaq", "load_config_daq_file_button_func", "! get_format_version()."); 
	
	if (!((*read_config_daq_data[version])(2, path, daq_config_2_kernel_spike_msgs)))
		return (void)print_message(ERROR_MSG ,"ConfigDaq", "ConfigDaq", "load_config_daq_file_button_func", "! *read_config_daq_data[version]()."); 

	return (void)print_message(INFO_MSG ,"ConfigDaq", "ConfigDaq", "load_config_daq_file_button_func", "Succesuflly loaded ConfigDaq data file.");
}

static void save_config_daq_file_button_func(void)
{
	char *path_temp = NULL, *path = NULL;
	path_temp = gtk_file_chooser_get_uri (GTK_FILE_CHOOSER (btn_select_folder_to_save_config_daq_file));
	path = &path_temp[7];   // since     uri returns file:///home/....	
	if (! (*write_config_daq_data[DATA_FORMAT_VERSION])(2, path, daq_mwa_data))
		return (void)print_message(ERROR_MSG ,"ConfigDaq", "ConfigDaq", "save_config_daq_file_button_func", "! *write_config_daq_data()."); 		
	return (void)print_message(INFO_MSG ,"ConfigDaq", "ConfigDaq", "save_config_daq_file_button_func", "Succesuflly saved ConfigDaq data file.");
}

static bool set_directory_btn_select_directory_to_load(void)
{
	char line[600];
	FILE *fp = NULL;
	GFile *gfile_path; 
       	if ((fp = fopen("./path_initial_directory", "r")) == NULL)  
       	{ 
       		printf ("ERROR: Recorder: Couldn't find file: ./path_initial_directory\n"); 
       		printf ("ERROR: Recorder: /home is loaded as initial directory.\n");
		gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (btn_select_config_daq_file_to_load),"/home");
		return FALSE;
       	}
       	else
       	{
		if (fgets(line, sizeof line, fp ) == NULL) 
		{ 
			printf("ERROR: Recorder: Couldn' t read ./path_initial_directory\n"); 
       			printf ("ERROR: Recorder: /home is loaded as initial directory.\n");
			gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (btn_select_config_daq_file_to_load),"/home");
			fclose(fp); 		
			return FALSE;
		}
		else
		{
			gfile_path = g_file_new_for_path (line); 
			gtk_file_chooser_set_file (GTK_FILE_CHOOSER (btn_select_config_daq_file_to_load), gfile_path, NULL);
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
       		printf ("ERROR: Recorder: Couldn't find file: ./path_initial_directory\n"); 
       		printf ("ERROR: Recorder: /home is loaded as initial direcoty to create data folder\n");
		gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (btn_select_folder_to_save_config_daq_file),"/home");
       	}
       	else
       	{
		if (fgets(line, sizeof line, fp ) == NULL) 
		{ 
			printf("ERROR: Recorder: Couldn' t read ./path_initial_directory\n"); 
       			printf ("ERROR: Recorder: /home is loaded as initial direcoty to create data folder\n");
			gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (btn_select_folder_to_save_config_daq_file),"/home");
		}
		else
		{
			line[strlen(line)-10] = 0;   /// to get the folder instead of ../../DaqConfig file
			gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (btn_select_folder_to_save_config_daq_file),line);
		}
		fclose(fp); 		
	}  	 
}
