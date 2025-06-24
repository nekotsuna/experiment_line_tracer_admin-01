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
	    
      for(int i = 0; i < 5; i++){
           printf("%d ", output[i]);
      }
      printf("\n");
  
      motor_drive(pfd,10,10);
	    
      if(output[1] == 1){
	motor_drive(pfd,6,9);
      }
      else if(output[3] == 1){
	motor_drive(pfd,9,6);
      }
      else if(output[1] == 0 && output[2] == 0 && output[3] == 0){
	motor_drive(pfd,10,10);
      }
	    
     
	    
    time_sleep(0.01);
   }
	
}
