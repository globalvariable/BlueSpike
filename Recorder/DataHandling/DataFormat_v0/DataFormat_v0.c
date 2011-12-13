#include "DataFormat_v0.h"


int create_main_directory_v0(int num, ...)
{
	char *path_chooser, temp_path[600];
	FILE *temp_fp;
	DIR	*dir_main_folder;
	
  	va_list arguments;
	va_start ( arguments, num );   
    	path_chooser = va_arg ( arguments, char *);   	
	va_end ( arguments );
	
	strcpy(main_directory_path, path_chooser);	
	strcat(main_directory_path, "/BlueSpikeData");
	if ((dir_main_folder = opendir(main_directory_path)) != NULL)
        {
        	printf ("Recorder: ERROR: path: %s already has BlueSpikeData folder.\n", path_chooser);		
        	printf ("Recorder: ERROR: Select another folder.\n\n");		        		
                return 0;
        }
	mkdir(main_directory_path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH | S_IWOTH);

        printf ("Recorder: Created BlueSpikeData folder in: %s.\n", path_chooser);
        printf ("Recorder: BlueSpikeData path is: %s.\n\n", main_directory_path); 
        
 	strcpy(temp_path, main_directory_path);
 	strcat(temp_path, "/meta");
	if ((temp_fp = fopen(temp_path, "w")) == NULL)  { printf ("ERROR: Recorder: Couldn't create file: %s\n\n", temp_path); return 0; }

 	strcpy(temp_path, main_directory_path);
 	strcat(temp_path, "/notes");
	if ((temp_fp = fopen(temp_path, "w")) == NULL)  { printf ("ERROR: Recorder: Couldn't create file: %s\n\n", temp_path); return 0; }	

 	strcpy(temp_path, main_directory_path);
 	strcat(temp_path, "/map");
	if ((temp_fp = fopen(temp_path, "w")) == NULL)  { printf ("ERROR: Recorder: Couldn't create file: %s\n\n", temp_path); return 0; }

 	strcpy(temp_path, main_directory_path);
 	strcat(temp_path, "/spike_thres");
	if ((temp_fp = fopen(temp_path, "w")) == NULL)  { printf ("ERROR: Recorder: Couldn't create file: %s\n\n", temp_path); return 0; }
	
 	strcpy(temp_path, main_directory_path);
 	strcat(temp_path, "/templates");
	if ((temp_fp = fopen(temp_path, "w")) == NULL)  { printf ("ERROR: Recorder: Couldn't create file: %s\n\n", temp_path); return 0; }	
	
 	strcpy(temp_path, main_directory_path);
 	strcat(temp_path, "/logs");
	if ((temp_fp = fopen(temp_path, "w")) == NULL)  { printf ("ERROR: Recorder: Couldn't create file: %s\n\n", temp_path); return 0; }		
	
	return 1;
}


int create_data_directory_v0(int num, ...)
{
	char data_directory_name[10];
	char data_directory_num[10];
	DIR	*dir_data_directory;	
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

	if (create_data_files())
		data_directory_cntr++;
		
	return 1;
}

int create_data_files(void)
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
	FILE *temp_fp;
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
			if ((temp_fp = fopen(temp, "w")) == NULL)  { printf ("ERROR: Recorder: Couldn't create file: %s\n\n", temp); return 0; }
			file_ptr_arr[RECORDING_DATA_FILE_IDX + (i*MAX_NUM_OF_CHAN_PER_MWA)+j] =  temp_fp;
		}
	}

	return 1;
}
int create_spike_timestamp_data(void)
{
	char temp[600];
	FILE *temp_fp;
	
	strcpy(temp, data_directory_path);	
	strcat(temp, "/SpikeTimeStamp");
	if ((temp_fp = fopen(temp, "w")) == NULL)  { printf ("ERROR: Recorder: Couldn't create file: %s\n\n", temp); return 0; }
	file_ptr_arr[SPIKE_TIMESTAMP_DATA_FILE_IDX] =  temp_fp;		

	return 1;
}
int create_exp_envi_event_data(void)
{
	char temp[600];
	FILE *temp_fp;
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
		if ((temp_fp = fopen(temp, "w")) == NULL)  { printf ("ERROR: Recorder: Couldn't create file: %s\n\n", temp); return 0; }
		file_ptr_arr[EXP_ENVI_EVENT_DATA_FILE_IDX+i] =  temp_fp;		
	}
	return 1;
}	
int create_exp_envi_command_data(void)
{
	char temp[600];
	FILE *temp_fp;
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
		if ((temp_fp = fopen(temp, "w")) == NULL)  { printf ("ERROR: Recorder: Couldn't create file: %s\n\n", temp); return 0; }
		file_ptr_arr[EXP_ENVI_COMMAND_DATA_FILE_IDX+i] =  temp_fp;		
	}	
	
	return 1;
}		
int create_mov_obj_event_data(void)
{
	char temp[600];
	FILE *temp_fp;
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
		if ((temp_fp = fopen(temp, "w")) == NULL)  { printf ("ERROR: Recorder: Couldn't create file: %s\n\n", temp); return 0; }
		file_ptr_arr[MOV_OBJ_EVENT_DATA_FILE_IDX + i] =  temp_fp;
	}
	return 1;
}		
int create_mov_obj_command_data(void)
{
	char temp[600];
	FILE *temp_fp;
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
		if ((temp_fp = fopen(temp, "w")) == NULL)  { printf ("ERROR: Recorder: Couldn't create file: %s\n\n", temp); return 0; }
		file_ptr_arr[MOV_OBJ_COMMAND_DATA_FILE_IDX + i] =  temp_fp;
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

