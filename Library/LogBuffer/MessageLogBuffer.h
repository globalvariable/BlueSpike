#ifndef LOG_BUFFER_H
#define LOG_BUFFER_H

#include <stdbool.h>
#include <pthread.h>
#include "../../System/TimeStamp/TimeStamp.h"
#include "../Misc/Misc.h"

#define MAX_MESSAGE_LOG_BUFF_PROCESS_NAME_LEN	20
#define MAX_MESSAGE_LOG_BUFF_MODULE_NAME_LEN		20
#define MAX_MESSAGE_LOG_BUFF_FUNCTION_NAME_LEN	20
#define MAX_MESSAGE_LOG_BUFF_MESSAGE_LEN			80

typedef struct 
{
	int			message_type;
	TimeStamp	time;
	char 		process[MAX_MESSAGE_LOG_BUFF_PROCESS_NAME_LEN];
	char 		module[MAX_MESSAGE_LOG_BUFF_MODULE_NAME_LEN];
	char 		function[MAX_MESSAGE_LOG_BUFF_FUNCTION_NAME_LEN];
	char 		message[MAX_MESSAGE_LOG_BUFF_MESSAGE_LEN];
} MessageLogBufferItem;

typedef struct 
{
	pthread_mutex_t 		read_mutex; 	// two thread can read this (e.g. log printing non-rt thread and mov_obj_handler_rt_task can read it before terminating the program during an exit(1);)
	MessageLogBufferItem	*buffer;
	unsigned int			buffer_size;
	unsigned int			write_idx;
	unsigned int			read_idx;
} MessageLogBuffer;


MessageLogBuffer* allocate_message_log_buffer(MessageLogBuffer *log_buffer, unsigned int buffer_size);
MessageLogBuffer* deallocate_message_log_buffer(MessageLogBuffer *log_buffer);
bool write_to_message_log_buffer(MessageLogBuffer *log_buffer, int message_type, TimeStamp time, char *process, char *module, char *function, char *message);
bool print_message_log_buffer(MessageLogBuffer *log_buffer, unsigned int max_item_num_to_print);


#endif


