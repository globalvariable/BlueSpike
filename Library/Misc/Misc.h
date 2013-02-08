#ifndef MISC_H
#define MISC_H

#include <gtk/gtk.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

#define ALLOCATION_FAILED 		0
#define ALLOCATION_WARNING 	1
#define ALLOCATION_BUG	 	2

#define EXPECTS_ALLOCATED	 	1
#define EXPECTS_NOT_ALLOCATED 	0

#define ERROR_MSG 		0
#define WARNING_MSG	 	1
#define INFO_MSG	 		2
#define BUG_MSG		 	3

int get_word_in_line(char separator, int word_order, char *word, char *line, bool print_warning);
bool is_allocated(void* data, bool print_message, char *module, char *function, int message_type, char *message, bool expects_allocated);
bool print_message(int message_type, char* program, char *module, char *function, char *message);
bool is_buffer_getting_full(unsigned int buffer_read_idx,  unsigned int buffer_write_idx, unsigned int buffer_size, bool printf_message);
unsigned int get_size_of_binary_file(FILE *fp);
bool get_num_of_items_in_binary_file(FILE *fp, unsigned int size_of_item, unsigned int *num_of_items);

#endif
