#include "DataFormat_v0.h"


int create_data_directory_v0(int num, ...)
{
	char *main_path;

	GPtrArray **main_file_ptr_arr;

  	va_list arguments;
	va_start ( arguments, num );   

    	main_path = va_arg ( arguments, char *);   	
    	main_file_ptr_arr= va_arg ( arguments, GPtrArray **);   	

	va_end ( arguments );
	
	(*main_file_ptr_arr) = g_ptr_array_new();
	
	create_recording_data_folder(*main_file_ptr_arr, main_path);
	
	return 0;
}

int write_data_in_buffer_v0(int num, ...)
{
	GPtrArray *ptr_dat_files;
	FILE **recording_data_file_ptr_arr;	
		
  	va_list arguments;
  	va_start ( arguments, num );  
  	
  	ptr_dat_files = va_arg ( arguments, GPtrArray*);  

	recording_data_file_ptr_arr = g_ptr_array_index(ptr_dat_files, 0);
	fprintf (recording_data_file_ptr_arr[1], "selam\n");


	fclose (recording_data_file_ptr_arr[1]);
   	va_end ( arguments );  	
	return 0;
}


int create_recording_data_folder(GPtrArray *file_ptr_arr, char *m_path)
{
	FILE **recording_data_file_ptr_arr;	
	char temp[500];
	FILE *temp_fp;
	
	recording_data_file_ptr_arr = g_new0(FILE*, 6);    // meta, maps, spike_thresholds, templates, notes, data   // three ptr

	g_ptr_array_add  (file_ptr_arr, recording_data_file_ptr_arr);
	
	strcpy(temp, m_path);		
	strcat(temp, "/RecordingData");	
	mkdir(temp, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH | S_IWOTH);	
	
	strcpy(temp, m_path);	
	strcat(temp, "/RecordingData/Data");
	if ((temp_fp = fopen(temp, "w")) == NULL)  { printf ("ERROR: Recorder: Couldn't create file: %s\n\n", temp); return 0; }
	recording_data_file_ptr_arr[RECORDING_DATA_DATA_FILE_IDX] =  temp_fp;

	strcpy(temp, m_path);	
	strcat(temp, "/RecordingData/Meta");
	if ((temp_fp = fopen(temp, "w")) == NULL)  { printf ("ERROR: Recorder: Couldn't create file: %s\n\n", temp); return 0; }
	recording_data_file_ptr_arr[RECORDING_DATA_META_FILE_IDX] =  temp_fp;
	
	strcpy(temp, m_path);	
	strcat(temp, "/RecordingData/Notes");
	if ((temp_fp = fopen(temp, "w")) == NULL)  { printf ("ERROR: Recorder: Couldn't create file: %s\n\n", temp); return 0; }
	recording_data_file_ptr_arr[RECORDING_DATA_NOTES_FILE_IDX] =  temp_fp;
	
	strcpy(temp, m_path);	
	strcat(temp, "/RecordingData/Maps");
	if ((temp_fp = fopen(temp, "w")) == NULL)  { printf ("ERROR: Recorder: Couldn't create file: %s\n\n", temp); return 0; }
	recording_data_file_ptr_arr[RECORDING_DATA_MAPS_FILE_IDX] =  temp_fp;	
	
	strcpy(temp, m_path);	
	strcat(temp, "/RecordingData/SpikeThres");
	if ((temp_fp = fopen(temp, "w")) == NULL)  { printf ("ERROR: Recorder: Couldn't create file: %s\n\n", temp); return 0; }
	recording_data_file_ptr_arr[RECORDING_DATA_SPIKE_THRES_FILE_IDX] =  temp_fp;
	
	strcpy(temp, m_path);	
	strcat(temp, "/RecordingData/Template");
	if ((temp_fp = fopen(temp, "w")) == NULL)  { printf ("ERROR: Recorder: Couldn't create file: %s\n\n", temp); return 0; }
	recording_data_file_ptr_arr[RECORDING_DATA_TEMPLATE_FILE_IDX] =  temp_fp;			
	
	return 1;
}


