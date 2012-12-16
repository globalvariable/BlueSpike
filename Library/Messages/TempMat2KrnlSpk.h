#ifndef TEMPLATE_MATCHING_2_KERNEL_SPIKE_H
#define TEMPLATE_MATCHING_2_KERNEL_SPIKE_H

//  MESSAGES FROM SPIKE THRESHOLDING TO KERNEL SPIKE

typedef struct __TempMat2KrnlSpkMsg TempMat2KrnlSpkMsg;
typedef struct __TempMat2KrnlSpkMsgItem TempMat2KrnlSpkMsgItem;
typedef unsigned int TempMat2KrnlSpkMsgType;
typedef unsigned int TempMat2KrnlSpkMsgAdditional;

#define TEMPLATE_MATCHING_2_KERNEL_SPIKE_MSG_ADDITIONAL_NULL		0


#define TEMPLATE_MATCHING_2_KERNEL_SPIKE_MSG_STRING_LENGTH		50

#define TEMPLATE_MATCHING_2_KERNEL_SPIKE_MSG_NULL							0
#define TEMPLATE_MATCHING_2_KERNEL_SPIKE_MSG_SET_UNIT_SORTING_ON		1
#define TEMPLATE_MATCHING_2_KERNEL_SPIKE_MSG_SET_UNIT_SORTING_OFF		2
#define TEMPLATE_MATCHING_2_KERNEL_SPIKE_MSG_INCLUDE_UNIT				3
#define TEMPLATE_MATCHING_2_KERNEL_SPIKE_MSG_EXCLUDE_UNIT				4
#define TEMPLATE_MATCHING_2_KERNEL_SPIKE_MSG_SET_PROBAB_THRES			5

#include <rtai_shm.h>
#include <rtai_nam2num.h>
#include "MessageBuffersConfig.h"
#include "../../MicroWireArray.h"
#include "../../TemplateMatchingData.h"
#ifndef KERNELSPIKE_H		/// insmod KernelSpike errors:  Unknown symbol __strcat_chk, Unknown symbol __sprintf_chk
#include <stdbool.h>
#include <stdio.h>
#include "../Misc/Misc.h"
#include "../../System/ShmSemNum/ShmSemNum.h"
#endif

struct __TempMat2KrnlSpkMsgItem
{
	TempMat2KrnlSpkMsgType		msg_type;
	MwaNum					mwa;
	MwaChanNum				mwa_chan_num;
	UnitNum						unit_num;
	TemplateProbabilityThres		threshold;
	TempMat2KrnlSpkMsgAdditional	additional_data;
};

struct __TempMat2KrnlSpkMsg		// Requests to KernelSpike
{
	TempMat2KrnlSpkMsgItem		buff[TEMPLATE_MATCHING_2_KERNEL_SPIKE_MSG_BUFFER_SIZE];
	unsigned int				buff_write_idx;	// only one message sender can write into this buffer and edit this write index
	unsigned int				buff_read_idx;	// only one request handler can edit this read index
};

#ifndef KERNELSPIKE_H    /// not necessary for KernelSpike
TempMat2KrnlSpkMsg* allocate_shm_client_template_matching_2_kernel_spike_msg_buffer(TempMat2KrnlSpkMsg* msg_buffer);
TempMat2KrnlSpkMsg* deallocate_shm_client_template_matching_2_kernel_spike_msg_buffer(TempMat2KrnlSpkMsg* msg_buffer);
bool write_to_template_matching_2_kernel_spike_msg_buffer(TempMat2KrnlSpkMsg* msg_buffer, TempMat2KrnlSpkMsgType msg_type, MwaNum mwa, MwaChanNum mwa_chan_num, UnitNum unit_num, TemplateProbabilityThres threshold, TempMat2KrnlSpkMsgAdditional additional_data);
#endif

#endif


