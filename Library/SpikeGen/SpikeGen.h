#ifndef SPIKE_GEN_H
#define SPIKE_GEN_H

#include <stdbool.h>
#include <gtk/gtk.h>
#include "../../SpikeTimeStamp.h"


bool write_generated_spike_to_blue_spike_buffer(SpikeTimeStamp *spike_time_stamp, unsigned int layer_num, unsigned int neuron_grp_num, unsigned int neuron_num, TimeStamp spike_time);



#endif
