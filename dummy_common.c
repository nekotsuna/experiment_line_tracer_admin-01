#include <stdio.h> 
#include <stdlib.h> 

typedef struct pfd{
	int pd;
	int fd;
} PFD;

PFD init();
int set_pwm_output(PFD,int,int);
int motor_drive(PFD,int,int);
void get_sensor(PFD, int []);
void time_sleep(double seconds);


// プリスケーラレジスタと値（100Hzで動作）
#define PWM_PRESCALE 254


int set_pwm_output(PFD pfd, int pwmch, int outval) {
    int ef = 0; 
    return ef; // エラーがなければ0
}

int motor_drive(PFD pfd, int lm, int rm) {
    return 0;
}

PFD init(){
    PFD pfd;
    return pfd;
}

void get_sensor(PFD pfd, int output[]){
}

void time_sleep(double seconds){
}
