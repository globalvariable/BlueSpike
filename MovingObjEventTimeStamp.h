#define MOVINGOBJEVENTTIMESTAMP_H


#include "MovingObjCommandTimeStamp.h"
#ifndef TIMESTAMP_H
#include "TimeStamp.h"
#endif

#define MOVING_OBJ_EVENT_TIMESTAMP_BUFF_SIZE 10000		// Should adjust its size according to the writing frequency to and reading frequency from this buffer. for hihger writing and lower reading, you should increase buffer size to avoid over-writing.  



typedef struct __MovingObjEventTimeStampItem
{
	TimeStamp 			time;				// BE CAREFUL WHILE WRITING INTO THIS BUFFER: DO NOT FORGET TO CLEAR EACH COMPONENT OF MOVING OBJECT IF NOT USED.
	MovingObjSpeed		Speed[MAX_NUM_OF_COMPONENT_PER_MOVING_OBJECT];             // Speed data for each component of moving object 
	MovingObjDirection		Direction[MAX_NUM_OF_COMPONENT_PER_MOVING_OBJECT];	     // Current direction of each component of moving object 
	MovingObjLocation		Location[MAX_NUM_OF_COMPONENT_PER_MOVING_OBJECT];  	    // Location data of each component of moving object 
} MovingObjEventTimeStampItem;

typedef MovingObjEventTimeStampItem	MovingObjEventTimeStampBuff[MAX_NUM_OF_MOVING_OBJECTS][MOVING_OBJ_EVENT_TIMESTAMP_BUFF_SIZE];

typedef struct __MovingObjEventTimeStamp
{
	MovingObjEventTimeStampBuff	timestamp_buff;
	int						buff_idx_write[MAX_NUM_OF_MOVING_OBJECTS];
} MovingObjEventTimeStamp;


