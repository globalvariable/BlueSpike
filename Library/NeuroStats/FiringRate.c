#include "FiringRate.h"

static int num_of_all_mwas = 0;		// num_of_mwas_or_layers * num_of_channels_or_nrn_groups * num_ofunits_or_neurons *
static int *num_of_channels_in_mwa = NULL;		// num_of_mwas_or_layers * num_of_channels_or_nrn_groups * num_ofunits_or_neurons *
static int **num_of_units_in_channel = NULL;		// num_of_mwas_or_layers * num_of_channels_or_nrn_groups * num_ofunits_or_neurons *

static int num_of_all_bins = 0;		

static int num_of_all_patterns = 0;

static TimeStamp pattern_length_ns = 0;	
static TimeStamp bin_size_ns = 0;		

static FiringRateTrain ***firing_rate_statistics = NULL;	// num_of_mwas_or_layers * num_of_channels_or_nrn_groups * num_of_units_or_neurons

static double ****sum_of_spikes_in_bin = NULL; 	// num_of_mwas_or_layers * num_of_channels_or_nrn_groups * num_of_units_or_neurons * num_of_all_bins
static double *****sum_of_spikes_in_bin_in_pattern = NULL; 	// num_of_mwas_or_layers * num_of_channels_or_nrn_groups * num_of_units_or_neurons * num_of_all_bins * num_of_patterns;

static double ***sum_of_spikes_per_neuron = NULL; 	// num_of_mwas_or_layers * num_of_channels_or_nrn_groups * num_of_units_or_neurons 
static double ****sum_of_spikes_per_neuron_per_pattern = NULL; 	// num_of_mwas_or_layers * num_of_channels_or_nrn_groups * num_of_units_or_neurons * num_of_patterns;

static bool calculate_firing_rate_means(void);
static bool calculate_firing_rate_variances(void);

bool firing_rate_submit_number_of_mwas(int num_of_mwas)
{
	if (num_of_all_mwas !=0)
	{
		printf ("FiringRate: BUG: Number of all MWAs was already submitted.\n"); 
		return FALSE;		
	
	}
	num_of_all_mwas  = num_of_mwas;
	num_of_channels_in_mwa = g_new0(int,num_of_all_mwas);	
	if (num_of_channels_in_mwa  == NULL)
	{
		printf ("FiringRate: ERROR: Couldn' t allocate Number of Channels in MWAs.\n");
		return FALSE;		
	
	}	
	num_of_units_in_channel = g_new0(int*,num_of_all_mwas);	
	if (num_of_units_in_channel  == NULL)
	{
		printf ("FiringRate: ERROR: Couldn' t allocate Number of Units in Channel.\n");
		return FALSE;		
	}	
	return TRUE;
}
bool firing_rate_submit_number_of_channels_in_mwa(int mwa_num, int num_of_channels)
{
	if (num_of_channels_in_mwa[mwa_num] != 0)
	{
		printf ("FiringRate: BUG: Number of Channels in %d was already submitted.\n", mwa_num);
		return FALSE;
	}
	num_of_channels_in_mwa[mwa_num] = num_of_channels;
	num_of_units_in_channel[mwa_num] = g_new0(int,num_of_channels);
	if (num_of_units_in_channel[mwa_num] == NULL)
	{
		printf ("FiringRate: ERROR: Couldn' t allocate Number of Units in Channel\n");
		return FALSE;	
	}
	return TRUE;
}
bool firing_rate_submit_number_of_units_in_mwa_channel(int mwa_num, int channel_num, int num_of_units)
{
	if (num_of_units_in_channel[mwa_num][channel_num] != 0)
	{
		printf ("FiringRate: BUG: Number of Units in MWA: %d Channel: %d was already submitted.\n", mwa_num, channel_num);
		return FALSE;
	}
	num_of_units_in_channel[mwa_num][channel_num] = num_of_units;
	return TRUE;
}
bool firing_rate_submit_number_of_patterns (int num_of_patterns)
{
	num_of_all_patterns = num_of_patterns;	
	return TRUE;	
}

int firing_rate_allocate_bins(TimeStamp pattern_length, TimeStamp bin_size)
{
	int i,j, k, m;
	num_of_all_bins = pattern_length / bin_size;
	if (num_of_all_bins <= 0)
	{
		printf ("FiringRate: ERROR: Number of Bins cannot be smaller than or equal to zero.\n");
		return -1;		
	}
	pattern_length_ns = pattern_length;
	bin_size_ns = bin_size;
	if (firing_rate_statistics != NULL)
	{
		printf ("FiringRate: BUG: Firing Rate Statistics was not free' d before.\n");
		return -1;	
	}
	firing_rate_statistics = g_new0(FiringRateTrain **, num_of_all_mwas);
	if (firing_rate_statistics == NULL)
	{
		printf ("FiringRate: ERROR: Couldn' t allocate firing_rate_statistics.\n");
		return -1;	
	}	
	if (sum_of_spikes_in_bin != NULL)
	{
		printf ("FiringRate: BUG: sum_of_spikes_in_bin was not free' d before.\n");
		return -1;	
	}
	sum_of_spikes_in_bin = g_new0(double ***, num_of_all_mwas);
	if (sum_of_spikes_in_bin == NULL)
	{
		printf ("FiringRate: ERROR: Couldn' t allocate sum_of_spikes_in_bin.\n");
		return -1;	
	}
	if (sum_of_spikes_in_bin_in_pattern != NULL)
	{
		printf ("FiringRate: BUG: sum_of_spikes_in_bin_in_pattern was not free' d before.\n");
		return -1;	
	}
	sum_of_spikes_in_bin_in_pattern = g_new0(double ****, num_of_all_mwas);
	if (sum_of_spikes_in_bin_in_pattern == NULL)
	{
		printf ("FiringRate: ERROR: Couldn' t allocate sum_of_spikes_in_bin_in_pattern.\n");
		return -1;	
	}	
	if (sum_of_spikes_per_neuron != NULL)
	{
		printf ("FiringRate: BUG: sum_of_spikes_per_neuron was not free' d before.\n");
		return -1;	
	}
	sum_of_spikes_per_neuron = g_new0(double **, num_of_all_mwas);
	if (sum_of_spikes_per_neuron  == NULL)
	{
		printf ("FiringRate: ERROR: Couldn' t allocate sum_of_spikes_per_neuron .\n");
		return -1;	
	}	
	if (sum_of_spikes_per_neuron_per_pattern != NULL)
	{
		printf ("FiringRate: BUG: sum_of_spikes_per_neuron_per_pattern was not free' d before.\n");
		return -1;	
	}
	sum_of_spikes_per_neuron_per_pattern = g_new0(double ***, num_of_all_mwas);
	if (sum_of_spikes_per_neuron_per_pattern  == NULL)
	{
		printf ("FiringRate: ERROR: Couldn' t allocate sum_of_spikes_per_neuron_per_pattern .\n");
		return -1;	
	}				
	for (i = 0; i < num_of_all_mwas; i++)
	{
		firing_rate_statistics[i] = g_new0(FiringRateTrain*, num_of_channels_in_mwa[i]);
		if (firing_rate_statistics[i] == NULL)
		{
			printf ("FiringRate: ERROR: Couldn' t allocate firing_rate_statistics[%d].\n", i);
			return -1;	
		}
		sum_of_spikes_in_bin[i] = g_new0(double**, num_of_channels_in_mwa[i]);
		if (sum_of_spikes_in_bin[i] == NULL)
		{
			printf ("FiringRate: ERROR: Couldn' t allocate sum_of_spikes_in_bin[%d].\n", i);
			return -1;	
		}	
		sum_of_spikes_in_bin_in_pattern[i] = g_new0(double***, num_of_channels_in_mwa[i]);
		if (sum_of_spikes_in_bin_in_pattern[i] == NULL)
		{
			printf ("FiringRate: ERROR: Couldn' t allocate sum_of_spikes_in_bin_in_pattern[%d].\n", i);
			return -1;	
		}		
		sum_of_spikes_per_neuron[i] = g_new0(double*, num_of_channels_in_mwa[i]);
		if (sum_of_spikes_per_neuron[i] == NULL)
		{
			printf ("FiringRate: ERROR: Couldn' t allocate sum_of_spikes_per_neuron[%d].\n", i);
			return -1;	
		}	
		sum_of_spikes_per_neuron_per_pattern[i] = g_new0(double**, num_of_channels_in_mwa[i]);
		if (sum_of_spikes_per_neuron_per_pattern[i] == NULL)
		{
			printf ("FiringRate: ERROR: Couldn' t allocate sum_of_spikes_per_neuron_per_pattern[%d].\n", i);
			return -1;	
		}								
		for (j = 0; j < num_of_channels_in_mwa[i]; j++)
		{	
			firing_rate_statistics[i][j] = g_new0(FiringRateTrain, num_of_units_in_channel[i][j]);
			if (firing_rate_statistics[i][j] == NULL)
			{
				printf ("FiringRate: ERROR: Couldn' t allocate firing_rate_statistics[%d][%d].\n", i, j);
				return -1;	
			}
			sum_of_spikes_in_bin[i][j] = g_new0(double*, num_of_units_in_channel[i][j]);
			if (sum_of_spikes_in_bin[i][j] == NULL)
			{
				printf ("FiringRate: ERROR: Couldn' t allocate sum_of_spikes_in_bin[%d][%d].\n", i, j);
				return -1;	
			}				
			sum_of_spikes_in_bin_in_pattern[i][j] = g_new0(double**, num_of_units_in_channel[i][j]);
			if (sum_of_spikes_in_bin_in_pattern[i][j] == NULL)
			{
				printf ("FiringRate: ERROR: Couldn' t allocate sum_of_spikes_in_bin_in_pattern[%d][%d].\n", i, j);
				return -1;	
			}
			sum_of_spikes_per_neuron[i][j] = g_new0(double, num_of_units_in_channel[i][j]);
			if (sum_of_spikes_per_neuron[i][j] == NULL)
			{
				printf ("FiringRate: ERROR: Couldn' t allocate sum_of_spikes_per_neuron[%d][%d].\n", i, j);
				return -1;	
			}	
			sum_of_spikes_per_neuron_per_pattern[i][j] = g_new0(double*, num_of_units_in_channel[i][j]);
			if (sum_of_spikes_per_neuron_per_pattern[i][j] == NULL)
			{
				printf ("FiringRate: ERROR: Couldn' t allocate sum_of_spikes_per_neuron_per_pattern[%d][%d].\n", i, j);
				return -1;	
			}											
			for (k = 0; k <num_of_units_in_channel[i][j]; k++)
			{
				firing_rate_statistics[i][j][k].train_bins= g_new0(FiringRateBinItem, num_of_all_bins);
				if (firing_rate_statistics[i][j][k].train_bins == NULL)
				{
					printf ("FiringRate: ERROR: Couldn' t allocate firing_rate_statistics[%d][%d][%d].train_bins.\n", i, j, k);
					return -1;	
				}	
				firing_rate_statistics[i][j][k].num_of_spikes_in_pattern = g_new0(double, num_of_all_patterns);
				if (firing_rate_statistics[i][j][k].num_of_spikes_in_pattern == NULL)
				{
					printf ("FiringRate: ERROR: Couldn' t allocate firing_rate_statistics[%d][%d][%d].train_bins.\n", i, j, k);
					return -1;	
				}
				sum_of_spikes_in_bin[i][j][k] = g_new0(double, num_of_all_bins);				
				if (sum_of_spikes_in_bin[i][j][k] == NULL)
				{
					printf ("FiringRate: ERROR: Couldn' t allocate sum_of_spikes_in_bin[%d][%d][%d].\n", i, j, k);
					return -1;	
				}
				sum_of_spikes_in_bin_in_pattern[i][j][k] = g_new0(double*, num_of_all_bins);				
				if (sum_of_spikes_in_bin_in_pattern[i][j][k] == NULL)
				{
					printf ("FiringRate: ERROR: Couldn' t allocate sum_of_spikes_in_bin_in_pattern[%d][%d][%d].\n", i, j, k);
					return -1;	
				}
				sum_of_spikes_per_neuron_per_pattern[i][j][k] = g_new0(double, num_of_all_patterns);				
				if (sum_of_spikes_per_neuron_per_pattern[i][j][k] == NULL)
				{
					printf ("FiringRate: ERROR: Couldn' t allocate sum_of_spikes_per_neuron_per_pattern[%d][%d][%d].\n", i, j, k);
					return -1;	
				}				
				for (m = 0; m <num_of_all_bins; m++)
				{
					sum_of_spikes_in_bin_in_pattern[i][j][k][m] = g_new0(double, num_of_all_patterns);				
					if (sum_of_spikes_in_bin_in_pattern[i][j][k][m]  == NULL)
					{
						printf ("FiringRate: ERROR: Couldn' t allocate sum_of_spikes_in_bin_in_pattern[%d][%d][%d][%d].\n", i, j, k, m);
						return -1;	
					}					
				}																			
			}						
		}
	}
	return num_of_all_bins;
}

void firing_rate_clear_all_statistics(void)
{
	int i,j, k, m, n;
	for (i = 0; i < num_of_all_mwas; i++)
	{
		for (j = 0; j < num_of_channels_in_mwa[i]; j++)
		{	
			for (k = 0; k <num_of_units_in_channel[i][j]; k++)
			{
				for (m = 0; m <num_of_all_bins; m++)
				{
					firing_rate_statistics[i][j][k].train_bins[m].mean= 0;
					firing_rate_statistics[i][j][k].train_bins[m].variance= 0;		
				}			
				for (m = 0; m <num_of_all_patterns; m++)
				{				
					firing_rate_statistics[i][j][k].num_of_spikes_in_pattern[m] = 0;
				}
				for (m = 0; m <num_of_all_bins; m++)
				{						
					sum_of_spikes_in_bin[i][j][k][m] = 0;
				} 
				for (m = 0; m <num_of_all_bins; m++)
				{	
					for (n = 0; n <num_of_all_bins; n++)
					{									
						sum_of_spikes_in_bin_in_pattern[i][j][k][m][n] = 0;
					}
				} 								
				sum_of_spikes_per_neuron[i][j][k] = 0;
				for (m = 0; m <num_of_all_patterns; m++)
				{						
					sum_of_spikes_per_neuron_per_pattern[i][j][k][m] = 0;
				} 				
			}						
		}
	}	
	
}

void firing_rate_deallocate_all(void)
{
	int i,j, k, m;

	for (i = 0; i < num_of_all_mwas; i++)
	{

		for (j = 0; j < num_of_channels_in_mwa[i]; j++)
		{	
			for (k = 0; k <num_of_units_in_channel[i][j]; k++)
			{
				for (m = 0; m <num_of_all_bins; m++)
				{
					g_free(sum_of_spikes_in_bin_in_pattern[i][j][k][m]); 									
				}		
				g_free(firing_rate_statistics[i][j][k].train_bins);
				g_free(firing_rate_statistics[i][j][k].num_of_spikes_in_pattern); 
				g_free(sum_of_spikes_in_bin[i][j][k]); 	
				g_free(sum_of_spikes_in_bin_in_pattern[i][j][k]); 
				g_free(sum_of_spikes_per_neuron_per_pattern[i][j][k]); 									
			}	
			g_free(firing_rate_statistics[i][j]);
			g_free(sum_of_spikes_in_bin[i][j]);	
			g_free(sum_of_spikes_in_bin_in_pattern[i][j]);	
			g_free(sum_of_spikes_per_neuron[i][j]); 																									
			g_free(sum_of_spikes_per_neuron_per_pattern[i][j]); 																						
		}
		g_free(firing_rate_statistics[i]);
		g_free(sum_of_spikes_in_bin[i]);	
		g_free(sum_of_spikes_in_bin_in_pattern[i]);	
		g_free(sum_of_spikes_per_neuron[i]); 																									
		g_free(sum_of_spikes_per_neuron_per_pattern[i]); 						
	}
	g_free(firing_rate_statistics);
	g_free(sum_of_spikes_in_bin);
	g_free(sum_of_spikes_in_bin_in_pattern);	
	g_free(sum_of_spikes_per_neuron); 																									
	g_free(sum_of_spikes_per_neuron_per_pattern); 	
				
	firing_rate_statistics = NULL;	
	sum_of_spikes_in_bin = NULL; 	
	sum_of_spikes_in_bin_in_pattern = NULL;	
	sum_of_spikes_per_neuron = NULL; 																									
	sum_of_spikes_per_neuron_per_pattern = NULL ; 		
	
	num_of_all_bins = 0;		
	num_of_all_patterns = 0;	

	for (i = 0; i < num_of_all_mwas; i++)
	{
		g_free(num_of_units_in_channel[i]);	
	}
	g_free(num_of_units_in_channel);	
	g_free(num_of_channels_in_mwa);		
	num_of_all_mwas = 0;
	num_of_channels_in_mwa = NULL;
	num_of_units_in_channel = NULL;
	
}

bool firing_rate_add_spike_to_statistics(TimeStamp pattern_start_time, TimeStamp pattern_end_time, SpikeTimeStampItem spike, int pattern_num)
{
	int mwa, chan, unit, bin;
	mwa = spike.mwa_or_layer;
	chan = spike.channel_or_neuron_group;
	unit = spike.unit_or_neuron;	
	
	if (pattern_end_time - pattern_start_time > pattern_length_ns)
	{
		printf ("FiringRate: ERROR: Submitted patterrn to calculate firing rate statistics is invalid!!!.\n");
		printf ("FiringRate: ERROR: Its length is %llu ns which is smaller than pattern length(%llu ns) to calculate statistics!!!.\n", pattern_end_time - pattern_start_time, pattern_length_ns);		
		return FALSE;
	}
	
	if ((spike.peak_time - pattern_start_time) >= pattern_length_ns)
		return TRUE;
	
	bin = (spike.peak_time - pattern_start_time) / bin_size_ns;

	sum_of_spikes_in_bin[mwa][chan][unit][bin]++;
	sum_of_spikes_in_bin_in_pattern[mwa][chan][unit][bin][pattern_num]++;
	sum_of_spikes_per_neuron[mwa][chan][unit]++;
	sum_of_spikes_per_neuron_per_pattern[mwa][chan][unit][pattern_num]++;	
	firing_rate_statistics[mwa][chan][unit].num_of_spikes_in_pattern[pattern_num]++;
	
		
	return TRUE;	
}


bool calculate_firing_rate_statistics(void)
{
	if (!calculate_firing_rate_means())
		return FALSE;
	if (!calculate_firing_rate_variances())
		return FALSE;		
		
	return TRUE;
}

static bool calculate_firing_rate_means(void)
{
	int i,j, k, m;
	for (i = 0; i < num_of_all_mwas; i++)
	{
		for (j = 0; j < num_of_channels_in_mwa[i]; j++)
		{	
			for (k = 0; k <num_of_units_in_channel[i][j]; k++)
			{
				for (m = 0; m <num_of_all_bins; m++)
				{
					firing_rate_statistics[i][j][k].train_bins[m].mean = sum_of_spikes_in_bin[i][j][k][m] / (bin_size_ns / 1000000000.0);   // convert to seconds
				}
				for (m = 0; m <num_of_all_bins; m++)
				{
					firing_rate_statistics[i][j][k].train_bins[m].mean = firing_rate_statistics[i][j][k].train_bins[m].mean / num_of_all_patterns;
				}				
			}						
		}
	}
	
	for (i = 0; i < num_of_all_mwas; i++)
	{
		for (j = 0; j < num_of_channels_in_mwa[i]; j++)
		{	
			for (k = 0; k <num_of_units_in_channel[i][j]; k++)
			{
				firing_rate_statistics[i][j][k].mean_num_of_spikes_per_neuron = sum_of_spikes_per_neuron[i][j][k]/ ( pattern_length_ns / 1000000000.0);			
			}
			for (k = 0; k <num_of_units_in_channel[i][j]; k++)
			{
				firing_rate_statistics[i][j][k].mean_num_of_spikes_per_neuron = firing_rate_statistics[i][j][k].mean_num_of_spikes_per_neuron / num_of_all_patterns;			
			}									
		}
	}
		
	return TRUE;
}

static bool calculate_firing_rate_variances(void)
{
	int i,j, k, m, n;
	for (i = 0; i < num_of_all_mwas; i++)
	{
		for (j = 0; j < num_of_channels_in_mwa[i]; j++)
		{	
			for (k = 0; k <num_of_units_in_channel[i][j]; k++)
			{
				for (m = 0; m <num_of_all_bins; m++)
				{	
					for (n = 0; n <num_of_all_bins; n++)
					{									
						sum_of_spikes_in_bin_in_pattern[i][j][k][m][n] = sum_of_spikes_in_bin_in_pattern[i][j][k][m][n] / (bin_size_ns / 1000000000.0);
					}
				} 
				for (m = 0; m <num_of_all_bins; m++)
				{	
					for (n = 0; n <num_of_all_bins; n++)
					{									
						sum_of_spikes_in_bin_in_pattern[i][j][k][m][n] = sum_of_spikes_in_bin_in_pattern[i][j][k][m][n] - firing_rate_statistics[i][j][k].train_bins[m].mean;
					}
				} 
				for (m = 0; m <num_of_all_bins; m++)
				{	
					for (n = 0; n <num_of_all_bins; n++)
					{									
						sum_of_spikes_in_bin_in_pattern[i][j][k][m][n] = sum_of_spikes_in_bin_in_pattern[i][j][k][m][n] * sum_of_spikes_in_bin_in_pattern[i][j][k][m][n];
					}
				} 
				for (m = 0; m <num_of_all_bins; m++)
				{	
					for (n = 0; n <num_of_all_bins; n++)
					{									
						firing_rate_statistics[i][j][k].train_bins[m].variance = firing_rate_statistics[i][j][k].train_bins[m].variance + sum_of_spikes_in_bin_in_pattern[i][j][k][m][n];
					}
				} 
				for (m = 0; m <num_of_all_bins; m++)
				{	
					firing_rate_statistics[i][j][k].train_bins[m].variance = firing_rate_statistics[i][j][k].train_bins[m].variance / num_of_all_patterns;
				} 																								
			}						
		}
	}
	
	for (i = 0; i < num_of_all_mwas; i++)
	{
		for (j = 0; j < num_of_channels_in_mwa[i]; j++)
		{	
			for (k = 0; k <num_of_units_in_channel[i][j]; k++)
			{
				for (m = 0; m <num_of_all_patterns; m++)
				{	
					sum_of_spikes_per_neuron_per_pattern[i][j][k][m] = sum_of_spikes_per_neuron_per_pattern[i][j][k][m] / (pattern_length_ns / 1000000000.0);
				} 
				for (m = 0; m <num_of_all_patterns; m++)
				{	
					sum_of_spikes_per_neuron_per_pattern[i][j][k][m] = sum_of_spikes_per_neuron_per_pattern[i][j][k][m] - firing_rate_statistics[i][j][k].mean_num_of_spikes_per_neuron;
				} 
				for (m = 0; m <num_of_all_patterns; m++)
				{	
					sum_of_spikes_per_neuron_per_pattern[i][j][k][m] = sum_of_spikes_per_neuron_per_pattern[i][j][k][m] * sum_of_spikes_per_neuron_per_pattern[i][j][k][m];
				} 
				for (m = 0; m <num_of_all_patterns; m++)
				{	
					firing_rate_statistics[i][j][k].variance_of_num_of_spikes_per_neuron = firing_rate_statistics[i][j][k].variance_of_num_of_spikes_per_neuron + sum_of_spikes_per_neuron_per_pattern[i][j][k][m];
				} 
				firing_rate_statistics[i][j][k].variance_of_num_of_spikes_per_neuron = firing_rate_statistics[i][j][k].variance_of_num_of_spikes_per_neuron / num_of_all_patterns;
			}						
		}
	}	
	return TRUE;
}
