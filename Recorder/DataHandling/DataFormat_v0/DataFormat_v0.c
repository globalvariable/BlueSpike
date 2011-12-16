#include "DataFormat_v0.h"


int create_main_directory_v0(int num, ...)
{
	FILE *fp;
	char *path_chooser;
	DIR	*dir_main_folder;
	char temp_dir_path[600];
  	va_list arguments;
	va_start ( arguments, num );   
    	path_chooser = va_arg ( arguments, char *);   	
	va_end ( arguments );
	
	strcpy(temp_dir_path, path_chooser);	
	strcat(temp_dir_path, "/BlueSpikeData");
	if ((dir_main_folder = opendir(temp_dir_path)) != NULL)
        {
        	printf ("Recorder: ERROR: path: %s already has BlueSpikeData folder.\n", path_chooser);		
        	printf ("Recorder: ERROR: Select another folder or delete BlueSpikeData directory.\n\n");		        		
                return 0;
        }
 	strcpy(main_directory_path, temp_dir_path);	
	mkdir(main_directory_path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH | S_IWOTH);

        printf ("Recorder: Created BlueSpikeData folder in: %s.\n", path_chooser);
        printf ("Recorder: BlueSpikeData path is: %s.\n\n", main_directory_path); 
        
	if (!create_main_meta_file())
		return 0;

	if (!create_main_logs_file())
		return 0;
		
	if ((fp = fopen("./path_initial_directory", "w")) == NULL)  { printf ("ERROR: Recorder: Couldn't find directory: %s\n\n", "./path_initial_directory"); return 0; }
	fprintf(fp, "%s", main_directory_path);
	fclose (fp);		

	return 1;
}


int create_data_directory_v0(int num, ...)
{
	char data_directory_name[10];
	char data_directory_num[10];
	DIR	*dir_data_directory;	
	
	TimeStamp rec_start;
	
  	va_list arguments;
	va_start ( arguments, num );   
    	rec_start = va_arg ( arguments, TimeStamp);   	
	va_end ( arguments );	
	
	if (data_directory_cntr <10)
	{
		strcpy(data_directory_name, "dat0000");
		sprintf(data_directory_num, "%d" , data_directory_cntr);
		strcat(data_directory_name, data_directory_num);
	}
	else if (data_directory_cntr <100)
	{
		strcpy(data_directory_name, "dat000");
		sprintf(data_directory_num, "%d" , data_directory_cntr);
		strcat(data_directory_name, data_directory_num);		
	}
	else if (data_directory_cntr <1000)
	{
		strcpy(data_directory_name, "dat00");
		sprintf(data_directory_num, "%d" , data_directory_cntr);
		strcat(data_directory_name, data_directory_num);		
	}	
	else if (data_directory_cntr <10000)
	{
		strcpy(data_directory_name, "dat0");
		sprintf(data_directory_num, "%d" , data_directory_cntr);
		strcat(data_directory_name, data_directory_num);		
	}	
	else if (data_directory_cntr <100000)
	{
		strcpy(data_directory_name, "dat");
		sprintf(data_directory_num, "%d" , data_directory_cntr);
		strcat(data_directory_name, data_directory_num);			
	}	
	else
	{
		printf("Recorder: ERROR: data directory counter is %d.\n", data_directory_cntr);
		printf("Recorder: ERROR: Supported range is 0<= x <100000.\n\n");		
		return 0;
	}
	
	strcpy(data_directory_path, main_directory_path);	
	strcat(data_directory_path, "/");
	strcat(data_directory_path, data_directory_name);	
	if ((dir_data_directory = opendir(data_directory_path)) != NULL)
        {
        	printf ("Recorder: ERROR: path: %s already has %s folder.\n", main_directory_path, data_directory_name);		
                return 0;
        }
	mkdir(data_directory_path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH | S_IWOTH);	

	if (create_data_files(rec_start))
		data_directory_cntr++;

	FILE *fp;	
	char  temp_path[600];
	time_t rawtime;
	struct tm * timeinfo;
 	strcpy(temp_path, main_directory_path);
 	strcat(temp_path, "/logs");
	if ((fp = fopen(temp_path, "a+")) == NULL)  { printf ("ERROR: Recorder: Couldn't create file: %s\n\n", temp_path); return 0; }	
	time ( &rawtime );
	timeinfo = localtime (&rawtime);
	fprintf(fp,"%s\t\tStarted recording & Created data folder.\n", asctime (timeinfo)); 	
	fprintf(fp,"---------------------------------------------------------------------------------\n");
	fclose(fp);	
	return 1;
}

int create_data_files(TimeStamp rec_start)
{
	file_ptr_arr = g_new0(FILE*, NUM_OF_DATA_FILE_PER_RECORDING);    
	
	int time_prev = rt_get_cpu_time_ns();
	int time_curr;

	if (!create_recording_data())
		return 0;
	if (!create_spike_timestamp_data())
		return 0;
	if (!create_exp_envi_event_data())
		return 0;		
	if (!create_exp_envi_command_data())
		return 0;			
	if (!create_mov_obj_event_data())
		return 0;		
	if (!create_mov_obj_command_data())
		return 0;
	if (!create_meta_data(rec_start))
		return 0;		
	time_curr = rt_get_cpu_time_ns();
	if ((time_curr - time_prev) > 100000000)		// if writing exceeds x milliseconds, there might be buffer reading error. (buffer might be overwrited before reading it.)
	{								
		printf ("Recorder: ERROR: Recording data files creation lasted longer than 50 msec\n");
		printf ("Recorder: ERROR: It lasted %d nanoseconds\n", time_curr - time_prev);
		printf ("Recorder: ERROR: It might lead to missing of data during buffer-reading\n");		
		printf("Recorder: Recording interrupted.\n\n");
		// delete_last_recorded_file();
		return 0;
	}					
		
	time_prev = time_curr;		
		
	return 1;
}

int create_recording_data(void)
{
	char temp[600];
	FILE *fp;
	char char_mwa_num[10], char_chan_num[10];
	int i,j;
	
	for (i=0; i < MAX_NUM_OF_MWA; i++)
	{
		for (j=0; j<MAX_NUM_OF_CHAN_PER_MWA; j++)
		{
			strcpy(temp, data_directory_path);	
			if (i<10)
			{
				strcat(temp, "/Rec_mwa_0");
				sprintf(char_mwa_num, "%d" , i);
				strcat(temp, char_mwa_num);
			}
			else if (i<100)
			{
				strcat(temp, "/Rec_mwa_");
				sprintf(char_mwa_num, "%d" , i);
				strcat(temp, char_mwa_num);				
			}
			else
			{
				printf ("ERROR: Recorder: MAX MWA NUM is larger than 99\n\n");
				printf ("ERROR: Recorder: Couldn't create all recording files requested\n\n");
				return 0; 
			}
			strcat(temp, "_");
			if (j<10)
			{
				strcat(temp, "chan_00");
				sprintf(char_chan_num, "%d" , j);
				strcat(temp, char_chan_num);
			}
			else if (j<100)
			{
				strcat(temp, "chan_0");
				sprintf(char_chan_num, "%d" , j);
				strcat(temp, char_chan_num);				
			}
			else if (j<1000)
			{
				strcat(temp, "chan_");
				sprintf(char_chan_num, "%d" , j);
				strcat(temp, char_chan_num);			
			}
			else
			{
				printf ("ERROR: Recorder: MAX NUM OF CHAN PER MWA is larger than 999\n\n");
				printf ("ERROR: Recorder: Couldn't create all recording files requested\n\n");
				return 0; 
			}			
			if ((fp = fopen(temp, "w")) == NULL)  { printf ("ERROR: Recorder: Couldn't create file: %s\n\n", temp); return 0; }
			file_ptr_arr[RECORDING_DATA_FILE_IDX + (i*MAX_NUM_OF_CHAN_PER_MWA)+j] =  fp;
			fprintf(fp,"----------BlueSpike - Recording Data File ( MWA #%s\tChannel #%s )----------\n", char_mwa_num, char_chan_num);						
		}
	}

	return 1;
}
int create_spike_timestamp_data(void)
{
	char temp[600];
	FILE *fp;
	
	strcpy(temp, data_directory_path);	
	strcat(temp, "/SpikeTimeStamp");
	if ((fp = fopen(temp, "w")) == NULL)  { printf ("ERROR: Recorder: Couldn't create file: %s\n\n", temp); return 0; }
	file_ptr_arr[SPIKE_TIMESTAMP_DATA_FILE_IDX] =  fp;		
	fprintf(fp,"----------BlueSpike - Spike TimeStamps File----------\n");						
	return 1;
}
int create_exp_envi_event_data(void)
{
	char temp[600];
	FILE *fp;
	char char_exp_envi_item[10];
	int i;	
		
	for (i=0; i < MAX_NUM_OF_EXP_ENVI_ITEMS; i++)
	{
		strcpy(temp, data_directory_path);	
		if (i<10)
		{
			strcat(temp, "/ExpEnvi_Event_item_");
			sprintf(char_exp_envi_item, "%d" , i);
			strcat(temp, char_exp_envi_item);
		}
		else
		{
			printf ("ERROR: Recorder: MAX EXPERIMETAL ENVIRONMENT ITEM is larger than 9\n\n");
			printf ("ERROR: Recorder: Couldn't create all recording files requested\n\n");
			return 0; 
		}
		if ((fp = fopen(temp, "w")) == NULL)  { printf ("ERROR: Recorder: Couldn't create file: %s\n\n", temp); return 0; }
		file_ptr_arr[EXP_ENVI_EVENT_DATA_FILE_IDX+i] =  fp;		
		fprintf(fp,"----------BlueSpike - Experimental Environment Item #%s Event TimeStamps File----------\n", char_exp_envi_item);						
	}
	return 1;
}	
int create_exp_envi_command_data(void)
{
	char temp[600];
	FILE *fp;
	char char_exp_envi_item[10];
	int i;
	
	for (i=0; i < MAX_NUM_OF_EXP_ENVI_ITEMS; i++)
	{
		strcpy(temp, data_directory_path);	
		if (i<10)
		{
			strcat(temp, "/ExpEnvi_Comnd_item_");
			sprintf(char_exp_envi_item, "%d" , i);
			strcat(temp, char_exp_envi_item);
		}
		else
		{
			printf ("ERROR: Recorder: MAX EXPERIMETAL ENVIRONMENT ITEM is larger than 9\n\n");
			printf ("ERROR: Recorder: Couldn't create all recording files requested\n\n");
			return 0; 
		}
		if ((fp = fopen(temp, "w")) == NULL)  { printf ("ERROR: Recorder: Couldn't create file: %s\n\n", temp); return 0; }
		file_ptr_arr[EXP_ENVI_COMMAND_DATA_FILE_IDX+i] = fp;
		fprintf(fp,"----------BlueSpike - Experimental Environment Item #%s Command TimeStamps File----------\n", char_exp_envi_item);				
	}	
	
	return 1;
}		
int create_mov_obj_event_data(void)
{
	char temp[600];
	FILE *fp;
	char char_mov_obj_num[10];	
	int i;
	
	for (i=0; i < MAX_NUM_OF_MOVING_OBJECTS; i++)
	{
		strcpy(temp, data_directory_path);	
		if (i<10)
		{
			strcat(temp, "/MovObj_Event_obj_");
			sprintf(char_mov_obj_num, "%d" , i);
			strcat(temp, char_mov_obj_num);
		}
		else
		{
			printf ("ERROR: Recorder: MAX MOVING OBJECT NUM is larger than 9\n\n");
			printf ("ERROR: Recorder: Couldn't create all recording files requested\n\n");
			return 0; 
		}
		if ((fp = fopen(temp, "w")) == NULL)  { printf ("ERROR: Recorder: Couldn't create file: %s\n\n", temp); return 0; }
		file_ptr_arr[MOV_OBJ_EVENT_DATA_FILE_IDX + i] =  fp;
		fprintf(fp,"----------BlueSpike - Moving Object #%s Event TimeStamps File----------\n", char_mov_obj_num);
	}
	return 1;
}	

int create_mov_obj_command_data(void)
{
	char temp[600];
	FILE *fp;
	char char_mov_obj_num[10];	
	int i;
	
	for (i=0; i < MAX_NUM_OF_MOVING_OBJECTS; i++)
	{
		strcpy(temp, data_directory_path);	
		if (i<10)
		{
			strcat(temp, "/MovObj_Comnd_obj_");
			sprintf(char_mov_obj_num, "%d" , i);
			strcat(temp, char_mov_obj_num);
		}
		else
		{
			printf ("ERROR: Recorder: MAX MOVING OBJECT NUM is larger than 9\n\n");
			printf ("ERROR: Recorder: Couldn't create all recording files requested\n\n");
			return 0; 
		}
		if ((fp = fopen(temp, "w")) == NULL)  { printf ("ERROR: Recorder: Couldn't create file: %s\n\n", temp); return 0; }
		file_ptr_arr[MOV_OBJ_COMMAND_DATA_FILE_IDX + i] =  fp;
		fprintf(fp,"----------BlueSpike - Moving Object #%s Command TimeStamps File----------\n", char_mov_obj_num);		
	}
	return 1;
}

int write_data_in_buffer_v0(int num, ...)
{
	int time_prev = rt_get_cpu_time_ns();
	int time_curr;
	int part_num;
	
  	va_list arguments;
	va_start ( arguments, num );   
    	part_num = va_arg ( arguments, int);   	
	if (num == 2)	// it is last part to write. write end of recording time into meta
	{
		end_meta_data(va_arg ( arguments, TimeStamp));		
	}
	va_end ( arguments );
	
	write_recording_data();
	write_spike_timestamp_data();
	write_exp_envi_event_data();
	write_exp_envi_command_data();
	write_mov_obj_event_data();
	write_mov_obj_command_data();

	time_curr = rt_get_cpu_time_ns();
	if ((time_curr - time_prev) > 100000000)		// if writing exceeds x milliseconds, there might be buffer reading error. (buffer might be overwrited before reading it.)
	{								
		printf ("Recorder: ERROR: Recording data files (Part: %d) writing process lasted longer than 50 msec\n", part_num);
		printf ("Recorder: ERROR: It lasted %d nanoseconds\n", time_curr - time_prev);
		printf ("Recorder: ERROR: It might lead to missing of data during buffer-reading\n");		
		printf("Recorder: Recording interrupted.\n\n");
		// delete_last_recorded_file();
		return 0;
	}					
		
	time_prev = time_curr;	
	
	
	return 1;
}

int write_recording_data(void)
{
	int i, j, idx, end_idx;
	int file_ptr_arr_start_idx = RECORDING_DATA_FILE_IDX;
	RecordingDataChanBuff	*recording_data_chan_buff;
	for (i=0; i < MAX_NUM_OF_MWA; i++)
	{
		for (j=0; j<MAX_NUM_OF_CHAN_PER_MWA; j++)
		{
			recording_data_chan_buff = &(recording_data->recording_data_buff[i][j]);
			idx = recording_data_buff_prev_idx[i][j];
			end_idx = recording_data_buff_end_idx[i][j];
			while (idx != end_idx)
			{
				fprintf(file_ptr_arr[(file_ptr_arr_start_idx+(i*MAX_NUM_OF_CHAN_PER_MWA))+j],"%.15f\n", (*recording_data_chan_buff)[idx] );
				idx++;	
				if (idx ==	RECORDING_DATA_BUFF_SIZE)
					idx = 0;	
			}
		}
	}		
	return 1;
}

int write_spike_timestamp_data(void)
{
	int idx, end_idx;
	int file_ptr_start_idx = SPIKE_TIMESTAMP_DATA_FILE_IDX;
	int buff_size = SPIKE_TIMESTAMP_BUFF_SIZE;

	idx = spike_timestamp_buff_prev_idx;
	end_idx = spike_timestamp_buff_end_idx;
	while (idx != end_idx)
	{
		fprintf(file_ptr_arr[file_ptr_start_idx],"%llu\t", spike_time_stamp->spike_timestamp_buff[idx].peak_time);
		fprintf(file_ptr_arr[file_ptr_start_idx],"%d\t", spike_time_stamp->spike_timestamp_buff[idx].mwa);	
		fprintf(file_ptr_arr[file_ptr_start_idx],"%d\t", spike_time_stamp->spike_timestamp_buff[idx].channel);
		fprintf(file_ptr_arr[file_ptr_start_idx],"%d\t", spike_time_stamp->spike_timestamp_buff[idx].unit);
		fprintf(file_ptr_arr[file_ptr_start_idx],"%d\n", spike_time_stamp->spike_timestamp_buff[idx].include_unit);						
		idx++;	
		if (idx ==	buff_size)
			idx = 0;	
	}
	return 1;
}

int write_exp_envi_event_data(void)
{
	int idx, end_idx;
	int file_ptr_start_idx = EXP_ENVI_EVENT_DATA_FILE_IDX;
	int buff_size = EXP_ENVI_EVENT_TIMESTAMP_BUFF_SIZE;
	int i;
	
	for (i=0; i < MAX_NUM_OF_EXP_ENVI_ITEMS; i++)
	{
		idx = exp_envi_event_buff_prev_idx[i];
		end_idx = exp_envi_event_buff_end_idx[i];
		while (idx != end_idx)
		{
			fprintf(file_ptr_arr[file_ptr_start_idx+i],"%llu\t", exp_envi_event_time_stamp->timestamp_buff[i][idx].time);			
			fprintf(file_ptr_arr[file_ptr_start_idx+i],"%u\t", exp_envi_event_time_stamp->timestamp_buff[i][idx].type);
			fprintf(file_ptr_arr[file_ptr_start_idx+i],"%f\n", exp_envi_event_time_stamp->timestamp_buff[i][idx].value);											
			idx++;	
			if (idx ==	buff_size)
				idx = 0;	
		}
	}		
	return 1;
}	
	
int write_exp_envi_command_data(void)
{
	int idx, end_idx;
	int file_ptr_start_idx = EXP_ENVI_COMMAND_DATA_FILE_IDX;
	int buff_size = EXP_ENVI_COMMAND_TIMESTAMP_BUFF_SIZE;
	int i;
	
	for (i=0; i < MAX_NUM_OF_EXP_ENVI_ITEMS; i++)
	{
		idx = exp_envi_command_buff_prev_idx[i];
		end_idx = exp_envi_command_buff_end_idx[i];
		while (idx != end_idx)
		{
			fprintf(file_ptr_arr[file_ptr_start_idx+i],"%llu\t", exp_envi_command_time_stamp->timestamp_buff[i][idx].time);			
			fprintf(file_ptr_arr[file_ptr_start_idx+i],"%u\t", exp_envi_command_time_stamp->timestamp_buff[i][idx].type);
			fprintf(file_ptr_arr[file_ptr_start_idx+i],"%f\n", exp_envi_command_time_stamp->timestamp_buff[i][idx].value);											
			idx++;	
			if (idx ==	buff_size)
				idx = 0;	
		}
	}		
	return 1;
}	
		
int write_mov_obj_event_data(void)
{
	int i, j, idx, end_idx;
	int file_ptr_arr_start_idx = MOV_OBJ_EVENT_DATA_FILE_IDX;
	int buff_size = MOVING_OBJ_EVENT_TIMESTAMP_BUFF_SIZE;
	
	for (i=0; i < MAX_NUM_OF_MOVING_OBJECTS; i++)
	{
		idx = mov_obj_event_buff_prev_idx[i];
		end_idx = mov_obj_event_buff_end_idx[i];
		while (idx != end_idx)
		{
			fprintf(file_ptr_arr[file_ptr_arr_start_idx+i],"%llu", mov_obj_event_time_stamp->timestamp_buff[i][idx].time );
			for (j=0;j<MAX_NUM_OF_COMPONENT_PER_MOVING_OBJECT; j++)
			{
				fprintf(file_ptr_arr[file_ptr_arr_start_idx+i],"\t%f", mov_obj_event_time_stamp->timestamp_buff[i][idx].Speed[j]);
				fprintf(file_ptr_arr[file_ptr_arr_start_idx+i],"\t%d", mov_obj_event_time_stamp->timestamp_buff[i][idx].Direction[j]);	
				fprintf(file_ptr_arr[file_ptr_arr_start_idx+i],"\t%f", mov_obj_event_time_stamp->timestamp_buff[i][idx].Location[j]);								
			}
			fprintf(file_ptr_arr[file_ptr_arr_start_idx+i],"\n");
			idx++;	
			if (idx == buff_size)
				idx = 0;	
		}
	}
	return 1;
}	
	
int write_mov_obj_command_data(void)
{
	int i, j, idx, end_idx;
	int file_ptr_arr_start_idx = MOV_OBJ_COMMAND_DATA_FILE_IDX;
	int buff_size = MOVING_OBJ_COMMAND_TIMESTAMP_BUFF_SIZE;
	
	for (i=0; i < MAX_NUM_OF_MOVING_OBJECTS; i++)
	{
		idx = mov_obj_command_buff_prev_idx[i];
		end_idx = mov_obj_command_buff_end_idx[i];
		while (idx != end_idx)
		{
			fprintf(file_ptr_arr[file_ptr_arr_start_idx+i],"%llu", mov_obj_command_time_stamp->timestamp_buff[i][idx].time );
			for (j=0;j<MAX_NUM_OF_COMPONENT_PER_MOVING_OBJECT; j++)
			{
				fprintf(file_ptr_arr[file_ptr_arr_start_idx+i],"\t%f", mov_obj_command_time_stamp->timestamp_buff[i][idx].Speed[j]);
				fprintf(file_ptr_arr[file_ptr_arr_start_idx+i],"\t%d", mov_obj_command_time_stamp->timestamp_buff[i][idx].Direction[j]);	
				fprintf(file_ptr_arr[file_ptr_arr_start_idx+i],"\t%f", mov_obj_command_time_stamp->timestamp_buff[i][idx].Location[j]);								
			}
			fprintf(file_ptr_arr[file_ptr_arr_start_idx+i],"\n");
			idx++;	
			if (idx == buff_size)
				idx = 0;	
		}
	}
	return 1;
}

int create_meta_data(TimeStamp rec_start)
{
	char temp[600];
	FILE *fp;
	time_t rawtime;
	struct tm * timeinfo;
		
	strcpy(temp, data_directory_path);
	strcat(temp, "/meta");
	if ((fp = fopen(temp, "w")) == NULL)  { printf ("ERROR: Recorder: Couldn't create file: %s\n\n", temp); return 0; }
	fprintf(fp,"----------BlueSpike - Meta File----------\n");
	time ( &rawtime );
	timeinfo = localtime (&rawtime);
	fprintf(fp,"DATE\t%s\n", asctime (timeinfo)); 	
	fprintf(fp,"START\t%llu\n", rec_start);		
	fclose(fp);	
	return 1;	
}

int end_meta_data(TimeStamp rec_end)
{
	char temp_path[600];
	FILE *fp;
	time_t rawtime;
	struct tm * timeinfo;
		
	strcpy(temp_path, data_directory_path);
	strcat(temp_path, "/meta");
	if ((fp = fopen(temp_path, "a+")) == NULL)  { printf ("ERROR: Recorder: Couldn't create file: %s\n\n", temp_path); return 0; }
	fprintf(fp,"END\t%llu\n", rec_end);
	fprintf(fp,"----------BlueSpike - End of Meta File----------\n");	
	fclose(fp);	
		
 	strcpy(temp_path, main_directory_path);
 	strcat(temp_path, "/logs");
	if ((fp = fopen(temp_path, "a+")) == NULL)  { printf ("ERROR: Recorder: Couldn't create file: %s\n\n", temp_path); return 0; }	
	time ( &rawtime );
	timeinfo = localtime (&rawtime);
	fprintf(fp,"%s\t\tFinished recording.\n", asctime (timeinfo)); 	
	fprintf(fp,"---------------------------------------------------------------------------------\n");
	fclose(fp);
	return 1;		
}


int fclose_all_data_files_v0(int num, ...)
{
	int i , num_of_data_files = NUM_OF_DATA_FILE_PER_RECORDING;
		
	for (i = 0; i<num_of_data_files; i++)
	{
		if (file_ptr_arr[i] != NULL)
			fclose(file_ptr_arr[i]);
	}
	g_free(file_ptr_arr);

	return 1;
}

int create_main_meta_file(void)
{
	char  temp_path[600];
	time_t rawtime;
	struct tm * timeinfo;
	FILE *fp;

 	strcpy(temp_path, main_directory_path);
 	strcat(temp_path, "/meta");
	if ((fp = fopen(temp_path, "w")) == NULL)  { printf ("ERROR: Recorder: Couldn't create file: %s\n\n", temp_path); return 0; }
		
	fprintf(fp,"----------BlueSpike - Main Meta File----------\n");
	fprintf(fp,"DATA_FORMAT_VERSION\t%d\n", 0);	
	time ( &rawtime );
	timeinfo = localtime (&rawtime);
	fprintf(fp,"CREATION_DATE\t%s\n", asctime (timeinfo)); 	
	fprintf(fp,"MAX_NUM_OF_MWA\t%d\n", MAX_NUM_OF_MWA);
	fprintf(fp,"MAX_NUM_OF_CHAN_PER_MWA\t%d\n",MAX_NUM_OF_CHAN_PER_MWA);
	fprintf(fp,"MAX_NUM_OF_UNIT_PER_CHAN\t%d\n",MAX_NUM_OF_UNIT_PER_CHAN);
	fprintf(fp,"NUM_OF_SAMP_PER_SPIKE\t%d\n",NUM_OF_SAMP_PER_SPIKE);
	fprintf(fp,"MAX_NUM_OF_DAQ_CARD\t%d\n",MAX_NUM_OF_DAQ_CARD);
	fprintf(fp,"MAX_NUM_OF_CHANNEL_PER_DAQ_CARD\t%d\n",MAX_NUM_OF_CHANNEL_PER_DAQ_CARD);	
	fprintf(fp,"SAMPLING_INTERVAL\t%d\tNANOSECONDS\n", SAMPLING_INTERVAL);
	fprintf(fp,"LOWEST_VOLTAGE_MV\t%f\tMILLIVOLTS\n",LOWEST_VOLTAGE_MV);
	fprintf(fp,"HIGHEST_VOLTAGE_MV\t%f\tMILLIVOLTS\n",HIGHEST_VOLTAGE_MV);	
	fprintf(fp,"DAQ_0_MODEL\t%s\n",DAQ_0_MODEL);	
	fprintf(fp,"KERNELSPIKE_TICK_PERIOD\t%d\n", KERNELSPIKE_TICK_PERIOD);
	fprintf(fp,"KERNELSPIKE_CPUID\t%d\n", KERNELSPIKE_CPUID);
	fprintf(fp,"KERNELSPIKE_TASK_PRIORITY\t%d\n", KERNELSPIKE_TASK_PRIORITY);
	fprintf(fp,"MAX_NUM_OF_EXP_ENVI_ITEMS\t%d\n",MAX_NUM_OF_EXP_ENVI_ITEMS);
	fprintf(fp,"MAX_NUM_OF_MOVING_OBJECTS\t%d\n",MAX_NUM_OF_MOVING_OBJECTS);
	fprintf(fp,"MAX_NUM_OF_COMPONENT_PER_MOVING_OBJECT\t%d\n",MAX_NUM_OF_COMPONENT_PER_MOVING_OBJECT); 		
	fprintf(fp,"RECORDING_DATA_BUFF_SIZE\t%d\n",RECORDING_DATA_BUFF_SIZE);
	fprintf(fp,"SPIKE_TIMESTAMP_BUFF_SIZE\t%d\n",SPIKE_TIMESTAMP_BUFF_SIZE);
	fprintf(fp,"EXP_ENVI_EVENT_TIMESTAMP_BUFF_SIZE\t%d\n",EXP_ENVI_EVENT_TIMESTAMP_BUFF_SIZE);
	fprintf(fp,"EXP_ENVI_COMMAND_TIMESTAMP_BUFF_SIZE\t%d\n",EXP_ENVI_COMMAND_TIMESTAMP_BUFF_SIZE);
	fprintf(fp,"MOVING_OBJ_EVENT_TIMESTAMP_BUFF_SIZE\t%d\n",MOVING_OBJ_EVENT_TIMESTAMP_BUFF_SIZE);		
	fprintf(fp,"MOVING_OBJ_COMMAND_TIMESTAMP_BUFF_SIZE\t%d\n",MOVING_OBJ_COMMAND_TIMESTAMP_BUFF_SIZE);	
	fprintf(fp,"----------BlueSpike - End of Main Meta File----------\n");
	fclose(fp);
	return 1;
}

int write_notes_to_files_v0(int num, ...)
{
	char *text_buffer;
	GtkTextIter start, end;
	int char_count;
	char  temp_path[600];
	int i;
	FILE *fp;
	GtkWidget *text_view; 
	time_t rawtime;
	struct tm * timeinfo;		
	
  	va_list arguments;
	va_start ( arguments, num );   
    	text_view = va_arg ( arguments, GtkWidget *);   	
	va_end ( arguments );
	
	GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));	
       	gtk_text_buffer_get_start_iter ( buffer, &start);
        gtk_text_buffer_get_end_iter ( buffer, &end);
        text_buffer = gtk_text_buffer_get_text (buffer, &start, &end, TRUE);
	char_count = gtk_text_buffer_get_char_count(buffer);

 	strcpy(temp_path, main_directory_path);
 	strcat(temp_path, "/notes");
	if ((fp = fopen(temp_path, "w")) == NULL)  { printf ("ERROR: Recorder: Couldn't create file: %s\n\n", temp_path); return 0; }
	for (i = 0; i < char_count; i++)
	{
		fprintf(fp, "%c", text_buffer[i]);		
	}
	fclose(fp);

	if ((fp = fopen("./path_initial_note", "w")) == NULL)  { printf ("ERROR: Recorder: Couldn't find directory: %s\n\n", "./path_initial_directory"); return 0; }
	fprintf(fp, "%s", temp_path); fclose (fp);	
	
	time ( &rawtime );
	timeinfo = localtime (&rawtime);	
 	strcpy(temp_path, main_directory_path);
 	strcat(temp_path, "/logs");
	if ((fp = fopen(temp_path, "a+")) == NULL)  { printf ("ERROR: Recorder: Couldn't create file: %s\n\n", temp_path); return 0; }	
	fprintf(fp,"%s\t\tCreated /notes file and submitted notes.\n", asctime (timeinfo)); 	
	fprintf(fp,"---------------------------------------------------------------------------------\n");	 
	fclose(fp);
	
	return 1;
}

int write_additional_notes_to_files_v0(int num, ...)
{
	char *text_buffer;
	GtkTextIter start, end;
	int char_count;
	int i;
	GtkWidget *text_view; 
	FILE *fp;
	char  temp_path[600];
	time_t rawtime;
	struct tm * timeinfo;	
	
  	va_list arguments;
	va_start ( arguments, num );   
    	text_view = va_arg ( arguments, GtkWidget *);   	
	va_end ( arguments );
	
	GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));	
       	gtk_text_buffer_get_start_iter ( buffer, &start);
        gtk_text_buffer_get_end_iter ( buffer, &end);
        text_buffer = gtk_text_buffer_get_text (buffer, &start, &end, TRUE);
	char_count = gtk_text_buffer_get_char_count(buffer);

	time ( &rawtime );
	timeinfo = localtime (&rawtime);
 	strcpy(temp_path, main_directory_path);
 	strcat(temp_path, "/notes");
	if ((fp = fopen(temp_path, "a+")) == NULL)  { printf ("ERROR: Recorder: Couldn't append text to file: %s\n\n", temp_path); return 0; }		

	fprintf(fp,"---------------------------------------------------------------------------------\n");	
	fprintf(fp,"%s\t\tAdditional Note:\n", asctime (timeinfo)); 		
	
	for (i = 0; i < char_count; i++)
	{
		fprintf(fp, "%c", text_buffer[i]);		
	}	
	fclose(fp);
		
 	strcpy(temp_path, main_directory_path);
 	strcat(temp_path, "/logs");
	if ((fp = fopen(temp_path, "a+")) == NULL)  { printf ("ERROR: Recorder: Couldn't create file: %s\n\n", temp_path); return 0; }	
	fprintf(fp,"%s\t\tSubmitted additonal note to /notes.\n", asctime (timeinfo)); 	
	fprintf(fp,"---------------------------------------------------------------------------------\n");	 
	fclose(fp);
				
	return 1;
}

int create_main_logs_file(void)
{
	char  temp_path[600];
	time_t rawtime;
	struct tm * timeinfo;
	FILE *fp;
	
 	strcpy(temp_path, main_directory_path);
 	strcat(temp_path, "/logs");
	if ((fp = fopen(temp_path, "w")) == NULL)  { printf ("ERROR: Recorder: Couldn't create file: %s\n\n", temp_path); return 0; }	
	time ( &rawtime );
	timeinfo = localtime (&rawtime);
	fprintf(fp,"---------BlueSpike - Logs File---------------\n"); 		
	fprintf(fp,"%s\t\tCreated BlueSpikeData folder.\n", asctime (timeinfo)); 	
	fprintf(fp,"---------------------------------------------------------------------------------\n");
	fprintf(fp,"%s\t\tCreated /logs file (this file).\n", asctime (timeinfo)); 	
	fprintf(fp,"---------------------------------------------------------------------------------\n");	
	fclose(fp);
	return 1; 		
}

int write_maps_templates_to_files_v0(int num, ...)
{
	char  temp_path[600];
	time_t rawtime;
	struct tm * timeinfo;
	FILE *fp;

	if (!write_maps_to_files())
		return 0;
	if (!write_spike_thresholds_to_files())
		return 0;
	if (!write_templates_to_files())
		return 0;
		
 	strcpy(temp_path, main_directory_path);
 	strcat(temp_path, "/logs");
	if ((fp = fopen(temp_path, "a+")) == NULL)  { printf ("ERROR: Recorder: Couldn't append to file: %s\n\n", temp_path); return 0; }	
	time ( &rawtime );
	timeinfo = localtime (&rawtime);
	fprintf(fp,"%s\t\tSaved daq card mwa mappings and templates.\n", asctime (timeinfo)); 	
	fprintf(fp,"---------------------------------------------------------------------------------\n");	
	fclose(fp);	
	
	return 1;		
}

int write_maps_to_files(void)
{
	FILE *fp;	
	char  temp_path[600];
	int i, j;
 	strcpy(temp_path, main_directory_path);
 	strcat(temp_path, "/maps");
 	
	if (!interrogate_mapping())
		return 0;
	 	
	if ((fp = fopen(temp_path, "w")) == NULL)  { printf ("ERROR: Recorder: Couldn't create file: %s\n\n", temp_path); return 0; }
	
	fprintf(fp, "BlueSpike - Data Acquisiton Cards vs Microwire Arrays Mapping File\n");	
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
	fprintf(fp, "BlueSpike - End of Data Acquisiton Cards vs Microwire Arrays Mapping File\n");	
	fprintf(fp, "---------------------------------------------------------------------------------------------\n");			
	fprintf(fp, "---------------------------------------------------------------------------------------------\n");					
	fprintf(fp, "MAX_NUM_OF_DAQ_CARD: %d\n", MAX_NUM_OF_DAQ_CARD);
	fprintf(fp, "MAX_NUM_OF_CHANNEL_PER_DAQ_CARD: %d\n", MAX_NUM_OF_CHANNEL_PER_DAQ_CARD);
	fprintf(fp, "MAX_NUM_OF_MWA: %d\n", MAX_NUM_OF_MWA);
	fprintf(fp, "MAX_NUM_OF_MWA: %d\n", MAX_NUM_OF_CHAN_PER_MWA);	
	fprintf(fp, "DAQ Card Channels vs MWA Channels Mapping:\n");		
	for (i = 0; i<MAX_NUM_OF_DAQ_CARD; i++)
	{	
		for (j = 0; j<MAX_NUM_OF_CHANNEL_PER_DAQ_CARD; j++)
		{	
			fprintf(fp, "DAQ: %d   Channel: %d  ----> MWA: %d   Channel: %d\n", i, j, shared_memory->daq_mwa_map[i][j].mwa, shared_memory->daq_mwa_map[i][j].channel);	
		}
	}
	fprintf(fp, "---------------------------------------------------------------------------------------------\n");							
	fprintf(fp, "MWA Channels vs DAQ Card Channels Mapping:\n");		
	for (i = 0; i<MAX_NUM_OF_MWA; i++)
	{	
		for (j = 0; j<MAX_NUM_OF_CHAN_PER_MWA; j++)
		{	
			fprintf(fp, "MWA: %d   Channel: %d  ----> DAQ Card: %d   Channel: %d\n", i, j, shared_memory->mwa_daq_map[i][j].daq_card , shared_memory->mwa_daq_map[i][j].daq_chan );	
		}
	}		
	fclose(fp);

	if ((fp = fopen("../ConfigDaq/path_initial_directory", "w")) == NULL)  { printf ("ERROR: Recorder: Couldn't find directory: %s\n\n", "./path_initial_directory"); return 0; }
	fprintf(fp, "%s", temp_path); fclose (fp);		
	
	return 1;
}

int write_spike_thresholds_to_files(void)
{
	FILE *fp;	
	char  temp_path[600];
	int i,j;
	
 	strcpy(temp_path, main_directory_path);
 	strcat(temp_path, "/spike_thres");
	if ((fp = fopen(temp_path, "w")) == NULL)  { printf ("ERROR: Recorder: Couldn't create file: %s\n\n", temp_path); return 0; }	

	fprintf(fp, "----------------BlueSpike - Spike Thresholds File---------------\n");	
	for (i=0; i<MAX_NUM_OF_MWA; i++)
	{
		for (j=0; j<MAX_NUM_OF_CHAN_PER_MWA; j++)
		{
			fprintf(fp, "%f\t", shared_memory->spike_thresholding.amplitude_thres[i][j]);																			
		}
		fprintf(fp, "\n");																					
	}
	fprintf(fp, "----------------BlueSpike - End of Spike Thresholds File----------------\n");
	fclose(fp);

	if ((fp = fopen("../SpikeViewer/path_initial_directory", "w")) == NULL)  { printf ("ERROR: Recorder: Couldn't find directory: %s\n\n", "./path_initial_directory"); return 0; }
	fprintf(fp, "%s", temp_path); fclose (fp);
	
	return 1;
}

int write_templates_to_files(void)
{
	FILE *fp;	
	char  temp_path[600];
	int i,j,k,m,n;
 	strcpy(temp_path, main_directory_path);
 	strcat(temp_path, "/templates");
	if ((fp = fopen(temp_path, "w")) == NULL)  { printf ("ERROR: Recorder: Couldn't create file: %s\n\n", temp_path); return 0; }

	fprintf(fp, "----------------BlueSpike - Template Matching File---------------\n");	
	fprintf(fp, "%d\n", MAX_NUM_OF_DAQ_CARD);
	fprintf(fp, "%d\n", MAX_NUM_OF_CHANNEL_PER_DAQ_CARD);
	fprintf(fp, "%d\n", MAX_NUM_OF_MWA);
	fprintf(fp, "%d\n", MAX_NUM_OF_CHAN_PER_MWA);
	fprintf(fp, "%d\n", MAX_NUM_OF_UNIT_PER_CHAN);
	fprintf(fp, "--------- Inverted Covariance Matrix ------\n");																					
	for (i=0; i<MAX_NUM_OF_MWA; i++)
	{
		for (j=0; j<MAX_NUM_OF_CHAN_PER_MWA; j++)
		{
			fprintf(fp, "--------- MWA: %d\tChannel: %d ------\n", i,j);																					
			for (k=0; k<MAX_NUM_OF_UNIT_PER_CHAN; k++)
			{
				for (m=0; m<NUM_OF_SAMP_PER_SPIKE; m++)
				{
					for (n=0; n<NUM_OF_SAMP_PER_SPIKE; n++)
					{				
						fprintf(fp, "%.20f\t", shared_memory->template_matching_data[i][j][k].inv_S[m][n]);
					}
					fprintf(fp, "\n");																										
				}
				fprintf(fp, "\n");																															
			}
		}
	}
	fprintf(fp, "--------- sqrt(det(S)) / log(det(S)) / probability_threshold / sorting_on_off / include_unit ------\n");																					
	for (i=0; i<MAX_NUM_OF_MWA; i++)
	{
		fprintf(fp, "--------- MWA: %d\n", i);																							
		for (j=0; j<MAX_NUM_OF_CHAN_PER_MWA; j++)
		{
			for (k=0; k<MAX_NUM_OF_UNIT_PER_CHAN; k++)
			{
				fprintf(fp, "%.20f\t", shared_memory->template_matching_data[i][j][k].sqrt_det_S);
				fprintf(fp, "%.20f\t", shared_memory->template_matching_data[i][j][k].log_det_S);	
				fprintf(fp, "%E\t", shared_memory->template_matching_data[i][j][k].probability_thres);
				fprintf(fp, "%d\t", shared_memory->template_matching_data[i][j][k].sorting_on);		
				fprintf(fp, "%d\n", shared_memory->template_matching_data[i][j][k].include_unit);																
			}
		}
	}

	fprintf(fp, "----------------BlueSpike - End of Template Matching File---------------\n");		
	fclose(fp);
	
	if ((fp = fopen("../SpikeSorter/path_initial_directory", "w")) == NULL)  { printf ("ERROR: Recorder: Couldn't find directory: %s\n\n", "./path_initial_directory"); return 0; }
	fprintf(fp, "%s", temp_path); fclose (fp);
		
	return 1;		
}

int delete_last_recording_v0(int num, ...)
{
 	char path_item[600];
	FILE *fp;
	time_t rawtime;
	struct tm * timeinfo;
	DIR	*dir_data_directory;
        struct dirent   *dirent_item;
     	int status;
    	struct stat item_stat;
    	     	       	
	if ((dir_data_directory = opendir(data_directory_path)) == NULL)
        {
        	printf ("Recorder: ERROR: Couldn't opendir: %s\n\n", data_directory_path);		
                return 0;
        }

	while ((dirent_item = readdir(dir_data_directory)) !=NULL)
	{	
		if ((strcmp( dirent_item->d_name, "." ) == 0) || (strcmp(dirent_item->d_name, ".." ) == 0)) 	
			continue;
		strcpy(path_item, data_directory_path);
		strcat(path_item, "/");
		strcat(path_item, dirent_item->d_name);	
		if ((status = stat(path_item, &item_stat)) != 0)
		{
        		printf ("ERROR: Couldn' t retrieve item status (file or folder?): %s\n", path_item);
			return -1;
		} 
		if (S_ISREG (item_stat.st_mode))
		{
			remove(path_item);
		}			
	}
	closedir(dir_data_directory);		
	rmdir(data_directory_path);					
	data_directory_cntr--;	

 	strcpy(path_item, main_directory_path);
 	strcat(path_item, "/logs");
	if ((fp = fopen(path_item, "a+")) == NULL)  { printf ("ERROR: Recorder: Couldn't append to file: %s\n\n", path_item); return 0; }	
	time ( &rawtime );
	timeinfo = localtime (&rawtime);
	fprintf(fp,"%s\t\tDeleted last recording. Folder %s\n", asctime (timeinfo), data_directory_path); 	
	fprintf(fp,"---------------------------------------------------------------------------------\n");	
	fclose(fp);	
	
	return 1;
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
	{
		printf("---------------------------------------------------------------------------------------------\n");						
		for (i = 0; i<MAX_NUM_OF_MWA; i++)
		{	
			for (j = 0; j<MAX_NUM_OF_CHAN_PER_MWA; j++)
			{	
				printf("MWA: %d   Channel: %d  ----> DAQ Card: %d   Channel: %d\n", i, j, shared_memory->mwa_daq_map[i][j].daq_card , shared_memory->mwa_daq_map[i][j].daq_chan );	
			}
		}
		return TRUE;
	}
	else
	{
		printf("ERROR: None of the channels of any DAQ Card was mapped\n");
		return FALSE;
	}	
}
