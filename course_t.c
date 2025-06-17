#include <stdio.h> 
#include <stdlib.h> 
#include <pigpiod_if2.h>

#include "common.c"
//#include "dummy_common.c"

#define ONLINE 1
#define OFFLINE 0

void straight(PFD pfd, int sm, int wm);

void main() {
  PFD pfd;
  int output[5];
  pfd = init();

  motor_drive(pfd, 3, 3);

  straight(pfd, 3, 2);

}

/*void straight(PFD pfd, int sm, int wm){
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
      printf("left turn\n");
    }
    else if(output[3] == 1 && leftflag == 1){
      motor_drive(pfd, sm, wm);
      rightflag = 1;
      leftflag = 0;
      printf("right turn\n");
    }

    time_sleep(0.1);
  }
}*/

void straight(PFD pfd, int sm, int wm){
  int output[5];

  enum STATE{
    STR,
    LET,
    RIT,
    END,
  };

  enum STATE lastt = LET;
  enum STATE state = STR;

  while(state != END){
    get_sensor(pfd, output);

    if(output[2] == ONLINE){
      if(state != STR){
        lastt = state;
        state = STR;  
        motor_drive(pfd, sm, sm); 
      }
    }
    else if(output[2] == OFFLINE){
      if(state == STR){
        if(lastt == LET){
         state = RIT;
         motor_drive(pfd, sm, wm);
        }
        else if(lastt == RIT){
         state = LET;
         motor_drive(pfd, wm, sm);
        }
      }
    } 

    printf("%d\n", state);
    time_sleep(0.1); 
  }
}
