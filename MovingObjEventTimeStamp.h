#define MOVINGOBJEVENTTIMESTAMP_H


#include "MovingObjCommandTimeStamp.h"
#ifndef TIMESTAMP_H
#include "TimeStamp.h"
#endif

#define MOVING_OBJ_EVENT_TIMESTAMP_BUFF_SIZE 10000

typedef double MovingObjLocation;

typedef struct __MovingObjEventTimeStampItem
{
	TimeStamp 			time;
	MovingObjLocation		Location[MAX_NUM_OF_COMPONENT_PER_MOVING_OBJECT];             // Location data for each component of moving object 
	MovingObjDirection		Direction[MAX_NUM_OF_COMPONENT_PER_MOVING_OBJECT];	     // Current direction of each component of moving object 
} MovingObjEventTimeStampItem;

typedef MovingObjEventTimeStampItem	MovingObjEventTimeStampBuff[MAX_NUM_OF_MOVING_OBJECTS][MOVING_OBJ_EVENT_TIMESTAMP_BUFF_SIZE];

typedef struct __MovingObjEventTimeStamp
{
	MovingObjEventTimeStampBuff	timestamp_buff;
	int						buff_idx_write[MAX_NUM_OF_MOVING_OBJECTS];
} MovingObjEventTimeStamp;


