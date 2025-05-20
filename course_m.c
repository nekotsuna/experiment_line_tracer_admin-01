#include<stdio.h>
#include<stdlib.h>
#include<pigpiod.h>

#include "common.c"

void main(){
  PFD pfd;
  int output[5];
  pfd = init();
  //motor_drive(pfd, 8,0);

  while(1){
    get_sensor(pfd, output);
    if(output[1] == 1){
      moter_drive(pfd, 4, 8);
    }
    else if(output[3] == 1){
      moter_drive(pfd, 8, 4);
    }
    else if(output[4] == 1){
      while(1){
        motor_drive(pfd, 8, 0);
        if(output[3] == 1){
          break;
        }
      }
    }
    else if(output[0] == 1){
      while(1){
        motor_drive(pfd, 0, 8);
        if(output[1] == 1){
          break;
        }
      }
    }
    time_sleep(0.1);
  }
}
