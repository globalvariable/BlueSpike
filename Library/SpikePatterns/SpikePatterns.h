#ifndef SPIKE_PATTERNS
#define SPIKE_PATTERNS



typedef struct __SpikePattern SpikePattern;
typedef struct __SpikePatterns SpikePatterns;

#include "../../SpikeTimeStamp.h"
#include "../Misc/Misc.h"
#include <stdbool.h>
#include <gtk/gtk.h>

struct __SpikePatterns
{
	SpikePattern *patterns;
	int num_of_patterns;
};

struct __SpikePattern
{
	SpikeTimeStampItem *spikes;
	int num_of_spikes;
};



SpikePatterns* allocate_spike_patterns(SpikePatterns* spike_patterns);
SpikePatterns* deallocate_spike_patterns(SpikePatterns* spike_patterns);
bool add_spike_time_stamp_to_spike_pattern(SpikePatterns *spike_patterns, int pattern_num, SpikeTimeStampItem *spike_time_stamp);














#endif
