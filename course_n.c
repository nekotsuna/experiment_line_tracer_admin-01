#include<stdio.h>
#include<stdlib.h>
#include<pigpiod_if2.h>

#include "common.c"

void main(){
  PFD pfd;
  int output[5];
  pfd = init();
  motor_drive(pfd, 0, 0);

  while(1){
    get_sensor(pfd, output);

    if(output[0] == 0 && output[1] == 0 && output[2] == 0 && output[3] == 0 && output[4] == 0){
	motor_drive(pfd, 11, 11);
    }
	  
    if(output[0] == 1 && output[1] == 1 && output[3] == 0){
	motor_drive(pfd, 10, 7);
    }
    
    else if(output[3] == 1 && output[4] == 1 && output[1] == 0){
	motor_drive(pfd, 7, 10);
    }

    if(output[0] == 0 && output[1] == 0 && output[2] == 1 && output[3] == 1 && output[4] == 1){
	motor_drive(pfd, 10, 0);
	if(output[0] == 0 && output[1] == 0 && output[2] == 0 && output[3] == 0 && output[4] == 1){
    	motor_drive(pfd, 10, 0);
		if(output[0] == 0 && output[1] == 0 && output[2] == 1 && output[3] == 0 && output[4] == 0){
			motor_drive(pfd, 11, 11);
		}
	}
    }
    time_sleep(0.01);
  }
}
