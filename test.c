void test_motor_drive(int pd, int fd)
{
    printf("右モーター前進（中速）\n");
    motor_drive(pd, fd, 0, 8); // 右だけ前進
    time_sleep(2.0);

    printf("左モーター前進（中速）\n");
    motor_drive(pd, fd, 8, 0); // 左だけ前進
    time_sleep(2.0);

    printf("両モーター前進（全速）\n");
    motor_drive(pd, fd, 16, 16);
    time_sleep(2.0);

    printf("両モーター後進（中速）\n");
    motor_drive(pd, fd, -8, -8);
    time_sleep(2.0);

    printf("停止\n");
    motor_drive(pd, fd, 0, 0);
}

// -----------------------------
// メイン関数
// -----------------------------
int main()
{
    int pd, fd;

    // pigpiod 接続 & PWMユニット初期化
    initialize_pwm_unit(&pd, &fd);

    // モーター動作テスト
    test_motor_drive(pd, fd);

    // I2Cデバイスクローズ
    i2c_close(pd, fd);

    // pigpiod 接続解除
    pigpio_stop(pd);

    printf("テスト完了\n");
    return 0;
}
