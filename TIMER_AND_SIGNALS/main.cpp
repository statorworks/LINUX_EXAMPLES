//
//2020 Statorworks
//Basic Timer and interrupt signal handling example. 
//Includes a graphic window to show how real-time updates and window events can coexist. 
//
//Note that Linux can't guaranteee hard realtime at the uS level on the interval timer, 
//but for an application timeslice period of 20-100Hz it works real well.	

#include "main.h"


int main (int argc, char *argv[]){
    
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
    XEvent                  event;
    char                    hello_string[] = "Hello World";
    int                     hello_string_length = strlen(hello_string);

    
    //Open a connection to the X-Windows server and obtain a handle to the display, 
    //the visual type of display and the plane depth (Microsoft Windows uses a single 
    //plane display) of the display.
    display = XOpenDisplay(NULL);
    visual = DefaultVisual(display, 0);
    depth  = DefaultDepth(display, 0);
    
    //The next two lines sets the background colour for the window we are 
    //about to open and then opens the window. This is very similar to the 
    //Microsoft CreateWindow API call.    
    frame_attributes.background_pixel = XWhitePixel(display, 0);
   
    /* create the application window */
    frame_window = XCreateWindow(display, XRootWindow(display, 0),
                                 0, 0, 400, 400, 5, depth,
                                 InputOutput, visual, CWBackPixel,
                                 &frame_attributes);
   
   
    //sets the text in the title bar to 'Hello World Example' 
    //and is equivalent to the Microsoft SetWindowText API call.   
    XStoreName(display, frame_window, "Hello World Example");
    
    
    //Tell the X-Windows server the events that we are interested in for 
    //the window we have just created. Remember, because X-Windows is client/server 
    //based we have the ability to restrict the events that we receive for an 
    //individual window and thus reduce the amount of network traffic.    
    XSelectInput(display, frame_window, ExposureMask | StructureNotifyMask | ButtonPressMask);
    //
    //also need a mask with these for here for XCheckWindowEvent() non-blocking call
    long event_mask = ExposureMask | StructureNotifyMask | KeyPressMask | ButtonPressMask; 


    //The next four lines create a suitable font for our 'Hello World' text, 
    //and create a device context suitable for the display and window on that
    //display. These are equivalent to Microsoft CreateFont and GetDC API calls.
    fontinfo = XLoadQueryFont(display, "6x13");//"10x20");  //NO 10x20 on Raspbian! gives segmentation fault
    gr_values.font = fontinfo->fid;
    gr_values.foreground = XBlackPixel(display, 0);
    graphical_context = XCreateGC(display, frame_window, GCFont+GCForeground, &gr_values);
    
    
    //Display the frame window on the display and is equivalent to the Microsoft ShowWindow API call.
    XMapWindow(display, frame_window);


    //Allow intercepting the window close event
    Atom wm_delete_window = XInternAtom(display, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(display, frame_window, &wm_delete_window, 1);



    //--------------------------------

	
    //Signal action for interval timer
    sigemptyset(&saction.sa_mask); 			//begin with no signals blocked for process thread
    saction.sa_flags   	 = 0;				//reset signal on call
    //Note: sa_handler and sa_sigaction are union, do not assign to both.
    saction.sa_handler 	 = timer_handler;		//simple callback
    //saction.sa_sigaction = NULL;   			//callback for queued signals
    //
    if(sigaction(SIGALRM, &saction, NULL) == -1)  	//enable interrupt signal for timer completion
    { printf("Could not register SIG_TIMER signal action\n"); }
    else{ printf("Timer signal registered Ok\n"); }
    //	
    //If you want to block or unblock the signal 
    sigset_t smask;
    sigemptyset(&smask);
    sigaddset(&smask, SIGALRM);
    //sigprocmask(SIG_BLOCK,   &smask, NULL);//block
    sigprocmask(SIG_UNBLOCK, &smask, NULL);//unblock

	
    //Create the interval timer
    //sev.sigev_notify = SIGEV_SIGNAL;      //timer will produce this signal
    //sev.sigev_signo  = SIGALRM;           //
    //sev.sigev_value.sival_ptr = &timerid; // 
    //if(timer_create(CLOCK_REALTIME, &sev, &timerid) == -1)
    //{ printf("Could not create timer"); }
    //else{ printf("Timer created Ok\n"); }
    //

    //Start the timer by setting the interval.
    //Interval is reset automatically on completion. To stop timer you cna set interval to 0.
    //note that value and interval are the initial interval and subsequent intervals respectively.
    //interval.it_value.tv_sec  = 3;
    //interval.it_value.tv_nsec = 0;//20000000;
    //interval.it_interval.tv_sec  = 3;
    //interval.it_interval.tv_nsec = 0;//20000000; //20ms=50Hz
    //if(timer_settime(timerid, 0,  &interval, NULL) ==-1)
    //{ printf("Could not set timer\n"); }
    //else{ printf("Timer set Ok\n"); }


    //Alternatively, No need to create interval timer above, 
    //the system provides a default one for each process.
    interval.it_value.tv_sec  = 0;    
    interval.it_value.tv_usec = 20000;  //20ms=50Hz
    interval.it_interval.tv_sec  = 0;
    interval.it_interval.tv_usec = 20000;   
    if(setitimer(ITIMER_REAL, &interval, NULL)==-1)
    { printf("Could not start timer"); }
    else{ printf("Timer started Ok\n"); }	
	
	
    //Also handle the terminate process signal.
    //This is useful if there is no graphic window for the program to be closed 
    //or if the process is terminated from the command line 	
    //We handle the signal SIGTERM request, note that SIGKILL is immediate and cannot be caught. 
    sigemptyset(&saction.sa_mask);
    saction.sa_flags   	 = 0;
    saction.sa_handler 	 = close_handler;
    if(sigaction(SIGTERM, &saction, NULL) == -1)  //interrupt signal for normal process termination
    { printf("Could not register SIGTERM signal action\n"); }		
    else{ printf("SIGTERM signal registered Ok\n"); }	


    //flags
    closing=0;
    timeslice=0;
    timeslice_count=0;
		
		
    //Main loop
    while(1){
        
	//Wait for next timeslice.
	//Save CPU time and potentially some power by sleeping the thread.
	//The sleep terminates automatically when the interval timer signal comes.
	//In this example process CPU usage is 50% without sleep and 1% with sleep.
	//Note that ports as the i2c may be briefly affected as the process resumes though. 
        usleep(10000); 
	//
	if(timeslice==1){

	  timeslice=0;		
	  timeslice_count++;
	  printf("timeslice %d\n",  timeslice_count);
	  
	  //update(); //Application real time update 
	}      


	//Also handle window events
        //XNextEvent(display, (XEvent *)&event);//blocking
        memset(&event, 0, sizeof(XEvent));
	XCheckWindowEvent(display, frame_window, event_mask, (XEvent *)&event); //non-blocking
	switch(event.type) {
            case Expose:
            {
                //XWindowAttributes window_attributes;
		//int font_direction, font_ascent, font_descent;
		//XCharStruct text_structure;
		//XTextExtents(fontinfo, hello_string, hello_string_length, &font_direction, &font_ascent, &font_descent, &text_structure);
                //XGetWindowAttributes(display, frame_window, &window_attributes);
		//text_x = (window_attributes.width - text_structure.width)/2;
		//text_y = (window_attributes.height - (text_structure.ascent+text_structure.descent))/2;
                //XDrawString(display, frame_window, graphical_context, text_x, text_y, hello_string, hello_string_length);
                break;
            }
	    case ButtonPress:
	    {
		//any mouse button
		//printf("button\n");
		break;
	    } 
	    default:
	    break;
        }    
   
   
	//Detect close window event here. CanNot be done with XCheckWindowEvent above.
	if(XCheckTypedWindowEvent(display, frame_window, ClientMessage, (XEvent *)&event)==True)
	{ 
	  //handle window closing
	  if((Atom)event.xclient.data.l[0] == wm_delete_window){
	  printf("Close window event\n");
	  closing=1;
         }
	}
   
	//Exit loop after SIGTERM signal or Window close event
	if(closing ==1){ break; }

    }//

    	
  //Clean up
  if(display!=0){ XCloseDisplay(display); }	
  //
  interval.it_value.tv_usec = 0;
  interval.it_interval.tv_usec = 0; 
  setitimer(ITIMER_REAL, &interval, NULL);
  //
  printf("Closing process...\n");
  usleep(2000000);

	
  return(0);
}


void timer_handler(int signo)
{
  timeslice =1;
}


void close_handler(int signo)
{
  printf("Process Terminate Signal\n");//printf should normally not be called here, just for demonstration.
  closing =1;
}
