//
//Basic 2D drawing on X window
//capturing from UVC webcam with V4l on VLinux Debian


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
    u32 back = 0; //CWBackPixel; //no background so we have no flicker updating the rendering bitmap  //CWBackPixel
    frame_window = XCreateWindow(display, XRootWindow(display, 0),
                                 150, 150, WINDOW_WIDTH, WINDOW_HEIGHT, 15, depth,
                                 InputOutput, visual, back,
                                 &frame_attributes);  
   
    //Location and size are ignored on xcreatewindow for top level windows
    //So they have to be 'hinted' to the window manager    
    XSizeHints    my_hints = {0};
	my_hints.flags  = PPosition;// | PSize;     // I want to specify position and size 
	my_hints.x      = 150;    // The origin and size coords I want 
	my_hints.y      = 150;
	//my_hints.width  = (CAMERA_WIDTH*2);   //we're displaying two cameras side by side
    //my_hints.height =  CAMERA_HEIGHT;     
    //
    //Make window non-resizable:
    my_hints.flags  |= PMinSize | PMaxSize; 
    my_hints.min_width = my_hints.max_width = WINDOW_WIDTH;
    my_hints.min_height =  my_hints.max_height = WINDOW_HEIGHT;    
    XSetNormalHints(display, frame_window, &my_hints); 
   
   
    //sets the text in the title bar
    //and is equivalent to the Microsoft SetWindowText API call.   
    XStoreName(display, frame_window, "2D DRAW");
    
    
    //Tell the X-Windows server the events that we are interested in for 
    //the window we have just created. Remember, because X-Windows is client/server 
    //based we have the ability to restrict the events that we receive for an 
    //individual window and thus reduce the amount of network traffic.    
    XSelectInput(display, frame_window, ExposureMask | StructureNotifyMask);


    //Load a font 
    //This is equivalent to Microsoft CreateFont
    //Try several fonts. Program must check for null font for font functions so there is no crash.  
    fontinfo = XLoadQueryFont(display, "10x20");
    //if(fontinfo==NULL){ XLoadQueryFont(display, "another one"); } //etc
    
        
	//2D Needs graphic context asociated with bitmap, like win32's GetDC. We do this only once on init
    gr_values.font = fontinfo->fid;
    //gr_values.foreground = XBlackPixel(display, 0); //set an initial drawing color
    gr_values.foreground = 0xFF0000;     
	output_pixmap0 = XCreatePixmap(display, frame_window, WINDOW_WIDTH, WINDOW_HEIGHT, depth); 
 	gcx = XCreateGC(display, output_pixmap0, GCFont+GCForeground, &gr_values); 
	

    //Display the frame window on the display.  Equivalent to the Microsoft ShowWindow API call.
    XMapWindow(display, frame_window);	


	//allocate argb buffer for some bitmap if necessary 
    //myrgb0 = (u8*) calloc( (WINDOW_WIDTH * WINDOW_HEIGHT * 4), sizeof (u8));
    	
    	
	//rectangle test motion
	rect_x=100;
	rect_dx=3;
	
 	
	//Main loop
    while ( 1 ){
        
        //Wait tick timeslice
        usleep(20000);  //50Hz
        
        //Application update
        //update(timelapse);
		//rectangle test motion
		rect_x += rect_dx;
		if(rect_x<100){rect_dx=  3;}
		if(rect_x>250){rect_dx= -3;}       
        //printf("%d\n", rect_x);
        
        
        //Window events
        //XNextEvent(display, (XEvent *)&event);             //<blocking, useful for event-only style program,  
        XCheckTypedEvent(display, Expose, (XEvent *)&event); //<non-blocking, for periodic timeslice style
        //
        switch ( event.type ) {
            case Expose:
            {
                                
                //Render 2D
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
 
  //Check drawing context and other elements exist
  //Don't create any graphical objects on frame update, as that would leak the memory. Create them once on init.
  if(gcx==NULL){return 0;}
  
  
  //Fill full background. Avoid this as possible by just clipping and drawing the area to be updated.
  //XSetForeground(display, gcx, 0xFFAAAAAA);
  XSetForeground(display, gcx, (0xFFAAAAAA+rect_x)); //with some live color changing
  XFillRectangle(display, frame_window, gcx, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

  //Clipping
  //XRectangle rectangles[1] = {0,0,160,240};
  //XSetClipRectangles(display, gcx0, 0, 0, rectangles, 1, Unsorted);

  //Drawing color
  //XSetBackground(display, gcx, 0xFF0FFF0F); //<???
  XSetForeground(display, gcx, 0xFFFF0000); 

  //Points
  //XDrawPoints(display, d, gc, points, npoints, mode)  ;
  
  //Lines
  XDrawLine(display, frame_window, gcx, 100, 380, 250,280);
  //XDrawLines(display, d, gc, points, npoints, mode);
  //XDrawSegments(display, d, gc, segments, nsegments); //<disconnected
  
  //Rectangles
  XSetLineAttributes(display, gcx, 5, LineSolid, CapRound, JoinRound);
  XDrawRectangle(display, frame_window, gcx, rect_x, 280, 200, 40);  //<with some motion test
  //XDrawRectangles(display, d, gc, rectangles, nrectangles)
  //
  XSetLineAttributes(display, gcx, 1, LineSolid, CapRound, JoinRound);
  XSetForeground(display, gcx, 0xFF000088);
  XFillRectangle(display, frame_window, gcx, 400, 100, 100, 30);
  
  //Arcs
  XSetArcMode(display, gcx, ArcPieSlice); //You can pass ArcChord or ArcPieSlice (only affects filled arcs)  
  XDrawArc(display, frame_window, gcx, 300, 50, 200, 200, 0*64, 90*64); //angles are in x64 units  
  //XDrawArc(display, frame_window, gcx, 300, 50, 200, 200, 0*64, 360*64); //circle
  //XDrawArcs(display, d, gc, arcs, narcs);  //multiple
  //XFillArc(display, frame_window, gcx, 300, 50, 200, 200, 0*64, 90*64);
  //XFillArcs(display, d, gc, arcs, narcs);
  
  //Polygons
  //XDraw(display, d, gc, vlist, vcount);
  //XFillPolygon(display, d, gc, points, npoints, shape, mode);  
  
  //Bitmap
  //XReadBitmapFile(display, d, filename, width_return, height_return, bitmap_return, x_hot_return, y_hot_return);
  //image0 = XCreateImage(display, visual, depth, output_pixmap0, 0, (char*)myrgb0, WINDOW_WIDTH, WINDOW_HEIGHT, 32, ((WINDOW_WIDTH * WINDOW_HEIGHT)*4));
  //Pixmap = XCreateBitmapFromData(display, d, data, width, height);
  //XPutImage(display, output_pixmap0, gcx, image0, 0, 0, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
  //XCopyArea(display, output_pixmap0, frame_window, gcx, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0);

 
  //Get target image data.might be useful to save or colorize
  //XImage *XGetImage(display, d, x, y, width, height, plane_mask, format);
  //long pix = XGetPixel(ximage, x, y);
 

  //Text.
  if(fontinfo!=NULL){//must check font was succesfully loaded on startup             
  
	//get useful string dimensions
	int font_direction, font_ascent, font_descent;
	XCharStruct text_structure;
	XTextExtents(fontinfo, "abcd", 4,   &font_direction, &font_ascent, &font_descent, &text_structure);
	//              
	XSetForeground(display, gcx, 0xFF770000); 
	XDrawString(display, frame_window, gcx, 100, 200, "abcd", 4);	
	//XDrawImageString(display, d, gc, x, y, string, length);//<with a background color
  }

  //Can get window size if necessary to place elements
  //XWindowAttributes window_attributes;
  //XGetWindowAttributes(display, frame_window, &window_attributes);  //this perturbs subsequent text drawing for some reason


  //X-RENDER extension, which allows antialiased primitives and alpha blending
  //...
  //...
 
  return 1;	
}


bool close(void){
 
 //cleanup	
 free(myrgb0);
 XFreeGC(display, gcx);
 if(output_pixmap0 !=0) { XFreePixmap(display, output_pixmap0); } 
 //if(image !=NULL) { XDestroyImage(image); } //<crashes as it also tries to deallocate the buffer. Don't use it unless you want that too.

 
return 1;	 
}

