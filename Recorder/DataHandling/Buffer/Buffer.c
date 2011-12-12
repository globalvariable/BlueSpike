#include "Buffer.h"

TimeStamp initialize_buffer_reading_start_indexes_and_time_for_recording(void)
{
	int i, j;
	TimeStamp start_time;
	
	while (!(shared_memory->kernel_task_ctrl.kernel_task_idle)) { usleep(1); }
	
	for (i=0; i < MAX_NUM_OF_MWA; i++)
	{
		for (j=0; j<MAX_NUM_OF_CHAN_PER_MWA; j++)
		{
			recording_data_buff_start_idx[i][j] = recording_data->buff_idx_write[i][j];
		}
	}		
	spike_timestamp_buff_start_idx = spike_time_stamp->buff_idx_write;
	for (i=0; i <MAX_NUM_OF_EXP_ENVI_ITEMS; i++)
	{
		exp_envi_event_buff_start_idx[i] = exp_envi_event_time_stamp->buff_idx_write[i];
	}		
	for (i=0; i <MAX_NUM_OF_EXP_ENVI_ITEMS; i++)
	{
		exp_envi_command_buff_start_idx[i] = exp_envi_command_time_stamp->buff_idx_write[i];
	}			
	for (i=0; i <MAX_NUM_OF_MOVING_OBJECTS; i++)
	{
		mov_obj_event_buff_start_idx[i] = mov_obj_event_time_stamp->buff_idx_write[i];
	}		
	for (i=0; i <MAX_NUM_OF_MOVING_OBJECTS; i++)
	{
		mov_obj_command_buff_start_idx[i] = mov_obj_command_time_stamp->buff_idx_write[i];
	}	

	start_time = shared_memory->kernel_task_ctrl.current_time_ns;

	for (i=0; i < MAX_NUM_OF_MWA; i++)
	{
		for (j=0; j<MAX_NUM_OF_CHAN_PER_MWA; j++)
		{
			recording_data_buff_prev_idx[i][j] = recording_data_buff_start_idx[i][j];
		}
	}		
	spike_timestamp_buff_prev_idx = spike_timestamp_buff_start_idx;
	for (i=0; i <MAX_NUM_OF_EXP_ENVI_ITEMS; i++)
	{
		exp_envi_event_buff_prev_idx[i] = exp_envi_event_buff_start_idx[i];
	}		
	for (i=0; i <MAX_NUM_OF_EXP_ENVI_ITEMS; i++)
	{
		exp_envi_command_buff_prev_idx[i] = exp_envi_command_buff_start_idx[i];
	}			
	for (i=0; i <MAX_NUM_OF_MOVING_OBJECTS; i++)
	{
		mov_obj_event_buff_prev_idx[i] = mov_obj_event_buff_start_idx[i];
	}		
	for (i=0; i <MAX_NUM_OF_MOVING_OBJECTS; i++)
	{
		mov_obj_command_buff_prev_idx[i] = mov_obj_command_buff_start_idx[i];
	}	
	
	for (i=0; i < MAX_NUM_OF_MWA; i++)
	{
		for (j=0; j<MAX_NUM_OF_CHAN_PER_MWA; j++)
		{
			recording_data_buff_end_idx[i][j] = recording_data_buff_start_idx[i][j];
		}
	}		
	spike_timestamp_buff_end_idx = spike_timestamp_buff_start_idx;
	for (i=0; i <MAX_NUM_OF_EXP_ENVI_ITEMS; i++)
	{
		exp_envi_event_buff_end_idx[i] = exp_envi_event_buff_start_idx[i];
	}		
	for (i=0; i <MAX_NUM_OF_EXP_ENVI_ITEMS; i++)
	{
		exp_envi_command_buff_end_idx[i] = exp_envi_command_buff_start_idx[i];
	}			
	for (i=0; i <MAX_NUM_OF_MOVING_OBJECTS; i++)
	{
		mov_obj_event_buff_end_idx[i] = mov_obj_event_buff_start_idx[i];
	}		
	for (i=0; i <MAX_NUM_OF_MOVING_OBJECTS; i++)
	{
		mov_obj_command_buff_end_idx[i] = mov_obj_command_buff_start_idx[i];
	}	
	return start_time;	
}

void get_buffer_reading_range_indexes_for_recording(void)
{
	int i, j;
	
	for (i=0; i < MAX_NUM_OF_MWA; i++)
	{
		for (j=0; j<MAX_NUM_OF_CHAN_PER_MWA; j++)
		{
			recording_data_buff_prev_idx[i][j] = recording_data_buff_end_idx[i][j];
		}
	}		
	spike_timestamp_buff_prev_idx = spike_timestamp_buff_end_idx;
	for (i=0; i <MAX_NUM_OF_EXP_ENVI_ITEMS; i++)
	{
		exp_envi_event_buff_prev_idx[i] = exp_envi_event_buff_end_idx[i];
	}		
	for (i=0; i <MAX_NUM_OF_EXP_ENVI_ITEMS; i++)
	{
		exp_envi_command_buff_prev_idx[i] = exp_envi_command_buff_end_idx[i];
	}			
	for (i=0; i <MAX_NUM_OF_MOVING_OBJECTS; i++)
	{
		mov_obj_event_buff_prev_idx[i] = mov_obj_event_buff_end_idx[i];
	}		
	for (i=0; i <MAX_NUM_OF_MOVING_OBJECTS; i++)
	{
		mov_obj_command_buff_prev_idx[i] = mov_obj_command_buff_end_idx[i];
	}		
	
	while (!(shared_memory->kernel_task_ctrl.kernel_task_idle)) { usleep(1); }	
	
	for (i=0; i < MAX_NUM_OF_MWA; i++)
	{
		for (j=0; j<MAX_NUM_OF_CHAN_PER_MWA; j++)
		{
			recording_data_buff_end_idx[i][j] = recording_data->buff_idx_write[i][j];
		}
	}		
	spike_timestamp_buff_end_idx = spike_time_stamp->buff_idx_write;
	for (i=0; i <MAX_NUM_OF_EXP_ENVI_ITEMS; i++)
	{
		exp_envi_event_buff_end_idx[i] = exp_envi_event_time_stamp->buff_idx_write[i];
	}		
	for (i=0; i <MAX_NUM_OF_EXP_ENVI_ITEMS; i++)
	{
		exp_envi_command_buff_end_idx[i] = exp_envi_command_time_stamp->buff_idx_write[i];
	}			
	for (i=0; i <MAX_NUM_OF_MOVING_OBJECTS; i++)
	{
		mov_obj_event_buff_end_idx[i] = mov_obj_event_time_stamp->buff_idx_write[i];
	}		
	for (i=0; i <MAX_NUM_OF_MOVING_OBJECTS; i++)
	{
		mov_obj_command_buff_end_idx[i] = mov_obj_command_time_stamp->buff_idx_write[i];
	}	

	return;
}


TimeStamp get_buffer_reading_end_indexes_and_time_for_recording(void)
{
	int i, j;
	TimeStamp end_time;	
	for (i=0; i < MAX_NUM_OF_MWA; i++)
	{
		for (j=0; j<MAX_NUM_OF_CHAN_PER_MWA; j++)
		{
			recording_data_buff_prev_idx[i][j] = recording_data_buff_end_idx[i][j];
		}
	}		
	spike_timestamp_buff_prev_idx = spike_timestamp_buff_end_idx;
	for (i=0; i <MAX_NUM_OF_EXP_ENVI_ITEMS; i++)
	{
		exp_envi_event_buff_prev_idx[i] = exp_envi_event_buff_end_idx[i];
	}		
	for (i=0; i <MAX_NUM_OF_EXP_ENVI_ITEMS; i++)
	{
		exp_envi_command_buff_prev_idx[i] = exp_envi_command_buff_end_idx[i];
	}			
	for (i=0; i <MAX_NUM_OF_MOVING_OBJECTS; i++)
	{
		mov_obj_event_buff_prev_idx[i] = mov_obj_event_buff_end_idx[i];
	}		
	for (i=0; i <MAX_NUM_OF_MOVING_OBJECTS; i++)
	{
		mov_obj_command_buff_prev_idx[i] = mov_obj_command_buff_end_idx[i];
	}		
	
	while (!(shared_memory->kernel_task_ctrl.kernel_task_idle)) { usleep(1); }	
	
	for (i=0; i < MAX_NUM_OF_MWA; i++)
	{
		for (j=0; j<MAX_NUM_OF_CHAN_PER_MWA; j++)
		{
			recording_data_buff_end_idx[i][j] = recording_data->buff_idx_write[i][j];
		}
	}		
	spike_timestamp_buff_end_idx = spike_time_stamp->buff_idx_write;
	for (i=0; i <MAX_NUM_OF_EXP_ENVI_ITEMS; i++)
	{
		exp_envi_event_buff_end_idx[i] = exp_envi_event_time_stamp->buff_idx_write[i];
	}		
	for (i=0; i <MAX_NUM_OF_EXP_ENVI_ITEMS; i++)
	{
		exp_envi_command_buff_end_idx[i] = exp_envi_command_time_stamp->buff_idx_write[i];
	}			
	for (i=0; i <MAX_NUM_OF_MOVING_OBJECTS; i++)
	{
		mov_obj_event_buff_end_idx[i] = mov_obj_event_time_stamp->buff_idx_write[i];
	}		
	for (i=0; i <MAX_NUM_OF_MOVING_OBJECTS; i++)
	{
		mov_obj_command_buff_end_idx[i] = mov_obj_command_time_stamp->buff_idx_write[i];
	}	

	end_time = shared_memory->kernel_task_ctrl.current_time_ns;

	return end_time;	
}
