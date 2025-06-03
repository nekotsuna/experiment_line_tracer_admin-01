#include "common.c"

void straight(PFD, int*);

int main(){
   PFD pfd;
   int output[5];
   pfd = init();

   motor_drive(pfd, 0, 0); 
   straight(pfd, output);
}

void straight(PFD pfd, int* output){
   while(1){
      get_sensor(pfd, output);

      if(output[1] == 1){
         motor_drive(pfd, 0, 13);
      }
      else if(output[3] == 1){
         motor_drive(pfd, 13, 0);
      }
      else if(output[0] == 1 || output[4] == 1){
         motor_drive(pfd, 0, 0);
         break;
      }
      else{
         motor_drive(pfd, 13, 13);
      }
   } 
}
