#ifndef FIRING_RATE_H
#define FIRING_RATE_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>
#include "../../TimeStamp.h"
#include "../../SpikeTimeStamp.h"

typedef struct __FiringRateBinItem
{
	double 		mean;				
	double 		variance;		
} FiringRateBinItem;

typedef struct __NeuronFiringRate
{
	FiringRateBinItem 	*train_bins;
	double			*num_of_spikes_in_pattern;
	double			mean_num_of_spikes_per_neuron;	
	double			variance_of_num_of_spikes_per_neuron;		
} NeuronFiringRate;


/// Functions
bool firing_rate_submit_number_of_mwas(int num_of_mwas);
bool firing_rate_submit_number_of_channels_in_mwa(int mwa_num, int num_of_channels);
bool firing_rate_submit_number_of_units_in_mwa_channel(int mwa_num, int channel_num, int num_of_units);
bool firing_rate_submit_number_of_patterns (int num_of_patterns);
int firing_rate_allocate_bins(TimeStamp pattern_length, TimeStamp bin_size);
void firing_rate_clear_all_statistics(void);
void firing_rate_deallocate_all(void);
bool firing_rate_add_spike_to_statistics(TimeStamp pattern_start_time, TimeStamp pattern_end_time, SpikeTimeStampItem spike, int pattern_num);
bool calculate_firing_rate_statistics(void);

NeuronFiringRate* get_neuron_firing_rate_statistics(int mwa, int channel, int unit);


#endif
