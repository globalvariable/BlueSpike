#include "./v0/DataFormat_v0.h"

#define MAX_NUMBER_OF_DATA_FORMAT_VER	2

int (*create_data_directory[MAX_NUMBER_OF_DATA_FORMAT_VER])(int num, ... );
int (*write_data_in_buffer[MAX_NUMBER_OF_DATA_FORMAT_VER])(int num, ... );
int (*read_data_in_blue_spike_data_folder[MAX_NUMBER_OF_DATA_FORMAT_VER])(int num, ... );

int initialize_data_read_write_handlers(void);
