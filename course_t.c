#include <stdio.h> 
#include <stdlib.h> 
//#include <pigpiod_if2.h>

//#include "common.c"
#include "dummy_common.c"

void straight(PFD pfd, int sm, int wm);

void main() {
  PFD pfd;
  int output[5];
  pfd = init();

  motor_drive(pfd, 0, 0);

  straight(pfd, 10, 8);

}

void straight(PFD pfd, int sm, int wm){
  int output[5];
  int endflag = 1;
  int leftflag = 1;
  int rightflag = 1;

  while(endflag){
    get_sensor(pfd, output);

    if(output[1] == 1 && rightflag == 1){
      motor_drive(pfd, wm, sm); 
      leftflag = 1;
      rightflag = 0;
    }
    else if(output[3] == 1 && leftflag == 1){
      motor_drive(pfd, wm, sm);
      rightflag = 1;
      leftflag = 0;
    }

    time_sleep(0.1);
  }
}
