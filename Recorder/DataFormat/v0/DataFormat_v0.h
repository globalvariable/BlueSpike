
#include <stdarg.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <gtk/gtk.h>
#include <string.h>

#include "./../../../SharedMemory.h"

#define NUM_OF_DATA_FILE_PER_RECORDING 6
#define RECORDING_DATA_FILE_IDX 0
#define SPIKE_TIMESTAMP_DATA_FILE_IDX 1
#define EXP_ENVI_EVENT_DATA_FILE_IDX 2
#define EXP_ENVI_COMMAND_DATA_FILE_IDX 3
#define MOV_OBJ_EVENT_DATA_FILE_IDX 4
#define MOV_OBJ_COMMAND_DATA_FILE_IDX 5

FILE **file_ptr_arr;	

char main_directory_path[500];
char data_directory_path[500];

int data_directory_cntr;

int create_main_directory_v0(int num, ...);
int create_data_directory_v0(int num, ...);
int create_data_files(void);

int write_data_in_buffer_v0(int num, ...);


