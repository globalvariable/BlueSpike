#include "FiltCtrl2KrnlSpk.h"

FiltCtrl2KrnlSpkMsg* allocate_shm_client_filter_ctrl_2_kernel_spike_msg_buffer(FiltCtrl2KrnlSpkMsg* msg_buffer)
{
	if (msg_buffer != NULL)
	{
		msg_buffer = deallocate_shm_client_filter_ctrl_2_kernel_spike_msg_buffer(msg_buffer);
		msg_buffer = allocate_shm_client_filter_ctrl_2_kernel_spike_msg_buffer(msg_buffer);
		return msg_buffer;
	}  
	msg_buffer = rtai_malloc(SHM_NUM_FILTER_CTRL_2_KERNEL_SPIKE, 0);
	print_message(INFO_MSG ,"BlueSpike", "FiltCtrl2KrnlSpk", "allocate_shm_client_filter_ctrl_2_kernel_spike_msg_buffer", "Created shm_client_filter_ctrl_2_kernel_spike_msg_buffer.");
	return msg_buffer;	
}
FiltCtrl2KrnlSpkMsg* deallocate_shm_client_filter_ctrl_2_kernel_spike_msg_buffer(FiltCtrl2KrnlSpkMsg* msg_buffer)
{
	if (msg_buffer == NULL)
		return (FiltCtrl2KrnlSpkMsg*)print_message(INFO_MSG ,"BlueSpike", "FiltCtrl2KrnlSpk", "deallocate_shm_client_filter_ctrl_2_kernel_spike_msg_buffer", "msg_buffer == NULL.");
	rtai_free(SHM_NUM_FILTER_CTRL_2_KERNEL_SPIKE, msg_buffer);	
	return NULL;
}
bool write_to_filter_ctrl_2_kernel_spike_msg_buffer(FiltCtrl2KrnlSpkMsg* msg_buffer, FiltCtrl2KrnlSpkMsgType msg_type, FiltCtrl2KrnlSpkMsgAdditional additional_data)
{
	unsigned int *idx;
	idx = &(msg_buffer->buff_write_idx);
	FiltCtrl2KrnlSpkMsgItem *buff = msg_buffer->buff;
	buff[*idx].msg_type = msg_type;
	buff[*idx].additional_data = additional_data;
	if ((*idx + 1) == FILTER_CTRL_2_KERNEL_SPIKE_MSG_BUFFER_SIZE)
		*idx = 0;
	else
		(*idx)++;
	if (*idx == msg_buffer->buff_read_idx)
		return print_message(INFO_MSG ,"BlueSpike", "FiltCtrl2KrnlSpk", "write_to_filter_ctrl_2_kernel_spike_msg_buffer", "BUFFER IS FULL!!!.");    		
	return TRUE;
}

