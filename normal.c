#include <stdio.h> 
#include <stdlib.h> 
#include <pigpiod_if2.h>

#include "common.c"

void main() {
    PFD pfd;
    int output[5];
    pfd = init();

    motor_drive(pfd, 0, 0);

    /*while(1){
        get_sensor(pfd, output);

        printf("output: ");
        for(int i = 0; i < 5; i++){
           printf("%d ", output[i]);
        }
        printf("\n");

        if(output[0] == 1){
            motor_drive(pfd, 5, 8);
        }
        if(output[4] == 1){
            motor_drive(pfd, 8, 5);
        }

        time_sleep(0.01);
   }*/
	
//    test_motor_drive(pd,fd);
}
