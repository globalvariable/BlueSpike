#ifndef MISC_H
#define MISC_H

#include <string.h>
#include <stdbool.h>
#include <stdio.h>

#include "../SharedMemory.h"

int get_word_in_line(char separator, int word_order, char *word, char *line, bool print_warning);

#endif
