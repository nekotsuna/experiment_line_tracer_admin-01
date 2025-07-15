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
		motor_drive(pfd,0,0);	
		time_sleep(0.01);
	}
	else{
	  break;
	}
    }
	
    while(1){
      get_sensor(pfd,output);
	    
      for(int i = 0; i < 5; i++){
           printf("%d ", output[i]);
      }
      printf("\n");
  
      motor_drive(pfd,9,9);
	
      if(output[4] == 1){
	motor_drive(pfd,12,2);
      }
      else if(output[4] == 1 && output[3] == 1){
	motor_drive(pfd,12,2);
      }
      else if(output[3] == 1){
	motor_drive(pfd,11,6);
      }
      else if(output[1] == 1){
	motor_drive(pfd,6,11);
      }
      else if(output[1] == 0 && output[2] == 0 && output[3] == 0 || output[2] == 1){
	motor_drive(pfd,10,10);
      }


      if(output[0] == 1){
       while(1){
	get_sensor(pfd,output);
	       
	if(output[0] == 0 && output[1] == 0 && output[2] == 0 && output[3] == 0 && output[4] == 0){
	  break;
	}

        if(output[3] == 1){
	 motor_drive(pfd,8,5);
        }
        else if(output[1] == 1){
	 motor_drive(pfd,5,8);
        }
        else if(output[1] == 0 && output[2] == 0 && output[3] == 0 || output[2] == 1){
	 motor_drive(pfd,8,8);
        }

	time_sleep(0.01);
       }

	while(1){
	 get_sensor(pfd,output);
	 motor_drive(pfd,8,9);
	 time_sleep(0.01);
	}
      }

    time_sleep(0.01);
   }
	
}
