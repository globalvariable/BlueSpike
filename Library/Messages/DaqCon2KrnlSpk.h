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

#include <stdbool.h>
#include <stdio.h>
#include <rtai_shm.h>
#include <rtai_nam2num.h>
#include "MessageBuffersConfig.h"
#include "MessageBuffersSharedMem.h"
#include "../../DaqCard.h"
#include "../../MicroWireArray.h"
#include "../Misc/Misc.h"

struct __DaqCon2KrnlSpkMsgItem
{
	DaqCon2KrnlSpkMsgType		msg_type;
	MwaNum					mwa_num;
	MwaChanNum				mwa_chan_num;		
	DaqCardNum					daq_card_num;
	DaqCardChanNum			daq_card_chan_num;		
	DaqCon2KrnlSpkMsgAdditional	additional_data;
};

struct __DaqCon2KrnlSpkMsg		// Requests to KernelSpike
{
	DaqCon2KrnlSpkMsgItem	buff[DAQ_CONFIG_2_KERNEL_SPIKE_MSG_BUFFER_SIZE];
	unsigned int				buff_write_idx;	// only one message sender can write into this buffer and edit this write index
	unsigned int				buff_read_idx;	// only one request handler can edit this read index
};

DaqCon2KrnlSpkMsg* allocate_shm_client_daq_config_2_kernel_spike_msg_buffer(DaqCon2KrnlSpkMsg* msg_buffer);
DaqCon2KrnlSpkMsg* deallocate_shm_client_daq_config_2_kernel_spike_msg_buffer(DaqCon2KrnlSpkMsg* msg_buffer);
bool write_to_daq_config_2_kernel_spike_msg_buffer(DaqCon2KrnlSpkMsg* msg_buffer, DaqCon2KrnlSpkMsgType msg_type, MwaNum	mwa_num, MwaChanNum mwa_chan_num, DaqCardNum	daq_card_num,  DaqCardChanNum daq_card_chan_num, DaqCon2KrnlSpkMsgAdditional	additional_data);

#endif


