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


