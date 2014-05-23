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


AveragingStruct* allocate_averaging_struct(AveragingStruct *avg_struct, unsigned int size);
AveragingStruct* deallocate_averaging_struct(AveragingStruct *avg_struct);
void write_to_averaging_struct(AveragingStruct *avg_struct, double element);
double write_to_averaging_struct_and_get_mean(AveragingStruct *avg_struct, double element);
void reset_averaging_struct_elements(AveragingStruct *avg_struct, double value);
double averaging_struct_get_mean(AveragingStruct *avg_struct);










#endif
