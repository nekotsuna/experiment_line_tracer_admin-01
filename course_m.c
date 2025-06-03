#include<stdio.h>
#include<stdlib.h>
#include<pigpiod_if2.h>

#include "common.c"

int MODE = 1;
int TURN_SET = 1;

void main(){
  PFD pfd;
  int output[5];
  pfd = init();
  //motor_drive(pfd, 8,0);

  while(1){
    if(MODE == 1){        
  	while(1){
  	  get_sensor(pfd, output);
  	  if(output[1] == 1){
  	    motor_drive(pfd, 3, 5);
	    printf("straight\n");
	    }
	    else if(output[3] == 1){
	      motor_drive(pfd, 5, 3);
	      printf("straight\n");
    	    }else{
		motor_drive(pfd, 8, 8);
    	    }
    	    if((output[0] == 1 && output[3] == 1) || (output[0] == 1 && output[4] == 1) || (output[1] == 1 && output[3] == 1) || (output[1] == 1 && output[4] == 1)){
        	MODE = 2;
        	break;
    	    }
    	    time_sleep(0.1);
	}
    }
    else if(MODE == 2){
        get_sensor(pfd, output);
    if(TURN_SET == 1){
        motor_drive(pfd, 12, 2);
        TURN_SET++;    
	time_sleep(0.6);
    }else if(TURN_SET == 2){
        motor_drive(pfd, 2, 12);
        TURN_SET++;
	time_sleep(0.5);
    }else if(TURN_SET == 3){
        motor_drive(pfd, 12, 2);
        TURN_SET++;
	time_sleep(0.6);
    }
    MODE = 1;
    }
  }
}
