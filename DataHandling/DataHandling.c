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
	
	
	
	return 1;
}


