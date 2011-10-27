#define SPIKEENDDATA_H

#define SPIKE_END_DATA_BUFF_SIZE 10000

typedef struct __SpikeEndItem
{
	int idx;    // index of SpikeEndData in filtered recording data buffer 
	int mwa;
	int chan;
} SpikeEndItem;

typedef SpikeEndItem	SpikeEndBuff[SPIKE_END_DATA_BUFF_SIZE];

typedef struct __SpikeEnd
{
	SpikeEndBuff		spike_end_data_buff;
	int				buff_idx_write;
} SpikeEnd;

#define SPIKE_MIN_END_SAMP_NUM   15
