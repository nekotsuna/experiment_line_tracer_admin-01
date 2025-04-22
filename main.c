// 必要なヘッダー
#include <stdio.h> 
#include <stdlib.h> 
#include <pigpiod_if2.h>

// ----------------------------- 定数定義 -----------------------------

int set_pwm_output(int,int,int,int); int motor_drive(int,int,int,int);

// PWM ユニットの I2C アドレスとチャネル
#define PWMI2CADR 0x40 
#define PWMI2CCH 1

// 右モーター制御用PWMチャネル
#define ENA_PWM 8 
#define IN1_PWM 9 
#define IN2_PWM 10

// 左モーター制御用PWMチャネル
#define ENB_PWM 13 
#define IN3_PWM 11 
#define IN4_PWM 12

// PWM モジュールのレジスタ
#define PWM_MODE1 0 
#define PWM_MODE2 1 
#define PWM_SUBADR1 2 
#define PWM_SUBADR2 3 
#define PWM_SUBADR3 4 
#define PWM_ALLCALL 5

#define PWM_0_ON_L 6 
#define PWM_0_ON_H 7 
#define PWM_0_OFF_L 8 
#define PWM_0_OFF_H 9

// PWM 出力値の定数
#define PWMFULLON 16 
#define PWMFULLOFF 0

#define BCM_GPIO5 5 
#define BCM_GPIO6 6 
#define BCM_GPIO13 13 
#define BCM_GPIO19 19 
#define BCM_GPIO26 26 

// プリスケーラレジスタと値（100Hzで動作）
#define PWM_PRESCALE 254

void test_motor_drive(int pd, int fd) {
    printf("右モーター前進（中速）\n"); motor_drive(pd, fd, 0, 8); // 右だけ前進 time_sleep(2.0);

    printf("左モーター前進（中速）\n"); motor_drive(pd, fd, 8, 0); // 左だけ前進 time_sleep(2.0);

    printf("両モーター前進（全速）\n"); motor_drive(pd, fd, 16, 16); time_sleep(2.0);

    printf("両モーター後進（中速）\n"); motor_drive(pd, fd, -8, -8); time_sleep(2.0);

    printf("停止\n"); motor_drive(pd, fd, 0, 0);
}


// ----------------------------- PWM 出力を設定する関数 -----------------------------
int set_pwm_output(int pd, int fd, int pwmch, int outval) {
    int ef = 0; int regno;

    // チャネル範囲チェック（0〜15）
    if (pwmch < 0 || pwmch > 15) ef = 1;

    // 出力値チェック（0〜16）
    if (outval < 0 || outval > 16) ef += 2;

    if (ef == 0) { regno = PWM_0_ON_L + pwmch * 4;

        if (outval == 16) { i2c_write_byte_data(pd, fd, regno + 3, 0); i2c_write_byte_data(pd, fd, regno + 1, 0x10);
        } else {
            i2c_write_byte_data(pd, fd, regno + 1, 0); i2c_write_byte_data(pd, fd, regno + 3, outval);
        }
    }

    return ef; // エラーがなければ0
}

// ----------------------------- モーター制御関数 -----------------------------
int motor_drive(int pd, int fd, int lm, int rm) {
    // 一旦モーター無効化
    set_pwm_output(pd, fd, ENA_PWM, 0); set_pwm_output(pd, fd, ENB_PWM, 0);

    // 右モーター制御（回転方向）
    if (rm < 0) { set_pwm_output(pd, fd, IN1_PWM, 0); set_pwm_output(pd, fd, IN2_PWM, 16); rm = abs(rm);
    } else {
        set_pwm_output(pd, fd, IN1_PWM, 16); set_pwm_output(pd, fd, IN2_PWM, 0);
    }

    // 左モーター制御（回転方向）
    if (lm < 0) { set_pwm_output(pd, fd, IN3_PWM, 0); set_pwm_output(pd, fd, IN4_PWM, 16); lm = abs(lm);
    } else {
        set_pwm_output(pd, fd, IN3_PWM, 16); set_pwm_output(pd, fd, IN4_PWM, 0);
    }

    // 最大値制限
    if (lm > 16) lm = 16; if (rm > 16) rm = 16;

    // モーター駆動（PWM信号）
    set_pwm_output(pd, fd, ENA_PWM, rm); set_pwm_output(pd, fd, ENB_PWM, lm);

    return 0;
}

// ----------------------------- 初期化処理（pigpiod接続とPWM初期化） -----------------------------
void main() {
    int pd, fd; pd = pigpio_start(NULL, NULL); if (pd < 0) {
        printf("pigpiod の接続に失敗しました。\n"); 
        printf("pigpiod が起動しているか確認してください。\n"); 
        exit(EXIT_FAILURE);
    }

    fd = i2c_open(pd, PWMI2CCH, PWMI2CADR, 0); if (fd < 0) {
        printf("I2C の初期化に失敗しました。終了します。\n");
        printf("fd: %d\n", fd);
        printf("PI_BAD_I2C_BUS:%d\n",PI_BAD_I2C_BUS); 
        printf("PI_BAD_I2C_ADDR: %d\n",PI_BAD_I2C_ADDR); 
        printf("PI_BAD_FLAGS: %d\n",PI_BAD_FLAGS); 
	printf("PI_NO_HANDLE: %d\n",PI_NO_HANDLE); 
	exit(EXIT_FAILURE);
    }

    // PWM ユニットの初期設定
    i2c_write_byte_data(pd, fd, PWM_PRESCALE, 61); // 周期10msに設定 
    i2c_write_byte_data(pd, fd, PWM_MODE1, 0x10); // SLEEPモード 
    i2c_write_byte_data(pd, fd, PWM_MODE1, 0); // 通常モード 
    time_sleep(0.001); // 発振安定待ち 
    i2c_write_byte_data(pd, fd, 
    PWM_MODE1, 0x80); // 自動インクリメント設定

    set_mode(pd,BCM_GPIO5,PI_INPUT); 
    set_mode(pd,BCM_GPIO6,PI_INPUT);
    set_mode(pd,BCM_GPIO13,PI_INPUT); 
    set_mode(pd,BCM_GPIO19,PI_INPUT);
    set_mode(pd,BCM_GPIO26,PI_INPUT);

    while(1){
	 int output[5];
	 output[0] = gpio_read(pd,BCM_GPIO5);
	 output[1] = gpio_read(pd,BCM_GPIO6);
	 output[2] = gpio_read(pd,BCM_GPIO13);
	 output[3] = gpio_read(pd,BCM_GPIO19);
	 output[4] = gpio_read(pd,BCM_GPIO26);

	printf("output: ");
	for(int i = 0; i < 5; i++){
		printf("%d ", output[i]);
	}
	printf("\n");

	time_sleep(0.5);
   }
	
	//    test_motor_drive(pd,fd);
}

