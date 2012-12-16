#include "TempMat2KrnlSpk.h"

TempMat2KrnlSpkMsg* allocate_shm_client_template_matching_2_kernel_spike_msg_buffer(TempMat2KrnlSpkMsg* msg_buffer)
{
	if (msg_buffer != NULL)
	{
		msg_buffer = deallocate_shm_client_template_matching_2_kernel_spike_msg_buffer(msg_buffer);
		msg_buffer = allocate_shm_client_template_matching_2_kernel_spike_msg_buffer(msg_buffer);
		return msg_buffer;
	}  
	msg_buffer = rtai_malloc(SHM_NUM_TEMPLATE_MATCHING_2_KERNEL_SPIKE, 0);
	print_message(INFO_MSG ,"BlueSpike", "TempMat2KrnlSpk", "allocate_shm_client_template_matching_2_kernel_spike_msg_buffer", "Created shm_client_template_matching_2_kernel_spike_msg_buffer.");
	return msg_buffer;	
}
TempMat2KrnlSpkMsg* deallocate_shm_client_template_matching_2_kernel_spike_msg_buffer(TempMat2KrnlSpkMsg* msg_buffer)
{
	if (msg_buffer == NULL)
		return (TempMat2KrnlSpkMsg*)print_message(INFO_MSG ,"BlueSpike", "TempMat2KrnlSpk", "deallocate_shm_client_template_matching_2_kernel_spike_msg_buffer", "msg_buffer == NULL.");
	rtai_free(SHM_NUM_TEMPLATE_MATCHING_2_KERNEL_SPIKE, msg_buffer);	
	return NULL;
}
bool write_to_template_matching_2_kernel_spike_msg_buffer(TempMat2KrnlSpkMsg* msg_buffer, TempMat2KrnlSpkMsgType msg_type, MwaNum mwa, MwaChanNum mwa_chan_num, UnitNum unit_num, TemplateProbabilityThres threshold, TempMat2KrnlSpkMsgAdditional additional_data)
{
	unsigned int *idx;
	idx = &(msg_buffer->buff_write_idx);
	TempMat2KrnlSpkMsgItem *buff = msg_buffer->buff;
	buff[*idx].msg_type = msg_type;
	buff[*idx].mwa = mwa;
	buff[*idx].mwa_chan_num = mwa_chan_num;
	buff[*idx].unit_num = unit_num;
	buff[*idx].threshold = threshold;
	buff[*idx].additional_data =  additional_data;
	if ((*idx + 1) == TEMPLATE_MATCHING_2_KERNEL_SPIKE_MSG_BUFFER_SIZE)
		*idx = 0;
	else
		(*idx)++;
	if (*idx == msg_buffer->buff_read_idx)
		return print_message(INFO_MSG ,"BlueSpike", "TempMat2KrnlSpk", "write_to_template_matching_2_kernel_spike_msg_buffer", "BUFFER IS FULL!!!.");    		
	return TRUE;
}

