#ifndef MAIN_H
#define MAIN_H 

//2020 Statorworks
//Webcam loading and displaying
//capturing from UVC webcam with V4l on VLinux Debian
//This demo uses two cameras to show that fast simultaneous transfer and display is possible. 
//More cameras are possible, depending on USB bandwidth.

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


//Process priority
#include <sys/time.h>
#include <sys/resource.h>


//webcam
#include "camera.h"
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
 GC                      gcx;
 XEvent               	 event;
 char                    hello_string[] = "Multi WebCam";
 int                     hello_string_length = strlen(hello_string);
  
 
 //Resolution
 //#define CAMERA_WIDTH  160   
 //#define CAMERA_HEIGHT 120   
 #define CAMERA_WIDTH  320  
 #define CAMERA_HEIGHT 240 
 //#define CAMERA_WIDTH  640  
 //#define CAMERA_HEIGHT 480  
 
 
 //We'll show these many simultaneous cameras in this demo
 #define MAX_CAMERAS 4 
 camera cameras[MAX_CAMERAS];  
 #define FIRST_CAMERA_INDEX 0  //begin with /dev/video0 or /dev/video1 etc
 
 
 //frame bitmap drawing
 unsigned char* framergb[MAX_CAMERAS]; //todo:need to get rid of double copy
 XImage* ximages[MAX_CAMERAS];         //image abstraction for frame to display
 Pixmap  pixmaps[MAX_CAMERAS];         //bitmap to hold frame bfore display
 

 //functions
 bool draw_frame(void);
 bool close(void);
 
#endif

