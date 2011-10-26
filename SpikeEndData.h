#define SPIKE_END_DATA_H

#define SPIKE_END_DATA_BUFF_SIZE 10000

typedef struct __SpikeEndData
{
	int idx;    // index of SpikeEndData in buffer 
	int mwa;
	int chan;
} SpikeEndData;


