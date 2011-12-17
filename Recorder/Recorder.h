#ifndef RECORDER_H
#define RECORDER_H

#include <rtai_shm.h>
#include <rtai_nam2num.h>

#include "./../DataHandling/DataHandling.h"


GtkWidget *main_table;

GtkWidget *btn_select_directory_to_save;

GtkWidget *btn_start_stop_recording;
GtkWidget *btn_delete_last_recording;
GtkWidget *btn_create_bluespikedata_folder;
GtkWidget *btn_save_maps_templates;
GtkWidget *btn_submit_notes;
GtkWidget *btn_submit_additional_notes;

GtkWidget *txv_notes;
GtkWidget *txv_additional_notes;

bool start_recording_request;
bool stop_recording_request;
bool recording_ongoing;

pthread_t recording_thread;

// Functions
void create_gui(void);
void start_stop_recording_button_func (void);
void delete_last_recording_button_func (void);
void create_bluespikedata_folder_button_func (void);
void delete_bluespikedata_folder_button_func (void);
void save_maps_templates_button_func (void);   
void submit_notes_button_func (void);   
void submit_additional_notes_button_func (void);   

void *recording_handler(void *ptr);
void set_directory_btn_select_directory_to_save(void);
void fill_notes_text_view(void);


#endif
