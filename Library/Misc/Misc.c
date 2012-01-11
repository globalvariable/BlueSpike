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

bool is_allocated(void* data, bool print_message, char *module, char *function, int message_type, char *message)
{
	if (data == NULL)
	{
		if (print_message)
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
			else 
			{
				printf("%s : %s : BUG: Given data was not allocated\n", module, function);
				printf("Message: %s\n", message);						
			}
		}
		return FALSE;
	}
	else
	{
		if (print_message)
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
			else 
			{
				printf("%s : %s : BUG: Given data was allocated\n", module, function);
				printf("Message: %s\n", message);						
			}
		}	
		return TRUE;
	}
}

