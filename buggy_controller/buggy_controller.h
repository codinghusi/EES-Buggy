#pragma once

#include "../motors/adafruit/motorhat/adafruitmotorhat.h"
#include "../hcsr04/hcsr04.h"
#include "../mpu6050/mpu6050.h"
#include "../motors/motor_controller/motor_controller.h"

class Buggy_Controller
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
    Buggy_Controller(uint8_t motorLeftPort, uint8_t motorRightPort, void (*ultrasonic_handler)(), void (*gyro_handler)(), int8_t speed);
    void keyboard_control();
    void ultrasonic_handling();
    void ultrasonic_control();
    void gyro_handling();
    void gyro_control();
    void release();

    void runOver();

    void circumnavigateNoGyro();
    void circumnavigateGyro();

    void rectangleNoGyro();
    void rectangleGyro();

    void slalomMotors();
    void slalomUltrasonic();
    void slalomGyro();
};
