//
//2020 Statorworks
//Camera
//Capturing from UVC webcam on Linux Debian
//Class abstraction makes possible to run multiple cameras simultaneously
//Based on bellbind's fantastic example: https://gist.github.com/bellbind/6813905
//requires: libjpeg-devminmax for saving frame to jpg file
//apt-get install libjpeg-dev
//and need: -L/usr/lib -ljpeg on build link command


#include "camera.h"


bool camera::open(u32 index, u32 width, u32 height, u8 format){
  
  state=0;
  printf("-----------------\n");
  
  //Open device   /dev/video0, /dev/video1  this are assigned by the system in the order they are plugged in or found
  char device_name[12] = "/dev/video ";
  //
  if(index<CAMERA_MAX_CAMERAS){ device_name[10] = 0x30 + index; }  //insert ascii number
  else { printf("Camera index must be under MAX_CAMERAS\n"); return 0; }
  //
  int resdev = ::open(device_name, O_RDWR | O_NONBLOCK, 0);
  if (resdev == -1) { printf("Could not open camera "); printf(device_name); printf(" \n"); return 0; }
  //
  this->device 			= resdev;
  this->width 			= width;
  this->height 			= height;
  this->buffer_count 	= 0;
  this->buffers 		= NULL;
  //
  printf("Camera ");  printf(device_name); printf(" open Ok\n");
  
  
  //init
  //Get capture and stream capabilities
  struct v4l2_capability cap;
  if (xioctl(device, VIDIOC_QUERYCAP, &cap) == -1)  { printf("Could not get caps\n"); return 0;} 
  if (!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)) { printf("No capture cap\n"); 	  return 0;} 
  if (!(cap.capabilities & V4L2_CAP_STREAMING)) 	{ printf("No streaming cap\n");   return 0;} 
  
  
  //Check if cropping supported
  struct v4l2_cropcap cropcap;
  memset(&cropcap, 0, sizeof cropcap);
  cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  if (xioctl(device, VIDIOC_CROPCAP, &cropcap) == 0) {
    struct v4l2_crop crop;
    crop.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    crop.c = cropcap.defrect;
    if (xioctl(device, VIDIOC_S_CROP, &crop) == -1) { printf("No cropping supported\n"); }   // cropping not supported   
   }
  
  
  //Attempt preferred format 
  //format=0: only yuyv  1:only mjpeg   2:try mjpeg but downgrade to yuyv
  struct v4l2_format vformat;  
  memset(&vformat, 0, sizeof(v4l2_format));
  //
  vformat.type 			= V4L2_BUF_TYPE_VIDEO_CAPTURE;
  vformat.fmt.pix.width 	= width;
  vformat.fmt.pix.height	= height;
  //  
  if(format>0){ 
	vformat.fmt.pix.pixelformat = V4L2_PIX_FMT_MJPEG; 
	//
	if(ioctl(device, VIDIOC_S_FMT, &vformat)==-1){ 
		if(format==2){
			printf("Mjpeg format not available, downgrading to YUYV\n");
			format=0;
		}
		else{
			printf("Mjpeg format not available\n"); ::close(resdev); return 0;
		}
	}  
  }
  //	  
  if(format==0){
   vformat.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;   //uncompressed
   //
   if(ioctl(device, VIDIOC_S_FMT, &vformat)==-1)
   { printf("YUYV format not available\n"); ::close(resdev); return 0;} 
  } 
    
  //Success. 
  //Driver may have downgraded resolution setting as necessary. 
  //Actual setting returned on format struct.
  ioctl(device, VIDIOC_G_FMT, &vformat);
  if(vformat.fmt.pix.pixelformat == V4L2_PIX_FMT_MJPEG) { 

		printf("Using MJPEG format\n"); 
		//
		//Now get mjpeg decoder module ready (based on ffmpeg, unrelated to v4l)
		printf("Starting ffmpeg MJPEG decoder.. "); 		
		//if( mjpeg_decoder.init()==0) {  printf("Could not start\n"); ::close(resdev); return 0; }
		//else printf("Ok\n");

	}
  else if(vformat.fmt.pix.pixelformat == V4L2_PIX_FMT_YUYV)  { 
	  
	printf("Using YUYV format\n"); 
	//attempt to set the 'uvcvideo quirks=128' trick so USB bandwith is not hogged.
	printf("\nAttempting to set UVC_QUIRK_FIX_BANDWIDTH option to limit YUYV bandwidth\n");
	printf("Otherwise use of multiple YUYV USB cameras may not be not possible.\n"); 
	printf("On some systems the UVC driver is fixed so you must adjust and recompile the kernel\n"); 
	printf("modprobe uvcvideo quirks=128 (must be root user)\n\n");	
	//
	system("modprobe uvcvideo quirks=128");	
   }
  else{ ::close(resdev); return 0;}
  
  
  //inform size was ok or changed by driver 	
  if((vformat.fmt.pix.width == width)&&(vformat.fmt.pix.height== height)){  printf("Size %dx%d Ok\n", width, height); }
  else { printf("Closest size %d x %d used\n", vformat.fmt.pix.width, vformat.fmt.pix.height); }
  
  
  //Ok store size so application can consult it easily
  current_format = vformat.fmt.pix.pixelformat;
  this->width 	 = vformat.fmt.pix.width;
  this->height	 = vformat.fmt.pix.height;  


  //Inquire V4L about buffer type we want
  struct v4l2_requestbuffers req;
  memset(&req, 0, sizeof(req));
  //
  req.count = 2; //2 to 4 queued buffers to avoid skipped frames etc
  req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  //
  req.memory = V4L2_MEMORY_MMAP;   //<<buffers reside in mapped kernel space 
  //option V4L2_MEMORY_USERPTR allows buffer to exist in arbitrary user space location, 
  //BUT this requires lots of other trickery. Not recommended.
  //
  if(xioctl(device, VIDIOC_REQBUFS, &req) == -1) { printf("V4L could not provide buffers requested\n"); ::close(resdev); return 0;} 
  if(errno == EINVAL){ printf ("Video capturing or user pointer streaming is not supported\\n"); }
  //
  //allocate struct abstraction (this is not the data space itself)
  buffer_count = req.count;
  buffers = (buffer*)calloc(req.count, sizeof (buffer)); 


  //Initialize buffers
  for(size_t i = 0; i < buffer_count; i++) {
    
    memset(&bufdesc, 0, sizeof bufdesc);
    bufdesc.type      = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    bufdesc.memory    = V4L2_MEMORY_MMAP; 
    bufdesc.index     = i;
    bufdesc.length    = 0;
    //
    if (xioctl(device, VIDIOC_QUERYBUF, &bufdesc) == -1) { printf("Could not register buffers with V4L\n"); ::close(resdev); return 0;} 
    buffers[i].length = bufdesc.length;
    //printf("\n %d \n " , buf.length);
    //
    //For V4L2_MEMORY_MMAP kernel buffer type, need to map them as if part of the application process
    buffers[i].start  = (uint8_t*)mmap(NULL, bufdesc.length, PROT_READ | PROT_WRITE, MAP_SHARED, device, bufdesc.m.offset);
    if (buffers[i].start == MAP_FAILED) { printf("V4L could not map buffers\n"); ::close(resdev); return 0;} 
  }
  printf("Camera initialized Ok\n"); 
  
  
  //Start
  for (size_t i = 0; i < buffer_count; i++) {
    memset(&bufdesc, 0, sizeof bufdesc);
    bufdesc.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    //
    bufdesc.memory = V4L2_MEMORY_MMAP; //<<must be same as requested above
    bufdesc.index = i;
    if (xioctl(device, VIDIOC_QBUF, &bufdesc) == -1) { printf("Could not initialize buffer\n"); ::close(resdev); return 0;}
  }
  //
  enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  if(xioctl(device, VIDIOC_STREAMON, &type) == -1)  { printf("Could not start capture\n"); ::close(resdev); return 0;} 
  
 
  //Allocate the user frame data buffer, fitting frame size 32bpp.  
  //Extra bytes are needed as per avlib documentation: https://ffmpeg.org/doxygen/3.4/group__lavc__decoding.html
  frame_data_allocated_size = (4* this->width * this->height);// + AV_INPUT_BUFFER_PADDING_SIZE;
  //
  frame_data = (u8*) calloc( frame_data_allocated_size, sizeof (u8));
  if(frame_data == NULL) { frame_data_allocated_size = 0; printf("Could not allocate user frame buffer\n"); ::close(resdev); return 0; } 
  //
  frame_data_length = 0;     

  //save /dev/video index
  this->index = index;

  //running 
  state=1;
  printf("Camera start capture Ok\n"); 
  
      
  //Skip first 5 garbage frames
  for (int i = 0; i < 5; i++) {  get_frame(); }

 return 1;
}


bool camera::get_frame(void){
  
  //check
  //if(this->state ==0){return 0;}

  //Request device access
  fd_set fds;
  FD_ZERO(&fds);
  FD_SET(this->device, &fds);
  //
  struct timeval timeout; //wait up to 1 second
  timeout.tv_sec  = 1;
  timeout.tv_usec = 0; 
  //
  int r = select(this->device + 1, &fds, 0, 0, &timeout); 
  if (r == -1){ this->state=0; printf("camera disconnected, no device\n");   return 0; }
  if (r == 0) { printf("device access timeout\n"); return 0; }


  //Capture
  bufdesc.type     = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  bufdesc.memory   = V4L2_MEMORY_MMAP; //<<must be same as requested on init
  bufdesc.flags    = 0;
  //bufdesc.bytesused= 0;  //not needed
  //
  frame_data_length=0;
  if(xioctl(device, VIDIOC_DQBUF, &bufdesc) == -1) { return 0; } //printf("could not retrieve camera frame\n");   return 0; }

  
  //Write down number of frame data bytes. 
  //value lost below on VIDIOC_QBUF request
  //printf("%d\n", bufdesc.bytesused);
  u32 bytes_returned = bufdesc.bytesused;
 
 
  //Transfer next frame??
  if(xioctl(device, VIDIOC_QBUF, &bufdesc) == -1) { return 0; } //printf("could not retrieve camera frame\n");  return 0; }  

  
  //Process data as needed below
  //user has option to redirect output to their own buffer
  u8* data_ptr;
  if(options_user_buffer_ptr!=NULL){ data_ptr = options_user_buffer_ptr; }
  else{ 
    if(frame_data==NULL){ return 0; }
    data_ptr = frame_data;
   }
  
  
  //frames
  frame_count++;
  
  
  //User might want YUYV converted to ARGB 
  if((current_format == V4L2_PIX_FMT_YUYV)&&(option_convert_yuyv_to_argb ==1)){

    //adjust the height we pass (number of rows) to match the actual amount of data. just to be sure
    //u32 height = (buf.bytesused/(2*this->width)); //yuyv: 16bpp  Note: bytesused is returning 0 ??
    u32 height = this->height;  
    //printf("\n %d \n", height);
    yuyv2rgb(data_ptr, buffers[bufdesc.index].start, this->width, height); 
    //
    frame_data_length  = 4*(this->width * height); //result is 32bpp
   }
  
  
  //MJPEG may need decoding.
  else if((current_format == V4L2_PIX_FMT_MJPEG)&&(option_decode_mjpeg==1)){

	//test, print raw jpeg header 
	//if(frame_count == 50){//once
	// mjpeg_decoder.PrintHeaderRawHex(buffers[bufdesc.index].start, 1024);
	//}
	
	//test, print parsed header info 
	//if(frame_count ==50){//once
	//  mjpeg_decoder.ReadHeader(buffers[bufdesc.index].start, 1024);
	//  mjpeg_decoder.PrintHeaderInfo();
    //}
	
	//decode
	//bool res= jpeg_decoder.DecodeFrame((u32*)data_ptr, frame_data_allocated_size, buffers[bufdesc.index].start, bytes_returned);
	//if(res==0){ frame_data_length=0; return 0; }   
    
    //frame_data_length = 4*(this->width * this->height);
    //printf("\nfdtl:  %d \n" , frame_data_length);
   
    frame_data_length  = 0; //<<until jpeg decoder module is finished
   }
  
  
  //Otherwise just accept data as is
  //User may want to do his own decoding or just transmit the frame over the internet.
  else{ 
    
    //Point to memory mapped buffer
    //frame_data_pointer = buffers[buf.index].start;
    //frame_data_length  = 4*(this->width * this->height);//buf.bytesused;
    //XX
    //***Can't properly use the memory mapped buffer. If the user reads past current size there is a segmentation fault. 
    //So we just copy normally to fixed size buffer. Not the most efficient way but this the fallback transfer anyways.
    memcpy(data_ptr, buffers[bufdesc.index].start, bytes_returned);
    frame_data_length = bytes_returned;
   }
  
 
   
  return 1;
}


bool camera::set_exposure(f32 val)
{
//
//  //check
//  if(val>100){val=100;}
//  
  //v4l2_control cont;
  //cont.id = V4L2_CID_EXPOSURE;  //see  linuxtv.org/downloads/v4l-dvb-apis/uapi/v4l/control.html#control
//  cont.value = val;
//
//  if (xioctl(device, VIDIOC_S_CTRL, &cont) == -1)  { printf("could not set exposure\n"); return 0;} 	
//  return 0;
 return 1;
}

bool camera::set_parameters(f32 exposure, f32 brightness, f32 contrast, f32 saturation, f32 hue){

 return 1;	 
}

int camera::xioctl(int dev, int request, void* arg)
{ //Good behavior wrapper for ioctl
  //use try/catch here if necessary
  
  //send request to io device, try command up to 20 times	 
  for (int i = 0; i < 20; i++) {
    int r = ioctl(dev, request, arg);
    if (r != -1 || errno != EINTR) {  return r; }
  }

  return -1;
}


bool camera::close(void){ 
  
  //free allocated buffer
  free(frame_data);
  
  enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  if (xioctl(device, VIDIOC_STREAMOFF, &type) == -1) { printf("Camera already stopped\n"); return 0;}
  printf("Camera stopped\n");
     
  if (::close(this->device) == -1) { printf("Could not close camera device\n"); return 0;}
    
  //for (size_t i = 0; i < buffer_count; i++) {  munmap(buffers[i].start, buffers[i].length);  }
  
  free(buffers);
  buffer_count = 0;
  buffers = NULL;
  
  //mjpeg decoder
  //no cropmjpeg_decoder.Close();
  
  printf("Camera closed\n"); 
  return 1; 
  
  state=0;
  
  return 1;
}


bool camera::save_argb_to_jpeg(const char* file_name, uint8_t* source_rgb, uint32_t width, uint32_t height, int quality)
{
  //User must ensure source buffer is correct size 
  /*	
  //Open/create file
  FILE* out = fopen(file_name, "w");
	
  //move rgb data to image object	
  JSAMPARRAY image;
  image = (JSAMPARRAY)calloc(height, sizeof (JSAMPROW));
  for (size_t i = 0; i < height; i++) {
    image[i] = (JSAMPROW) calloc(width * 3, sizeof (JSAMPLE));
    for (size_t j = 0; j < width; j++) {
      image[i][j * 3 + 0] = source_rgb[(i * width + j) * 3 + 0];
      image[i][j * 3 + 1] = source_rgb[(i * width + j) * 3 + 1];
      image[i][j * 3 + 2] = source_rgb[(i * width + j) * 3 + 2];
    }
  }
  
  //attribute
  struct jpeg_compress_struct compress;
  struct jpeg_error_mgr error;
  compress.err = jpeg_std_error(&error);
  jpeg_create_compress(&compress);
  jpeg_stdio_dest(&compress, out);
  
  
  //compress
  compress.image_width = width;
  compress.image_height = height;
  compress.input_components = 3;
  compress.in_color_space = JCS_RGB;
  jpeg_set_defaults(&compress);
  jpeg_set_quality(&compress, quality, TRUE);
  jpeg_start_compress(&compress, TRUE);
  jpeg_write_scanlines(&compress, image, height);
  jpeg_finish_compress(&compress);
  jpeg_destroy_compress(&compress);

  //free image construct
  for (size_t i = 0; i < height; i++) {
    free(image[i]);
  }
  free(image);
  
  //Close file
  fclose(out); 
  
  printf("camera jpeg saved\n"); 
  */
  return 1;
}


inline int camera::minmax(int min, int v, int max)
{
  return (v < min) ? min : (max < v) ? max : v;
}

bool camera::yuyv2rgb(uint8_t* dest_argb, uint8_t* yuyv, uint32_t width, uint32_t height)
{
  //caller must make sure buffer is correct size
   
  //shorthand
  uint8_t* rgb = dest_argb; 

  for (size_t i = 0; i < height; i++) {
    for (size_t j = 0; j < width; j += 2) {
      size_t index = i * width + j;
      //
      int y0 = yuyv[index * 2 + 0] << 8;
      int u  = yuyv[index * 2 + 1] - 128;
      int y1 = yuyv[index * 2 + 2] << 8;
      int v  = yuyv[index * 2 + 3] - 128;
      rgb[index * 4 + 0] = minmax(0, (y0 + 454 * u) >> 8, 255);
      rgb[index * 4 + 1] = minmax(0, (y0 + 88 * v - 183 * u) >> 8, 255);
      rgb[index * 4 + 2] = minmax(0, (y0 + 359 * v) >> 8, 255);
	  rgb[index * 4 + 3] = 0xFF; 
      //
      rgb[index * 4 + 4] = minmax(0, (y1 + 454 * u) >> 8, 255);
      rgb[index * 4 + 5] = minmax(0, (y1 + 88 * v - 183 * u) >> 8, 255);
      rgb[index * 4 + 6] = minmax(0, (y1 + 359 * v) >> 8, 255);
	  rgb[index * 4 + 7] = 0xFF;   
    }
  }
 
 return 1;   
}



