#include "RecordingData.h"
#include "ButterFilter.h"

bool handle_recording_data(unsigned int daq_num, lsampl_t *daq_data)
{
	unsigned int i;
	unsigned int mwa, mwa_chan;
	unsigned int daq_chan_num;
	unsigned int write_idx;
	RecordingDataSample	*recording_data_chan_buff;
	RecordingDataSample	*filtered_recording_data_chan_buff;

	for (i = 0; i < MAX_NUM_OF_CHANNEL_PER_DAQ_CARD*NUM_OF_SCAN; i++)
	{
		daq_chan_num = i % MAX_NUM_OF_CHANNEL_PER_DAQ_CARD;

		mwa = daq_mwa_map[daq_num].map[daq_chan_num].mwa;
		mwa_chan = daq_mwa_map[daq_num].map[daq_chan_num].channel;

		if (mwa == MAX_NUM_OF_MWA)   // this daq channel not mapped. skip this channel.
			continue;

		write_idx = recording_data.buff_idx_write[mwa][mwa_chan];

		recording_data_chan_buff = recording_data.recording_data_buff[mwa][mwa_chan];
		filtered_recording_data_chan_buff = recording_data.filtered_recording_data_buff[mwa][mwa_chan];

		recording_data_chan_buff[write_idx] = (daq_data[i] - BASELINE_QUANT_6259) / VOLTAGE_MULTIPLIER_MV_6259;

		butter_bandpass_filter_31250hz_4th_400hz_8000hz(recording_data_chan_buff, filtered_recording_data_chan_buff, write_idx);

		write_idx++;
		if (write_idx == RECORDING_DATA_BUFF_SIZE)
 			write_idx = 0;

		recording_data.buff_idx_write[mwa][mwa_chan] = write_idx;

	}

	return TRUE;
}




