#include "Global.h"

#include <gtk/gtk.h>
#include <gtkdatabox.h>
#include <gtkdatabox_ruler.h>
#include <gtkdatabox_lines.h>

// Variables
GtkWidget *databox_all_spike;
GtkWidget *databox_sorted_all_spike;
GtkWidget *databox_spike_0;
GtkWidget *databox_spike_1;
GtkWidget *databox_spike_2;
GtkWidget *databox_spike_nonsorted;

GtkWidget *box_all_spike;
GtkWidget *box_sorted_all_spike;
GtkWidget *box_spike_0;
GtkWidget *box_spike_1;
GtkWidget *box_spike_2;
GtkWidget *box_spike_nonsorted;

GtkWidget *combo;
GtkWidget *ch_slct_button;

GtkWidget *combo_spike;
GtkWidget *unit_slct_button;

GtkWidget *lbl_selected_chan;
GtkWidget *lbl_selected_unit;

char *str_selected_chan;
char *str_selected_unit;

GtkWidget *clear_button;
GtkWidget *clear_unit_button;
GtkWidget *clear_nonsorted_button;
GtkWidget *pause_button;
GtkWidget *diff_button;
GtkWidget *sorting_onoff_button;
GtkWidget *save_template_button;
GtkWidget *load_template_button;

char *strDiff;
GtkWidget *entryDiff;

char *strAddFileName;
GtkWidget *entryAddFileName;

char *strLoadFileName;
GtkWidget *entryLoadFileName;

FILE *fp;

GtkDataboxGraph *graph_all_spike;
GtkDataboxGraph *graph_sorted_all_spike;
GtkDataboxGraph *graph_spike_0;
GtkDataboxGraph *graph_spike_template_0;
GtkDataboxGraph *graph_spike_1;
GtkDataboxGraph *graph_spike_template_1;
GtkDataboxGraph *graph_spike_2;
GtkDataboxGraph *graph_spike_template_2;
GtkDataboxGraph *graph_spike_nonsorted;
	
int GraphIdx;	

float Y_ex;

int disp_chan;
int selected_spike_unit;
int front, back, size;
float *X;
float *Y;

float *X_axis;
float *Y_all_spike;
float *Y_spike_0;
float *Y_spike_1;
float *Y_spike_2;
float *Y_spike_nonsorted;

float *Y_spike_0_template;
float *Y_spike_1_template;
float *Y_spike_2_template;

GPtrArray* Y_all_spikes_ptr;
GPtrArray* Y_spikes_0_ptr;
GPtrArray* Y_spikes_1_ptr;
GPtrArray* Y_spikes_2_ptr;
GPtrArray* Y_spikes_nonsorted_ptr;

int Y_all_spikes_idx;
int Y_spikes_0_idx;
int Y_spikes_1_idx;
int Y_spikes_2_idx;
int Y_spikes_nonsorted_idx;

int disp_paused;

GdkColor color_bg;
GdkColor color_all_spike;
GdkColor color_spike_0;
GdkColor color_spike_1;
GdkColor color_spike_2;
GdkColor color_spike_nonsorted;
GdkColor color_spike_template;

// Functions
void create_gui(void);
gboolean timeout_callback(gpointer user_data) ;
gboolean ch_slct_func (GtkDatabox * box);
gboolean spk_unit_slct_func (GtkDatabox * box);
gboolean pause_button_func (GtkDatabox * box);
gboolean threshold_but_func (GtkDatabox * box);
gboolean clear_screen_but_func (GtkDatabox * box);
gboolean clear_nonsorted_but_func (GtkDatabox * box);
gboolean clear_unit_but_func (GtkDatabox * box);
gboolean diff_but_func (GtkDatabox * box);
gboolean rect_selection_func (GtkDatabox * box, GtkDataboxValueRectangle * selectionValues);
gboolean sorting_onoff_but_func (GtkDatabox * box);
gboolean save_template_but_func (GtkDatabox * box);
gboolean load_template_but_func (GtkDatabox * box);
