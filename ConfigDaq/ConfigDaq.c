#include "ConfigDaq.h"


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
	GtkWidget *window, *hbox, *hbox1, *vbox, *vbox1, *lbl;
 	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  	gtk_window_set_default_size(GTK_WINDOW(window), 400, 400);
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
	
    	hbox = gtk_hbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE,FALSE,10);		
	
	btn_interrogate_mapping = gtk_button_new_with_label("Interrogate Mapping");
	gtk_box_pack_start (GTK_BOX (hbox), btn_interrogate_mapping, TRUE, FALSE, 0);		
	
    	hbox = gtk_hbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE,FALSE, 20);		
	
	btn_finalize_mapping = gtk_button_new_with_label("Finalize Mapping");
	gtk_box_pack_start (GTK_BOX (hbox), btn_finalize_mapping, TRUE, FALSE, 0);
	
	hbox = gtk_hbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE,FALSE,20);	
	
	btn_select_config_file_to_load = gtk_file_chooser_button_new ("Select Config File", GTK_FILE_CHOOSER_ACTION_OPEN);
        gtk_box_pack_start(GTK_BOX(hbox),btn_select_config_file_to_load, TRUE,TRUE,0);
	gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (btn_select_config_file_to_load),"/home");	

	btn_load_config_file = gtk_button_new_with_label("Load Config File");
        gtk_box_pack_start(GTK_BOX(hbox),btn_load_config_file,TRUE,TRUE, 0);		
        
        hbox = gtk_hbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE,FALSE,20);	 
        		
	btn_select_config_file_directory_to_save = gtk_file_chooser_button_new ("Select Config Files Folder", GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER);
        gtk_box_pack_start(GTK_BOX(hbox), btn_select_config_file_directory_to_save, TRUE,TRUE,0);
	gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (btn_select_config_file_directory_to_save),"/home");		
	
        entry_config_file_name = gtk_entry_new();
        gtk_box_pack_start(GTK_BOX(hbox), entry_config_file_name, FALSE,FALSE,0);
	gtk_widget_set_size_request(entry_config_file_name, 150, 25) ;
	
	btn_save_config_file = gtk_button_new_with_label("Save Config File");
        gtk_box_pack_start(GTK_BOX(hbox),btn_save_config_file,TRUE,TRUE, 0);				
        
  	gtk_widget_show_all(window);

        g_signal_connect (GTK_OBJECT (window), "destroy", G_CALLBACK (gtk_main_quit), NULL);
        g_signal_connect(G_OBJECT(btn_map_channels), "clicked", G_CALLBACK(map_channels_button_func), NULL);
        g_signal_connect(G_OBJECT(btn_finalize_mapping), "clicked", G_CALLBACK(finalize_mapping_button_func), NULL);   
        g_signal_connect(G_OBJECT(btn_interrogate_mapping), "clicked", G_CALLBACK(interrogate_mapping_button_func), NULL);      
        g_signal_connect(G_OBJECT(btn_load_config_file), "clicked", G_CALLBACK(load_config_file_button_func), NULL);
        g_signal_connect(G_OBJECT(btn_save_config_file), "clicked", G_CALLBACK(save_config_file_button_func), NULL);
        
	return;
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
		shared_memory->daq_mwa_map[daq_num][i].mwa = mwa_num;
		shared_memory->daq_mwa_map[daq_num][i].channel = j;
		j++;  
	}
	printf ("DAQ Card -> Microwire Array mapping completed\n");
	interrogate_mapping();
	return;
}

void interrogate_mapping_button_func(void)
{
	printf("Interrogating config file\n");
	interrogate_mapping();
	return;
}

void finalize_mapping_button_func(void)
{
	printf("Finalized mapping of DAQ Card and MWA Channels\n");
	gtk_main_quit();
	return;
} 

void load_config_file_button_func(void)
{
	int i,j, line_cntr=0;
	int max_num_of_daq_card, max_num_of_channel_per_daq_card, max_num_of_mwa, max_num_of_channel_per_mwa;
	char *path = NULL, *path_file = NULL, line[200];
	FILE *fp=NULL;
	path = gtk_file_chooser_get_uri (GTK_FILE_CHOOSER (btn_select_config_file_to_load));

	printf("Loading config file...\n");

	if (path == NULL)
	{	
		printf("ERROR: No file is choosen with file chooser\n");
		return;
	}
	path_file = &path[7];   // since     uri returns file:///home/....

	fp = fopen(path_file, "r");
	if (fp == NULL)
	{
		printf("ERROR: Couldn't fopen the config file\n");
		return;		
	}
	
	line_cntr++;
	if (fgets(line, sizeof line, fp ) == NULL)   {  printf("ERROR: Couldn' t read %d th line of config file\n", line_cntr);  fclose(fp); return; }       
	if (strcmp(line, "Data Acquisiton Cards vs Microwire Arrays Mapping File\n" ) != 0)
	{
		printf("ERROR: Not a valid DAQ Card - MWA Mapping Config File\n");
		return;
	}  	
	
	line_cntr++;	
	if (fgets(line, sizeof line, fp ) == NULL)   {  printf("ERROR: Couldn' t read %d th line of config file\n", line_cntr);  fclose(fp); return; }         
	max_num_of_daq_card = (int)atof(line);	
	if (MAX_NUM_OF_DAQ_CARD	< max_num_of_daq_card )
	{
		printf("ERROR: Config file was saved when MAX_NUM_OF_DAQ_CARD = %d\n",max_num_of_daq_card);
		printf("ERROR: Now it is MAX_NUM_OF_DAQ_CARD = %d\n", MAX_NUM_OF_DAQ_CARD);	
		return;
	}
	else if (MAX_NUM_OF_DAQ_CARD	> max_num_of_daq_card )
	{
		printf("WARNING: Config file was saved when MAX_NUM_OF_DAQ_CARD = %d\n",max_num_of_daq_card);
		printf("WARNING: Now it is MAX_NUM_OF_DAQ_CARD = %d\n", MAX_NUM_OF_DAQ_CARD);		
		printf("WARNING: Configuration was done but you should check validity\n");	
	}
	
	line_cntr++;
	if (fgets(line, sizeof line, fp ) == NULL)   {  printf("ERROR: Couldn' t read %d th line of config file\n", line_cntr);  fclose(fp); return; }          
	max_num_of_channel_per_daq_card = (int)atof(line);
	if (MAX_NUM_OF_CHANNEL_PER_DAQ_CARD < max_num_of_channel_per_daq_card)
	{
		printf("ERROR: Config file was saved when MAX_NUM_OF_DAQ_CARD = %d\n", max_num_of_channel_per_daq_card);
		printf("ERROR: Now it is MAX_NUM_OF_DAQ_CARD = %d\n", MAX_NUM_OF_CHANNEL_PER_DAQ_CARD);	
		return;
	}
	else if (MAX_NUM_OF_CHANNEL_PER_DAQ_CARD > max_num_of_channel_per_daq_card)
	{
		printf("WARNING: Config file was saved when MAX_NUM_OF_DAQ_CARD = %d\n", max_num_of_channel_per_daq_card);
		printf("WARNING: Now it is MAX_NUM_OF_DAQ_CARD = %d\n", MAX_NUM_OF_CHANNEL_PER_DAQ_CARD);		
		printf("WARNING: Configuration was done but you should check validity\n");	
	}	
	
	line_cntr++;
	if (fgets(line, sizeof line, fp ) == NULL)   {  printf("ERROR: Couldn' t read %d th line of config file\n", line_cntr);  fclose(fp); return; }         
	max_num_of_mwa = (int)atof(line);
	if (MAX_NUM_OF_MWA < max_num_of_mwa)
	{
		printf("ERROR: Config file was saved when MAX_NUM_OF_MWA = %d\n", max_num_of_mwa);
		printf("ERROR: Now it is MAX_NUM_OF_MWA = %d\n", MAX_NUM_OF_MWA);	
		return;
	}
	else if (MAX_NUM_OF_MWA > max_num_of_mwa)
	{
		printf("WARNING: Config file was saved when MAX_NUM_OF_MWA = %d\n", max_num_of_mwa);
		printf("WARNING: Now it is MAX_NUM_OF_MWA= %d\n", MAX_NUM_OF_MWA);		
		printf("WARNING: Configuration was done but you should check validity\n");	
	}
	
	line_cntr++;
	if (fgets(line, sizeof line, fp ) == NULL)   {  printf("ERROR: Couldn' t read %d th line of config file\n", line_cntr);  fclose(fp); return; }          
	max_num_of_channel_per_mwa = (int)atof(line);
	if (MAX_NUM_OF_CHAN_PER_MWA < max_num_of_channel_per_mwa)
	{
		printf("ERROR: Config file was saved when MAX_NUM_OF_CHAN_PER_MWA = %d\n", max_num_of_channel_per_mwa);
		printf("ERROR: Now it is MAX_NUM_OF_CHAN_PER_MWA = %d\n", MAX_NUM_OF_CHAN_PER_MWA);	
		return;
	}
	else if (MAX_NUM_OF_CHAN_PER_MWA > max_num_of_channel_per_mwa)
	{
		printf("WARNING: Config file was saved when MAX_NUM_OF_CHAN_PER_MWA = %d\n", max_num_of_channel_per_mwa);
		printf("WARNING: Now it is MAX_NUM_OF_CHAN_PER_MWA = %d\n", MAX_NUM_OF_CHAN_PER_MWA);		
		printf("WARNING: Configuration was done but you should check validity\n");	
	}	
	
	for (i = 0; i<max_num_of_daq_card; i++)    /// Only configure the ones written in config file even though max_num_of_daq_card != MAX_NUM_OF_DAQ_CARD
	{	
		for (j = 0; j<max_num_of_channel_per_daq_card; j++)
		{	
			line_cntr++;
			if (fgets(line, sizeof line, fp ) == NULL)   {  printf("ERROR: Couldn' t read %d th line of config file\n", line_cntr);  fclose(fp); return; }          
			shared_memory->daq_mwa_map[i][j].mwa =(int)atof(line);
			line_cntr++;
			if (fgets(line, sizeof line, fp ) == NULL)   {  printf("ERROR: Couldn' t read %d th line of config file\n", line_cntr);  fclose(fp); return; }          			
			shared_memory->daq_mwa_map[i][j].channel =(int)atof(line);
		}
	}
	
	fclose(fp);

	interrogate_mapping();

	return;
}
void save_config_file_button_func(void)
{
	int i,j;
	char *path_temp = NULL, *path = NULL, path_file[500];
	FILE *fp;
	path_temp = gtk_file_chooser_get_uri (GTK_FILE_CHOOSER (btn_select_config_file_directory_to_save));
	path = &path_temp[7];   // since     uri returns file:///home/....
	strcpy(path_file, path);
	strcat(path_file, "/");
	strcat(path_file, gtk_entry_get_text(GTK_ENTRY(entry_config_file_name)));
	
	printf("Saving Config file...\n");
	
	if (interrogate_mapping())
	{
		fp = fopen(path_file, "w");
		fprintf(fp, "Data Acquisiton Cards vs Microwire Arrays Mapping File\n");	
		fprintf(fp, "%d\n", MAX_NUM_OF_DAQ_CARD);
		fprintf(fp, "%d\n", MAX_NUM_OF_CHANNEL_PER_DAQ_CARD);
		fprintf(fp, "%d\n", MAX_NUM_OF_MWA);
		fprintf(fp, "%d\n", MAX_NUM_OF_CHAN_PER_MWA);		
		for (i = 0; i<MAX_NUM_OF_DAQ_CARD; i++)
		{	
			for (j = 0; j<MAX_NUM_OF_CHANNEL_PER_DAQ_CARD; j++)
			{	
				fprintf(fp, "%d\n", shared_memory->daq_mwa_map[i][j].mwa);
				fprintf(fp, "%d\n", shared_memory->daq_mwa_map[i][j].channel);				
			}
		}
		if (fp != NULL)
			fclose(fp);
	}
	else
	{
		printf("Didn' t save any config file since no valid mapping was detected\n");
	}
	return;
}

bool interrogate_mapping(void)
{
	int i, j, check = 0;
	for (i = 0; i<MAX_NUM_OF_DAQ_CARD; i++)
	{	
		for (j = 0; j<MAX_NUM_OF_CHANNEL_PER_DAQ_CARD; j++)
		{	
			if ((shared_memory->daq_mwa_map[i][j].mwa == MAX_NUM_OF_MWA) && (shared_memory->daq_mwa_map[i][j].channel == MAX_NUM_OF_CHAN_PER_MWA))
			{
				printf ("DAQ: %d   Channel: %d  ----> MWA: Idle  Channel: Idle\n", i, j);
			}
			else if ((shared_memory->daq_mwa_map[i][j].mwa != MAX_NUM_OF_MWA) && (shared_memory->daq_mwa_map[i][j].channel == MAX_NUM_OF_CHAN_PER_MWA))
			{
				printf ("DAQ: %d   Channel: %d  ----> MWA: %d  Channel: Idle\n", i, j, shared_memory->daq_mwa_map[i][j].mwa);			
				printf ("****************************************************************\n");			
				printf ("*************************BUG*********************************\n");
				printf ("BUG: MWA was not mapped but its channel was mapped\n");
				printf ("*************************BUG*********************************\n");
				printf ("****************************************************************\n");	
				return FALSE;					
			}
			else if ((shared_memory->daq_mwa_map[i][j].mwa == MAX_NUM_OF_MWA) && (shared_memory->daq_mwa_map[i][j].channel != MAX_NUM_OF_CHAN_PER_MWA))
			{
				printf ("DAQ: %d   Channel: %d  ----> MWA: Idle Channel: %d\n", i, j, shared_memory->daq_mwa_map[i][j].channel);			
				printf ("*******************************************************************\n");			
				printf ("*************************BUG************************************\n");
				printf ("BUG: MWA was mapped but its channel was not mapped\n");
				printf ("*************************BUG************************************\n");
				printf ("******************************************************************\n");	
				return FALSE;					
			}			
			else
			{
				printf ("DAQ: %d   Channel: %d  ----> MWA: %d   Channel: %d\n", i, j, shared_memory->daq_mwa_map[i][j].mwa, shared_memory->daq_mwa_map[i][j].channel);
				check =1;
			}
		}
	}
	if (check == 1)	
		return TRUE;
	else
	{
		printf("ERROR: None of the channels of any DAQ Card was mapped\n");
		return FALSE;
	}	
}
