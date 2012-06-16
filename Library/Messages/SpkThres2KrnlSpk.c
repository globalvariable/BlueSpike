#include "SpkThres2KrnlSpk.h"

SpkThres2KrnlSpkMsg* allocate_shm_client_spike_thres_2_kernel_spike_msg_buffer(SpkThres2KrnlSpkMsg* msg_buffer)
{
	if (msg_buffer != NULL)
	{
		msg_buffer = deallocate_shm_client_spike_thres_2_kernel_spike_msg_buffer(msg_buffer);
		msg_buffer = allocate_shm_client_spike_thres_2_kernel_spike_msg_buffer(msg_buffer);
		return msg_buffer;
	}  
	msg_buffer = rtai_malloc(nam2num(SPIKE_THRES_2_KERNEL_SPIKE_SHM_NAME), 0);
	print_message(INFO_MSG ,"BlueSpike", "SpkThres2KrnlSpk", "allocate_shm_client_spike_thres_2_kernel_spike_msg_buffer", "Created shm_client_spike_thres_2_kernel_spike_msg_buffer.");
	return msg_buffer;	
}
SpkThres2KrnlSpkMsg* deallocate_shm_client_spike_thres_2_kernel_spike_msg_buffer(SpkThres2KrnlSpkMsg* msg_buffer)
{
	if (msg_buffer == NULL)
		return (SpkThres2KrnlSpkMsg*)print_message(INFO_MSG ,"BlueSpike", "SpkThres2KrnlSpk", "deallocate_shm_client_spike_thres_2_kernel_spike_msg_buffer", "msg_buffer == NULL.");
	rtai_free(nam2num(SPIKE_THRES_2_KERNEL_SPIKE_SHM_NAME), msg_buffer);	
	return NULL;
}
bool write_to_spike_thres_2_kernel_spike_msg_buffer(SpkThres2KrnlSpkMsg* msg_buffer, SpkThres2KrnlSpkMsgType msg_type, MwaNum mwa, MwaChanNum mwa_chan_num, SpikeThreshold threshold, SpkThres2KrnlSpkMsgAdditional additional_data)
{
	unsigned int *idx;
	idx = &(msg_buffer->buff_write_idx);
	SpkThres2KrnlSpkMsgItem *buff = msg_buffer->buff;
	buff[*idx].msg_type = msg_type;
	buff[*idx].mwa = mwa;
	buff[*idx].mwa_chan_num = mwa_chan_num;
	buff[*idx].threshold = threshold;
	buff[*idx].additional_data =  additional_data;
	if ((*idx + 1) == SPIKE_THRES_2_KERNEL_SPIKE_MSG_BUFFER_SIZE)
		*idx = 0;
	else
		(*idx)++;
	if (*idx == msg_buffer->buff_read_idx)
		return print_message(INFO_MSG ,"BlueSpike", "SpkThres2KrnlSpk", "write_to_spike_thres_2_kernel_spike_msg_buffer", "BUFFER IS FULL!!!.");    		
	return TRUE;
}

