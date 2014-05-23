#include "Averaging.h"






AveragingStruct* allocate_averaging_struct(AveragingStruct *avg_struct, unsigned int size)
{
	if (avg_struct != NULL)
	{
		avg_struct= deallocate_averaging_struct(avg_struct);
		avg_struct = allocate_averaging_struct(avg_struct, size);
		return avg_struct;
	}  
	avg_struct = g_new0(AveragingStruct,1);
	avg_struct->elements =  g_new0(double, size);
	avg_struct->size =  size;
	print_message(INFO_MSG ,"BlueSpike", "Averaging", "allocate_averaging_struct", "Created Averaging Struct.");
	return avg_struct;	
}

AveragingStruct* deallocate_averaging_struct(AveragingStruct *avg_struct)
{
	if (avg_struct != NULL)
		return (AveragingStruct*)print_message(BUG_MSG ,"BlueSpike", "Averaging", "deallocate_averaging_struct", "avg_struct == NULL.");   
	g_free(avg_struct->elements);		 
	g_free(avg_struct);	
	return NULL;
}

void write_to_averaging_struct(AveragingStruct *avg_struct, double element)
{
	unsigned int write_idx;

	write_idx = avg_struct->write_idx;
	avg_struct->elements[write_idx] = element;
	write_idx++;
	if (write_idx == avg_struct->size)
		write_idx = 0;
	avg_struct->write_idx = write_idx;
	return;
}

double write_to_averaging_struct_and_get_mean(AveragingStruct *avg_struct, double element)
{
	unsigned int i, size;
	unsigned int write_idx;
	double temp = 0;
	double *elements;

	elements = avg_struct->elements;
	write_idx = avg_struct->write_idx;
	size = avg_struct->size;

	elements[write_idx] = element;
	write_idx++;
	if (write_idx == size)
		write_idx = 0;
	avg_struct->write_idx = write_idx;

	for (i = 0; i < size; i++)
		temp += elements[i];

	return temp / size;
}

void reset_averaging_struct_elements(AveragingStruct *avg_struct, double value)
{
	unsigned int i, size;
	double *elements;

	elements = avg_struct->elements;
	size = avg_struct->size;

	for (i = 0; i < size; i++)
	{
		elements[i] = value;
	}
	return;
}

double averaging_struct_get_mean(AveragingStruct *avg_struct)
{
	unsigned int i, size;
	double temp = 0;
	double *elements;

	elements = avg_struct->elements;
	size = avg_struct->size;	

	for (i = 0; i < size; i++)
		temp += elements[i];

	return temp / size;
}
