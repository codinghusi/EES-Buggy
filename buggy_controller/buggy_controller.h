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
    /**
     * @param motor_left_port number of the left motor on the motor hat
     * @param motor_right_port number of the right motor on the motor hat
     * @param ultrasonic_handler function pointer will be called when ultrasonic sensor triggers its echo interrupt
     * @param gyro_handler function will be callend when gyro sensor triggers its interrupt
     * @param speed initial speed of both motors
    */
    BuggyController(
        uint8_t motor_left_port,
        uint8_t motor_right_port,
        void (*ultrasonic_handler)(),
        void (*gyro_handler)(),
        int8_t speed);

    /**
     * Endless loop for keyboard handling.
     * Call this function in a separate thread.
    */
    void keyboard_control();

    /**
     * Call this method when interrupt of ultrasonics echo gets called.
    */
    void ultrasonic_handling();

    /**
     * Endless loop for ultrasonic emergency stop.
     * Call this function in a separate thread.
    */
    void ultrasonic_control();

    /**
     * Call this method when interrupt of gyro gets called.
    */
    void gyro_handling();

    /**
     * Endless loop for gyro stabilization for motors.
    */
    void gyro_control();

    /**
     * Release motors. Important when stopping the program.
    */
    void release();

    /**
     * Wait until buggy is stopped by ultrasonic sensor.
    */
    void wait_for_collision();

    /**
     * Little joke because 'umfahren' could mean 'to circumnavigate' and 'to run over' in german.
    */
    void run_over();

    /**
     * Circumnavigate an obstacle, but without using the gyro sensor.
    */
    void circumnavigate_no_gyro();

    /**
     * Circumnavigate an obstacle, with the help of the gyro sensor.
    */
    void circumnavigate_gyro();

    /**
     * Drive a rectangle, but without using the gyro sensor.
    */
    void rectangle_no_gyro();

    /**
     * Drive a rectangle, with the help of the gyro sensor.
    */
    void rectangle_gyro();

    /**
     * Drive a slalom, but without using the gyro sensor.
     * Needs to be a specific slalom layout.
    */
    void slalom_motors();

    /**
     * Drive a slalom, with the help of the ultrasonic sensor.
    */
    void slalom_ultrasonic();
  
    /**
     * Drive a slalom, with the help of gyro and ultrasonic sensor.
    */
    void slalom_gyro();

    /**
     * Buggy will be happy to say 'hello' to you.
    */
    void say_hello();
};
