#include<stdio.h>
#include<stdlib.h>
#include<pigpiod_if2.h>

#include "common.c"

void main(){
  PFD pfd;
  int output[5];
  pfd = init();
  motor_drive(pfd, 0, 0);
  int flag = 1;

  while(1){
    get_sensor(pfd, output);

    if((output[0] == 0 && output[1] == 0 && output[2] == 0 && output[3] == 0 && output[4] == 0) || (output[0] == 0 && output[1] == 0 && output[2] == 1 && output[3] == 0 && output[4] == 0)){
        flag = 1;//直進
    }

	  
    if(output[0] == 1){
        flag = 2;//右に方向調整
    }

    else if(output[4] == 1){
        flag = 3;//左に方向調整
    }

    if(output[3] == 1 && output[4] == 1){
        flag = 4;//全力の右折
    }
    
    

    if(flag == 4){
        printf("4\n");
        motor_drive(pfd, 10, 0);
    }

    if(flag == 1){
        printf("1\n");
        motor_drive(pfd, 10, 10);
    }
  
    else if(flag == 2){
        printf("2\n");
        motor_drive(pfd, 7, 5);
    }

    else if(flag == 3){
        printf("3\n");
        motor_drive(pfd, 5, 7);
    }

    time_sleep(0.01);
  }
}
