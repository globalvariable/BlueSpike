#ifndef FILTER_CTRL_2_KERNEL_SPIKE_H
#define FILTER_CTRL_2_KERNEL_SPIKE_H

//  MESSAGES FROM FILTER CONTROLLER TO KERNEL SPIKE

typedef struct __FiltCtrl2KrnlSpkMsg FiltCtrl2KrnlSpkMsg;
typedef struct __FiltCtrl2KrnlSpkMsgItem FiltCtrl2KrnlSpkMsgItem;
typedef unsigned int FiltCtrl2KrnlSpkMsgType;
typedef unsigned int FiltCtrl2KrnlSpkMsgAdditional;

#define FILTER_CTRL_2_KERNEL_SPIKE_MSG_ADDITIONAL_NULL		0


#define FILTER_CTRL_2_KERNEL_SPIKE_MSG_STRING_LENGTH		50

#define FILTER_CTRL_2_KERNEL_SPIKE_MSG_NULL							0
#define FILTER_CTRL_2_KERNEL_SPIKE_MSG_TURN_HIGH_PASS_150HZ_ON	1
#define FILTER_CTRL_2_KERNEL_SPIKE_MSG_TURN_HIGH_PASS_150HZ_OFF	2
#define FILTER_CTRL_2_KERNEL_SPIKE_MSG_TURN_HIGH_PASS_400HZ_ON	3
#define FILTER_CTRL_2_KERNEL_SPIKE_MSG_TURN_HIGH_PASS_400HZ_OFF	4
#define FILTER_CTRL_2_KERNEL_SPIKE_MSG_TURN_LOW_PASS_8KHZ_ON	5
#define FILTER_CTRL_2_KERNEL_SPIKE_MSG_TURN_LOW_PASS_8KHZ_OFF	6



#include <rtai_shm.h>
#include <rtai_nam2num.h>
#include "MessageBuffersConfig.h"
#include "MessageBuffersSharedMem.h"
#ifndef KERNELSPIKE_H		/// insmod KernelSpike errors:  Unknown symbol __strcat_chk, Unknown symbol __sprintf_chk
#include <stdbool.h>
#include <stdio.h>
#include "../Misc/Misc.h"
#endif

struct __FiltCtrl2KrnlSpkMsgItem
{
	FiltCtrl2KrnlSpkMsgType		msg_type;
	FiltCtrl2KrnlSpkMsgAdditional	additional_data;
};

struct __FiltCtrl2KrnlSpkMsg		// Requests to KernelSpike
{
	FiltCtrl2KrnlSpkMsgItem		buff[FILTER_CTRL_2_KERNEL_SPIKE_MSG_BUFFER_SIZE];
	unsigned int				buff_write_idx;	// only one message sender can write into this buffer and edit this write index
	unsigned int				buff_read_idx;	// only one request handler can edit this read index
};

#ifndef KERNELSPIKE_H    /// not necessary for KernelSpike
FiltCtrl2KrnlSpkMsg* allocate_shm_client_filter_ctrl_2_kernel_spike_msg_buffer(FiltCtrl2KrnlSpkMsg* msg_buffer);
FiltCtrl2KrnlSpkMsg* deallocate_shm_client_filter_ctrl_2_kernel_spike_msg_buffer(FiltCtrl2KrnlSpkMsg* msg_buffer);
bool write_to_filter_ctrl_2_kernel_spike_msg_buffer(FiltCtrl2KrnlSpkMsg* msg_buffer, FiltCtrl2KrnlSpkMsgType msg_type, FiltCtrl2KrnlSpkMsgAdditional additional_data);
#endif

#endif


