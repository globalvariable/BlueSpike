
#include <stdarg.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <gtk/gtk.h>
#include <string.h>

#include "./../../../SharedMemory.h"

#define RECORDING_DATA_DATA_FILE_IDX 0
#define RECORDING_DATA_META_FILE_IDX 1
#define RECORDING_DATA_NOTES_FILE_IDX 2
#define RECORDING_DATA_MAPS_FILE_IDX 3
#define RECORDING_DATA_SPIKE_THRES_FILE_IDX 4
#define RECORDING_DATA_TEMPLATE_FILE_IDX 5

int create_data_directory_v0(int num, ...);
int write_data_in_buffer_v0(int num, ...);
int create_recording_data_folder(GPtrArray *file_ptr_arr, char *m_path);

