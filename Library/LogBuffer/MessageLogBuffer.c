#include "MessageLogBuffer.h"


MessageLogBuffer* allocate_message_log_buffer(MessageLogBuffer *log_buffer, unsigned int buffer_size)
{
	if (log_buffer != NULL)
	{
		log_buffer = deallocate_message_log_buffer(log_buffer);
		log_buffer = allocate_message_log_buffer(log_buffer, buffer_size);
		return log_buffer;
	}  
	log_buffer = g_new0(MessageLogBuffer,1);
	pthread_mutex_init(&(log_buffer->read_mutex), NULL);	
	log_buffer->buffer =  g_new0(MessageLogBufferItem, buffer_size);
	log_buffer->buffer_size =  buffer_size;
	print_message(INFO_MSG ,"BlueSpike", "MessageLogBuffer", "allocate_message_log_buffer", "Created Message Log Buffer.");
	return log_buffer;	
}
MessageLogBuffer* deallocate_message_log_buffer(MessageLogBuffer *log_buffer)
{
	if (log_buffer == NULL)
		return (MessageLogBuffer*)print_message(BUG_MSG ,"BlueSpike", "MessageLogBuffer", "deallocate_message_log_buffer", "log_buffer == NULL.");   
	g_free(log_buffer->buffer);		 
	g_free(log_buffer);	
	return NULL;
}
bool write_to_message_log_buffer(MessageLogBuffer *log_buffer, int message_type, TimeStamp time, char *process, char *module, char *function, char *message)
{
	unsigned int *idx;
	idx = &(log_buffer->write_idx);
	MessageLogBufferItem *buff_item = &(log_buffer->buffer[*idx]);

	buff_item->message_type = message_type;
	buff_item->time = time;
  	strncpy (buff_item->process, process, MAX_MESSAGE_LOG_BUFF_PROCESS_NAME_LEN-1);
  	strncpy (buff_item->module, module, MAX_MESSAGE_LOG_BUFF_MODULE_NAME_LEN-1);
  	strncpy (buff_item->function, function, MAX_MESSAGE_LOG_BUFF_FUNCTION_NAME_LEN-1);
  	strncpy (buff_item->message, message, MAX_MESSAGE_LOG_BUFF_MESSAGE_LEN-1);

	if ((*idx + 1) == log_buffer->buffer_size)
		*idx = 0;
	else
		(*idx)++;
	if (*idx == log_buffer->read_idx)
		return print_message(BUG_MSG ,"BlueSpike", "MessageLogBuffer", "write_to_message_log_buffer", "BUFFER FULL!!!");   	
	return TRUE;	
}
bool print_message_log_buffer(MessageLogBuffer *log_buffer, unsigned int max_item_num_to_print)
{
	unsigned int item_printed = 0;
	unsigned int *write_idx = &(log_buffer->write_idx);
	unsigned int *read_idx = &(log_buffer->read_idx);
	MessageLogBufferItem *buff_item; 
	pthread_mutex_lock(&(log_buffer->read_mutex));
	while ((*read_idx) != (*write_idx))
	{
		buff_item = &(log_buffer->buffer[*read_idx]);
		if (buff_item->message_type == ERROR_MSG)
		{
			printf("ERROR: %llu: %s: %s: %s: %s\n", buff_item->time, buff_item->process, buff_item->module, buff_item->function, buff_item->message);
		}
		else if (buff_item->message_type == WARNING_MSG)
		{
			printf("WARNING: %llu: %s: %s: %s: %s\n", buff_item->time, buff_item->process, buff_item->module, buff_item->function, buff_item->message);
		}
		else if (buff_item->message_type == INFO_MSG)
		{
			printf("INFO: %llu: %s: %s: %s: %s\n", buff_item->time, buff_item->process, buff_item->module, buff_item->function, buff_item->message);
		}	
		else if (buff_item->message_type == BUG_MSG)
		{
			printf("BUG: %llu: %s: %s: %s: %s\n", buff_item->time, buff_item->process, buff_item->module, buff_item->function, buff_item->message);
		} 
		else
		{
			pthread_mutex_unlock(&(log_buffer->read_mutex));
			return print_message(BUG_MSG, "BlueSpike", "MessageLogBuffer", "print_message_log_buffer", "Invalid message type");
		}
		if ((*read_idx + 1) == log_buffer->buffer_size)
			*read_idx = 0;
		else
			(*read_idx)++;
		item_printed++;
		if (item_printed == max_item_num_to_print)
			break;
	}
	pthread_mutex_unlock(&(log_buffer->read_mutex));
	return TRUE;
}
