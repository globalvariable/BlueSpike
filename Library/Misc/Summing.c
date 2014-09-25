#include "Summing.h"






SummingStruct* allocate_summing_struct(SummingStruct *avg_struct, unsigned int size)
{
	if (avg_struct != NULL)
	{
		avg_struct= deallocate_summing_struct(avg_struct);
		avg_struct = allocate_summing_struct(avg_struct, size);
		return avg_struct;
	}  
	avg_struct = g_new0(SummingStruct,1);
	avg_struct->elements =  g_new0(double, size);
	avg_struct->size =  size;
	print_message(INFO_MSG ,"BlueSpike", "Summing", "allocate_summing_struct", "Created Summing Struct.");
	return avg_struct;	
}

SummingStruct* deallocate_summing_struct(SummingStruct *avg_struct)
{
	if (avg_struct != NULL)
		return (SummingStruct*)print_message(BUG_MSG ,"BlueSpike", "Summing", "deallocate_summing_struct", "avg_struct == NULL.");   
	g_free(avg_struct->elements);		 
	g_free(avg_struct);	
	return NULL;
}

void write_to_summing_struct(SummingStruct *avg_struct, double element)
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

double write_to_summing_struct_and_get_sum(SummingStruct *avg_struct, double element)
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

	return temp;
}

void reset_summing_struct_elements(SummingStruct *avg_struct, double value)
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

double summing_struct_get_mean(SummingStruct *avg_struct)
{
	unsigned int i, size;
	double temp = 0;
	double *elements;

	elements = avg_struct->elements;
	size = avg_struct->size;	

	for (i = 0; i < size; i++)
		temp += elements[i];

	return temp;
}

SummingStructInteger* allocate_summing_struct_integer(SummingStructInteger *avg_struct, unsigned int size)
{
	if (avg_struct != NULL)
	{
		avg_struct= deallocate_summing_struct_integer(avg_struct);
		avg_struct = allocate_summing_struct_integer(avg_struct, size);
		return avg_struct;
	}  
	avg_struct = g_new0(SummingStructInteger,1);
	avg_struct->elements =  g_new0(int, size);
	avg_struct->size =  size;
	print_message(INFO_MSG ,"BlueSpike", "Summing", "allocate_summing_struct_integer", "Created Summing Struct.");
	return avg_struct;	
}

SummingStructInteger* deallocate_summing_struct_integer(SummingStructInteger *avg_struct)
{
	if (avg_struct != NULL)
		return (SummingStructInteger*)print_message(BUG_MSG ,"BlueSpike", "Summing", "deallocate_summing_struct_integer", "avg_struct == NULL.");   
	g_free(avg_struct->elements);		 
	g_free(avg_struct);	
	return NULL;
}

int write_to_summing_struct_and_get_mean_integer(SummingStructInteger *avg_struct, int element)
{
	unsigned int i, size;
	unsigned int write_idx;
	int temp = 0;
	int *elements;

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

	return temp;
}


SummingStructLLInteger* allocate_summing_struct_ll_integer(SummingStructLLInteger *avg_struct, unsigned int size)
{
	if (avg_struct != NULL)
	{
		avg_struct= deallocate_summing_struct_ll_integer(avg_struct);
		avg_struct = allocate_summing_struct_ll_integer(avg_struct, size);
		return avg_struct;
	}  
	avg_struct = g_new0(SummingStructLLInteger,1);
	avg_struct->elements =  g_new0(long long int, size);
	avg_struct->size =  size;
	print_message(INFO_MSG ,"BlueSpike", "Summing", "allocate_summing_struct_ll_integer", "Created Summing Struct.");
	return avg_struct;	
}
SummingStructLLInteger* deallocate_summing_struct_ll_integer(SummingStructLLInteger *avg_struct)
{
	if (avg_struct != NULL)
		return (SummingStructLLInteger*)print_message(BUG_MSG ,"BlueSpike", "Summing", "deallocate_summing_struct_ll_integer", "avg_struct == NULL.");   
	g_free(avg_struct->elements);		 
	g_free(avg_struct);	
	return NULL;
}

long long int write_to_summing_struct_and_get_sum_ll_integer(SummingStructLLInteger *avg_struct, long long int element)
{
	unsigned int i, size;
	unsigned int write_idx;
	long long int temp = 0;
	long long int *elements;

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

	return temp;
}

void write_to_summing_struct_ll_integer(SummingStructLLInteger *avg_struct, long long int element)
{
	unsigned int size;
	unsigned int write_idx;
	long long int *elements;

	elements = avg_struct->elements;
	write_idx = avg_struct->write_idx;
	size = avg_struct->size;

	elements[write_idx] = element;
	write_idx++;
	if (write_idx == size)
		write_idx = 0;
	avg_struct->write_idx = write_idx;
	return;
}

