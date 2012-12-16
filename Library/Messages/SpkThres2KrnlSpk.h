#ifndef SPIKE_THRES_2_KERNEL_SPIKE_H
#define SPIKE_THRES_2_KERNEL_SPIKE_H

//  MESSAGES FROM SPIKE THRESHOLDING TO KERNEL SPIKE

typedef struct __SpkThres2KrnlSpkMsg SpkThres2KrnlSpkMsg;
typedef struct __SpkThres2KrnlSpkMsgItem SpkThres2KrnlSpkMsgItem;
typedef unsigned int SpkThres2KrnlSpkMsgType;
typedef unsigned int SpkThres2KrnlSpkMsgAdditional;

#define SPIKE_THRES_2_KERNEL_SPIKE_MSG_ADDITIONAL_NULL		0


#define SPIKE_THRES_2_KERNEL_SPIKE_MSG_STRING_LENGTH		50

#define SPIKE_THRES_2_KERNEL_SPIKE_MSG_NULL							0
#define SPIKE_THRES_2_KERNEL_SPIKE_MSG_SET_THRESHOLD				1




#include <rtai_shm.h>
#include <rtai_nam2num.h>
#include "MessageBuffersConfig.h"
#include "../../MicroWireArray.h"
#include "../../SpikeThresholding.h"
#ifndef KERNELSPIKE_H		/// insmod KernelSpike errors:  Unknown symbol __strcat_chk, Unknown symbol __sprintf_chk
#include <stdbool.h>
#include <stdio.h>
#include "../Misc/Misc.h"
#include "../../System/ShmSemNum/ShmSemNum.h"
#endif

struct __SpkThres2KrnlSpkMsgItem
{
	SpkThres2KrnlSpkMsgType		msg_type;
	MwaNum					mwa;
	MwaChanNum				mwa_chan_num;
	SpikeThreshold				threshold;
	SpkThres2KrnlSpkMsgAdditional	additional_data;
};

struct __SpkThres2KrnlSpkMsg		// Requests to KernelSpike
{
	SpkThres2KrnlSpkMsgItem		buff[SPIKE_THRES_2_KERNEL_SPIKE_MSG_BUFFER_SIZE];
	unsigned int				buff_write_idx;	// only one message sender can write into this buffer and edit this write index
	unsigned int				buff_read_idx;	// only one request handler can edit this read index
};

#ifndef KERNELSPIKE_H    /// not necessary for KernelSpike
SpkThres2KrnlSpkMsg* allocate_shm_client_spike_thres_2_kernel_spike_msg_buffer(SpkThres2KrnlSpkMsg* msg_buffer);
SpkThres2KrnlSpkMsg* deallocate_shm_client_spike_thres_2_kernel_spike_msg_buffer(SpkThres2KrnlSpkMsg* msg_buffer);
bool write_to_spike_thres_2_kernel_spike_msg_buffer(SpkThres2KrnlSpkMsg* msg_buffer, SpkThres2KrnlSpkMsgType msg_type, MwaNum mwa, MwaChanNum mwa_chan_num, SpikeThreshold threshold, SpkThres2KrnlSpkMsgAdditional additional_data);
#endif

#endif


