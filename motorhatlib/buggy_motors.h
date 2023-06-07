#pragma once
#include "adafruitmotorhat.h"

class Buggy_Motors{
    enum class State
    {
        STOPPED,
        FORWARD,
        BACKWARD,
        ROTATING
    };
    State leftState = State::STOPPED;
    State rightState = State::STOPPED;
    float turnfactor;
    uint8_t motorspeed;
    AdafruitMotorHAT hat;
    std::shared_ptr<AdafruitDCMotor> motorLeft, motorRight;

    inline void forwardLeft() { motorLeft->run(AdafruitDCMotor::kForward); }
    inline void forwardRight() { motorRight->run(AdafruitDCMotor::kBackward); }
    inline void backwardLeft() { motorLeft->run(AdafruitDCMotor::kBackward); }
    inline void backwardRight() { motorRight->run(AdafruitDCMotor::kForward); }
    inline void brakeLeft() { motorLeft->run(AdafruitDCMotor::kBrake); }
    inline void brakeRight() { motorRight->run(AdafruitDCMotor::kBrake); }

public:
    Buggy_Motors(uint8_t speed = 100, float turnfactor = 0.9);
    ~Buggy_Motors();
    void sayHello();
    void setSpeedLeft(int8_t motorspeed);
    void setSpeedRight(int8_t motorspeed);
    void setSpeed(int8_t motorspeed);
    void setTurnfactor(float turnfactor);
    void forward();
    void backward();
    void brake();
    void rotateLeft();
    void rotateRight();
    void forwardTurnLeft();
    void forwardTurnLeft(float turnfactor);
    void forwardTurnRight();
    void forwardTurnRight(float turnfactor);
    void backwardTurnLeft();
    void backwardTurnLeft(float turnfactor);
    void backwardTurnRight();
    void backwardTurnRight(float turnfactor);
    void release();
};
