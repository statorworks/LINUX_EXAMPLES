#ifndef CAMERA_H
#define CAMERA_H 

//2020 Statorworks
//Camera
//Capturing from UVC webcam on Linux with V4L (Video for Linux) driver 
//Class abstraction makes possible to run multiple cameras simultaneously
//Based on bellbind's fantastic example: https://gist.github.com/bellbind/6813905
//requires: libjpeg-dev for saving frame to jpg file
//apt-get install libjpeg-dev
//and need: -L/usr/lib -ljpeg on build link command
//Also 'livab' to decode compressed MJPEG video supported by some cameras.


#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <asm/types.h>

#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

//V4L Video for linux 
#include <linux/videodev2.h>       


//Decoding MJPEG, jpeg compressed video frames
//thin wrapper over either custom decoder or, openjpeg or ffmpeg or jpeglib
//#include "jpeg_decoder.h"  //not implemented yet

 
#include "types.h"    


#define CAMERA_MAX_CAMERAS 8


class camera{ 

 
 public:
 //
 typedef struct {
  u8* start;
  size_t length;
 }buffer;
 
 //local properties
 u32 state =0;  //0=closed 1=running+ready;
 int device;    //device handle
 int index =0;
 u32 current_format;
 u32  width=0;
 u32  height=0;
  
  
 //queried device capabilities
 struct v4l2_capability cap;
 

 //mjpeg decoder
 //thin wrapper over either openjpeg or ffmpeg
 //MJPEG_DECODER //mjpeg_decoder;
 
  
 //output frame buffer for user 
 u8*  frame_data; //allocated on start
 // 
 u32  frame_data_length;  
 u32  frame_data_allocated_size;
 
 //options
 bool option_decode_mjpeg 			=1;
 bool option_convert_yuyv_to_argb	=1;
 u8*  options_user_buffer_ptr = NULL; //user can redirect output to their own buffer

 //frames
 u32 frame_count=0; //test
  

 //API
 bool open(u32 index, u32 width, u32 height, u8 format);
 bool close(void);
 bool get_frame(void); //User may not want to decode mjpeg if they're just streaming online 
 //
 bool set_exposure(f32 val);  
 //bool set_saturation(f32 val);
 //bool set_brightness(f32 val);  
 //bool set_contrast(f32 val); 
 //bool set_hue(f32 val); 
 //bool enable_microphone(bool enable);
 bool set_parameters(f32 exposure, f32 brightness, f32 contrast, f32 saturation, f32 hue); 
 //
 bool save_argb_to_jpeg(const char* file_name, uint8_t* source_rgb, uint32_t width, uint32_t height, int quality); 
 bool yuyv2rgb(uint8_t* dest_argb, uint8_t* yuyv, uint32_t width, uint32_t height);	
  
 //internal
 private:
 
 //V4l frame buffer abstraction
 size_t    buffer_count;
 buffer*   buffers;
 struct v4l2_buffer bufdesc;
 //
 int xioctl(int fd, int request, void* arg);  //Good behavior wrapper for ioctl	
 inline int minmax(int min, int v, int max);

};

 
#endif
