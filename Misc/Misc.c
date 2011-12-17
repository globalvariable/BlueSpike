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

bool interrogate_mapping(void)
{
	int i, j, check = 0;
	
	for (i = 0; i<MAX_NUM_OF_DAQ_CARD; i++)
	{	
		for (j = 0; j<MAX_NUM_OF_CHANNEL_PER_DAQ_CARD; j++)
		{	
			if ((shared_memory->daq_mwa_map[i][j].mwa == MAX_NUM_OF_MWA) && (shared_memory->daq_mwa_map[i][j].channel == MAX_NUM_OF_CHAN_PER_MWA))
			{
				printf ("DAQ: %d   Channel: %d  ----> MWA: Idle  Channel: Idle\n", i, j);
			}
			else if ((shared_memory->daq_mwa_map[i][j].mwa != MAX_NUM_OF_MWA) && (shared_memory->daq_mwa_map[i][j].channel == MAX_NUM_OF_CHAN_PER_MWA))
			{
				printf ("DAQ: %d   Channel: %d  ----> MWA: %d  Channel: Idle\n", i, j, shared_memory->daq_mwa_map[i][j].mwa);			
				printf ("****************************************************************\n");			
				printf ("*************************BUG*********************************\n");
				printf ("BUG: MWA was not mapped but its channel was mapped\n");
				printf ("*************************BUG*********************************\n");
				printf ("****************************************************************\n");	
				return 0;					
			}
			else if ((shared_memory->daq_mwa_map[i][j].mwa == MAX_NUM_OF_MWA) && (shared_memory->daq_mwa_map[i][j].channel != MAX_NUM_OF_CHAN_PER_MWA))
			{
				printf ("DAQ: %d   Channel: %d  ----> MWA: Idle Channel: %d\n", i, j, shared_memory->daq_mwa_map[i][j].channel);			
				printf ("*******************************************************************\n");			
				printf ("*************************BUG************************************\n");
				printf ("BUG: MWA was mapped but its channel was not mapped\n");
				printf ("*************************BUG************************************\n");
				printf ("******************************************************************\n");	
				return 0;					
			}			
			else
			{
				printf ("DAQ: %d   Channel: %d  ----> MWA: %d   Channel: %d\n", i, j, shared_memory->daq_mwa_map[i][j].mwa, shared_memory->daq_mwa_map[i][j].channel);
				check =1;
			}
		}
	}
	if (check == 1)
	{
		printf("---------------------------------------------------------------------------------------------\n");						
		for (i = 0; i<MAX_NUM_OF_MWA; i++)
		{	
			for (j = 0; j<MAX_NUM_OF_CHAN_PER_MWA; j++)
			{	
				printf("MWA: %d   Channel: %d  ----> DAQ Card: %d   Channel: %d\n", i, j, shared_memory->mwa_daq_map[i][j].daq_card , shared_memory->mwa_daq_map[i][j].daq_chan );	
			}
		}
		return 1;
	}
	else
	{
		printf("ERROR: None of the channels of any DAQ Card was mapped\n");
		return 0;
	}	
}
