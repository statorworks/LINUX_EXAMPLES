//
//2020 Statorworks
//Webcam loading and displaying
//capturing from UVC webcam with V4l on VLinux Debian
//This demo uses two cameras to show that fast simultaneous transfer and display is possible
//More cameras are possible, depending on USB bandwidth.

//compile commands
//++ -Wall -std=c++11 -o "WEBCAM.x" -I$HOME/ffmpeg/include  *.cpp -L/usr/lib/x86_64-linux-gnu -lX11 -L/usr/lib -L$HOME/ffmpeg/lib -lav-deb -lavcodec -lavutil -ljpeg

#include "main.h"


int main (int argc, char *argv[]){
    
     
    //Open a connection to the X-Windows server and obtain a handle to the display, 
    //the visual type of display and the plane depth (Microsoft Windows uses a single plane display) of the display.
    display = XOpenDisplay(NULL);
    visual = DefaultVisual(display, 0);
    depth  = DefaultDepth(display, 0);
     
    
    //The next two lines sets the background colour for the window we are 
    //about to open and then opens the window. This is very similar to the 
    //Microsoft CreateWindow API call.
    frame_attributes.background_pixel = 0xEEEEEE;   
    //frame_attributes.bit_gravity = StaticGravity;


    //Create the application window
    u32 back = 0; //CWBackPixel; //no background so e have no flicker updating the camera bitmap  //CWBackPixel
    frame_window = XCreateWindow(display, XRootWindow(display, 0),
                                 150, 150, (CAMERA_WIDTH*2), CAMERA_HEIGHT, 15, depth,
                                 InputOutput, visual, back,
                                 &frame_attributes);  
   
    //Location and size are ignored on xcreatewindow for top level windows
    //So they have to be 'hinted' to the window manager    
    XSizeHints    my_hints = {0};
	my_hints.flags  = PPosition;// | PSize;     // I want to specify position and size 
	my_hints.x      = 150;    // The origin and size coords I want 
	my_hints.y      = 150;
  
   
    //sets the text in the title bar to 'Hello World Example' 
    //and is equivalent to the Microsoft SetWindowText API call.   
    XStoreName(display, frame_window, "Webcam Example");
    
    
    //Tell the X-Windows server the events that we are interested in for 
    //the window we have just created. Remember, because X-Windows is client/server 
    //based we have the ability to restrict the events that we receive for an 
    //individual window and thus reduce the amount of network traffic.    
    XSelectInput(display, frame_window, ExposureMask | StructureNotifyMask);


    //The next four lines create a suitable font for our 'Hello World' text, 
    //and create a device context suitable for the display and window on that
    //display. These are equivalent to Microsoft CreateFont and GetDC API calls.
    fontinfo = XLoadQueryFont(display, "10x20");
    gr_values.font = fontinfo->fid;
    //gr_values.foreground = XBlackPixel(display, 0);
    gr_values.foreground = 0xFF0000; 
    gcx = XCreateGC(display, frame_window, GCFont+GCForeground, &gr_values);
    
    
    //Request desired process priority
    //setpriority(PRIO_PROCESS, 0, -20);    

	
	//Some startup info
	printf("\n------------------------\n");
	printf("MAX_CAMERAS = %d\n", MAX_CAMERAS);
	printf("FIRST_CAMERA_INDEX = %d\n", FIRST_CAMERA_INDEX);
    
    
    //Webcams
    u32 w, h, ci;
    for(u8 i=0; i<MAX_CAMERAS;i++){
		
		//for testing convenience, we can begin with /dev/video0 or /dev/video1 etc 
		ci = FIRST_CAMERA_INDEX +i;
		
		//Usually fixed camera (i.e. laptop) is /dev/video0, 
		//and usb cams are /dev/video1, 2  and so on, as they are plugged in 
		//format 0:yuyv  1:mjpeg  2:try mjpeg, fallback to yuyv
		cameras[i].open(ci, CAMERA_WIDTH, CAMERA_HEIGHT, 0); //yuyv  
		
		//Set camera picture parameters
		//cameras[i].set_parameters(0.5, 0.5, 0.5, 0.5, 0.5); //not implemented yet
		
		//allocate camera argb frame buffers to fit actual camera resolution obtained
		framergb[i] = (uint8_t*) calloc((cameras[i].width * cameras[i].width * 4), sizeof (uint8_t));
       
		//image abstraction for xlib drawing
		ximages[i] = XCreateImage(display, visual, depth, ZPixmap, 0, NULL, 0, 0, 32, 0);
		w = cameras[i].width;
		h = cameras[i].height;
		if(w==0){ w= CAMERA_WIDTH;  }  //must be non-zero
		if(h==0){ h= CAMERA_HEIGHT; }  //below in case camera failed initializing
		//
		pixmaps[i] = XCreatePixmap(display, frame_window, w, h, depth); 
	 }

	//Determine size for window to fit number of cameras supported on this demo.
	//for one camera, just fit it
	if(MAX_CAMERAS==1)
	{ w = CAMERA_WIDTH; h = CAMERA_HEIGHT; }
	//for multi, use 2 columns and n rows. i.e for max cameras = 5, there would be 2x3 tiles  
	else
	{ w = (CAMERA_WIDTH*2); h = (CAMERA_HEIGHT*((MAX_CAMERAS+1)/2));  }
	
	//resize window
	XResizeWindow(display, frame_window, w, h); 
	
    //Make window non-resizable:
    my_hints.flags  |= PMinSize | PMaxSize; 
    my_hints.min_width = my_hints.max_width = w;
    my_hints.min_height =  my_hints.max_height = h;    
    XSetNormalHints(display, frame_window, &my_hints); 	
	
	
    //Display the frame window on the display.  Equivalent to the Microsoft ShowWindow API call.
    XMapWindow(display, frame_window);
	XFlush(display);
	
	
	//Main loop
    while ( 1 ){
        
        //Wait tick timeslice
        usleep(30000);  //~30Hz
        
        //Application update
        //update(timelapse);
        
        //Window events
        //XNextEvent(display, (XEvent *)&event);             //<blocking, useful for event-only style program,  
        XCheckTypedEvent(display, Expose, (XEvent *)&event); //<non-blocking, for periodic timeslice style
        //
        switch ( event.type ) {
            case Expose:
            {
                                
                //Render camera frames
                draw_frame();
                 
                break;
            }
            default:
                break;
        }
    }

    //cleanup
    close();


    return(0);
    
}

bool draw_frame(void){
  
  //checks 
  if(gcx == NULL)			 { return 0; }
    
  u32 orx, ory;   
     
  for(u8 i=0; i<MAX_CAMERAS; i++){
  
    
	//Coordinates where to draw the image for each of our cameras
	orx = (i%2) * CAMERA_WIDTH;   //two columns 
	ory = (i/2) * CAMERA_HEIGHT;
      
	//flat color if camera not running
	if(cameras[i].state==0){ 
		XSetForeground(display, gcx, 0xFF005500); //dark green 
		XFillRectangle(display, frame_window, gcx, orx, ory, CAMERA_WIDTH, CAMERA_HEIGHT);  
		continue;
	}
  

	//Grab frame
	//measured about 5ms for this @320x240YUYV with gettimeofday function
	if(cameras[i].get_frame() ==0) {continue;}  

        
    //Setup/refresh image descriptor entity	
	ximages[i]->data 	= (char*)cameras[i].frame_data;
	ximages[i]->width  	= cameras[i].width;
	ximages[i]->height	= cameras[i].height;
	ximages[i]->depth 	= depth; 
	ximages[i]->bitmap_pad = 32;
	ximages[i]->bytes_per_line = 4* cameras[i].width;
    
    //Set clip rectangle in case webcam has different resolution than intended
        
    
	//Render bitmap on window.  todo: need to bea able to do it with only PutImage
	if(pixmaps[i] == 0) { continue; }
	XPutImage(display, pixmaps[i], gcx, ximages[i], 0, 0, 0, 0, cameras[i].width, cameras[i].height);
	XCopyArea(display, pixmaps[i], frame_window, gcx, 0, 0, cameras[i].width, cameras[i].height, orx, ory);

	//any text or other drawing goes on same graphics context
	//XSetForeground(display, gcx, 0xFFFF0000); 
	//XDrawRectangle(display, frame_window, gcx, 30, 180, 250, 40);

	//some text (font loaded on init) 
	//XDrawString(display, frame_window, gcx, 100, 210, "abc", 3);  
  
  }//next camera
 
 return 1;	
}

bool close(void){
 
 //cleanup	
 for(u8 i = 0; i<MAX_CAMERAS; i++){
	free(framergb[i]);
	if(ximages[i] !=NULL) { XDestroyImage(ximages[i]); } //<crashes as it also tries to deallocate the buffer. Don't use it unless you want that too.
	if(pixmaps[i] !=0) { XFreePixmap(display, pixmaps[i]); } 
	cameras[0].close(); 
 }
 
 XFreeGC(display, gcx);
 
 return 1;	 
}

