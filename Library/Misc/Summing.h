#ifndef SUMMING_H
#define SUMMING_H


#include <gtk/gtk.h>
#include "../Misc/Misc.h"

typedef struct 
{
	double				*elements;
	unsigned int			size;
	unsigned int			write_idx;
} SummingStruct;

typedef struct 
{
	int				*elements;
	unsigned int			size;
	unsigned int			write_idx;
} SummingStructInteger;

typedef struct 
{
	long long int			*elements;
	unsigned int			size;
	unsigned int			write_idx;
} SummingStructLLInteger;

SummingStruct* allocate_summing_struct(SummingStruct *avg_struct, unsigned int size);
SummingStruct* deallocate_summing_struct(SummingStruct *avg_struct);
void write_to_summing_struct(SummingStruct *avg_struct, double element);
double write_to_summing_struct_and_get_sum(SummingStruct *avg_struct, double element);
void reset_summing_struct_elements(SummingStruct *avg_struct, double value);
double summing_struct_get_sum(SummingStruct *avg_struct);

SummingStructInteger* allocate_summing_struct_integer(SummingStructInteger *avg_struct, unsigned int size);
SummingStructInteger* deallocate_summing_struct_integer(SummingStructInteger *avg_struct);
int write_to_summing_struct_and_get_sum_integer(SummingStructInteger *avg_struct, int element);


SummingStructLLInteger* allocate_summing_struct_ll_integer(SummingStructLLInteger *avg_struct, unsigned int size);
SummingStructLLInteger* deallocate_summing_struct_ll_integer(SummingStructLLInteger *avg_struct);
long long int write_to_summing_struct_and_get_sum_ll_integer(SummingStructLLInteger *avg_struct, long long int element);
void write_to_summing_struct_ll_integer(SummingStructLLInteger *avg_struct, long long int element);



#endif
