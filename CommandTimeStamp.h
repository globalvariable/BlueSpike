#define COMMANDTIMESTAMP_H

#ifndef TYPES_H
#include "Types.h"
#endif

#define COMMAND_TIMESTAMP_BUFF_SIZE 10000

#define COMMAND_TYPE_BYTE	1
typedef char CommandType[COMMAND_TYPE_BYTE];

#define COMMAND_MESSAGE_BYTE	4
typedef char CommandMessage[COMMAND_MESSAGE_BYTE];

typedef struct __CommandTimeStampItem
{
	TimeStamp 		time;
	CommandType 		type;
	CommandMessage 	message;
} CommandTimeStampItem;


typedef CommandTimeStampItem	CommandTimeStampBuff[COMMAND_TIMESTAMP_BUFF_SIZE];

typedef struct __CommandTimeStamp
{
	CommandTimeStampBuff	command_timestamp_buff;
	int					buff_idx_write;
} CommandTimeStamp;

