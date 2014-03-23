#include "SpikeSorting.h"


static bool is_index_between_indexes(int start_idx, int end_idx, int this_idx);
static void handle_spike_end_handling_buffer(void);
static void find_spike_end(unsigned int mwa,  unsigned int mwa_chan, TimeStamp previous_daq_time_ns);


void spike_sorting(unsigned int daq_num, TimeStamp previous_daq_time_ns)
{
	unsigned int i;
	DaqMwaMapItem *map_item;

	handle_spike_end_handling_buffer();
	for (i = 0; i < MAX_NUM_OF_CHANNEL_PER_DAQ_CARD; i++)
	{
		map_item = &(daq_mwa_map[daq_num].map[i]);
		find_spike_end(map_item->mwa, map_item->channel, previous_daq_time_ns);	
	}
}	


static unsigned int filtered_recording_data_buff_idx_prev[MAX_NUM_OF_MWA][MAX_NUM_OF_CHAN_PER_MWA] ={{0}};      // Do not read data pointed by buff_idx_write. Read up to buff_idx_write by excluding buff_idx_write
static bool in_spike_arr[MAX_NUM_OF_MWA][MAX_NUM_OF_CHAN_PER_MWA] = {{0}};
static bool after_in_spike_arr[MAX_NUM_OF_MWA][MAX_NUM_OF_CHAN_PER_MWA] = {{0}};
static int in_spike_sample_cntr_arr[MAX_NUM_OF_MWA][MAX_NUM_OF_CHAN_PER_MWA] = {{0}};

static void find_spike_end(unsigned int mwa,  unsigned int mwa_chan, TimeStamp previous_daq_time_ns)
{
	RecordingDataSample	*filtered_recording_data_chan_buff;
	int previous_acquisition_time_cntr =0; 
	int idx, start_idx, end_idx, min_idx, i, spike_end_idx, handle_for_peak_idx;
	float amplitude_thres, min;
	bool in_spike;
	bool after_in_spike;
	int in_spike_sample_cntr;
	SpikeEndHandlingItem 	*spike_end_handling_buff;	
	TimeStamp peak_time;			

	amplitude_thres = spike_thresholding.amplitude_thres[mwa][mwa_chan];
	if (amplitude_thres == 0.0)
	{
		filtered_recording_data_buff_idx_prev[mwa][mwa_chan] = recording_data.buff_idx_write[mwa][mwa_chan];
		spike_end_handling.buff_start_idx = spike_end_handling.buff_write_idx;
		return;
	}
	
	filtered_recording_data_chan_buff = recording_data.filtered_recording_data_buff[mwa][mwa_chan];
	spike_end_handling_buff = spike_end_handling.spike_end_handling_buff;
	
	start_idx = filtered_recording_data_buff_idx_prev[mwa][mwa_chan];
	end_idx = recording_data.buff_idx_write[mwa][mwa_chan];
	in_spike = in_spike_arr[mwa][mwa_chan];
	after_in_spike = after_in_spike_arr[mwa][mwa_chan];
	in_spike_sample_cntr = in_spike_sample_cntr_arr[mwa][mwa_chan];
	idx = start_idx;

	while (idx != end_idx)
	{	
		previous_acquisition_time_cntr++;


		if ( (filtered_recording_data_chan_buff[idx] < amplitude_thres) && (!after_in_spike))
		{
			in_spike = 1;
		}
		if (in_spike)
		{
			in_spike_sample_cntr++; 
			if (in_spike_sample_cntr > IN_SPIKE_SAMPLE_CNTR_MAX)	// not a spike if lasts longer than  ~320 us  
			{
				in_spike_sample_cntr = 0;	
				in_spike = 0;				// discard this noisy thing
				after_in_spike = 1;
			}
		}
		if (( filtered_recording_data_chan_buff[idx] > amplitude_thres) && after_in_spike)
		{
			after_in_spike = 0;
		}
		if ( (filtered_recording_data_chan_buff[idx] > amplitude_thres) && (in_spike))
		{
			in_spike = 0;
			in_spike_sample_cntr = 0;
			min = filtered_recording_data_chan_buff[idx];
			min_idx = idx;

			for (i=0; i< IN_SPIKE_SAMPLE_CNTR_MAX; i++)	
			{
				if (idx < i)   // if (idx - i <0) 
				{
					handle_for_peak_idx = idx-i+RECORDING_DATA_BUFF_SIZE;
					if (filtered_recording_data_chan_buff[handle_for_peak_idx] < min)
					{
						min =  filtered_recording_data_chan_buff[handle_for_peak_idx];
						min_idx = handle_for_peak_idx;
						previous_acquisition_time_cntr--;
					}	
				}
				else
				{
					handle_for_peak_idx = idx-i;
					if (filtered_recording_data_chan_buff[handle_for_peak_idx] < min)
					{
						min =  filtered_recording_data_chan_buff[handle_for_peak_idx];
						min_idx = handle_for_peak_idx;
						previous_acquisition_time_cntr--;
					}		
				}
			}
			if ((min_idx+SPIKE_MIN_END_SAMP_NUM) >= RECORDING_DATA_BUFF_SIZE)
				spike_end_idx = min_idx+SPIKE_MIN_END_SAMP_NUM - RECORDING_DATA_BUFF_SIZE;
			else		
				spike_end_idx = min_idx+SPIKE_MIN_END_SAMP_NUM;
			
			peak_time = previous_daq_time_ns + (previous_acquisition_time_cntr * SAMPLING_INTERVAL);  // SAMPLING_INTERVAL = 25000 nanoseconds;
				
			if (is_index_between_indexes(filtered_recording_data_buff_idx_prev[mwa][mwa_chan], recording_data.buff_idx_write[mwa][mwa_chan], spike_end_idx))
			{
				run_template_matching(mwa, mwa_chan, spike_end_idx, peak_time);  // SAMPLING_INTERVAL = 25000 nanoseconds;
			}			
			else 	//   Write spike end into shared_memory->spike_end_handing
			{
				spike_end_handling_buff[spike_end_handling.buff_write_idx].mwa = mwa;		// push to the end off buffer to be handled later on
				spike_end_handling_buff[spike_end_handling.buff_write_idx].chan = mwa_chan;
				spike_end_handling_buff[spike_end_handling.buff_write_idx].end_of_spike_in_filtered_recording_data_buff = spike_end_idx;
				spike_end_handling_buff[spike_end_handling.buff_write_idx].peak_time = peak_time;
				if ((spike_end_handling.buff_write_idx + 1) == SPIKE_END_HANDLING_DATA_BUFF_SIZE)
					spike_end_handling.buff_write_idx = 0;
				else
					spike_end_handling.buff_write_idx++;		
			}
		}
		idx ++;
		if (idx ==	RECORDING_DATA_BUFF_SIZE)
			idx = 0;
	}
	in_spike_arr[mwa][mwa_chan] = in_spike;
	after_in_spike_arr[mwa][mwa_chan] = after_in_spike;
	in_spike_sample_cntr_arr[mwa][mwa_chan] = in_spike_sample_cntr;
	filtered_recording_data_buff_idx_prev[mwa][mwa_chan] = recording_data.buff_idx_write[mwa][mwa_chan];
}


static void handle_spike_end_handling_buffer(void)
{
	SpikeEndHandlingItem 	*spike_end_handling_buff;			

	int mwa, chan, filtered_recording_data_buff_idx;	
	TimeStamp	peak_time;
	
	int idx, start_idx, end_idx;	
	
	spike_end_handling_buff = spike_end_handling.spike_end_handling_buff;

	start_idx = spike_end_handling.buff_start_idx;
	end_idx = spike_end_handling.buff_write_idx;
	
	idx = start_idx;
	while (idx != end_idx)
	{	
		mwa = spike_end_handling_buff[idx].mwa;
		chan = spike_end_handling_buff[idx].chan;
		filtered_recording_data_buff_idx = spike_end_handling_buff[idx].end_of_spike_in_filtered_recording_data_buff;
		peak_time = spike_end_handling_buff[idx].peak_time;
		
		if (is_index_between_indexes(filtered_recording_data_buff_idx_prev[mwa][chan], recording_data.buff_idx_write[mwa][chan], filtered_recording_data_buff_idx))
		{
			run_template_matching(mwa, chan, filtered_recording_data_buff_idx, peak_time);
		}	
		else
		{
			spike_end_handling_buff[spike_end_handling.buff_write_idx].mwa = mwa;		// push to the end off buffer to be handled later on
			spike_end_handling_buff[spike_end_handling.buff_write_idx].chan = chan;
			spike_end_handling_buff[spike_end_handling.buff_write_idx].end_of_spike_in_filtered_recording_data_buff = filtered_recording_data_buff_idx;
			spike_end_handling_buff[spike_end_handling.buff_write_idx].peak_time = peak_time;
			if ((spike_end_handling.buff_write_idx + 1) == SPIKE_END_HANDLING_DATA_BUFF_SIZE)
				spike_end_handling.buff_write_idx = 0;
			else
				spike_end_handling.buff_write_idx++;
		}
		idx ++;
		if (idx ==	SPIKE_END_HANDLING_DATA_BUFF_SIZE)
			idx = 0;
	}
	spike_end_handling.buff_start_idx = end_idx;  
}




static bool is_index_between_indexes(int start_idx, int end_idx, int this_idx)
{
	if (
		(
			(start_idx < end_idx) && (start_idx <= this_idx) && (this_idx < end_idx )
		)
		||
		(
			( start_idx > end_idx) &&
								(
									(start_idx <= this_idx ) || (this_idx < end_idx )
																		)
		)
	)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
