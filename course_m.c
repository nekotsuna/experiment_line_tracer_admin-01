#include<stdio.h>
#include<stdlib.h>
#include<pigpiod_if2.h>

#include "common.c"

void main(){
  PFD pfd;
  int output[5];
  pfd = init();
  //motor_drive(pfd, 8,0);

  while(1){
    get_sensor(pfd, output);
    if(output[1] == 1){
      motor_drive(pfd, 3, 5);
		printf("straight\n");
    }
    else if(output[3] == 1){
      motor_drive(pfd, 5, 3);
		printf("straight\n");
    }
    else if(output[4] == 1){
      motor_drive(pfd, 12, 0);
      time_sleep(1.0);
      while(1){
	get_sensor(pfd, output);
        motor_drive(pfd,12, 0);
	printf("turn_left\n");
        if(output[3] == 1){
          printf("turn_left_end\n");
          break;
        }
	time_sleep(0.1);
      }
    }
    else if(output[0] == 1){
      motor_drive(pfd, 0, 12);
      time_sleep(1.0);
      while(1){
        get_sensor(pfd, output);
        motor_drive(pfd, 0, 12);
        printf("turn_right\n");
        if(output[1] == 1){
          printf("turn_right_en\n");
          break;
        }
	time_sleep(0.1);
      }
    }
    else{
      motor_drive(pfd, 8, 8);
    }
    time_sleep(0.1);
  }
}
