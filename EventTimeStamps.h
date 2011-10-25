#ifndef TYPES_H
#include "Types.h"
#endif

#define EVENT_TIMESTAMPS_BUFF_SIZE 10000

#define EVENT_TYPE_BYTE	1
typedef char EventType[EVENT_TYPE_BYTE];

#define EVENT_MESSAGE_BYTE	4
typedef char EventMessage[EVENT_MESSAGE_BYTE];

typedef struct __EventTimeStamp
{
	TimeStamp 	time;
	EventType 	type;
	EventMessage 	message;
}EventTimeStamp;







