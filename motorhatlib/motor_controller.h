#pragma once

#include <stdint.h>
#include "../helper/timer/timer.h"
#include "../helper/angle/angle.h"
#include "../helper/pid/pid.h"
#include "./buggy_motors.h"
#include <mutex>


class MotorController {
public:
    enum class State {
        STOPPED,
        FORWARD,
        ROTATING
    };

private:
    uint8_t speed;
    bool drivingForward = true;
    
    Angle startAngle = 0.f;
    Angle targetAngle = 0.f;
    Angle currentAngle = 0.f;
    float anglePerSecond = 0.f;
    PID pid{0.1f, 0.1f, 0.8f, -100.f, 100.f};
    Timer delta;
    std::mutex motorMtx;

public:
    State state = State::STOPPED;
    State prevState = State::STOPPED;

    Buggy_Motors *motors;

    MotorController(uint8_t speed);
    ~MotorController();

    void setSpeed(uint8_t speed);
    void slower(float factor);
    uint8_t getSpeed() { return speed; }
    void setTargetAngle(Angle targetAngle);
    void forwards();
    void backwards();
    void drive();
    void driveRelative(Angle targetAngle, float anglePerSecond);
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
