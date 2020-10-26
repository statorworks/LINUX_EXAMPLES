#ifndef TYPES_H
#define TYPES_H 

//Types for Linux Debian Geany
//This makes data types more clear, consistent, and compact
//Reduces typing time

//#include <vector.h>
//#include "/usr/include/c++/4.9.2/vector" //**a bit of a different location, and no .h extension
#include <string.h>

//
#define types_h_version 1
#define types_h_target "linux_debian"
//
//#define dynamic_array(type) std::vector<type>
//
//typedef unsigned char		bool;
typedef unsigned char		u8;
typedef unsigned short		u16;
typedef unsigned int        u32;
typedef unsigned long long  u64;
//
typedef signed char			s8;
typedef signed short		s16;
typedef signed int			s32;
typedef signed long long	s64;
//
typedef float				f32;
typedef double				f64;   
//
//color related
typedef unsigned int		argb32;
#define col32(a,r,g,b)		((u32)a<<24 | (u32)r<<16 | (u32)g<<8 | b)
struct gradient_point{ f32 position; argb32 col;}; 
//
//strings
//typedef std::string			str8;
//typedef std::wstring		str16;
#define value_to_str8(v)      std::to_string((long double)v)
#define value_to_str16(v)     std::to_wstring((long double)v)
#define value_to_lpcwstr(v)   value_to_str16(v).c_str()                
//
//#define msgbox(val)	MessageBox(0, value_to_lpcwstr(val), L"",0);
//
//void inline clamp(f32* value, f32 low, f32 high){ if(*value < low){*value=low;}  else if(*value > high){*value=high;}  }
//void inline clamp(u32* value, u32 low, u32 high){ if(*value < low){*value=low;}  else if(*value > high){*value=high;}  }
//void inline clamp(s32* value, s32 low, s32 high){ if(*value < low){*value=low;}  else if(*value > high){*value=high;}  }


#endif
