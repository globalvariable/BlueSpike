#define EXPENVIEVENTTIMESTAMP_H    // Experimental Environment Event Timestamps

#ifndef TIMESTAMP_H
#include "TimeStamp.h"
#endif

#include "ExpEnviCommandTimeStamp.h"

#define EXP_ENVI_EVENT_TIMESTAMP_BUFF_SIZE 10000

#define MAX_NUM_OF_EXP_ENVI_EVENT_OWNER MAX_NUM_OF_EXP_ENVI_COMMAND_RECEIVER

typedef short unsigned int ExpEnviEventType;

typedef struct __ExpEnviEventTimeStampItem
{
	TimeStamp 		time;
	ExpEnviEventType 	type;
}ExpEnviEventTimeStampItem;

typedef ExpEnviEventTimeStampItem	ExpEnviEventTimeStampBuff[MAX_NUM_OF_EXP_ENVI_EVENT_OWNER][EXP_ENVI_EVENT_TIMESTAMP_BUFF_SIZE];

typedef struct __ExpEnviEventTimeStamp
{
	ExpEnviEventTimeStampBuff	timestamp_buff;
	int						buff_idx_write[MAX_NUM_OF_EXP_ENVI_EVENT_OWNER];
} ExpEnviEventTimeStamp;


