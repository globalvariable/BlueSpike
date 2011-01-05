#include <stdlib.h>
#include <stdbool.h>
#include <rtai_shm.h>
#include <rtai_nam2num.h>

#include <rtai_comedi.h>

#include <gtk/gtk.h>
#include <gtkdatabox.h>
#include <gtkdatabox_ruler.h>
#include <gtkdatabox_lines.h>


#define SHMSIZ  0
#define NUM_OF_CHAN 16 
#define NUM_OF_SAMP_IN_BUFF 40000  
#define BUFFNAME  "SHBUFF"

#define INIT_POINT_OF_SPIKE 59
#define NUM_OF_SAMP_PER_SPIKE 60
#define SPIKE_MEM_TO_DISPLAY 30 
#define NUM_OF_TEMP_PER_CHAN 3

typedef struct{
	union
	{
		struct{
			unsigned Bit0:1;
			unsigned Bit1:1;
			unsigned Bit2:1;
			unsigned Bit3:1;
			unsigned Bit4:1;
			unsigned Bit5:1;
			unsigned Bit6:1;						
			unsigned Bit7:1;
		};
		unsigned AllCommand;
	} Command;
	union
	{
		struct{
			unsigned Bit0:1;
			unsigned Bit1:1;
			unsigned Bit2:1;
			unsigned Bit3:1;
			unsigned Bit4:1;
			unsigned Bit5:1;
			unsigned Bit6:1;						
			unsigned Bit7:1;
		};
		unsigned AllStatus;
	} Status;
	short int ShortInt_Status0;
	short int ShortInt_Status1;	 	
} ExpEnv; 

typedef struct sample_data
{
	float data[NUM_OF_CHAN];
} scan_data;

typedef struct spk_data
{
	int data[NUM_OF_CHAN];
} spike_data;

typedef struct templ_data
{
	float template[NUM_OF_CHAN][NUM_OF_TEMP_PER_CHAN][NUM_OF_SAMP_PER_SPIKE];
	float diff_thres[NUM_OF_CHAN][NUM_OF_TEMP_PER_CHAN];
} template_matching_data;

typedef struct buff_data 
{
	int scan_number_write;
	int scan_number_read;
	scan_data scan[NUM_OF_SAMP_IN_BUFF];
	scan_data filtered_scan[NUM_OF_SAMP_IN_BUFF];
	spike_data spike_peak[NUM_OF_SAMP_IN_BUFF];
	spike_data spike_end[NUM_OF_SAMP_IN_BUFF];	
	float Threshold[NUM_OF_CHAN];
	bool in_spike[NUM_OF_CHAN];	
	bool filter_on;
	ExpEnv Environment[NUM_OF_SAMP_IN_BUFF];
	ExpEnv Curr_Environment;
	template_matching_data spike_template; 	
	bool sorting_on;
	spike_data sorted_spike_data[NUM_OF_SAMP_IN_BUFF];
	bool highpass_4th_on;
} buff_data_struct;

buff_data_struct *buff;

GtkWidget *box1;
GtkWidget *box2;
GtkWidget *box3;
GtkWidget *box4;

GtkWidget *databox1;
GtkWidget *databox2;
GtkWidget *databox3;
GtkWidget *databox4;

GtkDataboxGraph *graph1;
GtkDataboxGraph *graph2;
GtkDataboxGraph *graph3;
GtkDataboxGraph *graph4;

GtkDataboxGraph *graphgrid;

GtkWidget *combo;
GtkWidget *ch_slct_button;
GtkWidget *pause_button;

int disp_page;

int front, back, size;
float *X;
float *Y1;
float *Y2;
float *Y3;
float *Y4;

int disp_paused;
GdkColor color_bg;
GdkColor color_signal;
GdkColor color_grid;



// Functions
void create_gui(void);
gboolean timeout_callback(gpointer user_data) ;
gboolean ch_slct_func (GtkDatabox * box);
gboolean pause_button_func (GtkDatabox * box);


int main( int argc, char *argv[])
{
	buff = (buff_data_struct *)rtai_malloc(nam2num(BUFFNAME), SHMSIZ);
  	if (buff == NULL) 
	{
		printf("rtai_malloc() failed (maybe /dev/rtai_shm is missing)!\n");
		return -1;
   	}
	gtk_init(&argc, &argv);
	create_gui(); 	
	gtk_main();

	return 0;
}


void create_gui(void)
{

	color_bg.red = 65535;
	color_bg.green = 65535;
	color_bg.blue = 65535;

	color_signal.red = 0;
	color_signal.green = 0;
	color_signal.blue = 0;

	color_grid.red = 50000;
	color_grid.green = 50000;
	color_grid.blue = 50000;

	GtkWidget *window;
 	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  	gtk_window_set_default_size(GTK_WINDOW(window), 1200, 900);
  	gtk_window_set_title(GTK_WINDOW(window), "ChannelReviewer");
  	gtk_container_set_border_width(GTK_CONTAINER(window), 10);	

	GtkWidget *vbox;
  	vbox = gtk_vbox_new(FALSE, 0);
	gtk_container_add(GTK_CONTAINER(window), vbox);

	GtkWidget *hbox;
  	hbox = gtk_hbox_new(FALSE, 0);
  	gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 10);

 	combo = gtk_combo_box_new_text();

 	gtk_combo_box_append_text(GTK_COMBO_BOX(combo), "1-4");
 	gtk_combo_box_append_text(GTK_COMBO_BOX(combo), "5-8");
	gtk_combo_box_append_text(GTK_COMBO_BOX(combo), "9-12");
	gtk_combo_box_append_text(GTK_COMBO_BOX(combo), "13-16");

	gtk_box_pack_start (GTK_BOX (hbox), combo, FALSE, FALSE, 10);

	ch_slct_button = gtk_button_new_with_label("Select Ch");
	pause_button = gtk_button_new_with_label("Pause/Resume");

	gtk_widget_set_size_request(ch_slct_button, 70, 30);
	gtk_widget_set_size_request(pause_button, 100, 30);
	gtk_box_pack_start (GTK_BOX (hbox), ch_slct_button, FALSE, FALSE, 50);
	gtk_box_pack_start (GTK_BOX (hbox), pause_button, TRUE, TRUE, 5);


  	hbox = gtk_hbox_new(FALSE, 0);
  	gtk_box_pack_start (GTK_BOX (vbox), hbox, TRUE, TRUE, 0);


	graphgrid = gtk_databox_grid_new (21, 21, &color_grid, 0);


	gtk_databox_create_box_with_scrollbars_and_rulers (&box1, &databox1,TRUE, TRUE, TRUE, TRUE);
  	gtk_box_pack_start (GTK_BOX (hbox), databox1, TRUE, TRUE, 0);
  	gtk_widget_modify_bg (GTK_DATABOX (box1), GTK_STATE_NORMAL, &color_bg);
   	gtk_databox_graph_add (GTK_DATABOX (box1), graphgrid);
	gtk_databox_set_total_limits (GTK_DATABOX (box1), -10., 4500., +2200, -2200);

	gtk_databox_create_box_with_scrollbars_and_rulers (&box2, &databox2,TRUE, TRUE, TRUE, TRUE);
  	gtk_box_pack_start (GTK_BOX (hbox), databox2, TRUE, TRUE, 0);
  	gtk_widget_modify_bg (GTK_DATABOX (box2), GTK_STATE_NORMAL, &color_bg);
   	gtk_databox_graph_add (GTK_DATABOX (box2), graphgrid);
	gtk_databox_set_total_limits (GTK_DATABOX (box2), -10., 4500., +2200, -2200);

  	hbox = gtk_hbox_new(FALSE, 0);
  	gtk_box_pack_start (GTK_BOX (vbox), hbox, TRUE, TRUE, 0);
	
	gtk_databox_create_box_with_scrollbars_and_rulers (&box3, &databox3,TRUE, TRUE, TRUE, TRUE);
  	gtk_box_pack_start (GTK_BOX (hbox), databox3, TRUE, TRUE, 0);
  	gtk_widget_modify_bg (GTK_DATABOX (box3), GTK_STATE_NORMAL, &color_bg);
   	gtk_databox_graph_add (GTK_DATABOX (box3), graphgrid);
	gtk_databox_set_total_limits (GTK_DATABOX (box3), -10., 4500., +2200, -2200);

	gtk_databox_create_box_with_scrollbars_and_rulers (&box4, &databox4,TRUE, TRUE, TRUE, TRUE);
  	gtk_box_pack_start (GTK_BOX (hbox), databox4, TRUE, TRUE, 0);
  	gtk_widget_modify_bg (GTK_DATABOX (box4), GTK_STATE_NORMAL, &color_bg);
   	gtk_databox_graph_add (GTK_DATABOX (box4), graphgrid);
	gtk_databox_set_total_limits (GTK_DATABOX (box4), -10., 4500., +2200, -2200);


  	g_signal_connect (GTK_OBJECT (window), "destroy", G_CALLBACK (gtk_main_quit), NULL);

	g_signal_connect_swapped(G_OBJECT(ch_slct_button), "clicked", G_CALLBACK(ch_slct_func), G_OBJECT(box1));
	g_signal_connect_swapped(G_OBJECT(pause_button), "clicked", G_CALLBACK(pause_button_func), G_OBJECT(box1));

 	gtk_widget_show_all(window);

	g_timeout_add(100, timeout_callback, box1);

	return; 
}


gboolean timeout_callback(gpointer user_data) 
{
	
   	gint i;

	front = buff->scan_number_write;
	if (front < back)
		size = front + NUM_OF_SAMP_IN_BUFF - back;
	else
		size = front - back;	


	if (size == 0)
		return TRUE;  
		
	if (!disp_paused)
	{
		g_free(X);
		g_free(Y1);
		g_free(Y2);
		g_free(Y3);
		g_free(Y4);

		X = g_new0 (float, size);
		Y1 = g_new0 (float, size);
		Y2 = g_new0 (float, size);
		Y3 = g_new0 (float, size);
		Y4 = g_new0 (float, size);

		if (graph1 != NULL)
		{
			gtk_databox_graph_remove (GTK_DATABOX (box1), graph1);
			g_object_unref(graph1);
		}
		if (graph2 != NULL)
		{
			gtk_databox_graph_remove (GTK_DATABOX (box2), graph2);
			g_object_unref(graph2);
		}
		if (graph3 != NULL)
		{
			gtk_databox_graph_remove (GTK_DATABOX (box3), graph3);
			g_object_unref(graph3);
		}
		if (graph4 != NULL)
		{
			gtk_databox_graph_remove (GTK_DATABOX (box4), graph4);
			g_object_unref(graph4);
		}


		for (i = 0; i < size; i++)
		{
			X[i] = (float)i;
		}


		if (buff->filter_on)
		{

			for (i = 0; i < size; i++)
			{
				if ((back+i) >= NUM_OF_SAMP_IN_BUFF)
				{
					Y1[i] = buff->filtered_scan[back+i-NUM_OF_SAMP_IN_BUFF].data[disp_page*4];  
					Y2[i] = buff->filtered_scan[back+i-NUM_OF_SAMP_IN_BUFF].data[(disp_page*4)+1]; 
					Y3[i] = buff->filtered_scan[back+i-NUM_OF_SAMP_IN_BUFF].data[(disp_page*4)+2]; 
					Y4[i] = buff->filtered_scan[back+i-NUM_OF_SAMP_IN_BUFF].data[(disp_page*4)+3]; 
				}
				else
				{
					Y1[i] = buff->filtered_scan[back+i].data[disp_page*4];  
					Y2[i] = buff->filtered_scan[back+i].data[(disp_page*4)+1]; 
					Y3[i] = buff->filtered_scan[back+i].data[(disp_page*4)+2]; 
					Y4[i] = buff->filtered_scan[back+i].data[(disp_page*4)+3]; 
				} 
			}
		}
		else
		{
			for (i = 0; i < size; i++)
			{
				if ((back+i) >= NUM_OF_SAMP_IN_BUFF)
				{
					Y1[i] = buff->scan[back+i-NUM_OF_SAMP_IN_BUFF].data[disp_page*4];
					Y2[i] = buff->scan[back+i-NUM_OF_SAMP_IN_BUFF].data[(disp_page*4)+1];
					Y3[i] = buff->scan[back+i-NUM_OF_SAMP_IN_BUFF].data[(disp_page*4)+2];
					Y4[i] = buff->scan[back+i-NUM_OF_SAMP_IN_BUFF].data[(disp_page*4)+3];
				}  
				else
				{
					Y1[i] = buff->scan[back+i].data[disp_page*4];
					Y2[i] = buff->scan[back+i].data[(disp_page*4)+1];
					Y3[i] = buff->scan[back+i].data[(disp_page*4)+2];
					Y4[i] = buff->scan[back+i].data[(disp_page*4)+3];
				}
			}
		}
		graph1 = GTK_DATABOX_GRAPH(gtk_databox_lines_new (size, X, Y1, &color_signal, 0));
		gtk_databox_graph_add (GTK_DATABOX (box1), graph1);
		gtk_databox_set_total_limits (GTK_DATABOX (box1), -10., 4500., +2200, -2200);

		graph2 = GTK_DATABOX_GRAPH(gtk_databox_lines_new (size, X, Y2, &color_signal, 0));
		gtk_databox_graph_add (GTK_DATABOX (box2), graph2);
		gtk_databox_set_total_limits (GTK_DATABOX (box2), -10., 4500., +2200, -2200);

		graph3 = GTK_DATABOX_GRAPH(gtk_databox_lines_new (size, X, Y3, &color_signal, 0));
		gtk_databox_graph_add (GTK_DATABOX (box3), graph3);
		gtk_databox_set_total_limits (GTK_DATABOX (box3), -10., 4500., +2200, -2200);

		graph4 = GTK_DATABOX_GRAPH(gtk_databox_lines_new (size, X, Y4, &color_signal, 0));
		gtk_databox_graph_add (GTK_DATABOX (box4), graph4);
		gtk_databox_set_total_limits (GTK_DATABOX (box4), -10., 4500., +2200, -2200);

	}

	back = front;

	return TRUE;  

}

gboolean ch_slct_func (GtkDatabox * box)
{
	if (gtk_combo_box_get_active (GTK_COMBO_BOX(combo)) >= 0)
	{
		disp_page = gtk_combo_box_get_active (GTK_COMBO_BOX(combo));
	}
	return TRUE;
}

gboolean pause_button_func (GtkDatabox * box)
{
	if (disp_paused)
	{
		disp_paused = 0;
		gtk_button_set_label (pause_button,"Pause");
	}
	else
	{
		disp_paused = 1;
		gtk_button_set_label (pause_button,"Resume");
	}
	return TRUE;
}

