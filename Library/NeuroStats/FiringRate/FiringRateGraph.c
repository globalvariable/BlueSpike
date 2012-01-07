#include "FiringRateGraph.h"


static GtkWidget *bin_means_box = NULL;
static GtkWidget *bin_means_databox = NULL;
static GtkDataboxGraph **bin_means_graph = NULL;
static GdkColor color_bg_bin_means;
static GdkColor color_bin_means_line;
static GdkColor color_bin_means_grid;
static float **bin_means_graph_y_axis = NULL;    
static float **bin_means_graph_x_axis = NULL;  

static GtkWidget *bin_variances_box = NULL;
static GtkWidget *bin_variances_databox = NULL;
static GtkDataboxGraph **bin_variances_graph = NULL;
static GdkColor color_bg_bin_variances;
static GdkColor color_bin_variances_line;
static GdkColor color_bin_variances_grid;
static float **bin_variances_graph_y_axis = NULL;    
static float **bin_variances_graph_x_axis = NULL;  

bool create_firing_rate_bin_means_boxes_and_graphs(GtkWidget *hbox, TimeStamp pattern_length, TimeStamp bin_size)
{
	int i;
	int num_of_bins = pattern_length/bin_size ;
	
	color_bg_bin_means.red = 65535;
	color_bg_bin_means.green = 65535;
	color_bg_bin_means.blue = 65535;
	
	color_bin_means_grid.red = 50000;
	color_bin_means_grid.green = 50000;
	color_bin_means_grid.blue = 50000;
	
	color_bin_means_line.red = 0;
	color_bin_means_line.green = 0;
	color_bin_means_line.blue = 0;

	if ((bin_means_box != NULL) || (bin_means_databox != NULL))
	{
		printf ("FiringRateGraph: BUG: Allocated bin_means_boxes previously. Not free' d\n");
		return FALSE;
	}	
	gtk_databox_create_box_with_scrollbars_and_rulers (&bin_means_box, &bin_means_databox,TRUE, TRUE, TRUE, TRUE);
	if ((bin_means_box == NULL) || (bin_means_databox == NULL))
	{
		printf ("FiringRateGraph: ERROR: Couldn' t allocate bin_means_boxes\n");
		return FALSE;
	}
  	gtk_widget_modify_bg (bin_means_box, GTK_STATE_NORMAL, &color_bg_bin_means);
 	gtk_databox_graph_add (GTK_DATABOX (bin_means_box), gtk_databox_grid_new (9, num_of_bins, &color_bin_means_grid, 0)); 	
	gtk_box_pack_start (GTK_BOX (hbox), bin_means_databox, TRUE, TRUE, 0);
	if (bin_means_graph != NULL)
	{
		printf ("FiringRateGraph: BUG: Allocated bin_means_graphs previously. Not free' d\n");
		return FALSE;	
	}
	bin_means_graph = g_new0(GtkDataboxGraph *, num_of_bins);
	if (bin_means_graph == NULL) 
	{
		printf ("FiringRateGraph: ERROR: Couldn' t allocate bin_means_graphs\n");
		return FALSE;
	}	
	bin_means_graph_y_axis = g_new0(float *, num_of_bins);
	bin_means_graph_x_axis = g_new0(float *, num_of_bins);
	for (i = 0; i< num_of_bins; i++)
	{
		bin_means_graph_y_axis[i] = g_new0(float, 2);		// start and end point of vertical bar
		bin_means_graph_x_axis[i] = g_new0(float, 2);	
	}
	for (i = 0; i< num_of_bins; i++)
	{
		bin_means_graph_x_axis[i][0] = i;
		bin_means_graph_x_axis[i][1] = i;		 	
	}	
	for (i = 0; i< num_of_bins; i++)
	{
		bin_means_graph[i] = GTK_DATABOX_GRAPH(gtk_databox_lines_new (2, bin_means_graph_x_axis[i], bin_means_graph_y_axis[i], &color_bin_means_line, 20));
		gtk_databox_graph_add (GTK_DATABOX (bin_means_box), bin_means_graph[i]);		
	}
		
	gtk_databox_set_total_limits (GTK_DATABOX (bin_means_box), -1, num_of_bins, MAX_NEURON_FIRING_RATE, MIN_NEURON_FIRING_RATE);	
	return TRUE;			
}

bool create_firing_rate_bin_variances_boxes_and_graphs(GtkWidget *hbox, TimeStamp pattern_length, TimeStamp bin_size)
{
	int i;
	int num_of_bins = pattern_length/bin_size ;
	
	color_bg_bin_variances.red = 65535;
	color_bg_bin_variances.green = 65535;
	color_bg_bin_variances.blue = 65535;
	
	color_bin_variances_grid.red = 50000;
	color_bin_variances_grid.green = 50000;
	color_bin_variances_grid.blue = 50000;
	
	color_bin_variances_line.red = 0;
	color_bin_variances_line.green = 0;
	color_bin_variances_line.blue = 0;

	if ((bin_variances_box != NULL) || (bin_variances_databox != NULL))
	{
		printf ("FiringRateGraph: BUG: Allocated bin_variances_boxes previously. Not free' d\n");
		return FALSE;
	}	
	gtk_databox_create_box_with_scrollbars_and_rulers (&bin_variances_box, &bin_variances_databox,TRUE, TRUE, TRUE, TRUE);
	if ((bin_variances_box == NULL) || (bin_variances_databox == NULL))
	{
		printf ("FiringRateGraph: ERROR: Couldn' t allocate bin_variances_boxes\n");
		return FALSE;
	}
  	gtk_widget_modify_bg (bin_variances_box, GTK_STATE_NORMAL, &color_bg_bin_variances);
 	gtk_databox_graph_add (GTK_DATABOX (bin_variances_box), gtk_databox_grid_new (9, num_of_bins, &color_bin_variances_grid, 0)); 	
	gtk_box_pack_start (GTK_BOX (hbox), bin_variances_databox, TRUE, TRUE, 0);
	if (bin_variances_graph != NULL)
	{
		printf ("FiringRateGraph: BUG: Allocated bin_variances_graphs previously. Not free' d\n");
		return FALSE;	
	}
	bin_variances_graph = g_new0(GtkDataboxGraph *, num_of_bins);
	if (bin_variances_graph == NULL) 
	{
		printf ("FiringRateGraph: ERROR: Couldn' t allocate bin_variances_graphs\n");
		return FALSE;
	}	
	bin_variances_graph_y_axis = g_new0(float *, num_of_bins);
	bin_variances_graph_x_axis = g_new0(float *, num_of_bins);
	for (i = 0; i< num_of_bins; i++)
	{
		bin_variances_graph_y_axis[i] = g_new0(float, 2);		// start and end point of vertical bar
		bin_variances_graph_x_axis[i] = g_new0(float, 2);	
	}
	for (i = 0; i< num_of_bins; i++)
	{
		bin_variances_graph_x_axis[i][0] = i;
		bin_variances_graph_x_axis[i][1] = i;		 	
	}	
	for (i = 0; i< num_of_bins; i++)
	{
		bin_variances_graph[i] = GTK_DATABOX_GRAPH(gtk_databox_lines_new (2, bin_variances_graph_x_axis[i], bin_variances_graph_y_axis[i], &color_bin_variances_line, 20));
		gtk_databox_graph_add (GTK_DATABOX (bin_variances_box), bin_variances_graph[i]);		
	}
		
	gtk_databox_set_total_limits (GTK_DATABOX (bin_variances_box), -1, num_of_bins, MAX_NEURON_FIRING_RATE_VARIANCE, MIN_NEURON_FIRING_RATE_VARIANCE);	
	return TRUE;			
}

bool refresh_neuron_firing_rate_means_graph(int mwa, int channel, int unit)
{
	int i;
	int num_of_bins = firing_rate_get_num_of_bins();
	NeuronFiringRate* stats = get_neuron_firing_rate_statistics(mwa, channel, unit);
	
	for (i = 0; i< num_of_bins; i++)
	{
		bin_means_graph_y_axis[i][1] = stats->train_bins[i].mean;		 	
	}
	gtk_databox_set_total_limits (GTK_DATABOX (bin_means_box), -1, num_of_bins, MAX_NEURON_FIRING_RATE, MIN_NEURON_FIRING_RATE);	
	
	return TRUE;		
}

bool refresh_neuron_firing_rate_variances_graph(int mwa, int channel, int unit)
{
	int i;
	int num_of_bins = firing_rate_get_num_of_bins();
	NeuronFiringRate* stats = get_neuron_firing_rate_statistics(mwa, channel, unit);
		
	for (i = 0; i< num_of_bins; i++)
	{
		bin_variances_graph_y_axis[i][1] = stats->train_bins[i].variance;		 	
	}
	gtk_databox_set_total_limits (GTK_DATABOX (bin_variances_box), -1, num_of_bins , MAX_NEURON_FIRING_RATE_VARIANCE, MIN_NEURON_FIRING_RATE_VARIANCE);	
	
	return TRUE;					
}
