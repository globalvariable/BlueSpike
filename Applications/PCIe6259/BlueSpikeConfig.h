#ifndef BLUESPIKE_CONFIG_H
#define BLUESPIKE_CONFIG_H



///               RT TASK CONFIG

#define BLUESPIKE_DAQ_PERIOD 							SAMPLING_INTERVAL*NUM_OF_SCAN		//1 ms
#define BLUESPIKE_DAQ_TASK_NAME						nam2num("BLUDAQ")
#define BLUESPIKE_DAQ_TASK_PRIORITY 					0	// highest priority

#define BLUESPIKE_DAQ_STACK_SIZE 						10000
#define BLUESPIKE_DAQ_MSG_SIZE 						1000
#define BLUESPIKE_DAQ_CPU_ID 							1
#define BLUESPIKE_DAQ_CPU_THREAD_ID					0
#define BLUESPIKE_DAQ_CPU_THREAD_TASK_ID			1
#define BLUESPIKE_DAQ_PASS_DATA 						0
#define BLUESPIKE_DAQ_SIGNAL 							0
#define BLUESPIKE_DAQ_POLICY 							SCHED_FIFO
#define BLUESPIKE_DAQ_USES_FLOATING_POINT 			1
#define BLUESPIKE_DAQ_POSITIVE_JITTER_THRES	 		BLUESPIKE_DAQ_PERIOD / 5		// 100 us
#define BLUESPIKE_DAQ_NEGATIVE_JITTER_THRES	 		BLUESPIKE_DAQ_PERIOD / 5

#define BLUESPIKE_PERIODIC_PERIOD 							BLUESPIKE_DAQ_PERIOD		//1 ms
#define BLUESPIKE_PERIODIC_TASK_NAME						nam2num("BLUPER")
#define BLUESPIKE_PERIODIC_TASK_PRIORITY 					1

#define BLUESPIKE_PERIODIC_STACK_SIZE 					BLUESPIKE_DAQ_STACK_SIZE
#define BLUESPIKE_PERIODIC_MSG_SIZE 						BLUESPIKE_DAQ_MSG_SIZE
#define BLUESPIKE_PERIODIC_CPU_ID 							BLUESPIKE_DAQ_CPU_ID 
#define BLUESPIKE_PERIODIC_CPU_THREAD_ID					BLUESPIKE_DAQ_CPU_THREAD_ID
#define BLUESPIKE_PERIODIC_CPU_THREAD_TASK_ID			0	
#define BLUESPIKE_PERIODIC_PASS_DATA 						BLUESPIKE_DAQ_PASS_DATA
#define BLUESPIKE_PERIODIC_SIGNAL 							BLUESPIKE_DAQ_SIGNAL
#define BLUESPIKE_PERIODIC_POLICY 							BLUESPIKE_DAQ_POLICY
#define BLUESPIKE_PERIODIC_USES_FLOATING_POINT 			BLUESPIKE_DAQ_USES_FLOATING_POINT 
#define BLUESPIKE_PERIODIC_POSITIVE_JITTER_THRES	 		BLUESPIKE_PERIODIC_PERIOD / 5
#define BLUESPIKE_PERIODIC_NEGATIVE_JITTER_THRES	 	BLUESPIKE_PERIODIC_PERIOD / 5


#define START_RT_TIMER_PERIOD 				50000		// 50 us

// KernelSpike


// TrialHandler
#define TRIAL_HANDLER_PERIOD 						2000000		//2 ms
#define TRIAL_HANDLER_TASK_NAME					nam2num("TRIALHND")
#define TRIAL_HANDLER_TASK_PRIORITY				300
#define TRIAL_HANDLER_STACK_SIZE 					10000
#define TRIAL_HANDLER_MSG_SIZE 					1000
#define TRIAL_HANDLER_POLICY 						SCHED_FIFO
#define TRIAL_HANDLER_CPU_ID						0
#define TRIAL_HANDLER_CPU_THREAD_ID				0
#define TRIAL_HANDLER_CPU_THREAD_TASK_ID		0
#define TRIAL_HANDLER_POSITIVE_JITTER_THRES	 	200000		//0.1 ms
#define TRIAL_HANDLER_NEGATIVE_JITTER_THRES	 	200000

// TrialDurationHandler
#define TRIAL_DURATION_HANDLER_PERIOD 						TRIAL_HANDLER_PERIOD		//1 ms
#define TRIAL_DURATION_HANDLER_TASK_NAME					nam2num("TRIALDUR")
#define TRIAL_DURATION_HANDLER_TASK_PRIORITY				301
#define TRIAL_DURATION_HANDLER_STACK_SIZE 					10000
#define TRIAL_DURATION_HANDLER_MSG_SIZE 						1000
#define TRIAL_DURATION_HANDLER_POLICY 						SCHED_FIFO
#define TRIAL_DURATION_HANDLER_CPU_ID						TRIAL_HANDLER_CPU_ID
#define TRIAL_DURATION_HANDLER_CPU_THREAD_ID				TRIAL_HANDLER_CPU_THREAD_ID
#define TRIAL_DURATION_HANDLER_CPU_THREAD_TASK_ID			TRIAL_HANDLER_CPU_THREAD_TASK_ID + 1
#define TRIAL_DURATION_HANDLER_POSITIVE_JITTER_THRES	 	TRIAL_HANDLER_POSITIVE_JITTER_THRES		//0.1 ms
#define TRIAL_DURATION_HANDLER_NEGATIVE_JITTER_THRES	 	TRIAL_HANDLER_NEGATIVE_JITTER_THRES

// ExpEnviHandler
#define EXP_ENVI_HANDLER_PERIOD 						TRIAL_HANDLER_PERIOD		//1 ms
#define EXP_ENVI_HANDLER_TASK_NAME					nam2num("ENVIHND")
#define EXP_ENVI_HANDLER_TASK_PRIORITY				302
#define EXP_ENVI_HANDLER_STACK_SIZE 					10000
#define EXP_ENVI_HANDLER_MSG_SIZE 					1000
#define EXP_ENVI_HANDLER_POLICY 						SCHED_FIFO
#define EXP_ENVI_HANDLER_CPU_ID						TRIAL_HANDLER_CPU_ID
#define EXP_ENVI_HANDLER_CPU_THREAD_ID				TRIAL_HANDLER_CPU_THREAD_ID
#define EXP_ENVI_HANDLER_CPU_THREAD_TASK_ID		TRIAL_DURATION_HANDLER_CPU_THREAD_TASK_ID + 1
#define EXP_ENVI_HANDLER_POSITIVE_JITTER_THRES	 	TRIAL_HANDLER_POSITIVE_JITTER_THRES		//0.1 ms
#define EXP_ENVI_HANDLER_NEGATIVE_JITTER_THRES	 	TRIAL_HANDLER_NEGATIVE_JITTER_THRES

// ExpEnviDurationHandler
#define EXP_ENVI_DURATION_HANDLER_PERIOD 						TRIAL_HANDLER_PERIOD		//1 ms
#define EXP_ENVI_DURATION_HANDLER_TASK_NAME					nam2num("ENVIDHND")
#define EXP_ENVI_DURATION_HANDLER_TASK_PRIORITY				303
#define EXP_ENVI_DURATION_HANDLER_STACK_SIZE 					10000
#define EXP_ENVI_DURATION_HANDLER_MSG_SIZE 						1000
#define EXP_ENVI_DURATION_HANDLER_POLICY 						SCHED_FIFO
#define EXP_ENVI_DURATION_HANDLER_CPU_ID						TRIAL_HANDLER_CPU_ID
#define EXP_ENVI_DURATION_HANDLER_CPU_THREAD_ID				TRIAL_HANDLER_CPU_THREAD_ID
#define EXP_ENVI_DURATION_HANDLER_CPU_THREAD_TASK_ID			EXP_ENVI_HANDLER_CPU_THREAD_TASK_ID + 1 
#define EXP_ENVI_DURATION_HANDLER_POSITIVE_JITTER_THRES	 	TRIAL_HANDLER_POSITIVE_JITTER_THRES		//0.1 ms
#define EXP_ENVI_DURATION_HANDLER_NEGATIVE_JITTER_THRES	 	TRIAL_HANDLER_NEGATIVE_JITTER_THRES

// MovObjHandler
#define MOV_OBJ_HANDLER_PERIOD 						TRIAL_HANDLER_PERIOD		//1 ms
#define MOV_OBJ_HANDLER_TASK_NAME					nam2num("MOBJHND")
#define MOV_OBJ_HANDLER_TASK_PRIORITY				304
#define MOV_OBJ_HANDLER_STACK_SIZE 					10000
#define MOV_OBJ_HANDLER_MSG_SIZE 					1000
#define MOV_OBJ_HANDLER_POLICY 						SCHED_FIFO
#define MOV_OBJ_HANDLER_CPU_ID						TRIAL_HANDLER_CPU_ID
#define MOV_OBJ_HANDLER_CPU_THREAD_ID				TRIAL_HANDLER_CPU_THREAD_ID
#define MOV_OBJ_HANDLER_CPU_THREAD_TASK_ID			EXP_ENVI_DURATION_HANDLER_CPU_THREAD_TASK_ID + 1
#define MOV_OBJ_HANDLER_POSITIVE_JITTER_THRES	 	TRIAL_HANDLER_POSITIVE_JITTER_THRES		//0.1 ms
#define MOV_OBJ_HANDLER_NEGATIVE_JITTER_THRES	 	TRIAL_HANDLER_NEGATIVE_JITTER_THRES

// MovObjDurationHandler
#define MOV_OBJ_DURATION_HANDLER_PERIOD 						TRIAL_HANDLER_PERIOD		//1 ms
#define MOV_OBJ_DURATION_HANDLER_TASK_NAME					nam2num("MOBDHND")
#define MOV_OBJ_DURATION_HANDLER_TASK_PRIORITY					305
#define MOV_OBJ_DURATION_HANDLER_STACK_SIZE 					10000
#define MOV_OBJ_DURATION_HANDLER_MSG_SIZE 						1000
#define MOV_OBJ_DURATION_HANDLER_POLICY 						SCHED_FIFO
#define MOV_OBJ_DURATION_HANDLER_CPU_ID						TRIAL_HANDLER_CPU_ID
#define MOV_OBJ_DURATION_HANDLER_CPU_THREAD_ID				TRIAL_HANDLER_CPU_THREAD_ID
#define MOV_OBJ_DURATION_HANDLER_CPU_THREAD_TASK_ID			MOV_OBJ_HANDLER_CPU_THREAD_TASK_ID + 1
#define MOV_OBJ_DURATION_HANDLER_POSITIVE_JITTER_THRES	 	TRIAL_HANDLER_POSITIVE_JITTER_THRES		//0.1 ms
#define MOV_OBJ_DURATION_HANDLER_NEGATIVE_JITTER_THRES	 	TRIAL_HANDLER_NEGATIVE_JITTER_THRES

// FirstBMICageInterfacer
#define FIRST_BMI_CAGE_INTERF_PERIOD 						TRIAL_HANDLER_PERIOD		//1 ms
#define FIRST_BMI_CAGE_INTERF_TASK_NAME					nam2num("FIRSTBCM")
#define FIRST_BMI_CAGE_INTERF_TASK_PRIORITY				306
#define FIRST_BMI_CAGE_INTERF_STACK_SIZE 					10000
#define FIRST_BMI_CAGE_INTERF_MSG_SIZE 					1000
#define FIRST_BMI_CAGE_INTERF_POLICY 						SCHED_FIFO
#define FIRST_BMI_CAGE_INTERF_CPU_ID						TRIAL_HANDLER_CPU_ID
#define FIRST_BMI_CAGE_INTERF_CPU_THREAD_ID				TRIAL_HANDLER_CPU_THREAD_ID
#define FIRST_BMI_CAGE_INTERF_CPU_THREAD_TASK_ID		MOV_OBJ_DURATION_HANDLER_CPU_THREAD_TASK_ID + 1
#define FIRST_BMI_CAGE_INTERF_POSITIVE_JITTER_THRES	 	TRIAL_HANDLER_POSITIVE_JITTER_THRES		//0.1 ms
#define FIRST_BMI_CAGE_INTERF_NEGATIVE_JITTER_THRES	 	TRIAL_HANDLER_NEGATIVE_JITTER_THRES

// BlueSpike Buffer Handler
#define BLUE_SPIKE_BUFF_HANDLER_PERIOD 						TRIAL_HANDLER_PERIOD		//1 ms
#define BLUE_SPIKE_BUFF_HANDLER_TASK_NAME					nam2num("BLUEHAND")
#define BLUE_SPIKE_BUFF_HANDLER_TASK_PRIORITY				100
#define BLUE_SPIKE_BUFF_HANDLER_STACK_SIZE 					10000
#define BLUE_SPIKE_BUFF_HANDLER_MSG_SIZE		 			1000
#define BLUE_SPIKE_BUFF_HANDLER_POLICY			 			SCHED_FIFO
#define BLUE_SPIKE_BUFF_HANDLER_CPU_ID						TRIAL_HANDLER_CPU_ID
#define BLUE_SPIKE_BUFF_HANDLER_CPU_THREAD_ID				TRIAL_HANDLER_CPU_THREAD_ID + 1
#define BLUE_SPIKE_BUFF_HANDLER_CPU_THREAD_TASK_ID		0
#define BLUE_SPIKE_BUFF_HANDLER_POSITIVE_JITTER_THRES	 	TRIAL_HANDLER_POSITIVE_JITTER_THRES		//0.1 ms
#define BLUE_SPIKE_BUFF_HANDLER_NEGATIVE_JITTER_THRES	 	TRIAL_HANDLER_NEGATIVE_JITTER_THRES

// Trial Handler 2 Neural Network Messages Handler
#define TRIAL_HAND_2_NEURAL_NET_MSGS_HANDLER_PERIOD 					TRIAL_HANDLER_PERIOD		//1 ms
#define TRIAL_HAND_2_NEURAL_NET_MSGS_HANDLER_TASK_NAME				nam2num("TRTONN")
#define TRIAL_HAND_2_NEURAL_NET_MSGS_HANDLER_TASK_PRIORITY			101
#define TRIAL_HAND_2_NEURAL_NET_MSGS_HANDLER_STACK_SIZE 				10000
#define TRIAL_HAND_2_NEURAL_NET_MSGS_HANDLER_MSG_SIZE		 		1000
#define TRIAL_HAND_2_NEURAL_NET_MSGS_HANDLER_POLICY			 		SCHED_FIFO
#define TRIAL_HAND_2_NEURAL_NET_MSGS_HANDLER_CPU_ID					TRIAL_HANDLER_CPU_ID
#define TRIAL_HAND_2_NEURAL_NET_MSGS_HANDLER_CPU_THREAD_ID			TRIAL_HANDLER_CPU_THREAD_ID + 1
#define TRIAL_HAND_2_NEURAL_NET_MSGS_HANDLER_CPU_THREAD_TASK_ID	BLUE_SPIKE_BUFF_HANDLER_CPU_THREAD_TASK_ID + 1
#define TRIAL_HAND_2_NEURAL_NET_MSGS_HANDLER_POSITIVE_JITTER_THRES 	TRIAL_HANDLER_POSITIVE_JITTER_THRES		//0.1 ms
#define TRIAL_HAND_2_NEURAL_NET_MSGS_HANDLER_NEGATIVE_JITTER_THRES 	TRIAL_HANDLER_NEGATIVE_JITTER_THRES

// Mov Obj Handler 2 Neural Network Messages Handler
#define MOV_OBJ_HAND_2_NEURAL_NET_MSGS_HANDLER_PERIOD 					TRIAL_HANDLER_PERIOD		//1 ms
#define MOV_OBJ_HAND_2_NEURAL_NET_MSGS_HANDLER_TASK_NAME				nam2num("MOTONN")
#define MOV_OBJ_HAND_2_NEURAL_NET_MSGS_HANDLER_TASK_PRIORITY			102
#define MOV_OBJ_HAND_2_NEURAL_NET_MSGS_HANDLER_STACK_SIZE 				10000
#define MOV_OBJ_HAND_2_NEURAL_NET_MSGS_HANDLER_MSG_SIZE		 		1000
#define MOV_OBJ_HAND_2_NEURAL_NET_MSGS_HANDLER_POLICY			 		SCHED_FIFO
#define MOV_OBJ_HAND_2_NEURAL_NET_MSGS_HANDLER_CPU_ID					TRIAL_HANDLER_CPU_ID
#define MOV_OBJ_HAND_2_NEURAL_NET_MSGS_HANDLER_CPU_THREAD_ID			TRIAL_HANDLER_CPU_THREAD_ID + 1
#define MOV_OBJ_HAND_2_NEURAL_NET_MSGS_HANDLER_CPU_THREAD_TASK_ID	TRIAL_HAND_2_NEURAL_NET_MSGS_HANDLER_CPU_THREAD_TASK_ID + 1
#define MOV_OBJ_HAND_2_NEURAL_NET_MSGS_HANDLER_POSITIVE_JITTER_THRES 	TRIAL_HANDLER_POSITIVE_JITTER_THRES		//0.1 ms
#define MOV_OBJ_HAND_2_NEURAL_NET_MSGS_HANDLER_NEGATIVE_JITTER_THRES 	TRIAL_HANDLER_NEGATIVE_JITTER_THRES

// Trial Handler 2 SpikeGenerator Messages Handler
#define TRIAL_HAND_2_SPIKE_GEN_MSGS_HANDLER_PERIOD 					TRIAL_HANDLER_PERIOD		//1 ms
#define TRIAL_HAND_2_SPIKE_GEN_MSGS_HANDLER_TASK_NAME				nam2num("TRTOSG")
#define TRIAL_HAND_2_SPIKE_GEN_MSGS_HANDLER_TASK_PRIORITY				103
#define TRIAL_HAND_2_SPIKE_GEN_MSGS_HANDLER_STACK_SIZE 				10000
#define TRIAL_HAND_2_SPIKE_GEN_MSGS_HANDLER_MSG_SIZE		 			1000
#define TRIAL_HAND_2_SPIKE_GEN_MSGS_HANDLER_POLICY			 		SCHED_FIFO
#define TRIAL_HAND_2_SPIKE_GEN_MSGS_HANDLER_CPU_ID					TRIAL_HANDLER_CPU_ID
#define TRIAL_HAND_2_SPIKE_GEN_MSGS_HANDLER_CPU_THREAD_ID			TRIAL_HANDLER_CPU_THREAD_ID + 1
#define TRIAL_HAND_2_SPIKE_GEN_MSGS_HANDLER_CPU_THREAD_TASK_ID		MOV_OBJ_HAND_2_NEURAL_NET_MSGS_HANDLER_CPU_THREAD_TASK_ID + 1
#define TRIAL_HAND_2_SPIKE_GEN_MSGS_HANDLER_POSITIVE_JITTER_THRES 	TRIAL_HANDLER_POSITIVE_JITTER_THRES		//0.1 ms
#define TRIAL_HAND_2_SPIKE_GEN_MSGS_HANDLER_NEGATIVE_JITTER_THRES 	TRIAL_HANDLER_NEGATIVE_JITTER_THRES

// SpikeGenerator
#define SPIKE_GENERATOR_PERIOD 						TRIAL_HANDLER_PERIOD		//1 ms
#define SPIKE_GENERATOR_TASK_NAME					nam2num("SPIKEGEN")
#define SPIKE_GENERATOR_TASK_PRIORITY				201
#define SPIKE_GENERATOR_STACK_SIZE 					10000
#define SPIKE_GENERATOR_MSG_SIZE 						1000
#define SPIKE_GENERATOR_POLICY 						SCHED_FIFO
#define SPIKE_GENERATOR_CPU_ID						TRIAL_HANDLER_CPU_ID
#define SPIKE_GENERATOR_CPU_THREAD_ID				TRIAL_HANDLER_CPU_THREAD_ID + 1
#define SPIKE_GENERATOR_CPU_THREAD_TASK_ID			TRIAL_HAND_2_SPIKE_GEN_MSGS_HANDLER_CPU_THREAD_TASK_ID + 1
#define SPIKE_GENERATOR_POSITIVE_JITTER_THRES	 	TRIAL_HANDLER_POSITIVE_JITTER_THRES		//0.1 ms
#define SPIKE_GENERATOR_NEGATIVE_JITTER_THRES	 	TRIAL_HANDLER_NEGATIVE_JITTER_THRES

// InSilicoSimulators
#define IZ_PS_NETWORK_SIM_PERIOD						2000000		//1 ms
#define IZ_PS_NETWORK_SIM_TASK_NAME					"IZPS"
#define IZ_PS_NETWORK_SIM_TASK_PRIORITY				202
#define IZ_PS_NETWORK_SIM_STACK_SIZE 					10000
#define IZ_PS_NETWORK_SIM_MSG_SIZE 					1000
#define IZ_PS_NETWORK_SIM_POLICY 						SCHED_FIFO
#define IZ_PS_NETWORK_SIM_CPU_ID						2
#define IZ_PS_NETWORK_SIM_CPU_THREAD_ID				0
#define IZ_PS_NETWORK_SIM_POSITIVE_JITTER_THRES	 	200000		// 200 us
#define IZ_PS_NETWORK_SIM_NEGATIVE_JITTER_THRES	 	200000

#define IZ_PS_NETWORK_SIM_NUM_OF_DEDICATED_CPUS	2	// cpus startiing from IZ_PS_NETWORK_SIM_CPU_ID are dedicated to SNN simulations

#define IZ_PS_NETWORK_SIM_USER_SPACE_CPU_ID	TRIAL_HANDLER_CPU_ID	
#define IZ_PS_NETWORK_SIM_USER_SPACE_CPU_THREAD_ID	TRIAL_HANDLER_CPU_THREAD_ID

#define BLUESPIKE_USER_SPACE_CPU_ID				TRIAL_HANDLER_CPU_ID	
#define BLUESPIKE_USER_SPACE_CPU_THREAD_ID		TRIAL_HANDLER_CPU_THREAD_ID


/////////////// DAQ CARD CONFIG

#define MAX_NUM_OF_DAQ_CARD					1 					// Increase if there is an additional card
#define MAX_NUM_OF_CHANNEL_PER_DAQ_CARD	32 

#define NUM_OF_SCAN							16    // num of scan to trigger interrupt for 	TRIG_RT | TRIG_WAKE_EOS	

#define NUM_OF_DAQ_CARD_NULL					MAX_NUM_OF_DAQ_CARD				
#define NUM_OF_CHANNEL_PER_DAQ_CARD_NULL	MAX_NUM_OF_CHANNEL_PER_DAQ_CARD

#define DAQ_0_MODEL 				"NI-PCIe6259"
#define COMEDI_SUBDEVICE_AI 		0
#define SAMPLING_INTERVAL 			32000     /// DO NOT CHANGE!!!
// #define VOLTAGE_RANGE_6259 		0	//	+10V,-10V
//#define VOLTAGE_RANGE_6259 		1	//	Notworking
//#define VOLTAGE_RANGE_6259 		2	//	Not working
//#define VOLTAGE_RANGE_6259 		3	//	Not working
#define VOLTAGE_RANGE_6259 		4	//	+0.5,-0.5V
//#define VOLTAGE_RANGE_6259 		5	//	+0.25,-0.25V
//#define VOLTAGE_RANGE_6259 		6	//	+0.1,-0.1V
//#define VOLTAGE_RANGE_6259 		7	//	+0.05,-0.05V

#define BASELINE_QUANT_6259 		32768.0
#define LOWEST_VOLTAGE_MV 		-500.0    // when VOLTAGE_RANGE_6259E = 4
#define HIGHEST_VOLTAGE_MV 		500.0
#define VOLTAGE_MULTIPLIER_MV_6259 	65.536     //   4096/1000.0    (0.5V - (- 0.5V) = 1000 mV)


/////////////// MICROWIRE   ARRAY CONFIG


#define MAX_NUM_OF_MWA				2
#define MAX_NUM_OF_CHAN_PER_MWA	16
#define MAX_NUM_OF_UNIT_PER_CHAN	4

#define NUM_OF_MWA_NULL				MAX_NUM_OF_MWA
#define NUM_OF_CHAN_PER_MWA_NULL	MAX_NUM_OF_CHAN_PER_MWA

//   TEMPLATE MATCHING
#define NUM_OF_SAMP_PER_SPIKE 	18    // adjusted according to 31.25 KHz sampling rate

/////////////// SPIKE_END_HANDLING

#define SPIKE_END_HANDLING_DATA_BUFF_SIZE 	1000      // 1 seconds of buffer at worst scenario (if each channel spikes every millisecond )

#define SPIKE_MIN_END_SAMP_NUM   ((NUM_OF_SAMP_PER_SPIKE *3)/ 5) * 2   //   *2 due to interpolation

#define IN_SPIKE_SAMPLE_CNTR_MAX	((1000000/(SAMPLING_INTERVAL/2))/3)		// not a spike if lasts longer than  ~320 us  	(SAMPLING_INTERVAL/2) due to interpolation


#endif