#pragma once
#include "adafruitmotorhat.h"

class Buggy_Motors{
    float turnspeed;
    unsigned motorspeed;
    AdafruitMotorHAT hat;
    std::shared_ptr<AdafruitDCMotor> motorLeft, motorRight;

    inline void forwardLeft() { motorLeft->run(AdafruitDCMotor::kForward); }
    inline void forwardRight() { motorRight->run(AdafruitDCMotor::kBackward); }
    inline void backwardLeft() { motorLeft->run(AdafruitDCMotor::kBackward); }
    inline void backwardRight() { motorRight->run(AdafruitDCMotor::kForward); }
    inline void brakeLeft() { motorLeft->run(AdafruitDCMotor::kBrake); }
    inline void brakeRight() { motorRight->run(AdafruitDCMotor::kBrake); }

public:
    Buggy_Motors(unsigned speed = 100, unsigned turnspeed = 1.4);
    ~Buggy_Motors();
    void sayHello();
    inline void setSpeedLeft(unsigned speed) { motorLeft->setSpeed(speed); }
    inline void setSpeedRight(unsigned speed) { motorRight->setSpeed(speed); }
    void setSpeed(unsigned speed);
    void setTurnspeed(unsigned speed);
    void forward();
    void backward();
    void brake();
    void rotateLeft();
    void rotateRight();
    void forwardTurnLeft();
    void forwardTurnRight();
    void backwardTurnLeft();
    void backwardTurnRight();
    void release();
};