#include <stdio.h> 
#include <stdlib.h> 
#include <pigpiod_if2.h>

#include "common.c"

void main() {
    PFD pfd;
    int output[5];
    pfd = init();

    motor_drive(pfd, 0, 0);

    
    while(1){
      get_sensor(pfd,output);
      motor_drive(pfd,3,3);
	    
      if(output[1] == 1){
	motor_drive(pfd,3,5);
      }
      else if(output[3] == 1){
	motor_drive(pfd,5,3);
      }
      else if(output[1] == 0 && output[2] == 0 && output[3] == 0){
	motor_drive(pfd,3,3);
      }
	time_sleep(0.01);
   }
	
}
