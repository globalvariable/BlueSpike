#ifndef EXPENVIEVENTTIMESTAMP_H
#define EXPENVIEVENTTIMESTAMP_H    // Experimental Environment Event Timestamps

#include "TimeStamp.h"
#include "ExpEnviCommandTimeStamp.h"

#define EXP_ENVI_EVENT_TIMESTAMP_BUFF_SIZE 10000   // Should adjust its size according to the writing frequency to and reading frequency from this buffer. for hihger writing and lower reading, you should increase buffer size to avoid over-writing.  

typedef unsigned int ExpEnviEventType;

typedef double ExpEnviEventValue;

typedef struct __ExpEnviEventTimeStampItem
{
	TimeStamp 		time;
	ExpEnviEventType 	type;
	ExpEnviEventValue	value;
}ExpEnviEventTimeStampItem;

typedef ExpEnviEventTimeStampItem	ExpEnviEventTimeStampBuff[MAX_NUM_OF_EXP_ENVI_ITEMS][EXP_ENVI_EVENT_TIMESTAMP_BUFF_SIZE];

typedef struct __ExpEnviEventTimeStamp
{
	ExpEnviEventTimeStampBuff	timestamp_buff;
	int						buff_idx_write[MAX_NUM_OF_EXP_ENVI_ITEMS];
} ExpEnviEventTimeStamp;



#endif
