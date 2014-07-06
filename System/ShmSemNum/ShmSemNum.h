#ifndef SHM_NUM_H
#define SHM_NUM_H

#include "../RtTasksData/RtTasksData.h"

// NOTE:    SEMAPHORE AND SHARED MEMORY CANNOT USE THE SAME OBJECT NUMBER 

///////////////  SEMAPHORE /////////////////////////////////
#define SEM_NUM_EXP_ENVI_RX_BUFFER			101
#define SEM_NUM_EXP_ENVI_TX_BUFFER			102







///////////////  SHARED MEMORY /////////////////////////////////

#define SHM_NUM_RT_TASKS_DATA							200

#define SHM_NUM_BLUESPIKE_RECORDING_DATA				201
#define SHM_NUM_BLUESPIKE_SORTED_SPIKES				202


#define SHM_NUM_EXP_ENVI_RX_BUFFER			251   // should be shorter than or equal to 6 for correct shm numbering
#define SHM_NUM_EXP_ENVI_TX_BUFFER			252

#define SHM_NUM_DAQ_CONFIG_2_KERNEL_SPIKE			401
#define SHM_NUM_FILTER_CTRL_2_KERNEL_SPIKE			402
#define SHM_NUM_SPIKE_THRES_2_KERNEL_SPIKE			403	
#define SHM_NUM_TEMPLATE_MATCHING_2_KERNEL_SPIKE	404

#define SHM_NUM_TRIAL_HAND_2_EXP_ENVI_HAND			501			
#define SHM_NUM_TRIAL_HAND_2_MOV_OBJ_HAND			502
#define SHM_NUM_TRIAL_HAND_2_NEURAL_NET			503
#define SHM_NUM_TRIAL_HAND_2_SPIKE_GEN				504
#define SHM_NUM_TRIAL_HAND_2_NEU_REC_HAND			505

#define SHM_NUM_EXP_ENVI_HAND_2_TRIAL_HAND			520


#define SHM_NUM_MOV_OBJ_HAND_2_NEURAL_NET			540		// take care of multi threaded messaging btw mov obj hand and neural net
#define SHM_NUM_MOV_OBJ_HAND_2_TRIAL_HAND			550			


#define SHM_NUM_NEURAL_NET_2_MOV_OBJ_HAND			560		// take care of multi threaded messaging btw mov obj hand and neural net
#define SHM_NUM_NEURAL_NET_2_TRIAL_HAND			570

#define SHM_NUM_SPIKE_GEN_2_TRIAL_HAND				581

#define SHM_NUM_NEU_REC_HAND_2_TRIAL_HAND			591

#endif
