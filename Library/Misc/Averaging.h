#ifndef AVERAGING_H
#define AVERAGING_H


#include <gtk/gtk.h>
#include "../Misc/Misc.h"

typedef struct 
{
	double				*elements;
	unsigned int			size;
	unsigned int			write_idx;
} AveragingStruct;

typedef struct 
{
	int				*elements;
	unsigned int			size;
	unsigned int			write_idx;
} AveragingStructInteger;

typedef struct 
{
	long long int			*elements;
	unsigned int			size;
	unsigned int			write_idx;
} AveragingStructLLInteger;

AveragingStruct* allocate_averaging_struct(AveragingStruct *avg_struct, unsigned int size);
AveragingStruct* deallocate_averaging_struct(AveragingStruct *avg_struct);
void write_to_averaging_struct(AveragingStruct *avg_struct, double element);
double write_to_averaging_struct_and_get_mean(AveragingStruct *avg_struct, double element);
void reset_averaging_struct_elements(AveragingStruct *avg_struct, double value);
double averaging_struct_get_mean(AveragingStruct *avg_struct);

AveragingStructInteger* allocate_averaging_struct_integer(AveragingStructInteger *avg_struct, unsigned int size);
AveragingStructInteger* deallocate_averaging_struct_integer(AveragingStructInteger *avg_struct);
int write_to_averaging_struct_and_get_mean_integer(AveragingStructInteger *avg_struct, int element);


AveragingStructLLInteger* allocate_averaging_struct_ll_integer(AveragingStructLLInteger *avg_struct, unsigned int size);
AveragingStructLLInteger* deallocate_averaging_struct_ll_integer(AveragingStructLLInteger *avg_struct);
long long int write_to_averaging_struct_and_get_mean_ll_integer(AveragingStructLLInteger *avg_struct, long long int element);




#endif
