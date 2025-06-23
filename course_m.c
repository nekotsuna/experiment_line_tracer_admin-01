#include <stdio.h>
#include <stdlib.h>
#include <pigpiod_if2.h>
#include "common.c"

int MODE = 1;
int TURN_SET = 1;

void main() {
    PFD pfd;
    int output[5];
    pfd = init();
    // motor_drive(pfd, 8, 0);

    while (1) {
        if (MODE == 1) {
            while (1) {
                get_sensor(pfd, output);

                if (output[1] == 1) {
                    motor_drive(pfd, 3, 5);
                } else if (output[0] == 1) {
                    motor_drive(pfd, 1, 5);
                } else if (output[3] == 1) {
                    motor_drive(pfd, 5, 3);
                } else if (output[4] == 1) {
                    motor_drive(pfd, 5, 1);
                } else {
                    motor_drive(pfd, 8, 8);
                }

                if ((output[0] == 1 && output[2] == 1) ||
                    (output[0] == 1 && output[3] == 1) ||
                    (output[0] == 1 && output[4] == 1) ||
                    (output[1] == 1 && output[3] == 1) ||
                    (output[1] == 1 && output[4] == 1) ||
                    (output[2] == 1 && output[4] == 1)) {
                    MODE = 2;
                    break;
                }

                time_sleep(0.1);
            }
        }
        else if (MODE == 2) {
            int countdown = 0;
            while (1) {
                get_sensor(pfd, output);

                if (output[0] == 0 &&
                    output[1] == 0 &&
                    output[2] == 0 &&
                    output[3] == 0 &&
                    output[4] == 0) {
                    countdown++;
                    if (countdown > 5) {
                        while (output[4] == 0) {
                            motor_drive(pfd, 2, 5);
                            get_sensor(pfd, output);
                        }
                        MODE = 1;
                        break;
                    }
                } else {
                    motor_drive(pfd, 2, 2);
                    countdown = 0;
                }

                time_sleep(0.05);
            }
        }
        MODE = 1;
    }
}
