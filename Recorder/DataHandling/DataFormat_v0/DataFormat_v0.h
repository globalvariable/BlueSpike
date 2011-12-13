#include <rtai_shm.h>

#include <stdarg.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <gtk/gtk.h>
#include <string.h>


#include "../Buffer_v0/Buffer_v0.h"

//Index table of FILE **file_ptr_arr
#define RECORDING_DATA_FILE_IDX     			0																		
#define SPIKE_TIMESTAMP_DATA_FILE_IDX		(RECORDING_DATA_FILE_IDX + (MAX_NUM_OF_MWA*MAX_NUM_OF_CHAN_PER_MWA))	
#define EXP_ENVI_EVENT_DATA_FILE_IDX		(SPIKE_TIMESTAMP_DATA_FILE_IDX + 1)										
#define EXP_ENVI_COMMAND_DATA_FILE_IDX 	(EXP_ENVI_EVENT_DATA_FILE_IDX + MAX_NUM_OF_EXP_ENVI_ITEMS)
#define MOV_OBJ_EVENT_DATA_FILE_IDX 		(EXP_ENVI_COMMAND_DATA_FILE_IDX + MAX_NUM_OF_EXP_ENVI_ITEMS)
#define MOV_OBJ_COMMAND_DATA_FILE_IDX	(MOV_OBJ_EVENT_DATA_FILE_IDX + MAX_NUM_OF_MOVING_OBJECTS)

#define NUM_OF_DATA_FILE_PER_RECORDING 	(MOV_OBJ_COMMAND_DATA_FILE_IDX + MAX_NUM_OF_MOVING_OBJECTS)

FILE **file_ptr_arr;	

char main_directory_path[500];
char data_directory_path[500];

int data_directory_cntr;

int create_main_directory_v0(int num, ...);
int create_data_directory_v0(int num, ...);
int fclose_all_data_files_v0(int num, ...);

int create_data_files(void);

int write_data_in_buffer_v0(int num, ...);

int create_recording_data(void);
int create_spike_timestamp_data(void);
int create_exp_envi_event_data(void);		
int create_exp_envi_command_data(void);			
int create_mov_obj_event_data(void);		
int create_mov_obj_command_data(void);

int write_recording_data(void);
int write_spike_timestamp_data(void);
int write_exp_envi_event_data(void);		
int write_exp_envi_command_data(void);			
int write_mov_obj_event_data(void);		
int write_mov_obj_command_data(void);

