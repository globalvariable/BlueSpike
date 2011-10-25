#ifndef TYPES_H
#include "Types.h"
#endif

#define COMMAND_TIMESTAMPS_BUFF_SIZE 10000

#define COMMAND_TYPE_BYTE	1
typedef char CommandType[COMMAND_TYPE_BYTE];

#define COMMAND_MESSAGE_BYTE	4
typedef char CommandMessage[COMMAND_MESSAGE_BYTE];

typedef struct __CommandTimeStamp
{
	TimeStamp 		time;
	CommandType 		type;
	CommandMessage 	message;
} CommandTimeStamp;



