#define MOVINGOBJCOMMANDTIMESTAMP_H

#ifndef TYPES_H
#include "Types.h"
#endif

#define MOVING_OBJ_COMMAND_TIMESTAMP_BUFF_SIZE 10000

#define MAX_NUM_OF_MOVING_OBJECTS 1    // i.e. number of animals or robot in experimental environment

#define MAX_NUM_OF_COMPONENT_PER_MOVING_OBJECT 1     //  i.e. object moving in 3D with three stepper motors, animal moving in 2D, robot with 5 degree of freedom

typedef enum __MovingObjDirection{BACKWARD, STATIONARY, FORWARD} MovingObjDirection;   // Forward increases Location parameter in that dimension
 
 typedef short int MovingObjSpeed;
 
typedef struct __MovingObjCommandTimeStampItem
{
	TimeStamp 			time;
	MovingObjSpeed		Speed[MAX_NUM_OF_COMPONENT_PER_MOVING_OBJECT];             // Location data for each component of moving object 
	MovingObjDirection		Direction[MAX_NUM_OF_COMPONENT_PER_MOVING_OBJECT];	     // Current direction of each component of moving object 
} MovingObjCommandTimeStampItem;

typedef MovingObjCommandTimeStampItem	MovingObjCommandTimeStampBuff[MAX_NUM_OF_MOVING_OBJECTS][MOVING_OBJ_COMMAND_TIMESTAMP_BUFF_SIZE];

typedef struct __MovingObjCommandTimeStamp
{
	MovingObjCommandTimeStampBuff	timestamp_buff;
	int							buff_idx_write[MAX_NUM_OF_MOVING_OBJECTS];
} MovingObjCommandTimeStamp;


