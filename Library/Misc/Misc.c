#include "Misc.h"


int get_word_in_line(char separator, int word_order, char *word, char *line, bool print_warning)
{
	int i, j, start_idx, word_char_count = 0, word_count = 0, char_idx = 0;
	int len = strlen(line);
	
	for (i = 0; i < len; i++)
	{
		
		if ((line[i] == separator) || (line[i] == 0) || (line[i] == '\n'))
		{
			if (word_count == word_order)
			{
				start_idx = i - word_char_count;
				for (j = start_idx; j < i; j++)
				{
					word[char_idx] = line[j];
					char_idx++;
				}
				word[char_idx] = 0;   // end with null
				return 1;
			}
			word_count++;
			word_char_count = 0;
		}
		else
		{
			word_char_count++;
		}
	}
	if (print_warning)
		printf ("%dth word couldn't be found in line\n", word_order);

	return 0; // couldnt find that word
}

bool is_allocated(void* data, bool print_message, char *module, char *function, int message_type, char *message, bool expects_allocated)
{
	if (data == NULL)
	{
		if (print_message  && expects_allocated)
		{
			if (message_type == ALLOCATION_FAILED)
			{
				printf("%s : %s : ERROR: Given data was not allocated\n", module, function);
				printf("Message: %s\n", message);
			}
			else if (message_type == ALLOCATION_WARNING)
			{
				printf("%s : %s : WARNING: Given data was not allocated\n", module, function);
				printf("Message: %s\n", message);				
			}
			else if (message_type == ALLOCATION_BUG)
			{
				printf("%s : %s : BUG: Given data was not allocated\n", module, function);
				printf("Message: %s\n", message);						
			}
			else
			{
				printf("%s : %s : BUG: Inconvenient message type %d for Misc: is_allocated()\n", module, function, message_type);
				printf("Message: %s\n", message);						
			}			
		}
		return FALSE;
	}
	else 
	{
		if (print_message && (!expects_allocated))
		{
			if (message_type == ALLOCATION_FAILED)
			{
				printf("%s : %s : ERROR: Given data was allocated\n", module, function);
				printf("Message: %s\n", message);
			}
			else if (message_type == ALLOCATION_WARNING)
			{
				printf("%s : %s : WARNING: Given data was allocated\n", module, function);
				printf("Message: %s\n", message);				
			}
			else if (message_type == ALLOCATION_BUG)
			{
				printf("%s : %s : BUG: Given data was allocated\n", module, function);
				printf("Message: %s\n", message);						
			}
			else
			{
				printf("%s : %s : BUG: Inconvenient message type %d for Misc: is_allocated()\n", module, function, message_type);
				printf("Message: %s\n", message);						
			}				
		}	
		return TRUE;
	}
}


bool print_message(int message_type, char* program, char *module, char *function, char *message)
{
	if (message_type == ERROR_MSG)
	{
		printf("ERROR: %s: %s: %s: %s\n", program, module, function, message);
		return FALSE;		
	}
	else if (message_type == WARNING_MSG)
	{
		printf("WARNING: %s: %s: %s: %s\n", program, module, function, message);
		return TRUE;
	}
	else if (message_type == INFO_MSG)
	{
		printf("INFO: %s: %s: %s: %s\n", program, module, function, message);
		return TRUE;		
	}	
	else if (message_type == BUG_MSG)
	{
		printf("BUG: %s: %s: %s: %s\n", program, module, function, message);
		return FALSE;				
	} 
	else
	{
		printf("BUG: %s: %s: %s: Inconvenient message type (%d) for Misc: print_message\n", program, module, function, message_type);
		return FALSE;				
	}

}

bool is_buffer_getting_full(unsigned int buffer_read_idx,  unsigned int buffer_write_idx, unsigned int buffer_size, bool printf_message)	// check in every period of buffer reader/handler routines.
{
	if (buffer_read_idx > buffer_write_idx) 
	{
		if ((buffer_size - buffer_read_idx + buffer_write_idx) > (0.5 * buffer_size))	// check number of unread buffer items
		{
			if (printf_message)
				print_message(WARNING_MSG ,"BlueSpike", "Misc", "is_buffer_getting_full", "Buffer is getting full.");		
			return TRUE;
		}
	}
	else
	{
		if ((buffer_write_idx - buffer_read_idx) > (0.5 * buffer_size)) 		// check number of unread buffer items
		{
			if (printf_message)
				print_message(WARNING_MSG ,"BlueSpike", "Misc", "is_buffer_getting_full", "Buffer is getting full.");		
			return TRUE;
		}
	}	
	return FALSE;
}

unsigned int get_size_of_binary_file(FILE *fp)
{
	fseek(fp, 0L, SEEK_END);
	return ftell(fp);
}

bool get_num_of_items_in_binary_file(FILE *fp, unsigned int size_of_item, unsigned int *num_of_items)
{
	unsigned int size_bin;
	fseek(fp, 0L, SEEK_END);
	size_bin = ftell(fp);	
	if (size_bin%size_of_item)	
		return print_message(ERROR_MSG, "BlueSpike", "Misc", "get_num_of_items_in_binary_file", "Size of binary file - size of file item mismatch.");
	*num_of_items = size_bin/size_of_item;
	return TRUE;
}
