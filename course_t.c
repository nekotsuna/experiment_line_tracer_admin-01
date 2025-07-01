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

#define SETSMAX 6 
#define BUFMAX 256

typedef struct setting{
  double sec;
  int straight;
  int bending_sm;
  int bending_wm;
  int curve_sm;
  int curve_wm;
} Setting;

typedef enum STATE{
    STR,
    LET,
    RIT,
    CUV,
    STP,
    END,
} STATE;

typedef enum STRTERM{
    ONCROSS,
    NODETECT,
} STRTERM;

Setting* setting();
void print_sensor(PFD pfd);
void start(PFD pfd);
STATE initialize(PFD pfd, int straight, int sm, int wm, double sec);
void straight(PFD pfd, int straight, int sm, int wm, double sec, STATE state, STRTERM strterm, int nodetect_countmax);
void curve(PFD pfd, int lm, int rm, double sec);
void straight_oncross(PFD pfd, int sm, double sec);

void main() {
  PFD pfd;
  int output[5];
  pfd = init();
  Setting* set = setting();

  printf("sec = %f\n", set->sec);
  printf("straight = %d\n", set->straight);
  printf("bending sm = %d\n", set->bending_sm);
  printf("bending wm = %d\n", set->bending_wm);
  printf("curve sm = %d\n", set->curve_sm);
  printf("curve wm = %d\n", set->curve_wm);

  start(pfd);
  printf("out start\n");
  motor_drive(pfd, set->straight, set->straight);
  STATE lastt = initialize(pfd, set->straight, set->bending_sm, set->bending_wm, set->sec);
  printf("out init\n");
  straight(pfd, set->straight, set->bending_sm, set->bending_wm, set->sec, lastt, ONCROSS, 0);
  printf("out straight\n");
  curve(pfd, set->curve_sm, set->curve_wm, set->sec); //right curve
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
  set->curve_sm = sets[4];
  set->curve_wm = sets[5];

  fclose(fp);

  return set;
}

void print_sensor(PFD pfd){
  int output[5];
  get_sensor(pfd, output);

  for(int i = 0; i < 5; i++){
    printf("[%d] = %d, ", i, output[i]);
  }
  printf("\n");
}

void start(PFD pfd){
  int output[5];
  STATE state = STP;

  while(state != END){
    get_sensor(pfd, output);
    if(output[0] == ONLINE && output[1] == ONLINE && output[2] == ONLINE && output[3] == ONLINE && output[4] == ONLINE) state = END;
  }
}

STATE initialize(PFD pfd, int straight, int sm, int wm, double sec){
  int output[5];


  STATE lastt;
  STATE state = STR;
  while(state != END){
    get_sensor(pfd, output);
    if(output[1] == ONLINE){
      motor_drive(pfd, wm, sm);
      state = END;
      lastt = LET;
    }
    if(output[3] == ONLINE){
      motor_drive(pfd, sm, wm);
      state = END;
      lastt = RIT;
    }
    time_sleep(sec);
  }

  return lastt;
}

void straight(PFD pfd, int straight, int sm, int wm, double sec, STATE state, STRTERM strterm, int nodetect_countmax){
  int output[5];
  int nodetect_count = 0;

  STATE lastt;

  while(state != END){
    get_sensor(pfd, output);

    switch(strterm){
      case ONCROSS:
        if(output[0] == ONLINE || output[4] == ONLINE) state = END;
        break;

      case NODETECT:
        if(output[0] == OFFLINE && output[1] == OFFLINE && output[2] == OFFLINE && output[3] == OFFLINE && output[4] == OFFLINE){
          nodetect_count++;
          if(nodetect_count > nodetect_countmax){
            state = END;
          } 
        }
        else{
          nodetect_count = 0;
        }
        break;
    } 
    if(state == END){
      state = END;
    }
    else if(output[2] == ONLINE){
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

void curve(PFD pfd, int lm, int rm, double sec){
  int output[5];

  STATE state = CUV;
  motor_drive(pfd, lm, rm);

  while(state != END){
    get_sensor(pfd, output);

    if(output[0] == ONLINE || output[4] == ONLINE){
      state = END;

      time_sleep(sec); 
    }
  }     
}

void straight_oncross(PFD pfd, int sm, double sec){
  int output[5];

  STATE state = STR;
  motor_drive(pfd, sm, sm);

  while(state != END){
    get_sensor(pfd, output);

    if(output[0] == OFFLINE && output[4] == OFFLINE) state = END;

    time_sleep(sec);
  }
}
