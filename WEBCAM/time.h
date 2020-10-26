#ifndef TIME_H
#define TIME_H 

#include <stdio.h>
#include <time.h>
#include <unistd.h>  ///<needed for usleep()
#include "types.h"


extern u32 tick_hz;


bool wait_tick(void);
//bool stopwatch_start(void);
//etc


#endif
