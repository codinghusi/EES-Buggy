#include <thread>
#include <chrono>
#include "buggy_motors.h"

Buggy_Motors::Buggy_Motors(uint8_t motorLeftPort, uint8_t motorRightPort, uint8_t speed, float turnfactor)
{
    motorLeft = hat.getMotor(motorLeftPort);
    motorRight = hat.getMotor(motorRightPort);
    setSpeed(speed);
    setTurnfactor(turnfactor);
}

Buggy_Motors::~Buggy_Motors()
{
    release();
}

void Buggy_Motors::sayHello()
{
    using namespace std::chrono_literals;
    setSpeed(motorspeed);
    rotateLeft();
    std::this_thread::sleep_for (200ms);
    rotateRight();
    std::this_thread::sleep_for (400ms);
    rotateLeft();
    std::this_thread::sleep_for (200ms);
    brake();
}

void Buggy_Motors::setSpeedLeft(int8_t speed)
{
    if (speed == 0) {
        brakeLeft();
        return;
    }
    motorLeft->setSpeed(mapSpeed(speed));
}

void Buggy_Motors::setSpeedRight(int8_t speed)
{
    if (speed == 0) {
        brakeRight();
        return;
    }
    motorRight->setSpeed(mapSpeed(speed));
}

void Buggy_Motors::setSpeed(int8_t speed)
{
    motorspeed = speed;
    setSpeedLeft(speed);
    setSpeedRight(speed);
}

void Buggy_Motors::setTurnfactor(float turnfactor)
{
    this->turnfactor = turnfactor;
}

void Buggy_Motors::forward()
{
    setSpeed(motorspeed);
    forwardLeft();
    forwardRight();
}

void Buggy_Motors::backward()
{
    setSpeed(motorspeed);
    backwardLeft();
    backwardRight();
}

void Buggy_Motors::brake()
{
    brakeLeft();
    brakeRight();
}

void Buggy_Motors::rotateLeft()
{
    setSpeed(motorspeed);
    backwardLeft();
    forwardRight();
}

void Buggy_Motors::rotateRight()
{
    setSpeed(motorspeed);
    forwardLeft();
    backwardRight();
}

void Buggy_Motors::forwardTurnLeft()
{
    forwardTurnLeft(turnfactor);
}

void Buggy_Motors::forwardTurnLeft(float turnfactor)
{
    setSpeedRight(motorspeed);
    setSpeedLeft(int(motorspeed * turnfactor));
    forward();
}

void Buggy_Motors::forwardTurnRight()
{
    forwardTurnRight(turnfactor);
}

void Buggy_Motors::forwardTurnRight(float turnfactor)
{
    setSpeedRight(int(motorspeed * turnfactor));
    setSpeedLeft(motorspeed);
    forward();
}

void Buggy_Motors::backwardTurnLeft()
{
    backwardTurnLeft(turnfactor);
}

void Buggy_Motors::backwardTurnLeft(float turnfactor)
{
    setSpeedRight(motorspeed);
    setSpeedLeft(int(motorspeed * turnfactor));
    backward();
}

void Buggy_Motors::backwardTurnRight()
{
    backwardTurnRight(turnfactor);
}

void Buggy_Motors::backwardTurnRight(float turnfactor)
{
    setSpeedRight(int(motorspeed * turnfactor));
    setSpeedLeft(motorspeed);
    backward();
}

void Buggy_Motors::release()
{
    motorLeft->run(AdafruitDCMotor::kRelease);
    motorRight->run(AdafruitDCMotor::kRelease);
}
