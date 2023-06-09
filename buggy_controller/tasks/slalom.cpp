#include "../buggy_controller.h"

void Buggy_Controller::slalom()
{
    uint8_t leftspeed = 19;
    uint8_t rightspeed = 22;
#ifndef ULTRASONIC
#ifdef MOTORS
    motors.motors->setSpeedLeft(leftspeed);
    motors.motors->setSpeedRight(rightspeed);
    motors.motors->forward();
    std::this_thread::sleep_for(200ms);
    circumnavigate();
    motors.motors->setSpeedLeft(leftspeed);
    motors.motors->setSpeedRight(rightspeed);
    motors.motors->forward();
    std::this_thread::sleep_for(5000ms);
    circumnavigate();
#endif
#endif
#ifdef ULTRASONIC
    motors.motors->setSpeedLeft(leftspeed);
    motors.motors->setSpeedRight(rightspeed);
    motors.motors->forward();
    while (!prevent_forward)
        ;
    circumnavigate();
    motors.motors->setSpeedLeft(leftspeed);
    motors.motors->setSpeedRight(rightspeed);
    motors.motors->forward();
    while (!prevent_forward)
        ;
    circumnavigate();
#endif
#ifdef GYRO
#endif
}
