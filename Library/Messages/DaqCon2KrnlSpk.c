#include "DaqCon2KrnlSpk.h"

DaqCon2KrnlSpkMsg* allocate_shm_client_daq_config_2_kernel_spike_msg_buffer(DaqCon2KrnlSpkMsg* msg_buffer)
{
	if (msg_buffer != NULL)
	{
		msg_buffer = deallocate_shm_client_daq_config_2_kernel_spike_msg_buffer(msg_buffer);
		msg_buffer = allocate_shm_client_daq_config_2_kernel_spike_msg_buffer(msg_buffer);
		return msg_buffer;
	}  
	msg_buffer = rtai_malloc(nam2num(DAQ_CONFIG_2_KERNEL_SPIKE_SHM_NAME), 0);
	print_message(INFO_MSG ,"BlueSpike", "DaqCon2KrnlSpk", "allocate_shm_client_daq_config_2_kernel_spike_msg_buffer", "Created shm_client_daq_config_2_kernel_spike_msg_buffer.");
	return msg_buffer;	
}
DaqCon2KrnlSpkMsg* deallocate_shm_client_daq_config_2_kernel_spike_msg_buffer(DaqCon2KrnlSpkMsg* msg_buffer)
{
	if (msg_buffer == NULL)
		return (DaqCon2KrnlSpkMsg*)print_message(INFO_MSG ,"BlueSpike", "DaqCon2KrnlSpk", "deallocate_shm_client_daq_config_2_kernel_spike_msg_buffer", "msg_buffer == NULL.");
	rtai_free(nam2num(DAQ_CONFIG_2_KERNEL_SPIKE_SHM_NAME), msg_buffer);	
	return NULL;
}
bool write_to_daq_config_2_kernel_spike_msg_buffer(DaqCon2KrnlSpkMsg* msg_buffer, DaqCon2KrnlSpkMsgType msg_type, MwaNum	mwa_num, MwaChanNum mwa_chan_num, DaqCardNum	daq_card_num,  DaqCardChanNum daq_card_chan_num, DaqCon2KrnlSpkMsgAdditional	additional_data)
{
	unsigned int *idx;
	idx = &(msg_buffer->buff_write_idx);
	DaqCon2KrnlSpkMsgItem *buff = msg_buffer->buff;
	buff[*idx].msg_type = msg_type;
	buff[*idx].mwa_num = mwa_num;
	buff[*idx]. mwa_chan_num =  mwa_chan_num;
	buff[*idx].daq_card_num = daq_card_num;
	buff[*idx]. daq_card_chan_num =  daq_card_chan_num;
	buff[*idx].additional_data = additional_data;
	if ((*idx + 1) == DAQ_CONFIG_2_KERNEL_SPIKE_MSG_BUFFER_SIZE)
		*idx = 0;
	else
		(*idx)++;
	if (*idx == msg_buffer->buff_read_idx)
		return print_message(INFO_MSG ,"BlueSpike", "DaqCon2KrnlSpk", "write_to_daq_config_2_kernel_spike_msg_buffer", "BUFFER IS FULL!!!.");    		
	return TRUE;
}

