#include "PatternData.h"

PatternData* allocate_pattern_data(PatternData* pattern_data)
{
	if (pattern_data == NULL)
	{
		pattern_data = g_new0(PatternData, 1);
		if (pattern_data == NULL)
		{
			print_message(ERROR_MSG ,"BlueSpike", "PatternData", "allocate_pattern_data", "Couldn' t allocete pattern_data\n");
			return NULL;
		}
		print_message(INFO_MSG ,"BlueSpike", NULL, NULL, "Created Pattern Data.");	
		return pattern_data;
	}
	else		
	{
		pattern_data = deallocate_pattern_data(pattern_data);
		pattern_data = allocate_pattern_data(pattern_data);
		return pattern_data;			
	}
}

PatternData* deallocate_pattern_data(PatternData* pattern_data)
{
	g_free(pattern_data->pattern_length_ms);
	g_free(pattern_data);
	print_message(INFO_MSG ,"BlueSpike", NULL, NULL, "Destroyed Pattern Data.");	
	return NULL;	
}

bool add_pattern_to_pattern_data(PatternData* pattern_data, TimeStampMs pattern_length_ms)
{
	int i;
	TimeStampMs	*lcl_pattern_length_ms = NULL;
	
	if (pattern_data == NULL)
		return print_message(ERROR_MSG ,"BlueSpike", "PatternData", "add_pattern_to_pattern_data", "patern_data was not allocated.");	
		
	lcl_pattern_length_ms = g_new0 (TimeStampMs, pattern_data->num_of_patterns+1);	
	if (lcl_pattern_length_ms == NULL)
		return print_message(ERROR_MSG ,"BlueSpike", "PatternData", "add_pattern_to_pattern_data", "Couldn't allocate lcl_pattern_length_ms.");		
		
	for (i = 0; i < pattern_data->num_of_patterns; i++ )
	{
		lcl_pattern_length_ms[i] = pattern_data->pattern_length_ms[i];
	}
	lcl_pattern_length_ms[i] = pattern_length_ms;
	g_free(pattern_data->pattern_length_ms);
	pattern_data->pattern_length_ms = lcl_pattern_length_ms;
	pattern_data->num_of_patterns++;
	return TRUE;
}

bool get_num_of_patterns(PatternData* pattern_data, int *num_of_patterns)
{
	*num_of_patterns = 0;
	if (pattern_data == NULL)
		return print_message(ERROR_MSG ,"BlueSpike", "PatternData", "get_num_of_patterns", "patern_data was not allocated.");
		
	*num_of_patterns = pattern_data-> num_of_patterns;		
	return TRUE;		
}

bool get_pattern_length_ms(PatternData* pattern_data, int pattern_num, TimeStampMs *pattern_length_ms)
{
	*pattern_length_ms = 0;
	if (pattern_data == NULL)
		return print_message(ERROR_MSG ,"BlueSpike", "PatternData", "get_pattern_length_ms", "patern_data was not allocated.");
	if (pattern_num < 0)
		return print_message(ERROR_MSG ,"BlueSpike", "PatternData", "get_pattern_length_ms", "pattern_num is smaller than ZERO.");		
	if (pattern_num >= pattern_data-> num_of_patterns)
		return print_message(ERROR_MSG ,"BlueSpike", "PatternData", "get_pattern_length_ms", "pattern_num is too large.");
		
	*pattern_length_ms = pattern_data->pattern_length_ms[pattern_num];
	return TRUE;		
				
}
