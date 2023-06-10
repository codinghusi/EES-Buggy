#pragma once

#include "../motors/adafruit/motorhat/adafruitmotorhat.h"
#include "../hcsr04/hcsr04.h"
#include "../mpu6050/mpu6050.h"
#include "../motors/motor_controller/motor_controller.h"

class BuggyController
{
private:
    MotorController motors;
    HCSR04 ultrasonic_sensor;
    MPU6050 gyro_sensor;
    float target_angle = 0;
    float motor_relation = 1.f;
    bool prevent_forward;
    bool circumnavigate_right = true;

public:
    BuggyController(uint8_t motorLeftPort, uint8_t motorRightPort, void (*ultrasonicHandler)(), void (*gyroHandler)(), int8_t speed);
    void keyboard_control();
    void ultrasonic_handling();
    void ultrasonic_control();
    void gyro_handling();
    void gyro_control();
    void release();

    void run_over();

    void circumnavigate_no_gyro();
    void circumnavigate_gyro();

    void rectangle_no_gyro();
    void rectangle_gyro();

    void slalom_motors();
    void slalom_ultrasonic();
    void slalom_gyro();
};
