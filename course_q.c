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
	
	if(output[0] == 1 || output[1] == 1){
	 motor_drive(pfd,15,16);
        }
	else if(output[2] == 1){
	 motor_drive(pfd,16,16);
	}
        else if(output[1] == 0 && output[2] == 0 && output[3] == 0){
	 motor_drive(pfd,11,11);
        }

	if(output[0] == 0 && output[1] == 0 && output[2] == 0 && output[3] == 0 && output[4] == 0){
	  break;
	}

	time_sleep(0.01);
     }


 while(1){
	motor_drive(pfd,15,16);
	time_sleep(0.01);
    }
}
