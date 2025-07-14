#include <stdio.h> 
#include <stdlib.h> 
#include "course_t_setter.h"

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

typedef enum STATE{
    UND,
    STR,
    LET,
    RIT,
    SLET,
    SRIT,
    CUV,
    STP,
    END,
} STATE;

typedef enum STRTERM{
    ONCROSS,
    NODETECT,
} STRTERM;

void print_sensor(PFD pfd);
void start(PFD pfd);
STATE initialize(PFD pfd, int straight, int sm, int wm, double sec);
void straight(PFD pfd, int straight, int sm, int wm, double sec, STATE state, STRTERM strterm, int nodetect_countmax);
void straight_v2(PFD pfd, int straight, int sm, int wm, int ssm, int swm, double sec, STRTERM strterm, int nodetect_countmax);
void curve(PFD pfd, int lm, int rm, double sec);
void straight_oncross(PFD pfd, int sm, double sec);
void uturn(PFD pfd, int lm, int rm, double sec); 
void stop(PFD pfd, double sec);

int main() {
  PFD pfd;
  int output[5];
  pfd = init();
  int settings[STRLEN];
  setting(settings);

  for(int i = 0; i < STRLEN; i++){ 
    printf("%s = %d\n", str[i], settings[i]);
  }

  double sec = (double)settings[SEC] / (double)(SECDIV);

  start(pfd);
  printf("out start\n");

  //motor_drive(pfd, settings[STRAIGHT], settings[STRAIGHT]);
  //STATE lastt = initialize(pfd, settings[STRAIGHT], settings[BENDING_SM], settings[BENDING_WM], sec);
  //printf("out init\n");

  //straight(pfd, settings[STRAIGHT], settings[BENDING_SM], settings[BENDING_WM], sec, lastt, ONCROSS, 0);
  //printf("out straight\n");

  straight_v2(pfd, settings[STRAIGHT], settings[BENDING_SM], settings[BENDING_WM], settings[BENDING_STSM], settings[BENDING_STWM], sec, ONCROSS, 0);
  printf("out straight\n");
  //stop(pfd, 2.0);

  curve(pfd, settings[CURVE_SM], settings[CURVE_WM], sec); //right curve
  printf("out curve\n");

  stop(pfd, 0.5);

  straight_v2(pfd, settings[STRAIGHT], settings[BENDING_SM], settings[BENDING_WM], settings[BENDING_STSM], settings[BENDING_STWM], sec, NODETECT, 0);
  printf("out straight\n");

  uturn(pfd, settings[UTURN_SM], settings[UTURN_WM], settings[SEC]);
  printf("out u-turn\n");

  straight_v2(pfd, settings[STRAIGHT], settings[BENDING_SM], settings[BENDING_WM], settings[BENDING_STSM], settings[BENDING_STWM], sec, ONCROSS, settings[UTURN_SEC] / settings[SEC]);

  stop(pfd, 0);
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

  time_sleep(1);
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
        if(output[0] == ONLINE && output[4] == ONLINE) state = END;
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

void straight_v2(PFD pfd, int straight, int sm, int wm, int ssm, int swm, double sec, STRTERM strterm, int nodetect_countmax){
  int output[5];
  int nodetect_count = 0;

  STATE state = STR;
  STATE laststate = UND;

  motor_drive(pfd, straight, straight);

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
        laststate = state;
        state = STR;  
        motor_drive(pfd, straight, straight); 
        printf("straight\n");
      }
    }
    else if(output[1] == ONLINE){
      if(state != SLET){
        state = SLET;
        motor_drive(pfd, swm, ssm);
        printf("SLET\n");
      }
    }
    else if(output[3] == ONLINE){
      if(state != SRIT){
        state = SRIT;
        motor_drive(pfd, ssm, swm);
        printf("SRIT\n");
      }
    }
    else if(output[2] == OFFLINE){
      if(state == STR){
        if(laststate == LET){
         state = RIT;
         motor_drive(pfd, sm, wm);
         printf("RIT\n");
        }
        else if(laststate == RIT){
         state = LET;
         motor_drive(pfd, wm, sm);
         printf("LET\n");
        }
      }
    } 

    time_sleep(sec); 
  }

}

void curve(PFD pfd, int lm, int rm, double sec){
  int output[5];
  int old_line_detect;
  int line_detect;
  int line_detect_count = 0;

  get_sensor(pfd, output);
  if(output[2] == ONLINE){
    old_line_detect = ONLINE;
    line_detect = ONLINE;
    line_detect_count = 1;
    printf("output[2] == ONLINE\n");
  }
  else{
    old_line_detect = OFFLINE;
    line_detect = OFFLINE;
    if(output[1] == ONLINE){
      line_detect_count = 1;
      printf("output[1] == ONLINE\n");
    }
    else if(output[3] == ONLINE){
      line_detect_count = 0;
      printf("output[3] = ONLINE\n");
    }
  }

  STATE state = CUV;
  motor_drive(pfd, lm, rm);

  while(state != END){
    get_sensor(pfd, output);
  
    old_line_detect = line_detect;
    line_detect = output[2];

    if(old_line_detect != line_detect){
      if(line_detect == ONLINE){
        line_detect_count++;
        printf("line detected\n");
      }
    }

    if(line_detect_count >= 2){
      state = END;
    }

    time_sleep(sec); 
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

void uturn(PFD pfd, int lm, int rm, double sec){
  int output[5];

  STATE state = CUV;
  motor_drive(pfd, lm, rm);

  while(state != END){
    get_sensor(pfd, output);

    if(output[2] == ONLINE) state = END; 

    time_sleep(sec); 
  }     
}

void stop(PFD pfd, double sec){
  motor_drive(pfd, 0, 0);
  time_sleep(sec);
}
