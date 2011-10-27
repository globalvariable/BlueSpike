#define KERNELTASKSTAT_H

#define KERNEL_TASK_STAT_BUFF_SIZE 1000

typedef struct __KernelTaskStatItem
{
	float 		cpu_usage;
	unsigned int	task_jitter;
} KernelTaskStatItem;


typedef KernelTaskStatItem	KernelTaskStatBuff[KERNEL_TASK_STAT_BUFF_SIZE];

typedef struct __KernelTaskStat
{
	KernelTaskStatBuff		kernel_task_stat_buff;
	int					buff_idx_write;
} KernelTaskStat;
