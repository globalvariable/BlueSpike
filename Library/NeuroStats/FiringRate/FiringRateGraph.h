#ifndef FIRING_RATE_GRAPH_H
#define FIRING_RATE_GRAPH_H

#include <gtkdatabox.h>
#include <gtkdatabox_ruler.h>
#include <gtkdatabox_lines.h>
#include <gtkdatabox_grid.h>
#include <gtk/gtk.h>
#include "FiringRate.h"

bool create_firing_rate_bin_means_boxes_and_graphs(GtkWidget *hbox, TimeStamp pattern_length, TimeStamp bin_size);
bool create_firing_rate_bin_variances_boxes_and_graphs(GtkWidget *hbox, TimeStamp pattern_length, TimeStamp bin_size);
bool refresh_neuron_firing_rate_means_graph(int mwa, int channel, int unit);
bool refresh_neuron_firing_rate_variances_graph(int mwa, int channel, int unit);

#endif
