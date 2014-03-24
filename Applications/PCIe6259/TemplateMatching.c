#include "TemplateMatching.h"

static double template_matching_g_x[MAX_NUM_OF_UNIT_PER_CHAN];
static double template_matching_diff[NUM_OF_SAMP_PER_SPIKE];
static double template_matching_diff_temporary[NUM_OF_SAMP_PER_SPIKE];
static double template_matching_exponent;	
static double template_matching_probabl[MAX_NUM_OF_UNIT_PER_CHAN];	

void run_template_matching(int mwa, int chan, int filtered_recording_data_buff_idx, TimeStamp peak_time)
{

	TemplateMatchingUnitData *unit_template_data;	
	RecordingDataChanBuff	*filtered_recording_data_chan_buff;
	BlueSpikeTimeStampItem *blue_spike_time_stamp_item;
	SpikeTimeStampItem *spike_time_stamp_item;		
	double greatest;	
	int i, j, unit, greatest_idx;
	int blue_spike_time_stamp_buff_idx_write;
	int include_unit;
	int spike_time_stamp_buff_idx_write;


	filtered_recording_data_chan_buff = &(recording_data.filtered_recording_data_buff[mwa][chan]);

	greatest = -DBL_MAX;
	greatest_idx = MAX_NUM_OF_UNIT_PER_CHAN;   // If doesnt match any one it will be classified as unsorted (MAX_NUM_OF_UNIT_PER_CHAN)

	for (unit = 0; unit <MAX_NUM_OF_UNIT_PER_CHAN; unit++)
	{
		unit_template_data =  &(template_matching_data[mwa][chan][unit]);
		if (unit_template_data->sorting_on)
		{
			template_matching_g_x[unit] = 0.0;
			for (i=0; i<NUM_OF_SAMP_PER_SPIKE; i++)
			{
				{
					if (filtered_recording_data_buff_idx < i)   //  spike_end_idx_in_filtered_recording - i  < 0
						template_matching_diff[i] = (*filtered_recording_data_chan_buff)[filtered_recording_data_buff_idx-i+RECORDING_DATA_BUFF_SIZE] - unit_template_data->template[NUM_OF_SAMP_PER_SPIKE-i-1];
					else
						template_matching_diff[i] = (*filtered_recording_data_chan_buff)[filtered_recording_data_buff_idx-i] - unit_template_data->template[NUM_OF_SAMP_PER_SPIKE-i-1];
				}
			}
			for (i=0; i <NUM_OF_SAMP_PER_SPIKE;i++)
			{
				template_matching_diff_temporary[i] = 0;
			}
			for (i=0; i<NUM_OF_SAMP_PER_SPIKE; i++)
			{
				for (j=0; j<NUM_OF_SAMP_PER_SPIKE; j++)
				{
					template_matching_diff_temporary[i] = template_matching_diff_temporary[i] + (template_matching_diff[j] * unit_template_data->inv_S[i][j]);
				}
			}
			for (i=0; i<NUM_OF_SAMP_PER_SPIKE; i++)
			{
				template_matching_g_x[unit] = template_matching_g_x[unit] + (template_matching_diff_temporary[i] * template_matching_diff[i]);
			}
			template_matching_exponent = exp((-0.5)*template_matching_g_x[unit]);
			template_matching_probabl[unit] = (1.06488319787324016356e-12/unit_template_data->sqrt_det_S)*template_matching_exponent;       //   ( 1/ (   ((2*pi)^(d/2)) * (det_S^(1/2)) ) * exp( (-1/2) * (x-u)' * (S^ (-1)) - (x-u) )   d= 30

			template_matching_g_x[unit] = 0 - (unit_template_data->log_det_S) - (template_matching_g_x[unit]);	
			
			if ((template_matching_g_x[unit] > greatest) && (template_matching_probabl[unit] >= unit_template_data->probability_thres))    // assign spike to a unit.
			{
				greatest = template_matching_g_x[unit];
				greatest_idx = unit;
			}	
		}
	}
	
	if (greatest_idx < MAX_NUM_OF_UNIT_PER_CHAN)
	{
		unit_template_data =  &(template_matching_data[mwa][chan][greatest_idx]);
		if (unit_template_data->alarm_on)
		{
			if (template_matching_probabl[greatest_idx] < unit_template_data->alarm_thres)
			{
				if (unit_template_data->alarm_count < 999)   // increment up to 999.
					unit_template_data->alarm_count++;
			}
			else
			{
				if (unit_template_data->alarm_count > 0)
					unit_template_data->alarm_count--;
			}
		}
		else
		{
			unit_template_data->alarm_count = 0;
		}		
	}


	//   Write spike time stamp into shared_memory->spike_time_stamp
	blue_spike_time_stamp_buff_idx_write = blue_spike_time_stamp.buff_idx_write;
	include_unit = template_matching_data[mwa][chan][greatest_idx].include_unit;   /// cannot include non_sorted unit. so no problem for spike_time_stamp_buff
	blue_spike_time_stamp_item = &(blue_spike_time_stamp.blue_spike_time_stamp_buff[blue_spike_time_stamp_buff_idx_write]);
	blue_spike_time_stamp_item->peak_time = peak_time;
	blue_spike_time_stamp_item->mwa = mwa;
	blue_spike_time_stamp_item->channel = chan;
	blue_spike_time_stamp_item->unit = greatest_idx;
	blue_spike_time_stamp_item->recording_data_buff_idx = filtered_recording_data_buff_idx;
	blue_spike_time_stamp_item->include_unit = include_unit;
	if ((blue_spike_time_stamp_buff_idx_write +1) ==  BLUE_SPIKE_TIME_STAMP_BUFF_SIZE )	   // first check then increment. if first increment and check end of buffer might lead to problem during reading.
		blue_spike_time_stamp.buff_idx_write = 0;
	else
		blue_spike_time_stamp.buff_idx_write++;	
	
	if (include_unit)	// fill in spike time stamp buff
	{
		spike_time_stamp_buff_idx_write = spike_time_stamp->buff_idx_write;
		spike_time_stamp_item = &(spike_time_stamp->spike_time_stamp_buff[spike_time_stamp_buff_idx_write]);
		spike_time_stamp_item->peak_time = peak_time;
		spike_time_stamp_item->mwa_or_layer = mwa;
		spike_time_stamp_item->channel_or_neuron_group = chan;
		spike_time_stamp_item->unit_or_neuron = greatest_idx;	
		if ((spike_time_stamp_buff_idx_write +1) ==  SPIKE_TIME_STAMP_BUFF_SIZE )	   // first check then increment. if first increment and check end of buffer might lead to problem during reading.
			spike_time_stamp->buff_idx_write = 0;
		else
			spike_time_stamp->buff_idx_write++;			
	}
}

