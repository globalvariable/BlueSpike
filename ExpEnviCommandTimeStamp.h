#define EXPENVICOMMANDTIMESTAMP_H

#ifndef TIMESTAMP_H
#include "TimeStamp.h"
#endif

#define EXP_ENVI_COMMAND_TIMESTAMP_BUFF_SIZE 10000

#define MAX_NUM_OF_EXP_ENVI_COMMAND_RECEIVER 1

typedef short unsigned int ExpEnviCommandType;

typedef struct __ExpEnviCommandTimeStampItem
{
	TimeStamp 			time;
	ExpEnviCommandType 	type;
} ExpEnviCommandTimeStampItem;


typedef ExpEnviCommandTimeStampItem	ExpEnviCommandTimeStampBuff[MAX_NUM_OF_EXP_ENVI_COMMAND_RECEIVER][EXP_ENVI_COMMAND_TIMESTAMP_BUFF_SIZE];

typedef struct __ExpEnviCommandTimeStamp
{
	ExpEnviCommandTimeStampBuff	timestamp_buff;
	int							buff_idx_write[MAX_NUM_OF_EXP_ENVI_COMMAND_RECEIVER];
} ExpEnviCommandTimeStamp;

