#include <stdio.h> 
#include <stdlib.h> 

#include <pigpiod_if2.h>
#include "common.c"

void main() {
    PFD pfd;
    int flag=0;
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
	motor_drive(pfd,5,9);
      }
      else if(output[3] == 1){
	motor_drive(pfd,9,5);
      }
      else if(output[1] == 0 && output[2] == 0 && output[3] == 0){
	motor_drive(pfd,10,10);
      }

     if(flag==0){
	if(output[0] == 1){ 
	while(1){ 
	 motor_drive(pfd,7,7); 
	 time_sleep(0.01); 
	 if(output[0] == 0 && output[1] == 0 && output[2] == 0 && output[3] == 0 && output[4] == 0){
	  break;
	 }
	}

	while(1){
	 motor_drive(pfd,-8,8);
	 time_sleep(0.01);
	 if(output[1] == 1){
	  break;
	 }
	}
      }
	flag++;
     }	
	    
    time_sleep(0.01);
   }
	
}
