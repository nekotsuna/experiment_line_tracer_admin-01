#include <stdio.h> 
#include <stdlib.h> 

#ifdef IN_RASPBERRYPI

#include <pigpiod_if2.h>
#include "common.c"

#else

#include "dummy_common.c"

#endif

#define ONLINE 1
#define OFFLINE 0

#define SETSMAX 3 
#define BUFMAX 256

typedef struct setting{
  int straight_sm;
  int straight_wm;
  double sec;
} Setting;

Setting* setting();
void straight(PFD pfd, int sm, int wm, double sec);

void main() {
  PFD pfd;
  int output[5];
  pfd = init();
  Setting* set = setting();

  printf("sec = %f\n", set->sec);
  printf("straight sm = %d\n", set->straight_sm);
  printf("straight wm = %d\n", set->straight_wm);

  motor_drive(pfd, set->straight_sm, set->straight_sm);
  straight(pfd, set->straight_sm, set->straight_wm, set->sec);
}

Setting* setting(){
  int sets[SETSMAX];
  Setting* set = (Setting*)malloc(sizeof(Setting));

  FILE* fp = fopen("course_t_setting.txt", "r");
  if(fp == NULL){
    printf("error: cannnot find file\n");
    exit(1);
  }

  char c;
  int i = 0;
  int line = 0;
  char buf[BUFMAX];
  while((c = fgetc(fp)) != EOF){
    if(line >= SETSMAX) break; 
    
    if(c == '\n'){
      buf[i] = '\0';
      sets[line] = atoi(buf);
      i = 0;  
      line++;
    }
    else{
      buf[i] = c; 
      i++;
    }
  }

  set->sec = (double)sets[0] / (double)1000;
  set->straight_sm = sets[1];
  set->straight_wm = sets[2];

  fclose(fp);

  return set;
}

void straight(PFD pfd, int sm, int wm, double sec){
  int output[5];

  enum STATE{
    STR,
    LET,
    RIT,
    END,
  };

  enum STATE lastt; 
  enum STATE state = STR;
  while(state != END){
    get_sensor(pfd, output);
    if(output[1] == ONLINE){
      motor_drive(pfd, wm, sm);
      state = LET;
    }
    if(output[3] == ONLINE){
      motor_drive(pfd, sm, wm);
      state = RIT;
    }
    time_sleep(sec);
  }

  printf("out init\n");

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
    time_sleep(sec); 
  }
}
