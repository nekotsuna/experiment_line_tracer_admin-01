#include <stdio.h>
#include <stdlib.h>
#include <pigpiod_if2.h>
#include "common.c"
//complete now

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
            
            while(startcheck == 0){
                get_sensor(pfd, output);
                int calc = output[0] + output[1] + output[2] + output[3] + output[4];
                int calckeep = calc;
                if(calckeep == 5){
                    get_sensor(pfd, output);
                    int calc = output[0] + output[1] + output[2] + output[3] + output[4];
                    while(calc < 1){
                        startcheck = 1;
                        calckeep = 0;
                    }
                    time_sleep(0.25);
                }
                time_sleep(0.25);
            }
            
            int check = 0;
            
            while (1) {
                printf("1");
                get_sensor(pfd, output);

                if (output[4] == 1) {
                    motor_drive(pfd, 11, 1);
                } else if (output[3] == 1) {
                    motor_drive(pfd, 8, 4);
                } else if (output[2] == 1) {
                    motor_drive(pfd, 6, 6);
                } else if (output[1] == 1) {
                    motor_drive(pfd, 4, 8);
                } else if(output[0] == 1){
                    motor_drive(pfd, 1, 11);
                }
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
                    */if(output[0]==0&&output[1]==0&&output[2]==0&&output[3]==0&&output[4]==0){
                    check++;
                    if(check == 6){
                        MODE = 2;
                        check = 0;
                        break;
                    }
                }else{
                    check = 0;
                }

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
            if(i < 14)motor_drive(pfd, -5, 10);
            else motor_drive(pfd, 9, 9);
            time_sleep(0.05);
            }
            MODE = 1;
        }
    }
}
