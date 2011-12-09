#include "DataFormat.h"


int initialize_data_read_write_handlers(void)
{
	create_data_directory[0] = &create_data_directory_v0;
	write_data_in_buffer[0] = &write_data_in_buffer_v0;
	read_data_in_blue_spike_data_folder[0] = &read_data_in_blue_spike_data_folder_v0;
	
	return 1;
}


