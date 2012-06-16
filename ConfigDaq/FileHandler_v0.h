#ifndef FILE_HANDLER_V0_H
#define FILE_HANDLER_V0_H

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../DaqMwaData.h"
#include "../Library/Misc/Misc.h"
#include "../Library/Messages/DaqCon2KrnlSpk.h"
#include "Interrogate.h"

int write_config_daq_data_v0(int num, ...);
int read_config_daq_data_v0(int num, ...);






#endif
