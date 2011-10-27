#define DAQCARD_H

#include <linux/comedi.h>
#include <linux/comedilib.h>


#define MAX_NUM_OF_DAQ_CARD 1 
#define MAX_NUM_OF_CHANNEL_PER_DAQ_CARD 16 

#define COMEDI_SUBDEVICE_AI 0
#define SAMPLING_INTERVAL 25000
#define VOLTAGE_RANGE_6070E 4

comedi_t* ni6070_comedi_dev[MAX_NUM_OF_DAQ_CARD];
comedi_cmd ni6070_comedi_cmd[MAX_NUM_OF_DAQ_CARD];
unsigned ni6070_comedi_chanlist[MAX_NUM_OF_DAQ_CARD][MAX_NUM_OF_CHANNEL_PER_DAQ_CARD];
void *comedi_map_ptr[MAX_NUM_OF_DAQ_CARD];
int comedi_buff_size[MAX_NUM_OF_DAQ_CARD];

int ni6070_comedi_configure(int card_number);
void print_cmd(int card_number);
