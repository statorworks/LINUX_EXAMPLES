#ifndef MAIN_H
#define MAIN_H 

//Basic 2D Drawing with XLib on a Window
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sched.h>

//For multiple module project need *.cpp instead of "%f" on build command
//so all modules get compiled. Otherwise you get 'undefined reference' errors

//For X11, need: apt-get install libx11-dev
//Then find location: locate libX11
//On project->properties->build, add location found and reference to x11 for Linker:
//Example: g++ -Wall -o "%e" "%f" -L/usr/lib/x86_64-linux-gnu -lX11
//Note that X11 does not provide support for window controls, only basic drawing
#include <X11/Xlib.h> 
#include <X11/Xutil.h>
#include <X11/Xresource.h>



//Also may want:
//apt-get install mesa-common-dev........ for GL/glx.h
//apt-get install libglu1-mesa-dev ..... for GL/glu.h
//apt-get install libxrandr-dev ........... for X11/extensions/Xrandr.h
//apt-get install libxi-dev ................... for X11/extensions/XInput.h


//Process priority
#include <sys/time.h>
#include <sys/resource.h>

//
#include "types.h"
#include "time.h"


//Window
 Display                 *display;
 Visual                  *visual;
 int                     depth;
 int                     text_x;
 int                     text_y;
 XSetWindowAttributes    frame_attributes;
 Window                  frame_window;
 XFontStruct             *fontinfo;
 XGCValues               gr_values;
 GC                      graphical_context;
 XEvent               	 event;
 char                    hello_string[] = "WebCam";
 int                     hello_string_length = strlen(hello_string);
  
 #define WINDOW_WIDTH 640 //320
 #define WINDOW_HEIGHT 480 //240 
  
 
 //frame buffer?
 Pixmap output_pixmap0=0;   
 
 //Need graphic context asociated with bitmap, like win32's DC
 XGCValues grv;
 GC gcx;
 
 //some bitmap
 unsigned char* myrgb0;     //buffer for some bitmap
 XImage* image0= 0;         //image abstraction for bitmap 
  
 //test motion
 s16 rect_x;
 s8 rect_dx;

 
 //functions
 bool draw_frame(void);
 bool draw_text(void);
 bool close(void);
 
#endif

