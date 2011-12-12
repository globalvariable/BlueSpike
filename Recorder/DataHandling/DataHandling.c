#include "DataHandling.h"


int initialize_data_read_write_handlers(void)
{
	create_main_directory[0] = &create_main_directory_v0;
	create_data_directory[0] = &create_data_directory_v0;
	write_data_in_buffer[0] = &write_data_in_buffer_v0;
	fclose_all_data_files[0] = &fclose_all_data_files_v0;
	return 1;
}


