#define EVENTTIMESTAMP_H

#ifndef TYPES_H
#include "Types.h"
#endif

#define EVENT_TIMESTAMP_BUFF_SIZE 10000

#define EVENT_TYPE_BYTE	1
typedef char EventType[EVENT_TYPE_BYTE];

#define EVENT_MESSAGE_BYTE	4
typedef char EventMessage[EVENT_MESSAGE_BYTE];

typedef struct __EventTimeStampItem
{
	TimeStamp 	time;
	EventType 	type;
	EventMessage 	message;
}EventTimeStampItem;

typedef EventTimeStampItem	EventTimeStampBuff[EVENT_TIMESTAMP_BUFF_SIZE];

typedef struct __EventTimeStamp
{
	EventTimeStampBuff	event_timestamp_buff;
	int					buff_idx_write;
} EventTimeStamp;


