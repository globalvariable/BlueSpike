#define PERFORMANCE_DATA_H

#define MAX_NUM_OF_CPU 8

#define PERFORMANCE_DATA_BUFF_SIZE 1000

typedef struct __PerformanceData
{
	float 		cpu_usage[MAX_NUM_OF_CPU][PERFORMANCE_DATA_BUFF_SIZE];
	unsigned int	jitter[MAX_NUM_OF_CPU][PERFORMANCE_DATA_BUFF_SIZE];
} PerformanceData;
