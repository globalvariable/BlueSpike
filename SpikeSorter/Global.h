/***************************************************************************
                          Global.h  -  description
                             -------------------
    copyright            : (C) 2011 by Mehmet Kocaturk
    email                : mehmet.kocaturk@boun.edu.tr
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include <stdlib.h>
#include <stdbool.h>
#include <rtai_shm.h>
#include <rtai_nam2num.h>

#include <rtai_comedi.h>

#include <meschach/matrix.h>

#define SHMSIZ  0
#define NUM_OF_CHAN 16 
#define NUM_OF_SAMP_IN_BUFF 40000  
#define BUFFNAME  "SHBUFF"
#define SPIKE_MIN_END_SAMP_NUM   15

#define NUM_OF_TEMP_PER_CHAN 3
#define NUM_OF_SAMP_PER_SPIKE 30

typedef struct{
	union
	{
		struct{
			unsigned Bit0:1;
			unsigned Bit1:1;
			unsigned Bit2:1;
			unsigned Bit3:1;
			unsigned Bit4:1;
			unsigned Bit5:1;
			unsigned Bit6:1;						
			unsigned Bit7:1;
		};
		unsigned AllCommand;
	} Command;
	union
	{
		struct{
			unsigned Bit0:1;
			unsigned Bit1:1;
			unsigned Bit2:1;
			unsigned Bit3:1;
			unsigned Bit4:1;
			unsigned Bit5:1;
			unsigned Bit6:1;						
			unsigned Bit7:1;
		};
		unsigned AllStatus;
	} Status;
	short int ShortInt_Status0;
	short int ShortInt_Status1;	 	
} ExpEnv; 

typedef struct sample_data
{
	float data[NUM_OF_CHAN];
} scan_data;

typedef struct spk_data
{
	int data[NUM_OF_CHAN];
} spike_data;

typedef struct templ_data
{
	double template[NUM_OF_CHAN][NUM_OF_TEMP_PER_CHAN][NUM_OF_SAMP_PER_SPIKE];   //mean
	double S[NUM_OF_CHAN][NUM_OF_TEMP_PER_CHAN][NUM_OF_SAMP_PER_SPIKE][NUM_OF_SAMP_PER_SPIKE]; //covariance
	double  inv_S[NUM_OF_CHAN][NUM_OF_TEMP_PER_CHAN][NUM_OF_SAMP_PER_SPIKE][NUM_OF_SAMP_PER_SPIKE];
	double sqrt_det_S[NUM_OF_CHAN][NUM_OF_TEMP_PER_CHAN];
	double log_det_S[NUM_OF_CHAN][NUM_OF_TEMP_PER_CHAN];
	double diff_thres[NUM_OF_CHAN][NUM_OF_TEMP_PER_CHAN];
	bool sorting_on[NUM_OF_CHAN][NUM_OF_TEMP_PER_CHAN];
	bool include_unit[NUM_OF_CHAN][NUM_OF_TEMP_PER_CHAN];
} template_matching_data;

typedef struct{
	union
	{
		struct{
			unsigned Bit0:1;
			unsigned Bit1:1;
			unsigned Bit2:1;
			unsigned Bit3:1;
			unsigned Bit4:1;
			unsigned Bit5:1;
			unsigned Bit6:1;						
			unsigned Bit7:1;
			unsigned Bit8:1;
			unsigned Bit9:1;
			unsigned Bit10:1;
			unsigned Bit11:1;
			unsigned Bit12:1;
			unsigned Bit13:1;
			unsigned Bit14:1;						
			unsigned Bit15:1;
		};
		unsigned AllFlags;
	} StatusFlags;
} StaFlag; 

typedef struct{
	union
	{
		struct{
			unsigned Bit0:1;
			unsigned Bit1:1;
			unsigned Bit2:1;
			unsigned Bit3:1;
			unsigned Bit4:1;
			unsigned Bit5:1;
			unsigned Bit6:1;						
			unsigned Bit7:1;
			unsigned Bit8:1;
			unsigned Bit9:1;
			unsigned Bit10:1;
			unsigned Bit11:1;
			unsigned Bit12:1;
			unsigned Bit13:1;
			unsigned Bit14:1;						
			unsigned Bit15:1;
			unsigned Bit16:1;
			unsigned Bit17:1;
			unsigned Bit18:1;
			unsigned Bit19:1;
			unsigned Bit20:1;						
			unsigned Bit21:1;
			unsigned Bit22:1;						
			unsigned Bit23:1;
		};
		unsigned AllFlags;
	} CommandFlags;
} CmndFlag; 

typedef struct buff_data 
{
	int scan_number_write;
	int scan_number_read;
	scan_data scan[NUM_OF_SAMP_IN_BUFF];
	scan_data filtered_scan[NUM_OF_SAMP_IN_BUFF];
	spike_data spike_peak[NUM_OF_SAMP_IN_BUFF];
	spike_data spike_end[NUM_OF_SAMP_IN_BUFF];	
	float Threshold[NUM_OF_CHAN];
	bool in_spike[NUM_OF_CHAN];	
	bool filter_on;
	ExpEnv Environment[NUM_OF_SAMP_IN_BUFF];
	ExpEnv Curr_Environment;
	template_matching_data spike_template; 	
	bool sorting_on;
	spike_data sorted_spike_data[NUM_OF_SAMP_IN_BUFF];
	bool highpass_400Hz_on;
	StaFlag	RTStatusFlags[NUM_OF_SAMP_IN_BUFF];
	StaFlag	Curr_RTStatusFlags;
	int jitter[1000];
	int jitter_idx;	
	bool lowpass_4th_on;
	CmndFlag RTCommandFlags[NUM_OF_SAMP_IN_BUFF];
	CmndFlag Curr_RTCommandFlags;
} buff_data_struct;

buff_data_struct *buff;
