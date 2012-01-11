#include "DataHandling.h"


int initialize_data_read_write_handlers(void)
{
	create_main_directory[0] = &create_main_directory_v0;
	create_data_directory[0] = &create_data_directory_v0;
	write_data_in_buffer[0] = &write_data_in_buffer_v0;
	fclose_all_data_files[0] = &fclose_all_data_files_v0;
	
	write_notes_to_files[0] = &write_notes_to_files_v0;
	write_additional_notes_to_files[0] = &write_additional_notes_to_files_v0;	
	write_maps_templates_to_files[0] = &write_maps_templates_to_files_v0;
	
	delete_last_recording[0] = &delete_last_recording_v0;
	
	initialize_buffer_reading_start_indexes_and_time_for_recording[0] = &initialize_buffer_reading_start_indexes_and_time_for_recording_v0;
	get_buffer_reading_range_indexes_for_recording[0] = &get_buffer_reading_range_indexes_for_recording_v0;
	get_buffer_reading_end_indexes_and_time_for_recording[0] = &get_buffer_reading_end_indexes_and_time_for_recording_v0;
	
	read_mapping[0] = &read_mapping_v0;
	read_spike_detection_files[0] = &read_spike_thresholds_v0;
	read_spike_sorting_files[0] = &read_spike_templates_v0;		
	
	return 1;
}

int is_blue_spike_data(char *blue_spike_data_path)
{
	char path[600];
	char line[200];
	FILE *fp;
	int line_cntr = 0;
	strcpy(path, blue_spike_data_path);
	strcat(path, "/meta");
	if ((fp = fopen(path, "r")) == NULL)  { printf ("ERROR: Recorder: Couldn't read file: %s\n\n", path); return 0; }
	if (fgets(line, sizeof line, fp ) == NULL)   {  printf("ERROR: Couldn' t read %d th line of %s\n", line_cntr, path);  fclose(fp); return 0; } else {line_cntr++;}   
	fclose(fp);   
	if (strcmp(line, "----------BlueSpike - Main Meta File----------\n" ) == 0)
		return 1;
	else
		return 0;	
}

int get_format_version(int *version, char *blue_spike_data_path)
{
	char path[600];
	char line[200];
	char word[10];
	int line_cntr = 0;
	FILE *fp;
	if (is_blue_spike_data(blue_spike_data_path))
	{
		strcpy(path, blue_spike_data_path);
		strcat(path, "/meta");		
		if ((fp = fopen(path, "r")) == NULL)  { printf ("ERROR: Recorder: Couldn't read file: %s\n\n", path); return 0; }
		if (fgets(line, sizeof line, fp ) == NULL)   {  printf("ERROR: Couldn' t read %d th line of %s\n", line_cntr, path);  fclose(fp); return 0; } else {line_cntr++;}   
		if (fgets(line, sizeof line, fp ) == NULL)   {  printf("ERROR: Couldn' t read %d th line of %s\n", line_cntr, path);  fclose(fp); return 0; } else {line_cntr++;}   
		if (!(get_word_in_line('\t', 1, word, line, TRUE)))
			return 0;
		fclose(fp);   	
		*version = (int)atof(word);
		return 1;
	}
	return 0;
}
