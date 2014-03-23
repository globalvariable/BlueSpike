#ifndef CONFIG_DAQ_GUI_H
#define CONFIG_DAQ_GUI_H


#include <gtk/gtk.h>
#include <stdbool.h>

#include "../../../../BlueSpike/Library/Misc/Misc.h"
#include "MicroWireArray.h"
#include "FileHandler.h"
#include "../RtTask.h"

bool create_config_daq_gui(GtkWidget *tabs);




#endif
