#ifndef DAQ_CONFIG_2_KERNEL_SPIKE_H
#define DAQ_CONFIG_2_KERNEL_SPIKE_H

//  MESSAGES FROM DAQ CONFIG TO KERNEL SPIKE

typedef struct __DaqCon2KrnlSpkMsg DaqCon2KrnlSpkMsg;
typedef struct __DaqCon2KrnlSpkMsgItem DaqCon2KrnlSpkMsgItem;
typedef unsigned int DaqCon2KrnlSpkMsgType;
typedef unsigned int DaqCon2KrnlSpkMsgAdditional;

#define DAQ_CONFIG_2_KERNEL_SPIKE_MSG_ADDITIONAL_NULL		0


#define DAQ_CONFIG_2_KERNEL_SPIKE_MSG_STRING_LENGTH		50

#define DAQ_CONFIG_2_KERNEL_SPIKE_MSG_NULL					0
#define DAQ_CONFIG_2_KERNEL_SPIKE_MSG_TURN_CARDS_ON		1
#define DAQ_CONFIG_2_KERNEL_SPIKE_MSG_TURN_CARDS_OFF		2
#define DAQ_CONFIG_2_KERNEL_SPIKE_MSG_MAP_MWA_CHAN		3
#define DAQ_CONFIG_2_KERNEL_SPIKE_MSG_CANCEL_ALL_MAPPING	4


#include <rtai_shm.h>
#include <rtai_nam2num.h>
#include "MessageBuffersConfig.h"
#ifndef KERNELSPIKE_H		/// insmod KernelSpike errors:  Unknown symbol __strcat_chk, Unknown symbol __sprintf_chk
#include <stdbool.h>
#include <stdio.h>
#include "../Misc/Misc.h"
#include "../../System/ShmSemNum/ShmSemNum.h"
#endif

struct __DaqCon2KrnlSpkMsgItem
{
	DaqCon2KrnlSpkMsgType		msg_type;
	unsigned int					mwa_num;
	unsigned int					mwa_chan_num;		
	unsigned int					daq_card_num;
	unsigned int					daq_card_chan_num;		
	DaqCon2KrnlSpkMsgAdditional	additional_data;
};

struct __DaqCon2KrnlSpkMsg		// Requests to KernelSpike
{
	DaqCon2KrnlSpkMsgItem	buff[DAQ_CONFIG_2_KERNEL_SPIKE_MSG_BUFFER_SIZE];
	unsigned int				buff_write_idx;	// only one message sender can write into this buffer and edit this write index
	unsigned int				buff_read_idx;	// only one request handler can edit this read index
};

#ifndef KERNELSPIKE_H    /// not necessary for KernelSpike
DaqCon2KrnlSpkMsg* allocate_shm_client_daq_config_2_kernel_spike_msg_buffer(DaqCon2KrnlSpkMsg* msg_buffer);
DaqCon2KrnlSpkMsg* deallocate_shm_client_daq_config_2_kernel_spike_msg_buffer(DaqCon2KrnlSpkMsg* msg_buffer);
bool write_to_daq_config_2_kernel_spike_msg_buffer(DaqCon2KrnlSpkMsg* msg_buffer, DaqCon2KrnlSpkMsgType msg_type, unsigned int mwa_num, unsigned int mwa_chan_num, unsigned int	daq_card_num,  unsigned int daq_card_chan_num, DaqCon2KrnlSpkMsgAdditional	additional_data);
bool get_next_daq_config_2_kernel_spike_msg_buffer_item( DaqCon2KrnlSpkMsg* msg_buffer, DaqCon2KrnlSpkMsgItem *msg_item);
#endif

#endif


