#include "TrialStats.h"





TrialMainStats* allocate_main_trial_stats(TrialMainStats* main_stats, unsigned int num_of_trials_to_allocate)
{
	if (main_stats != NULL)
	{
		main_stats = deallocate_main_trial_stats(main_stats);
		main_stats = allocate_main_trial_stats(main_stats, num_of_trials_to_allocate);
		return main_stats;
	}	
	
	main_stats = g_new0(TrialMainStats, 1);
	print_message(INFO_MSG ,"BlueSpike", "TrialStats", "allocate_main_trial_stats", "Created main_trial_stats.");
	main_stats->trial_starts_ns = g_new0(TimeStamp, num_of_trials_to_allocate);
	main_stats->trial_durations_ns = g_new0(TimeStamp, num_of_trials_to_allocate);
	main_stats->num_of_trials_allocated = num_of_trials_to_allocate;

	return main_stats;
}

TrialMainStats* deallocate_main_trial_stats(TrialMainStats* main_stats)
{
	if (main_stats == NULL)
	{
		print_message(ERROR_MSG ,"BlueSpike", "TrialStats", "deallocate_main_trial_stats", "main_stats is already NULL");	
		return NULL;	
	}

	g_free(main_stats->trial_starts_ns);
	g_free(main_stats->trial_durations_ns);	
	g_free(main_stats);
	
	print_message(INFO_MSG ,"BlueSpike", "TrialStats", "deallocate_main_trial_stats", "Destroyed main_stats.");	
	return NULL;	
}

bool reset_main_trial_stats_write_idx(TrialMainStats* main_stats)
{
	if (main_stats == NULL)
		return print_message(ERROR_MSG ,"BlueSpike", "TrialStats", "reset_main_trial_stats_write_idx", "main_stats was not allocated.");
	main_stats->num_of_trials_used = 0;
	return TRUE;
}

bool write_to_main_trial_stats(TrialMainStats* main_stats, TimeStamp trial_start_ns , TimeStamp trial_duration_ns)
{
	if (main_stats == NULL)
		return print_message(ERROR_MSG ,"BlueSpike", "TrialStats", "write_to_main_trial_stats", "main_stats was not allocated\n");
	
	if (main_stats->num_of_trials_used == main_stats->num_of_trials_allocated)
		return print_message(ERROR_MSG ,"BlueSpike", "TrialStats", "write_to_main_trial_stats", "main_stats->num_of_trials_used == main_stats->num_of_trials_allocated");	

	main_stats->trial_starts_ns[main_stats->num_of_trials_used] = trial_start_ns;
	main_stats->trial_durations_ns[main_stats->num_of_trials_used] = trial_duration_ns;
	main_stats->num_of_trials_used++;
	return TRUE;
}


