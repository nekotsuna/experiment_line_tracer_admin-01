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

#define SETSMAX 4 
#define BUFMAX 256

typedef struct setting{
  double sec;
  int straight;
  int bending_sm;
  int bending_wm;
} Setting;

Setting* setting();
void straight(PFD pfd, int straight, int sm, int wm, double sec);

void main() {
  PFD pfd;
  int output[5];
  pfd = init();
  Setting* set = setting();

  printf("sec = %f\n", set->sec);
  printf("straight = %d\n", set->straight);
  printf("bending sm = %d\n", set->bending_sm);
  printf("bending wm = %d\n", set->bending_wm);

  motor_drive(pfd, set->straight, set->straight);
  straight(pfd, set->straight, set->bending_sm, set->bending_wm, set->sec);
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
  set->straight = sets[1];
  set->bending_sm= sets[2];
  set->bending_wm = sets[3];

  fclose(fp);

  return set;
}

void straight(PFD pfd, int straight, int sm, int wm, double sec){
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
        motor_drive(pfd, straight, straight); 
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
