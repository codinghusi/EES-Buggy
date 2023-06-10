#pragma once

#include "../motors/adafruit/motorhat/adafruitmotorhat.h"
#include "../hcsr04/hcsr04.h"
#include "../mpu6050/mpu6050.h"
#include "../motors/motor_controller/motor_controller.h"

/**
 * Takes control over the buggy
 * Keyfeatures:
 *  - Keyboard control
 *  - Collision prevention (ultrasonic sensor)
 *  - Stabilization (gyro sensor)
 *  - Routines
 */
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
    BuggyController(
        uint8_t motor_left_port,
        uint8_t motor_right_port,
        void (*ultrasonic_handler)(),
        void (*gyro_handler)(),
        int8_t speed);
    void keyboard_control();
    void ultrasonic_handling();
    void ultrasonic_control();
    void gyro_handling();
    void gyro_control();
    void release();

    void wait_for_collision();

    void run_over();

    void circumnavigate_no_gyro();
    void circumnavigate_gyro();

    void rectangle_no_gyro();
    void rectangle_gyro();

    void slalom_motors();
    void slalom_ultrasonic();
    void slalom_gyro();

    void say_hello();
};
