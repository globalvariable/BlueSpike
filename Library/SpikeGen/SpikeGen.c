#include "SpikeGen.h"


static unsigned int spike_time_stamp_buff_size = SPIKE_TIME_STAMP_BUFF_SIZE;

bool write_generated_spike_to_blue_spike_buffer(SpikeTimeStamp *spike_time_stamp, unsigned int layer_num, unsigned int neuron_grp_num, unsigned int neuron_num, TimeStamp spike_time)
{
	SpikeTimeStampItem *item;
	unsigned int *buff_idx_write;

	buff_idx_write = &(spike_time_stamp->buff_idx_write);
	item = &(spike_time_stamp->spike_time_stamp_buff[*buff_idx_write]);

	item->peak_time = spike_time;
	item->mwa_or_layer = layer_num;
	item->channel_or_neuron_group = neuron_grp_num;
	item->unit_or_neuron = neuron_num;	
	if (((*buff_idx_write) +1) ==  spike_time_stamp_buff_size )	   // first check then increment. if first increment and check end of buffer might lead to problem during reading.
		*buff_idx_write = 0;
	else
		(*buff_idx_write)++;		
	return TRUE;			
}
