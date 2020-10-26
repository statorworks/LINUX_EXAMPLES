
#include "time.h"

u32 tick_hz;

bool wait_tick(void){

 //Get tick frequency	
 //system_tick_hz = sysconf(_SC_CLK_TCK);
 
 //sched_yield(); //<this just gets ignored if the thread is already high priority  
 
 usleep(10000); //100Hz
 
 //get actual timelapse	


 return 1; //timelapse_ms
}	
