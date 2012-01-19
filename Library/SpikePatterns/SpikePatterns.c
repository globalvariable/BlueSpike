#include "SpikePatterns.h"


SpikePatterns* allocate_spike_patterns(SpikePatterns* spike_patterns)
{
	if (spike_patterns == NULL)
	{
		spike_patterns = g_new0(SpikePatterns, 1);
		if (spike_patterns == NULL)
		{
			print_message(ERROR_MSG ,"BlueSpike", "SpikePatterns", "allocate_spike_patterns", "Couldn' t allocete spike_patterns\n");
			return NULL;
		}
		print_message(INFO_MSG ,"BlueSpike", NULL, NULL, "Created Spike_Patterns\n");	
		return spike_patterns;
	}
	else		
	{
		spike_patterns = deallocate_spike_patterns(spike_patterns);
		spike_patterns = allocate_spike_patterns(spike_patterns);
		return spike_patterns;			
	}
}

SpikePatterns* deallocate_spike_patterns(SpikePatterns* spike_patterns)
{
	int i;
	for (i = 0; i < spike_patterns->num_of_patterns; i++)
	{
		g_free(spike_patterns->patterns[i].spikes);
	}	
	g_free(spike_patterns->patterns);
	g_free(spike_patterns);
	print_message(INFO_MSG ,"BlueSpike", NULL, NULL, "Destroyed Spike_Patterns\n");	
	return NULL;	
}


bool add_spike_time_stamp_to_spike_pattern(SpikePatterns *spike_patterns, int pattern_num, SpikeTimeStampItem *spike_time_stamp)
{
	int i;
	SpikePattern *lcl_patterns = NULL;
	SpikeTimeStampItem *lcl_spikes = NULL;
	
	if (spike_patterns == NULL)
		return print_message(ERROR_MSG ,"BlueSpike", "SpikePatterns", "add_spike_time_stamp_to_spike_pattern", "SpikePatterns *spike_patterns was not allocated\n");
	

	if (pattern_num > spike_patterns->num_of_patterns)    //Invalid pattern num to incrememnt num of patterns
	{
		return print_message(ERROR_MSG ,"BlueSpike", "SpikePatterns", "add_spike_time_stamp_to_spike_pattern", "pattern number cannot be larger than current number of patterns.\n");			
	}
	else if (pattern_num < 0)    //Invalid pattern num to incrememnt num of patterns
	{
		return print_message(ERROR_MSG ,"BlueSpike", "SpikePatterns", "add_spike_time_stamp_to_spike_pattern", "pattern number cannot be smaller than ZERO.\n");			
	}	
	else if (pattern_num == spike_patterns->num_of_patterns)    //Valid pattern num to incrememnt num of patterns
	{
		lcl_patterns = g_new0 (SpikePattern, spike_patterns->num_of_patterns+1);
		if (lcl_patterns == NULL)
			return print_message(ERROR_MSG ,"BlueSpike", "SpikePatterns", "add_spike_time_stamp_to_spike_pattern", "Couldn' t allocate lcl_patterns.\n");	
		
		for (i = 0; i < spike_patterns->num_of_patterns; i++)
		{
			lcl_patterns[i] = spike_patterns->patterns[i];
		}
		g_free(spike_patterns->patterns);
		spike_patterns->patterns = lcl_patterns;
		spike_patterns->num_of_patterns++;

		spike_patterns->patterns[pattern_num].spikes = g_new0(SpikeTimeStampItem, 1);
		if (spike_patterns->patterns[pattern_num].spikes == NULL)
			return print_message(ERROR_MSG ,"BlueSpike", "SpikePatterns", "add_spike_time_stamp_to_spike_pattern", "Couldn' t allocate spike_patterns->patterns.spikes.\n");		
				
		spike_patterns->patterns[pattern_num].spikes[0].peak_time = spike_time_stamp->peak_time;               // first spike, no sorting required
		spike_patterns->patterns[pattern_num].spikes[0].mwa_or_layer = spike_time_stamp->mwa_or_layer;		
		spike_patterns->patterns[pattern_num].spikes[0].channel_or_neuron_group = spike_time_stamp->channel_or_neuron_group;
		spike_patterns->patterns[pattern_num].spikes[0].unit_or_neuron = spike_time_stamp->unit_or_neuron;						
		spike_patterns->patterns[pattern_num].num_of_spikes++;
		
		return TRUE;
	}
	else
	{
		lcl_spikes = g_new0 (SpikeTimeStampItem, spike_patterns->patterns[pattern_num].num_of_spikes+1);
		if (lcl_spikes == NULL)
			return print_message(ERROR_MSG ,"BlueSpike", "SpikePatterns", "add_spike_time_stamp_to_spike_pattern", "Couldn' t allocate lcl_spikes.\n");	
		
		for (i = 0; i < spike_patterns->patterns[pattern_num].num_of_spikes; i++)
		{
			lcl_spikes[i] = spike_patterns->patterns[pattern_num].spikes[i];
		}
		g_free(spike_patterns->patterns[pattern_num].spikes);
		spike_patterns->patterns[pattern_num].spikes = lcl_spikes;
		spike_patterns->patterns[pattern_num].num_of_spikes++;
		
		// sort spike time

		while(1) 
		{
			i --;
			if (i <0)
			{
				spike_patterns->patterns[pattern_num].spikes[0].peak_time = spike_time_stamp->peak_time;
				spike_patterns->patterns[pattern_num].spikes[0].mwa_or_layer = spike_time_stamp->mwa_or_layer;		
				spike_patterns->patterns[pattern_num].spikes[0].channel_or_neuron_group = spike_time_stamp->channel_or_neuron_group;
				spike_patterns->patterns[pattern_num].spikes[0].unit_or_neuron = spike_time_stamp->unit_or_neuron;		
				break;
			}			
			if (spike_time_stamp->peak_time < spike_patterns->patterns[pattern_num].spikes[i].peak_time)		// push item to the next index
			{
				spike_patterns->patterns[pattern_num].spikes[i+1].peak_time = spike_patterns->patterns[pattern_num].spikes[i].peak_time;
				spike_patterns->patterns[pattern_num].spikes[i+1].mwa_or_layer = spike_patterns->patterns[pattern_num].spikes[i].mwa_or_layer;		
				spike_patterns->patterns[pattern_num].spikes[i+1].channel_or_neuron_group = spike_patterns->patterns[pattern_num].spikes[i].channel_or_neuron_group;
				spike_patterns->patterns[pattern_num].spikes[i+1].unit_or_neuron = spike_patterns->patterns[pattern_num].spikes[i].unit_or_neuron;					
			}
			else
			{
				spike_patterns->patterns[pattern_num].spikes[i+1].peak_time = spike_time_stamp->peak_time;
				spike_patterns->patterns[pattern_num].spikes[i+1].mwa_or_layer = spike_time_stamp->mwa_or_layer;		
				spike_patterns->patterns[pattern_num].spikes[i+1].channel_or_neuron_group = spike_time_stamp->channel_or_neuron_group;
				spike_patterns->patterns[pattern_num].spikes[i+1].unit_or_neuron = spike_time_stamp->unit_or_neuron;		
				break;
			}
		} 
		return TRUE;			
	}
}



