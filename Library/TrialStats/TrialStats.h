#ifndef TRIAL_STATS
#define TRIAL_STATS



typedef struct __TrialMainStats TrialMainStats;


#include "../Misc/Misc.h"
#include <stdbool.h>
#include <gtk/gtk.h>
#include "../../TimeStamp.h"

struct __TrialMainStats
{
	TimeStamp	*trial_starts_ns;   // num_of_trials
	TimeStamp	*trial_durations_ns;   // num_of_trials
	unsigned int 	num_of_trials_allocated;
	unsigned int 	num_of_trials_used;	
};


TrialMainStats* allocate_main_trial_stats(TrialMainStats* main_stats, unsigned int num_of_trials_to_allocate);
TrialMainStats* deallocate_main_trial_stats(TrialMainStats* main_stats);
bool write_to_main_trial_stats(TrialMainStats* main_stats, TimeStamp trial_start_ns , TimeStamp trial_duration_ns);
bool reset_main_trial_stats_write_idx(TrialMainStats* main_stats);


#endif
