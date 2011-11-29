#define DAQCARD_H

#define MAX_NUM_OF_DAQ_CARD 1 					// Increase if there is an additional card
#define MAX_NUM_OF_CHANNEL_PER_DAQ_CARD 16 		

#define COMEDI_SUBDEVICE_AI 		0
#define SAMPLING_INTERVAL 			25000     /// DO NOT CHANGE!!!
#define VOLTAGE_RANGE_6070E 		4

#define BASELINE_QUANT_6070E 		2048.0
#define LOWEST_VOLTAGE_MV 		-1000.0    // when VOLTAGE_RANGE_6070E = 4
#define HIGHEST_VOLTAGE_MV 		1000.0
#define VOLTAGE_MULTIPLIER_MV_6070E 	4.096     //   4096/1000.0

