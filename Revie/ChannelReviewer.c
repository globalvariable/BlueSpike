#include "ChannelReviewer.h"


#define NUM_OF_RAW_SAMPLE_TO_DISPLAY 4000		// Adjusted for 40000KHz sampling rate 
#define RAW_DATA_DISP_DURATION_MS 100.0	

static RecordingData			*recording_data = NULL;
static RecordingData			*filtered_recording_data = NULL;
static FilterCtrl				*filter_ctrl = NULL;

static GdkColor color_bg_signal;
static GdkColor color_signal;
static GdkColor color_grid_signal;

static float **Y;
static float **X;

static GtkWidget **databox_signal;
static GtkWidget **box_signal;

static int *previous_start_idx_to_plot;

static int chan_group = 0;

static void create_gui(void);
static gboolean timeout_callback(gpointer user_data) ;

int main( int argc, char *argv[])
{
	recording_data = (RecordingData*)rtai_malloc(SHM_NUM_KERNEL_SPIKE_RECORDING_DATA, 0);
	if (recording_data == NULL) 
		return print_message(ERROR_MSG ,"SpikeViewer", "SpikeViewer", "main", "recording_data == NULL.");
	filtered_recording_data = (RecordingData*)rtai_malloc(SHM_NUM_KERNEL_SPIKE_FILTERED_RECORDING_DATA, 0);
	if (filtered_recording_data == NULL) 
		return print_message(ERROR_MSG ,"SpikeViewer", "SpikeViewer", "main", "filtered_recording_data == NULL.");
	filter_ctrl = (FilterCtrl*)rtai_malloc(SHM_NUM_KERNEL_SPIKE_FILTER_CTRL, 0);
	if (filter_ctrl == NULL) 
		return print_message(ERROR_MSG ,"SpikeViewer", "SpikeViewer", "main", "filter_ctrl == NULL.");


	if ((argc < 2) || (argc > 2)) 
		return print_message(ERROR_MSG ,"SpikeViewer", "SpikeViewer", "main", "((argc < 2) || (argc > 2))");

	chan_group = atoi(argv[1]);

	if ((chan_group < 0) || (chan_group > 1))
		return print_message(ERROR_MSG ,"SpikeViewer", "SpikeViewer", "main", "((chan_group < 0) || (chan_group > 1))");

	gtk_init(&argc, &argv);
	create_gui(); 	
	gtk_main();

	return 0;
}


static void create_gui(void)
{
	GtkWidget *window;
	GtkWidget *main_table;

	int i, j;
	unsigned int sink;
	char arg_str[50];

 	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  	gtk_window_set_default_size(GTK_WINDOW(window), 1920, 1020);
  	gtk_window_set_title(GTK_WINDOW(window), "SpikeViewer");
  	gtk_container_set_border_width(GTK_CONTAINER(window), 10);



	color_bg_signal.red = 65535;
	color_bg_signal.green = 65535;
	color_bg_signal.blue = 65535;

	color_signal.red = 0;
	color_signal.green = 0;
	color_signal.blue = 0;

	color_grid_signal.red = 50000;
	color_grid_signal.green = 50000;
	color_grid_signal.blue = 50000;
	
	X = g_new0 (float*, 4);
	Y = g_new0 (float*, 4);
	databox_signal = g_new0 (GtkWidget*, 4);
	box_signal = g_new0 (GtkWidget*, 4);

	for (i = 0; i < 4; i++)
	{
		X[i] = g_new0 (float, NUM_OF_RAW_SAMPLE_TO_DISPLAY);
		Y[i] = g_new0 (float, NUM_OF_RAW_SAMPLE_TO_DISPLAY);
		for (j=0; j<NUM_OF_RAW_SAMPLE_TO_DISPLAY; j++)
		{
			X[i][j] = ((float)j)*(RAW_DATA_DISP_DURATION_MS/NUM_OF_RAW_SAMPLE_TO_DISPLAY);
		}	

	}

	previous_start_idx_to_plot = g_new0(int, 4);

	main_table= gtk_table_new(2, 2, TRUE);    
	gtk_container_add(GTK_CONTAINER(window), main_table);


	gtk_databox_create_box_with_scrollbars_and_rulers (&(box_signal[0]), &(databox_signal[0]),TRUE, TRUE, TRUE, TRUE);
	gtk_widget_modify_bg (box_signal[0], GTK_STATE_NORMAL, &color_bg_signal);	
	gtk_databox_graph_add (GTK_DATABOX (box_signal[0]), gtk_databox_grid_new (21, 21, &color_grid_signal, 0));
	gtk_databox_graph_add (GTK_DATABOX (box_signal[0]), gtk_databox_lines_new (NUM_OF_RAW_SAMPLE_TO_DISPLAY, X[0], Y[0], &color_signal, 0));  	
	gtk_table_attach_defaults(GTK_TABLE(main_table), databox_signal[0], 0, 1, 0, 1);      // 1st row
	gtk_databox_set_total_limits (GTK_DATABOX (box_signal[0]), 0, RAW_DATA_DISP_DURATION_MS, HIGHEST_VOLTAGE_MV, LOWEST_VOLTAGE_MV);	

	gtk_databox_create_box_with_scrollbars_and_rulers (&(box_signal[1]), &(databox_signal[1]),TRUE, TRUE, TRUE, TRUE);
	gtk_widget_modify_bg (box_signal[1], GTK_STATE_NORMAL, &color_bg_signal);	
	gtk_databox_graph_add (GTK_DATABOX (box_signal[1]), gtk_databox_grid_new (21, 21, &color_grid_signal, 0));
	gtk_databox_graph_add (GTK_DATABOX (box_signal[1]), gtk_databox_lines_new (NUM_OF_RAW_SAMPLE_TO_DISPLAY, X[1], Y[1], &color_signal, 0));  	
	gtk_table_attach_defaults(GTK_TABLE(main_table), databox_signal[1], 1, 2, 0, 1);        // 1st row
	gtk_databox_set_total_limits (GTK_DATABOX (box_signal[1]), 0, RAW_DATA_DISP_DURATION_MS, HIGHEST_VOLTAGE_MV, LOWEST_VOLTAGE_MV);	

	gtk_databox_create_box_with_scrollbars_and_rulers (&(box_signal[2]), &(databox_signal[2]),TRUE, TRUE, TRUE, TRUE);
	gtk_widget_modify_bg (box_signal[2], GTK_STATE_NORMAL, &color_bg_signal);	
	gtk_databox_graph_add (GTK_DATABOX (box_signal[2]), gtk_databox_grid_new (21, 21, &color_grid_signal, 0));
	gtk_databox_graph_add (GTK_DATABOX (box_signal[2]), gtk_databox_lines_new (NUM_OF_RAW_SAMPLE_TO_DISPLAY, X[2], Y[2], &color_signal, 0));  	
	gtk_table_attach_defaults(GTK_TABLE(main_table), databox_signal[2], 0, 1, 1, 2);      // 2nd row
	gtk_databox_set_total_limits (GTK_DATABOX (box_signal[2]), 0, RAW_DATA_DISP_DURATION_MS, HIGHEST_VOLTAGE_MV, LOWEST_VOLTAGE_MV);	

	gtk_databox_create_box_with_scrollbars_and_rulers (&(box_signal[3]), &(databox_signal[3]),TRUE, TRUE, TRUE, TRUE);
	gtk_widget_modify_bg (box_signal[3], GTK_STATE_NORMAL, &color_bg_signal);	
	gtk_databox_graph_add (GTK_DATABOX (box_signal[3]), gtk_databox_grid_new (21, 21, &color_grid_signal, 0));
	gtk_databox_graph_add (GTK_DATABOX (box_signal[3]), gtk_databox_lines_new (NUM_OF_RAW_SAMPLE_TO_DISPLAY, X[3], Y[3], &color_signal, 0));  	
	gtk_table_attach_defaults(GTK_TABLE(main_table), databox_signal[3], 1, 2, 1, 2);      // 2nd row
	gtk_databox_set_total_limits (GTK_DATABOX (box_signal[3]), 0, RAW_DATA_DISP_DURATION_MS, HIGHEST_VOLTAGE_MV, LOWEST_VOLTAGE_MV);

 	gtk_widget_show_all(window);

	g_timeout_add(50, timeout_callback, box_signal);	

}

static gboolean timeout_callback(gpointer user_data) 
{
	int start_idx, i;

	RecordingData	*handling_data;
	RecordingDataChanBuff	*handling_data_chan_buff;
	
	if (filter_ctrl->highpass_150Hz_on || filter_ctrl->highpass_400Hz_on)
	{
		handling_data = filtered_recording_data;
	}
	else
	{
		handling_data = recording_data;
	}

	handling_data_chan_buff = &(handling_data->recording_data_buff[0][chan_group*4]);
	start_idx = handling_data->buff_idx_write[0][chan_group*4] / NUM_OF_RAW_SAMPLE_TO_DISPLAY;   // Handle previous NUM_OF_RAW_SAMPLE_TO_DISPLAY
	if (start_idx == 0)
		 start_idx = RECORDING_DATA_BUFF_SIZE - NUM_OF_RAW_SAMPLE_TO_DISPLAY;    // read previous section
	else
		start_idx = (start_idx*NUM_OF_RAW_SAMPLE_TO_DISPLAY) - NUM_OF_RAW_SAMPLE_TO_DISPLAY;  // read previous section
	if (start_idx != previous_start_idx_to_plot[0])
	{	
		previous_start_idx_to_plot[0] = start_idx;
		for (i = 0; i < NUM_OF_RAW_SAMPLE_TO_DISPLAY; i++)
		{
			Y[0][i] = (*handling_data_chan_buff)[i+start_idx];
		}		
		gtk_databox_set_total_limits (GTK_DATABOX (box_signal[0]), 0, RAW_DATA_DISP_DURATION_MS, HIGHEST_VOLTAGE_MV , LOWEST_VOLTAGE_MV);	
	}

	handling_data_chan_buff = &(handling_data->recording_data_buff[0][chan_group*4+1]);
	start_idx = handling_data->buff_idx_write[0][chan_group*4+1] / NUM_OF_RAW_SAMPLE_TO_DISPLAY;   // Handle previous NUM_OF_RAW_SAMPLE_TO_DISPLAY
	if (start_idx == 0)
		 start_idx = RECORDING_DATA_BUFF_SIZE - NUM_OF_RAW_SAMPLE_TO_DISPLAY;    // read previous section
	else
		start_idx = (start_idx*NUM_OF_RAW_SAMPLE_TO_DISPLAY) - NUM_OF_RAW_SAMPLE_TO_DISPLAY;  // read previous section

	if (start_idx != previous_start_idx_to_plot[1])
	{	
		previous_start_idx_to_plot[1] = start_idx;
		for (i = 0; i < NUM_OF_RAW_SAMPLE_TO_DISPLAY; i++)
		{
			Y[1][i] = (*handling_data_chan_buff)[i+start_idx];
		}		
		gtk_databox_set_total_limits (GTK_DATABOX (box_signal[1]), 0, RAW_DATA_DISP_DURATION_MS, HIGHEST_VOLTAGE_MV , LOWEST_VOLTAGE_MV);	
	}

	handling_data_chan_buff = &(handling_data->recording_data_buff[0][chan_group*4+2]);
	start_idx = handling_data->buff_idx_write[0][chan_group*4+2] / NUM_OF_RAW_SAMPLE_TO_DISPLAY;   // Handle previous NUM_OF_RAW_SAMPLE_TO_DISPLAY
	if (start_idx == 0)
		 start_idx = RECORDING_DATA_BUFF_SIZE - NUM_OF_RAW_SAMPLE_TO_DISPLAY;    // read previous section
	else
		start_idx = (start_idx*NUM_OF_RAW_SAMPLE_TO_DISPLAY) - NUM_OF_RAW_SAMPLE_TO_DISPLAY;  // read previous section

	if (start_idx != previous_start_idx_to_plot[2])
	{	
		previous_start_idx_to_plot[2] = start_idx;
		for (i = 0; i < NUM_OF_RAW_SAMPLE_TO_DISPLAY; i++)
		{
			Y[2][i] = (*handling_data_chan_buff)[i+start_idx];
		}		
		gtk_databox_set_total_limits (GTK_DATABOX (box_signal[2]), 0, RAW_DATA_DISP_DURATION_MS, HIGHEST_VOLTAGE_MV , LOWEST_VOLTAGE_MV);	
	}

	handling_data_chan_buff = &(handling_data->recording_data_buff[0][chan_group*4+3]);
	start_idx = handling_data->buff_idx_write[0][chan_group*4+3] / NUM_OF_RAW_SAMPLE_TO_DISPLAY;   // Handle previous NUM_OF_RAW_SAMPLE_TO_DISPLAY
	if (start_idx == 0)
		 start_idx = RECORDING_DATA_BUFF_SIZE - NUM_OF_RAW_SAMPLE_TO_DISPLAY;    // read previous section
	else
		start_idx = (start_idx*NUM_OF_RAW_SAMPLE_TO_DISPLAY) - NUM_OF_RAW_SAMPLE_TO_DISPLAY;  // read previous section

	if (start_idx != previous_start_idx_to_plot[3])
	{	
		previous_start_idx_to_plot[3] = start_idx;
		for (i = 0; i < NUM_OF_RAW_SAMPLE_TO_DISPLAY; i++)
		{
			Y[3][i] = (*handling_data_chan_buff)[i+start_idx];
		}		
		gtk_databox_set_total_limits (GTK_DATABOX (box_signal[3]), 0, RAW_DATA_DISP_DURATION_MS, HIGHEST_VOLTAGE_MV , LOWEST_VOLTAGE_MV);	
	}

	return TRUE;

}
