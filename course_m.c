#include <stdio.h>
#include <stdlib.h>
#include <pigpiod_if2.h>
#include "common.c"
//kaizennsita

int MODE = 1;
int TURN_SET = 1;

void main() {
    PFD pfd;
    int output[5];
    pfd = init();
    // motor_drive(pfd, 8, 0);
    int startcheck = 0;
    while (1) {
        if (MODE == 1) {
            int check = 0;
            while (1) {
                printf("1");
                get_sensor(pfd, output);
/*
                if(output[0] == 1 && output[1] == 1 && output[2] == 1 && output[3] == 1 && output[4] == 1){
                    motor_drive(pfd, 0, 0);
                }else if (output[4] == 1) {
                    motor_drive(pfd, 12, 3);
                } else if (output[3] == 1) {
                    motor_drive(pfd, 7, 4);
                } else if (output[2] == 1) {
                    motor_drive(pfd, 6, 6);
                } else if (output[1] == 1) {
                    motor_drive(pfd, 4, 7);
                } else if(output[0] == 1){
                    motor_drive(pfd, 3, 12);
                }
                */
                /*
                if ((output[0] == 1 && output[2] == 1) ||
                    (output[0] == 1 && output[3] == 1) ||
                    (output[0] == 1 && output[4] == 1) ||
                    (output[1] == 1 && output[3] == 1) ||
                    (output[1] == 1 && output[4] == 1) ||
                    (output[2] == 1 && output[4] == 1)) {
                    MODE = 2;
                    break;
                }else 
                    */

                if(output[0] == 1 && output[1] == 1 && output[2] == 1 && output[3] == 1 && output[4] == 1){
                    motor_drive(pfd, 0, 0);
                }else if (output[4] == 1) {
                   motor_drive(pfd, 16,-16);
                } else if (output[3] == 1) {
                    motor_drive(pfd, 16, -16);
                } else if (output[2] == 1) {
                    motor_drive(pfd, 16, 8);
                } else if (output[1] == 1) {
                    motor_drive(pfd, 13, 16);
                } else if(output[0] == 1){
                    motor_drive(pfd, 13, 16);
                }else{
                    motor_drive(pfd,-12,12);
                }

                /*
                    if(output[0]==0&&output[1]==0&&output[2]==0&&output[3]==0&&output[4]==0){
                    check++;
                    if(check == 6){
                        MODE = 2;
                        check = 0;
                        break;
                    }
                }else{
                    check = 0;
                }
                */

                time_sleep(0.025);
            }
        }
        else if (MODE == 2) {
            /*
            int countdown = 0;
            while (1) {
                get_sensor(pfd, output);

                if (output[0] == 0 &&
                    output[1] == 0 &&
                    output[2] == 0 &&
                    output[3] == 0 &&
                    output[4] == 0) {
                    countdown++;
                    if (countdown > 2) {
                        while (output[4] == 0) {
                            motor_drive(pfd, 2, 6);
                            get_sensor(pfd, output);
                        }
                        MODE = 1;
                        break;
                    }
                } else {
                    motor_drive(pfd, 3, 3);
                    countdown = 0;
                }

                time_sleep(0.2);
            }
            */
            
            for(int i =0; i < 20; i++){
            //printf("2");
            get_sensor(pfd, output);
            if(output[2] == 0)motor_drive(pfd, -12, 12);
            else {
                motor_drive(pfd, 9, 9);
                break;
            }time_sleep(0.02);
            }
            MODE = 1;
        }
    }
}
