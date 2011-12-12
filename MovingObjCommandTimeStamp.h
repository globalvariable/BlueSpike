#define MOVINGOBJCOMMANDTIMESTAMP_H

#ifndef TIMESTAMP_H
#include "TimeStamp.h"
#endif

#define MOVING_OBJ_COMMAND_TIMESTAMP_BUFF_SIZE 10000   // Should adjust its size according to the writing frequency to and reading frequency from this buffer. for hihger writing and lower reading, you should increase buffer size to avoid over-writing.  

#define MAX_NUM_OF_MOVING_OBJECTS 1    // i.e. number of animals or robot in experimental environment

#define MAX_NUM_OF_COMPONENT_PER_MOVING_OBJECT 1     //  i.e. object moving in 3D with three stepper motors, animal moving in 2D, robot with 5 degree of freedom

typedef enum __MovingObjDirection{NO_DIRECTION_DATA, BACKWARD, STATIONARY, FORWARD } MovingObjDirection;   // Forward increases Location parameter in that dimension
 
 typedef double MovingObjSpeed;
 
 typedef double MovingObjLocation;
 
typedef struct __MovingObjCommandTimeStampItem
{
	TimeStamp 			time;				// BE CAREFUL WHILE WRITING INTO THIS BUFFER: DO NOT FORGET TO CLEAR EACH COMPONENT OF MOVING OBJECT IF NOT USED.
	MovingObjSpeed		Speed[MAX_NUM_OF_COMPONENT_PER_MOVING_OBJECT];             // Location data for each component of moving object 
	MovingObjDirection		Direction[MAX_NUM_OF_COMPONENT_PER_MOVING_OBJECT];	     // Current direction of each component of moving object 
	MovingObjLocation		Location[MAX_NUM_OF_COMPONENT_PER_MOVING_OBJECT];  
} MovingObjCommandTimeStampItem;

typedef MovingObjCommandTimeStampItem	MovingObjCommandTimeStampBuff[MAX_NUM_OF_MOVING_OBJECTS][MOVING_OBJ_COMMAND_TIMESTAMP_BUFF_SIZE];

typedef struct __MovingObjCommandTimeStamp
{
	MovingObjCommandTimeStampBuff	timestamp_buff;
	int							buff_idx_write[MAX_NUM_OF_MOVING_OBJECTS];
} MovingObjCommandTimeStamp;


