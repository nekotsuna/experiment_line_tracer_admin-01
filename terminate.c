#include <stdio.h> 
#include <stdlib.h> 
#include <pigpiod_if2.h>

#include "common.c"

int main(){
  PFD pfd;
  pfd = init();

  motor_drive(pfd, 0, 0);

  return 0;
}
