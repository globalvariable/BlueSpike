#define EXPENVICOMMANDTIMESTAMP_H

#ifndef TIMESTAMP_H
#include "TimeStamp.h"
#endif

#define EXP_ENVI_COMMAND_TIMESTAMP_BUFF_SIZE 10000	// Should adjust its size according to the writing frequency to and reading frequency from this buffer. for hihger writing and lower reading, you should increase buffer size to avoid over-writing.  

#define MAX_NUM_OF_EXP_ENVI_ITEMS 1  // Number of event producers and command receiver (ie all skinner box elements belongs to 1 item. Another item added to experimental environment (i.e stimulator) is the second item )

typedef unsigned int ExpEnviCommandType;

typedef double ExpEnviCommandValue;

typedef struct __ExpEnviCommandTimeStampItem
{
	TimeStamp 			time;
	ExpEnviCommandType 	type;
	ExpEnviCommandValue	value;		// i.e stimulator current or water receptacle valve open time etc.
} ExpEnviCommandTimeStampItem;


typedef ExpEnviCommandTimeStampItem	ExpEnviCommandTimeStampBuff[MAX_NUM_OF_EXP_ENVI_ITEMS][EXP_ENVI_COMMAND_TIMESTAMP_BUFF_SIZE];

typedef struct __ExpEnviCommandTimeStamp
{
	ExpEnviCommandTimeStampBuff	timestamp_buff;
	int							buff_idx_write[MAX_NUM_OF_EXP_ENVI_ITEMS];
} ExpEnviCommandTimeStamp;

