#define DAQCARD_H

#define MAX_NUM_OF_DAQ_CARD 1 					// Increase if there is an additional card
#define MAX_NUM_OF_CHANNEL_PER_DAQ_CARD 16 		

#define DAQ_0_MODEL 				"NI6070E"
#define COMEDI_SUBDEVICE_AI 		0
#define SAMPLING_INTERVAL 			25000     /// DO NOT CHANGE!!!
// #define VOLTAGE_RANGE_6070E 		0	//	+10V,-10V
//#define VOLTAGE_RANGE_6070E 		1	//	Notworking
//#define VOLTAGE_RANGE_6070E 		2	//	Not working
//#define VOLTAGE_RANGE_6070E 		3	//	Not working
#define VOLTAGE_RANGE_6070E 		4	//	+0.5,-0.5V
//#define VOLTAGE_RANGE_6070E 		5	//	+0.25,-0.25V
//#define VOLTAGE_RANGE_6070E 		6	//	+0.1,-0.1V
//#define VOLTAGE_RANGE_6070E 		7	//	+0.05,-0.05V

#define BASELINE_QUANT_6070E 		2048.0
#define LOWEST_VOLTAGE_MV 		-500.0    // when VOLTAGE_RANGE_6070E = 4
#define HIGHEST_VOLTAGE_MV 		500.0
#define VOLTAGE_MULTIPLIER_MV_6070E 	4.096     //   4096/1000.0    (0.5V - (- 0.5V) = 1000 mV)

