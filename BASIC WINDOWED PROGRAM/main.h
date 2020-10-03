#ifndef MAIN_H
#define MAIN_H 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sched.h>


//For X11, need: apt-get install libx11-dev
//Then find location: locate libX11
//On project->properties->build,  add location found and reference to x11 for Linker:
//g++ -Wall -o "%e" "%f" -L/usr/lib/x86_64-linux-gnu -lX11
//On Raspbian:
//Compile: g++ -Wall -std=c++11 -c "%f"
//Build:   g++ -Wall -std=c++11 -o "%e" *.cpp -L/usr/lib/arm-linux-gnueabihf -lX11 -L/usr/lib
#include <X11/Xlib.h> 

//Also may want:
//apt-get install mesa-common-dev........ for GL/glx.h
//apt-get install libglu1-mesa-dev ..... for GL/glu.h
//apt-get install libxrandr-dev ........... for X11/extensions/Xrandr.h
//apt-get install libxi-dev ................... for X11/extensions/XInput.h


 
#endif

