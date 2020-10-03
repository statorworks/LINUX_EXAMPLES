#ifndef MAIN_H
#define MAIN_H 
//
//2020 Statorworks
//Basic Timer and interrupt signal handling example. 
//Includes a graphic window to show how real-time updates and window events can coexist. 
//
//Note that Linux can't guaranteee hard realtime at the uS level on the interval timer, 
//but for an application timeslice period of 20-100Hz it works real well.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>//bool
#include <sched.h>


//For X11, need: apt-get install libx11-dev
//Then find location: locate libX11
//On project->properties->build,  add location found and reference to x11 for Linker:
//g++ -Wall -o "%e" "%f" -L/usr/lib/x86_64-linux-gnu -lX11
//On Raspbian:
//Compile: g++ -Wall -std=c++11 -c "%f"
//Build:   g++ -Wall -std=c++11 -o "%e" *.cpp -L/usr/lib/arm-linux-gnueabihf -lX11 -L/usr/lib
#include <X11/Xlib.h> 


//Timer
//Note: Need to build with -lrt option for timer_create() to be seen. 
#include <signal.h>
#include <time.h>
#include <sys/time.h>

 
timer_t  timerid;
struct   sigevent sev;
sigset_t smask;
struct   sigaction saction;
struct   itimerval interval;
//struct itimerspec interval;

	
//flags	
bool timeslice;
bool closing;
int  timeslice_count; 
 
void timer_handler (int signo); 
void close_handler (int signo); 

 
#endif

