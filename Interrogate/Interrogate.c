#include "Interrogate.h"


bool interrogate_mapping(void)
{
	int i, j, check = 0;
	
	for (i = 0; i<MAX_NUM_OF_DAQ_CARD; i++)
	{	
		for (j = 0; j<MAX_NUM_OF_CHANNEL_PER_DAQ_CARD; j++)
		{	
			if ((shared_memory->daq_mwa_map[i][j].mwa == MAX_NUM_OF_MWA) && (shared_memory->daq_mwa_map[i][j].channel == MAX_NUM_OF_CHAN_PER_MWA))
			{
				printf ("DAQ: %d\tChannel: %d\t----> MWA: Idle\tChannel: Idle\n", i, j);
			}
			else if ((shared_memory->daq_mwa_map[i][j].mwa != MAX_NUM_OF_MWA) && (shared_memory->daq_mwa_map[i][j].channel == MAX_NUM_OF_CHAN_PER_MWA))
			{
				printf ("DAQ: %d\tChannel: %d\t----> MWA: %d\tChannel: Idle\n", i, j, shared_memory->daq_mwa_map[i][j].mwa);			
				printf ("****************************************************************\n");			
				printf ("*************************BUG*********************************\n");
				printf ("BUG: MWA was not mapped but its channel was mapped\n");
				printf ("*************************BUG*********************************\n");
				printf ("****************************************************************\n");	
				return 0;					
			}
			else if ((shared_memory->daq_mwa_map[i][j].mwa == MAX_NUM_OF_MWA) && (shared_memory->daq_mwa_map[i][j].channel != MAX_NUM_OF_CHAN_PER_MWA))
			{
				printf ("DAQ: %d\tChannel: %d\t----> MWA: Idle\tChannel: %d\n", i, j, shared_memory->daq_mwa_map[i][j].channel);			
				printf ("*******************************************************************\n");			
				printf ("*************************BUG************************************\n");
				printf ("BUG: MWA was mapped but its channel was not mapped\n");
				printf ("*************************BUG************************************\n");
				printf ("******************************************************************\n");	
				return 0;					
			}			
			else
			{
				printf ("DAQ: %d\tChannel: %d\t----> MWA: %d\tChannel: %d\n", i, j, shared_memory->daq_mwa_map[i][j].mwa, shared_memory->daq_mwa_map[i][j].channel);
				check =1;
			}
		}
	}

	printf("---------------------------------------------------------------------------------------------\n");						
	for (i = 0; i<MAX_NUM_OF_MWA; i++)
	{	
		for (j = 0; j<MAX_NUM_OF_CHAN_PER_MWA; j++)
		{	
			if ((shared_memory->mwa_daq_map[i][j].daq_card == MAX_NUM_OF_DAQ_CARD) && (shared_memory->mwa_daq_map[i][j].daq_chan == MAX_NUM_OF_CHANNEL_PER_DAQ_CARD))
			{
				printf ("MWA: %d\tChannel: %d\t----> DAQ: Idle\tChannel: Idle\n", i, j);
			}
			else if ((shared_memory->mwa_daq_map[i][j].daq_card != MAX_NUM_OF_DAQ_CARD) && (shared_memory->mwa_daq_map[i][j].daq_chan == MAX_NUM_OF_CHANNEL_PER_DAQ_CARD))
			{
				printf ("DAQ: %d\tChannel: %d\t----> DAQ: %d\tChannel: Idle\n", i, j,shared_memory->mwa_daq_map[i][j].daq_card);			
				printf ("****************************************************************\n");			
				printf ("*************************BUG*********************************\n");
				printf ("BUG: DAQ Card was not mapped but its channel was mapped\n");
				printf ("*************************BUG*********************************\n");
				printf ("****************************************************************\n");	
				return 0;					
			}
			else if ((shared_memory->mwa_daq_map[i][j].daq_card == MAX_NUM_OF_DAQ_CARD) && (shared_memory->mwa_daq_map[i][j].daq_chan != MAX_NUM_OF_CHANNEL_PER_DAQ_CARD))
			{
				printf ("DAQ: %d\tChannel: %d\t----> DAQ: Idle\tChannel: %d\n", i, j, shared_memory->mwa_daq_map[i][j].daq_chan);			
				printf ("*******************************************************************\n");			
				printf ("*************************BUG************************************\n");
				printf ("BUG: DAQ Card was mapped but its channel was not mapped\n");
				printf ("*************************BUG************************************\n");
				printf ("******************************************************************\n");	
				return 0;					
			}		
			else
			{
				printf("MWA: %d\tChannel: %d\t----> DAQ: %d\tChannel: %d\n", i, j, shared_memory->mwa_daq_map[i][j].daq_card , shared_memory->mwa_daq_map[i][j].daq_chan );	
			}
		}
	}
	if (check == 1)
	{
		return 1;
	}
	else
	{
		printf("ERROR: None of the channels of any DAQ Card was mapped\n");
		return 0;
	}	
}
