//
//Basic Window on Raspbian
//
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
    XEvent               event;
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
    XSelectInput(display, frame_window, ExposureMask | StructureNotifyMask);


    //The next four lines create a suitable font for our 'Hello World' text, 
    //and create a device context suitable for the display and window on that
    //display. These are equivalent to Microsoft CreateFont and GetDC API calls.
    fontinfo = XLoadQueryFont(display, "6x13");//"10x20");  //NO 10x20 on Raspbian! gives segmentation fault
    gr_values.font = fontinfo->fid;
    gr_values.foreground = XBlackPixel(display, 0);
    graphical_context = XCreateGC(display, frame_window, GCFont+GCForeground, &gr_values);
    
    
    //Display the frame window on the display and is equivalent to the Microsoft ShowWindow API call.
    XMapWindow(display, frame_window);

  

	//Message processing loop.
	//The only event we are interested in is the Expose event.
	//Upon receipt of an expose event, we get the bounding rectangle size for the 
	//text 'Hello World' using the XTextExtents call (Microsoft GetTextExtentPoint32 API call); 
	//get the width and height of the window using the XGetWindowAttributes call (Microsoft GetWindowInfo API call) 
	//and finally draw the string using XDrawString call (Microsoft DrawText API call).
    while ( 1 ) {
        
        //Wait tick timeslice
        //sched_yield();  
        //get timelapse
        
        //Application update
        //update(timelapse);
                //Window events
        XNextEvent(display, (XEvent *)&event);
        switch ( event.type ) {
            case Expose:
            {
                XWindowAttributes window_attributes;
             //   int font_direction, font_ascent, font_descent;
             //   XCharStruct text_structure;
             //   XTextExtents(fontinfo, hello_string, hello_string_length, 
              ////               &font_direction, &font_ascent, &font_descent, 
               //              &text_structure);
             //   XGetWindowAttributes(display, frame_window, &window_attributes);
              //  text_x = (window_attributes.width - text_structure.width)/2;
              //  text_y = (window_attributes.height - 
               //           (text_structure.ascent+text_structure.descent))/2;
               // XDrawString(display, frame_window, graphical_context,
               //             text_x, text_y, hello_string, hello_string_length);
                break;
            }
            default:
                break;
        }
    
    }


    return(0);
    
}





 



