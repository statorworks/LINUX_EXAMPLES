#ifndef MAIN_H
#define MAIN_H 
//
//2020 Statorworks
//Simple windowed demo with GTK2 widgets framework, instead of low level libX control.


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sched.h>
#include <stdint.h>
#include <unistd.h> //<usleep()

#include <gtk/gtk.h>
//#include <gdk/gdk.h>
#include <glib.h>



GtkWidget* window;
GtkWidget *fixed;
gint context_id;
//
GtkWidget* button1;
GtkWidget* checkbox1;
GtkWidget* toggle_button;
GtkWidget* spin_button;
GtkWidget* hscale;
GtkWidget* vscale;
GtkWidget* statusbar; guint sbid; //status bar needs an 'id' for the message.
GtkWidget* progressbar; 
GtkWidget* textbox;
//
//GtkWidget* vbox;
//GtkWidget* GtkHBox;
//GtkHBox* hbox;


//flags
bool window_closed;


//callbacks
void callback_button_clicked(GtkWidget *widget,gpointer data);
void callback_checkbox_changed(GtkWidget *widget,gpointer data);
void callback_rbutton1_clicked(GtkWidget *widget, gpointer data);
void callback_rbutton2_clicked(GtkWidget *widget, gpointer data);
void callback_radio_buttons_changed(GtkWidget *widget, gpointer data);
void callback_spin_button_changed(GtkWidget *widget,gpointer data);
void callback_toggle_button(GtkWidget *widget, gpointer data);
//
void callback_hscale_changed(GtkWidget *widget, gpointer data);
void callback_vscale_changed(GtkWidget *widget, gpointer data);
//
void callback_textbox_changed(GtkWidget *widget, gpointer data); //Any key, updated text
void callback_textbox_enter(GtkWidget *widget, gpointer data);   //Enter key was pressed to commit some value or string
//
void callback_event_box_mouse_move(GtkWidget *widget, gpointer data);
void callback_event_box_mouse_button_press(GtkWidget *widget, gpointer data);
void callback_event_box_mouse_button_release(GtkWidget *widget, gpointer data);
//
void callback_menu_file_open(GtkWidget* widget, gpointer data);
void callback_menu_file_save(GtkWidget* widget, gpointer data);
void callback_menu_file_close(GtkWidget* widget, gpointer data);
void callback_menu_about_legend(GtkWidget* widget, gpointer data); 
//
void callback_window_close(GtkWidget* widget, gpointer data);
 
#endif
