#include <stdio.h> 
#include <stdlib.h> 
#include <pigpiod_if2.h>

#include "common.c"

void main() {
    PFD pfd;
    int output[5];
    pfd = init();

    motor_drive(pfd, 0, 0);

    while(1){
      
   }
	
}
