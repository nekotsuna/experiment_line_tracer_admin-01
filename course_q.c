#include <stdio.h> 
#include <stdlib.h> 

#include <pigpiod_if2.h>
#include "common.c"

void main() {
    PFD pfd;
    int flag=0;
    int output[5];
    pfd = init();
	
    while(1){
	get_sensor(pfd,output);
	if(output[0] == 1 && output[1] == 1 && output[2] == 1 && output[3] == 1 && output[4] == 1){
		printf("stop\n");
		motor_drive(pfd,0,0);	
		time_sleep(0.01);
	}
	else {
	  printf("start\n");
	  break;
	}
    }
	
    while(1){
      get_sensor(pfd,output);
	    
      for(int i = 0; i < 5; i++){
           printf("%d ", output[i]);
      }
      printf("\n");
  
      motor_drive(pfd,9,10);
	
      if(output[3] == 1){
	motor_drive(pfd,10,5);
      }
      else if(output[4] == 1){
	motor_drive(pfd,10,5);
      }
      else if(output[1] == 1){
	motor_drive(pfd,5,10);
      }
      else if(output[1] == 0 && output[2] == 0 && output[3] == 0){
	motor_drive(pfd,9,10);
      }
      else if(output[2] == 1 && output[3] == 1){
	motor_drive(pfd,6,4);
      }
      else if(output[2] == 1 && output[1] == 1){
	motor_drive(pfd,4,6);
      }

    time_sleep(0.01);
   }
	
}
