#pragma once

#include <stdint.h>
#include "../helper/timer/timer.h"
#include "../helper/angle/angle.h"
#include "../helper/pid/pid.h"
#include "./buggy_motors.h"


template<typename T>
T map(T x, T in_min, T in_max, T out_min, T out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

class MotorController {
    enum class State {
        STOPPED,
        FORWARD,
        ROTATING
    };

    int8_t speed;
    uint8_t startSpeed = 80;
    Angle startAngle = 0.f;
    Angle targetAngle = 0.f;
    Angle currentAngle = 0.f;
    float anglePerSecond = 0.f;
    State state = State::STOPPED;
    State prevState = State::STOPPED;
    PID pid{0.3f, 0.3f, 0.3f, 0.f, 100.f};
    Timer delta;

    Buggy_Motors motors;

public:

    MotorController(Buggy_Motors motors);

    void setSpeed(uint8_t speed);
    uint8_t getSpeed() { return speed; }
    void setTargetAngle(Angle targetAngle);
    void drive();
    void drive(Angle targetAngle, float anglePerSecond);
    void rotate(Angle targetAngle);
    void brake();
    void release();

    // call periodically
    void setCurrentAngle(Angle currentAngle);

    // call periodically
    void correct();
};
