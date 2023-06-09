#pragma once

#include <stdint.h>
#include "../helper/timer/timer.h"
#include "../helper/angle/angle.h"
#include "../helper/pid/pid.h"
#include "./buggy_motors.h"


class MotorController {
    enum class State {
        STOPPED,
        FORWARD,
        ROTATING
    };

    int8_t speed;
    
    Angle startAngle = 0.f;
    Angle targetAngle = 0.f;
    Angle currentAngle = 0.f;
    float anglePerSecond = 0.f;
    State state = State::STOPPED;
    State prevState = State::STOPPED;
    PID pid{0.3f, 0.3f, 0.3f, 0.f, 100.f};
    Timer delta;

    Buggy_Motors* motors;

public:

    MotorController(int8_t speed);
    ~MotorController();

    void setSpeed(int8_t speed);
    void slower(float factor);
    int8_t getSpeed() { return speed; }
    void setTargetAngle(Angle targetAngle);
    void drive();
    void forwards();
    void backwards();
    void drive(Angle targetAngle, float anglePerSecond);
    void rotate(Angle targetAngle);
    void rotateRelative(Angle targetAngle);
    void brake();
    void release();

    // call periodically
    void setCurrentAngle(Angle currentAngle);

    // call periodically
    void correct();
};
