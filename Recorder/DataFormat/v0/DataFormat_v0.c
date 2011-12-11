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

	char temp[600];
	FILE *temp_fp;

	file_ptr_arr = g_new0(FILE*, NUM_OF_DATA_FILE_PER_RECORDING);    
	
	strcpy(temp, data_directory_path);	
	strcat(temp, "/Recording");
	if ((temp_fp = fopen(temp, "w")) == NULL)  { printf ("ERROR: Recorder: Couldn't create file: %s\n\n", temp); return 0; }
	file_ptr_arr[RECORDING_DATA_FILE_IDX] =  temp_fp;

	strcpy(temp, data_directory_path);	
	strcat(temp, "/SpikeTimeStamp");
	if ((temp_fp = fopen(temp, "w")) == NULL)  { printf ("ERROR: Recorder: Couldn't create file: %s\n\n", temp); return 0; }
	file_ptr_arr[SPIKE_TIMESTAMP_DATA_FILE_IDX] =  temp_fp;		

	strcpy(temp, data_directory_path);	
	strcat(temp, "/ExpEnviEvent");
	if ((temp_fp = fopen(temp, "w")) == NULL)  { printf ("ERROR: Recorder: Couldn't create file: %s\n\n", temp); return 0; }
	file_ptr_arr[EXP_ENVI_EVENT_DATA_FILE_IDX] =  temp_fp;		

	strcpy(temp, data_directory_path);	
	strcat(temp, "/ExpEnviCommand");
	if ((temp_fp = fopen(temp, "w")) == NULL)  { printf ("ERROR: Recorder: Couldn't create file: %s\n\n", temp); return 0; }
	file_ptr_arr[EXP_ENVI_COMMAND_DATA_FILE_IDX] =  temp_fp;	
	
	strcpy(temp, data_directory_path);	
	strcat(temp, "/MovObjEvent");
	if ((temp_fp = fopen(temp, "w")) == NULL)  { printf ("ERROR: Recorder: Couldn't create file: %s\n\n", temp); return 0; }
	file_ptr_arr[MOV_OBJ_EVENT_DATA_FILE_IDX] =  temp_fp;		
	
	strcpy(temp, data_directory_path);	
	strcat(temp, "/MovObjCommand");
	if ((temp_fp = fopen(temp, "w")) == NULL)  { printf ("ERROR: Recorder: Couldn't create file: %s\n\n", temp); return 0; }
	file_ptr_arr[MOV_OBJ_COMMAND_DATA_FILE_IDX] =  temp_fp;		
	
	return 1;
}

int write_data_in_buffer_v0(int num, ...)
{

	return 0;
}
