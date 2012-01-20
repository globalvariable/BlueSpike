#ifndef PATTERN_DATA_H
#define PATTERN_DATA_H

typedef struct 	__PatternData 	PatternData;

#include "../Misc/Misc.h"
#include "../../TimeStamp.h"
#include <stdbool.h>
#include <gtk/gtk.h>


struct __PatternData
{
	TimeStampMs	*pattern_length_ms;
	int 			num_of_patterns;
};


PatternData* allocate_pattern_data(PatternData* pattern_data);
PatternData* deallocate_pattern_data(PatternData* pattern_data);
bool add_pattern_to_pattern_data(PatternData* pattern_data, TimeStampMs pattern_length_ms);
bool get_num_of_patterns(PatternData* pattern_data, int *num_of_patterns);
bool get_pattern_length_ms(PatternData* pattern_data, int pattern_num, TimeStampMs *pattern_length_ms);
#endif
