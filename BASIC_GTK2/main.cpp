//
//2020 Statorworks
//Simple windowed demo with GTK2 widgets framework, instead of low level libX control.
//Using this template you can quickly implement a fixed size window application for a robot console or a machine HMI.
//Sizes and locations here are hardcoded for simplicity. You can make things more robust with some named constants.


//System preparation:
//apt-get install gtk2.0
//apt-get install gtk2.0-dev
//These are a brutal 256MB of files. Still need to see how much the final client actually needs.

//Compile command:
//g++ -Wall -std=c++11 -c "%f" 

//Build command (x86):
//g++ -Wall -o "%e" *.cpp -L/usr/lib/x86_64-linux-gnu -I/usr/include/gdk-pixbuf-2.0  -I/usr/lib/x86_64-linux-gnu/gtk-2.0/include -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/include/gtk-2.0 -I/usr/lib/gtk-2.0/include -I/usr/include/atk-1.0 -I/usr/include/cairo -I/usr/include/pango-1.0 -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include -I/usr/include/pixman-1 -I/usr/include/freetype2 -I/usr/include/libpng12 -lgtk-x11-2.0 -lgdk-x11-2.0 -lpangocairo-1.0 -latk-1.0 -lcairo -lgdk_pixbuf-2.0 -lgio-2.0 -lpangoft2-1.0 -lpango-1.0 -lgobject-2.0 -lglib-2.0 -lfontconfig -lfreetype
//A bit of a nightmare to setup, leaves to be desired.
//
//Build command (Rpi):
//Just replace:
//usr/lib/arm-linux-gnueabihf
//
//
//Most of the above required includes and libraries are found by running the following two commands:
//pkg-config --cflags gtk+-2.0
//pkg-config --libs gtk+-2.0

//Widgets function reference:
//developer.gnome.org/gtk2/stable/gtkobjects.html


#include "main.h"



int main(int argc, char *argv[]){
    
        
    //#1 Start GTK
    gtk_init(&argc, &argv); 	
    printf("\nGtk2 intialized Ok\n"); 


    //#2 Load test bitmaps
    GtkWidget* image1 = gtk_image_new_from_file("test1.png"); //simple
    GdkPixbuf* pbuf1 = gdk_pixbuf_new_from_file("test2.png", NULL); //as pixbuf so it can be scaled later
    
    	
	//#3 Window
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_widget_set_size_request (GTK_WIDGET (window), 620, 480);
    gtk_window_set_title (GTK_WINDOW (window), "BASIC GTK2 WINDOW");
	gtk_window_set_position (GTK_WINDOW (window), GTK_WIN_POS_CENTER);
    //gtk_window_fullscreen(GTK_WINDOW (window));//for full screen, make window resizeable below .
    
        
	//#4 For this basic demo, window and widgets will have fixed sizes, no resize or auto-arrangement boxes.
    gtk_window_set_resizable(GTK_WINDOW(window), false);
	fixed = gtk_fixed_new();
    gtk_container_add(GTK_CONTAINER(window), fixed);
	
    
    //#5 Window background color, optional. Otherwise takes system theme.
    //GdkColor color;
    //color.red   = 0xFFFF;  //each component is 16-bit
    //color.green = 0xFFFF;
    //color.blue  = 0x0;
    //gtk_widget_modify_bg(window, GTK_STATE_NORMAL, &color);	

		
	//Button
    //Note:You can change button background color just like for the window. Otherwise takes system theme.
	button1 = gtk_button_new_with_label("Button");
    gtk_fixed_put(GTK_FIXED(fixed), button1, 50, 240);
    gtk_widget_set_size_request(button1, 100, 60);
     
    
    //Add bitmap to button
    gtk_button_set_image(GTK_BUTTON(button1), image1);


	//Frame
	GtkWidget* frame = gtk_frame_new("Frame");
	gtk_fixed_put(GTK_FIXED(fixed), frame, 40, 40);
    gtk_widget_set_size_request(frame, 150, 140);
        
    
    //Radio buttons
	GSList* group;
	GtkWidget* rbutton1;
	GtkWidget* rbutton2;
    rbutton1 = gtk_radio_button_new_with_label(NULL, "rbutton1");
    gtk_fixed_put(GTK_FIXED(fixed), rbutton1, 50, 120);
	//
	group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (rbutton1));
    rbutton2 = gtk_radio_button_new_with_label(group, "rbutton2");
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (rbutton1), TRUE);
    gtk_fixed_put(GTK_FIXED(fixed), rbutton2, 50, 140);

	
	//Check box
	checkbox1 = gtk_check_button_new_with_label("Checkbox");
    gtk_fixed_put(GTK_FIXED(fixed), checkbox1, 50, 90);
    gtk_widget_set_size_request(checkbox1, 100, 20);
    

	//Toggle button
	toggle_button = gtk_toggle_button_new_with_label("Toggle");
	gtk_toggle_button_set_mode (GTK_TOGGLE_BUTTON(toggle_button), 0);//? 
	gtk_fixed_put(GTK_FIXED(fixed), toggle_button, 50, 190);
	
	
	//Switch (GTK3.0)
	//GtkWidget* on_off_switch  = gtk_switch_new();	
	//gtk_fixed_put(GTK_FIXED(fixed), on_off_switch, 350, 110);
    
    
    //Spin button (numeric updown) 
    spin_button = gtk_spin_button_new_with_range(0, 10, 1);
    gtk_fixed_put(GTK_FIXED(fixed), spin_button, 120, 190);
    gtk_widget_set_size_request(spin_button, 60, 25); 
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_button), 0);
    //uint8_t spin_val = gtk_spin_button_get_value( GTK_SPIN_BUTTON(spin_button) );  
        
    
    //Scales (sliders)
    hscale = gtk_hscale_new_with_range (0, 100, 1);
    gtk_fixed_put(GTK_FIXED(fixed), hscale, 230, 350);
    gtk_widget_set_size_request(hscale, 160, 30);
    gtk_scale_set_draw_value (GTK_SCALE(hscale), 1);
    gtk_range_set_value(GTK_RANGE(hscale), 0);
	//
    vscale = gtk_vscale_new_with_range (0, 100, 1);
    gtk_fixed_put(GTK_FIXED(fixed), vscale, 265, 200);
    gtk_widget_set_size_request(vscale, 80, 150);
    gtk_scale_set_draw_value (GTK_SCALE(vscale), 1);    
    gtk_range_set_inverted(GTK_RANGE(vscale), 1);//Down-Up
    gtk_range_set_value(GTK_RANGE(vscale), 0);
    
    
    //Label
    GtkWidget* label = gtk_label_new("");
    gtk_fixed_put(GTK_FIXED(fixed), label, 50, 65);
    gtk_label_set_text(GTK_LABEL(label), "This is a label");
     
     
    //Textbox
    textbox = gtk_entry_new();
    gtk_entry_set_max_length(GTK_ENTRY(textbox), 128);
    gtk_fixed_put(GTK_FIXED(fixed), textbox, 235, 45);
    gtk_widget_set_size_request(textbox, 150, 25);
    gtk_editable_set_editable(GTK_EDITABLE(textbox), 1);
    //todo: need to find how to gray it out when disabled
    gtk_entry_set_visibility(GTK_ENTRY(textbox), 1);//0=passwd hide    
    gtk_entry_set_text(GTK_ENTRY(textbox), "This is a ");
    gtk_entry_append_text(GTK_ENTRY(textbox), "textbox");
    //Get text:
    //const gchar* grab_text = gtk_entry_get_text(GTK_ENTRY(textbox));
    //uint16_t grab_text_length = gtk_entry_get_text_length(GTK_ENTRY(textbox));
    
    
    //Multi-line textbox (formatted text)
    GtkTextBuffer* multitext_buffer = gtk_text_buffer_new(NULL);
    GtkWidget* multiline_textbox = gtk_text_view_new_with_buffer(multitext_buffer);
    //gtk_fixed_put(GTK_FIXED(fixed), multiline_textbox, 150, 250);
    //gtk_widget_set_size_request(multiline_textbox, 150, 100);    
    //
    //Text options. There are many more options than shown here.
    gtk_text_buffer_set_text(multitext_buffer, "This is a multi-line text box\nThis can be used for some editor", -1);
    GdkColor text_color;
    gdk_color_parse("green", &text_color);
    gtk_widget_modify_text(multiline_textbox, GTK_STATE_NORMAL, &text_color);
    gtk_text_view_set_left_margin(GTK_TEXT_VIEW(multiline_textbox), 5);
    gtk_text_view_set_border_window_size(GTK_TEXT_VIEW(multiline_textbox), GTK_TEXT_WINDOW_LEFT, 4);//<Does nothing.
	//
	//The textbox must be inside a scrolled window for it to have a border and scrollbars:
    GtkWidget* scrolled_window = gtk_scrolled_window_new(NULL, NULL); 
    gtk_fixed_put(GTK_FIXED(fixed), scrolled_window, 235, 90);
    gtk_widget_set_size_request(scrolled_window, 150, 100);   
    gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(scrolled_window), GTK_SHADOW_IN);
    gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scrolled_window), multiline_textbox);	
    
    
    //Status bar
    statusbar =  gtk_statusbar_new();
    gtk_fixed_put(GTK_FIXED(fixed), statusbar, 0, 450);
    gtk_widget_set_size_request(statusbar, 620, 30);     
    sbid = gtk_statusbar_get_context_id(GTK_STATUSBAR(statusbar), "status_baar");
    gtk_statusbar_push(GTK_STATUSBAR(statusbar), sbid, " Status bar. Notifications or application help here");
    
    
    //Progress bar
    progressbar =  gtk_progress_bar_new();
    gtk_fixed_put(GTK_FIXED(fixed), progressbar, 220, 400);
    gtk_widget_set_size_request(progressbar, 180, 25);     
    gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progressbar), 0.3);
    gtk_progress_bar_set_text(GTK_PROGRESS_BAR(progressbar), "Progress bar");
    //
    //Tried to change bar color. Only changes label color. Maybe not possible.
    //A better approach for a custom look might be to just use a frame and an colored box or image bitmap.
    //GdkColor pbcolor;
    //gdk_color_parse("red",&pbcolor);
    //gtk_widget_modify_fg(progressbar, GTK_STATE_ACTIVE, &pbcolor); //nope
    
	
    
    //Image display on viewport frame (image2 loaded as pixbuf above)
    //Must be scaled manually to fit viewport.
  	GdkPixbuf* pbuf2 = gdk_pixbuf_scale_simple(pbuf1, 100, 100, GDK_INTERP_BILINEAR);    
    GtkWidget* image2 = gtk_image_new_from_pixbuf(pbuf2);  
    //
    GtkWidget* viewport1 = gtk_viewport_new(NULL, NULL);
    gtk_viewport_set_shadow_type(GTK_VIEWPORT(viewport1), GTK_SHADOW_IN);
    gtk_fixed_put(GTK_FIXED(fixed), viewport1, 455, 40);
    gtk_widget_set_size_request(viewport1, 106, 106);    
    gtk_container_add(GTK_CONTAINER(viewport1), image2);
    //
    //Alternatively, can just show floating image on its own, with no framing:
    //gtk_fixed_put(GTK_FIXED(fixed), image2, 220, 110);
    //gtk_widget_set_size_request(image2, 100, 100); //<does nothing
    //    
    //To detect mouse hover on image, just put an invisible event box over it 
    GtkWidget* event_box = gtk_event_box_new();
    gtk_fixed_put(GTK_FIXED(fixed), event_box, 455, 40);
    gtk_widget_set_size_request(event_box, 106, 106);	
	gtk_event_box_set_visible_window (GTK_EVENT_BOX(event_box), 0);
    gtk_event_box_set_above_child(GTK_EVENT_BOX(event_box), 1);//above child sends signals directly to it, simplest.
    //gtk_widget_set_events(event_box, GDK_POINTER_MOTION_MASK); 
    gtk_widget_add_events(event_box, (GDK_POINTER_MOTION_MASK | GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK ));   
    
    
    
    //Image, render arbitrarily generated bitmap data on viewport (i.e. use with camera or graphing, etc)
	//-You can draw with a gtk_drawing_area, but that uses the Cairo 2D system and might be slow and expensive.
	//-You can also draw with the X system and point here to the resulting frame buffer. See 2D_DRAW example project.
	//
	//For this example, just generate some random noise for a 100x100 32-bit image
	uint32_t rnd_data[(100*100)];
	for(uint32_t rn=0; rn<10000; rn++){ rnd_data[rn] = rand()%0xFFFFFFFF; }
	//
	const guchar*  dat = (uint8_t*)&rnd_data[0]; //argb
	GdkPixbuf* pbuf3 = gdk_pixbuf_new_from_data(dat, GDK_COLORSPACE_RGB, TRUE, 8, 100, 100, 400, NULL, NULL);
	GtkWidget* image3 = gtk_image_new_from_pixbuf(pbuf3);  
	//
	//On viewport with edge too
    GtkWidget* viewport2 = gtk_viewport_new(NULL, NULL);
    gtk_viewport_set_shadow_type(GTK_VIEWPORT(viewport2), GTK_SHADOW_IN);
    gtk_fixed_put(GTK_FIXED(fixed), viewport2, 455, 155);
    gtk_widget_set_size_request(viewport2, 106, 106);    
    gtk_container_add(GTK_CONTAINER(viewport2), image3);       

  
  
    //Notebook (Tab container)
    GtkWidget* notebook = gtk_notebook_new();
    gtk_notebook_set_show_tabs (GTK_NOTEBOOK(notebook), TRUE);
    gtk_notebook_set_show_border(GTK_NOTEBOOK(notebook), TRUE);
    gtk_fixed_put(GTK_FIXED(fixed), notebook, 450, 280);
    gtk_widget_set_size_request(notebook, 150, 150);	
	//
	//each page needs label and container
	GtkWidget* tab1_label = gtk_label_new("tab1");
	GtkWidget* tab1_frame = gtk_frame_new(NULL);
	GtkWidget* tab2_label = gtk_label_new("tab2");
	GtkWidget* tab2_frame = gtk_frame_new(NULL);
	//
	//make pages
	gtk_notebook_append_page(GTK_NOTEBOOK(notebook), tab1_frame, tab1_label);
	gtk_notebook_append_page(GTK_NOTEBOOK(notebook), tab2_frame, tab2_label);
	//
	//make a fixed reference point so contents are relative to tab, not main window
  	GtkWidget* tab1fixed = gtk_fixed_new();
    gtk_container_add(GTK_CONTAINER(tab1_frame), tab1fixed);
  	GtkWidget* tab2fixed = gtk_fixed_new();
    gtk_container_add(GTK_CONTAINER(tab2_frame), tab2fixed);
    //
	//add content to pages
	GtkWidget* tab1_content = gtk_label_new("content 1");
	gtk_fixed_put(GTK_FIXED(tab1fixed), tab1_content, 30, 30); //position relative to tab notebook
    //gtk_widget_set_size_request(tab1_content, 30, 30);
	//gtk_container_add(GTK_CONTAINER(tab1_frame), tab1_content);//<not needed since already 'put' on 'fixed' cointainer   
	//
	GtkWidget* tab2_content = gtk_label_new("content 2");
	gtk_fixed_put(GTK_FIXED(tab2fixed), tab2_content, 30, 50); //position relative to tab notebook
    //gtk_widget_set_size_request(tab2_content, 30, 30);	
	//gtk_container_add(GTK_CONTAINER(tab2_frame), tab2_content);//<not needed since already 'put' on 'fixed' cointainer      
	
	
	
    //Table (Array)
    //These can be buttons or other widgets. 
    //The table itself doesn't render if it's not populated.
    //
    GtkWidget* table = gtk_table_new(2, 2, TRUE);
    gtk_fixed_put(GTK_FIXED(fixed), table, 50, 325);
    gtk_widget_set_size_request(table, 100, 100);      
    //
    GtkWidget* table_button1 = gtk_button_new_with_label("B1");
    GtkWidget* table_button2 = gtk_button_new_with_label("B2");
    GtkWidget* table_button3 = gtk_button_new_with_label("B3");
    GtkWidget* table_button4 = gtk_button_new_with_label("B4");
    //        
    gtk_table_attach_defaults(GTK_TABLE(table), table_button1, 0, 1, 0, 1);	
    gtk_table_attach_defaults(GTK_TABLE(table), table_button2, 1, 2, 0, 1);	
    gtk_table_attach_defaults(GTK_TABLE(table), table_button3, 0, 1, 1, 2);	
    gtk_table_attach_defaults(GTK_TABLE(table), table_button4, 1, 2, 1, 2);    

        
    
    //Spreadsheet
    //Not available on gtk2, use array of text entries, although doesn't look quite right.
    //Gtk3 has a more appropriate 'Sheet' 
    
    
    
    //Menu
    //This is unnecessarily cumbersome lol.
    GtkWidget* menu_bar = gtk_menu_bar_new();
    gtk_fixed_put(GTK_FIXED(fixed), menu_bar, 0, 0);
    gtk_widget_set_size_request(menu_bar, 620, 25);   
    //
    //Menu 'File'
    GtkWidget* menu_file = gtk_menu_item_new_with_label("File");
    gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), menu_file);
    GtkWidget* menu_file_menu = gtk_menu_new();
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(menu_file), menu_file_menu);
    //Menu 'File' Items
    GtkWidget* menu_file_open = gtk_menu_item_new_with_label("Open");
	gtk_menu_shell_append(GTK_MENU_SHELL(menu_file_menu), menu_file_open);
    GtkWidget* menu_file_save = gtk_menu_item_new_with_label("Save");
	gtk_menu_shell_append(GTK_MENU_SHELL(menu_file_menu), menu_file_save);
    GtkWidget* menu_file_close = gtk_menu_item_new_with_label("Close");
	gtk_menu_shell_append(GTK_MENU_SHELL(menu_file_menu), menu_file_close);
    //
    //Menu 'About'
    GtkWidget* menu_about = gtk_menu_item_new_with_label("About");
    gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), menu_about);
    GtkWidget* menu_about_menu = gtk_menu_new();
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(menu_about), menu_about_menu);
    //Menu 'About' Items
    GtkWidget* menu_about_legend = gtk_menu_item_new_with_label("Statorworks Gtk2 Demo 2020");
	gtk_menu_shell_append(GTK_MENU_SHELL(menu_about_menu), menu_about_legend);    



	//GTK EVENT CALLBACKS
	g_signal_connect(button1, "clicked", G_CALLBACK(callback_button_clicked),NULL);
	g_signal_connect(checkbox1, "clicked", G_CALLBACK(callback_checkbox_changed),NULL);	
	g_signal_connect(rbutton1, "clicked", G_CALLBACK(callback_rbutton1_clicked),NULL);	
	g_signal_connect(rbutton2, "clicked", G_CALLBACK(callback_rbutton2_clicked),NULL);
	//The 2x2 button array would be same as other buttons, omitted here.
	g_signal_connect(toggle_button, "toggled", G_CALLBACK(callback_toggle_button),NULL);
	g_signal_connect(spin_button, "value-changed", G_CALLBACK(callback_spin_button_changed),NULL);	
	//
	g_signal_connect(hscale, "value-changed", G_CALLBACK(callback_hscale_changed),NULL);
	g_signal_connect(vscale, "value-changed", G_CALLBACK(callback_vscale_changed),NULL);
	//
	g_signal_connect(textbox, "changed", G_CALLBACK(callback_textbox_changed),NULL); //Any key pressed
	g_signal_connect(textbox, "activate", G_CALLBACK(callback_textbox_enter),NULL);	 //'Enter' key presed
	//
	//g_signal_connect(event_box, "motion_notify_event", G_CALLBACK(callback_event_box_mouse_click),NULL); //<?		
	g_signal_connect(event_box, "motion-notify-event", G_CALLBACK(callback_event_box_mouse_move),NULL);	   //mouse move	
    g_signal_connect(event_box, "button_press_event", G_CALLBACK(callback_event_box_mouse_button_press),NULL);  		
    g_signal_connect(event_box, "button_release_event", G_CALLBACK(callback_event_box_mouse_button_release),NULL);	
 	//	
	//For the menu bar, there should be a better way than doing one by one. for now this flies.
	g_signal_connect(G_OBJECT(menu_file_open), "activate", G_CALLBACK(callback_menu_file_open), NULL);
	g_signal_connect(G_OBJECT(menu_file_save), "activate", G_CALLBACK(callback_menu_file_save), NULL);	
	g_signal_connect(G_OBJECT(menu_file_close), "activate", G_CALLBACK(callback_menu_file_close), NULL);
	g_signal_connect(G_OBJECT(menu_about_legend), "activate", G_CALLBACK(callback_menu_about_legend), NULL);
	//
    g_signal_connect(G_OBJECT(window), "delete-event", G_CALLBACK(callback_window_close), NULL);//<window closed
	//g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(callback_close), NULL);//<won't happen if 'delete-event' happens
        	
	
	
	//Show
    //gtk_widget_show (window);
    gtk_widget_show_all(window);
	printf("Gtk2 window spawned, entering main loop\n"); 
	

	//Gtk blocking loop. Use this if you want gtk event-driven only, with no other main loop.
    //gtk_main (); 


	//With Regular main loop:
    while ( 1 ){
 
               
       //Simple wait tick timeslice
       usleep(5000);  //~100-200Hz for gtk to be responsive. Find a better way.
        
       //Application update
       //update(timelapse);
	   //printf("timeslice\n"); 
 	 
	   //Gtk window events update, non-blocking
	   gtk_main_iteration_do(0); 	   
 
       //Detect window close to actually exit program
       //This can happen from menu->close, or from the window close button. 
       if(window_closed==1){break;}
    }

    //cleanup
    //gtk_close();<?? 

    return(0);
}




void callback_button_clicked(GtkWidget *widget, gpointer data){
 
 //printf("Button Clicked\n");
 gtk_statusbar_push(GTK_STATUSBAR(statusbar), sbid, " Button clicked");
}

void callback_checkbox_changed(GtkWidget *widget,gpointer data){
 
 bool  state = gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON(checkbox1));
 
 if(state==0){
   //printf("Checkbox unchecked\n");
   gtk_statusbar_push(GTK_STATUSBAR(statusbar), sbid, " Checkbox unchecked");
 }
 else{
   //printf("Checkbox checked\n");
   gtk_statusbar_push(GTK_STATUSBAR(statusbar), sbid, " Checkbox checked");
 }
}

void callback_rbutton1_clicked(GtkWidget *widget, gpointer data){
 
 //printf("Radio Button 1\n");
 gtk_statusbar_push(GTK_STATUSBAR(statusbar), sbid, " Radio button 1 clicked");
}

void callback_rbutton2_clicked(GtkWidget *widget, gpointer data){
 
 //printf("Radio Button 2\n");
 gtk_statusbar_push(GTK_STATUSBAR(statusbar), sbid, " Radio button 2 clicked");
}

void callback_toggle_button(GtkWidget *widget, gpointer data){
 
 bool state = gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON(toggle_button));
 
 if(state==0){ 
	//printf("Toggle button: Off\n"); 
    gtk_statusbar_push(GTK_STATUSBAR(statusbar), sbid, " Toggle button: Off");
  }
 else{ 
	//printf("Toggle button: On\n"); 
    gtk_statusbar_push(GTK_STATUSBAR(statusbar), sbid, " Toggle button: On");	 
  }
}

void callback_spin_button_changed(GtkWidget *widget, gpointer data){
 
 uint16_t val = gtk_spin_button_get_value( GTK_SPIN_BUTTON(spin_button) );  
 
 //printf("Spin button changed. Value: %d\n", val);
 
 char str[64];
 sprintf(str, " Spin button changed. Value: %d", val);
 gtk_statusbar_push(GTK_STATUSBAR(statusbar), sbid, str);
}

void callback_hscale_changed(GtkWidget *widget, gpointer data){
 
 uint16_t val = gtk_range_get_value(GTK_RANGE(hscale));

 //just for fun, change the progress bar with this
 gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progressbar), ((float)val)/100); //0-100 to 0.0-1.0
 
 //printf("HScale changed. Value: %d\n", val);
 
 char str[64];
 sprintf(str, " HScale changed. Value: %d", val);
 gtk_statusbar_push(GTK_STATUSBAR(statusbar), sbid, str);
}

void callback_vscale_changed(GtkWidget *widget, gpointer data){
 
 uint16_t val = gtk_range_get_value(GTK_RANGE(vscale));
 
 //printf("VScale changed. Value: %d\n", val);
 
 char str[64];
 sprintf(str, " VScale changed. Value: %d", val);
 gtk_statusbar_push(GTK_STATUSBAR(statusbar), sbid, str);
}

void callback_textbox_changed(GtkWidget *widget, gpointer data){
	
 uint16_t len = gtk_entry_get_text_length(GTK_ENTRY(textbox));
 char str[len];
 	
 const gchar* text = gtk_entry_get_text(GTK_ENTRY(textbox));
 strcpy(str, text);

 //printf(str); printf("\n");
 gtk_statusbar_push(GTK_STATUSBAR(statusbar), sbid, str);
}


void callback_textbox_enter(GtkWidget *widget, gpointer data){
 //Enter key pressed to commit some text
	
 uint16_t len = gtk_entry_get_text_length(GTK_ENTRY(textbox));
 char str[len];
 	
 const gchar* text = gtk_entry_get_text(GTK_ENTRY(textbox));
 strcpy(str, text);

 //printf("Ener key pressed on textbox\n");
 //printf(str); printf("\n");
 gtk_statusbar_push(GTK_STATUSBAR(statusbar), sbid, str);
}


//------------MOUSE------------------


void callback_event_box_mouse_button_press(GtkWidget *widget, gpointer data){

 gint x, y;	
 gtk_widget_get_pointer(widget, &x, &y);

 //todo: figure out if left or right button
 
 //printf("Mouse down on Event Box at: x=%d, y=%d\n", x, y); 	

 char str[100];
 sprintf(str, " Mouse down on Event Box at: x=%d, y=%d", x, y);
 
 gtk_statusbar_push(GTK_STATUSBAR(statusbar), sbid, str);	
}

void callback_event_box_mouse_button_release(GtkWidget *widget, gpointer data){

 gint x, y;	
 gtk_widget_get_pointer(widget, &x, &y);

 //todo: figure out if left or right button
 
 //printf("Mouse up on Event Box: x=%d, y=%d\n", x, y); 	

 char str[100];
 sprintf(str, " Mouse up on Event Box at: x=%d, y=%d", x, y);
 
 gtk_statusbar_push(GTK_STATUSBAR(statusbar), sbid, str);	
}

void callback_event_box_mouse_move(GtkWidget *widget, gpointer data){

 gint x, y;	
 gtk_widget_get_pointer(widget, &x, &y);
 
 //printf("Mouse move on Event Box: x=%d, y=%d\n", x, y); 	

 char str[100];
 sprintf(str, " Mouse move on Event Box: x=%d, y=%d", x, y);
 
 gtk_statusbar_push(GTK_STATUSBAR(statusbar), sbid, str);	
}


//------------MENU BAR------------------


void callback_menu_file_open(GtkWidget* widget, gpointer data){
    
  //printf("menu: file->open\n");
  gtk_statusbar_push(GTK_STATUSBAR(statusbar), sbid, " Menu: file->open");
}

void callback_menu_file_save(GtkWidget* widget, gpointer data){
    
  //printf("menu: file->save\n");
  gtk_statusbar_push(GTK_STATUSBAR(statusbar), sbid, " Menu: file->save");
}

void callback_menu_file_close(GtkWidget* widget, gpointer data){
    
  //printf("menu: file->close\n");
  gtk_statusbar_push(GTK_STATUSBAR(statusbar), sbid, " Menu: file->close");
  
  //window_closed=1;//<tell main loop to exit
}

void callback_menu_about_legend(GtkWidget* widget, gpointer data){
    
  //printf("menu: about->legend\n");
  gtk_statusbar_push(GTK_STATUSBAR(statusbar), sbid, " Menu: about->legend");
}


//---------WINDOW CLOSE-----------


void callback_window_close(GtkWidget* widget, gpointer data){
    
  window_closed=1;//<tell main loop to exit
}
