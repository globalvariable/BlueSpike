#ifndef MISC_H
#define MISC_H

#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <gtk/gtk.h>

#define ALLOCATION_FAILED 		0
#define ALLOCATION_WARNING 	1
#define ALLOCATION_BUG	 	2

int get_word_in_line(char separator, int word_order, char *word, char *line, bool print_warning);
bool is_allocated(void* data, bool print_message, char *module, char *function, int message_type, char *message);

#endif
