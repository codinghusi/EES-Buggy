#include <thread>
#include <chrono>
#include "buggy_motors.h"

Buggy_Motors::Buggy_Motors(unsigned speed, unsigned turnspeed){
    motorLeft = hat.getMotor(1);
    motorRight = hat.getMotor(4);
    setSpeed(speed);
    setTurnspeed(turnspeed);
}

Buggy_Motors::~Buggy_Motors(){
    release();
}

void Buggy_Motors::sayHello(){
    setSpeed(motorspeed);
    using namespace std::chrono_literals;
    rotateLeft();
    std::this_thread::sleep_for (200ms);
    rotateRight();
    std::this_thread::sleep_for (400ms);
    rotateLeft();
    std::this_thread::sleep_for (200ms);
    brake();
}

void Buggy_Motors::setSpeed(unsigned speed){
    motorspeed = speed;
    setSpeedLeft(speed);
    setSpeedRight(speed);
}

void Buggy_Motors::setTurnspeed(unsigned turnspeed){
    this->turnspeed = turnspeed;
}

void Buggy_Motors::forward(){
    setSpeed(motorspeed);
    forwardLeft();
    forwardRight();
}

void Buggy_Motors::backward(){
    setSpeed(motorspeed);
    backwardLeft();
    backwardRight();
}

void Buggy_Motors::brake(){
    brakeLeft();
    brakeRight();
}

void Buggy_Motors::rotateLeft(){
    setSpeed(motorspeed);
    backwardLeft();
    forwardRight();
}

void Buggy_Motors::rotateRight(){
    setSpeed(motorspeed);
    forwardLeft();
    backwardRight();
}

void Buggy_Motors::forwardTurnLeft()
{
    setSpeedRight(unsigned(motorspeed * turnspeed));
    setSpeedLeft(motorspeed);
    forward();
}

void Buggy_Motors::forwardTurnRight()
{
    setSpeedRight(motorspeed);
    setSpeedLeft(unsigned(motorspeed * turnspeed));
    forward();
}

void Buggy_Motors::backwardTurnLeft()
{
    setSpeedRight(unsigned(motorspeed * turnspeed));
    setSpeedLeft(motorspeed);
    backward();
}

void Buggy_Motors::backwardTurnRight()
{
    setSpeedRight(motorspeed);
    setSpeedLeft(unsigned(motorspeed * turnspeed));
    backward();
}

void Buggy_Motors::release(){
    motorLeft->run(AdafruitDCMotor::kRelease);
    motorRight->run(AdafruitDCMotor::kRelease);
}

void Buggy_Motors::release(){
    motorLeft->run(AdafruitDCMotor::kRelease);
    motorRight->run(AdafruitDCMotor::kRelease);
}
