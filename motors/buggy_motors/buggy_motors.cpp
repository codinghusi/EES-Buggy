#include <thread>
#include <chrono>
#include "buggy_motors.h"

BuggyMotors::BuggyMotors(uint8_t motor_left_port, uint8_t motor_right_port, uint8_t speed, float turnfactor)
{
    motor_left = hat.getMotor(motor_left_port);
    motor_right = hat.getMotor(motor_right_port);
    set_speed(speed);
    set_turnfactor(turnfactor);
}

BuggyMotors::~BuggyMotors()
{
    release();
}

void BuggyMotors::say_hello()
{
    using namespace std::chrono_literals;
    set_speed(motorspeed);
    rotate_left();
    std::this_thread::sleep_for (200ms);
    rotate_right();
    std::this_thread::sleep_for (400ms);
    rotate_left();
    std::this_thread::sleep_for (200ms);
    brake();
}

void BuggyMotors::set_speed_left(int8_t speed)
{
    if (speed == 0) {
        brake_left();
        return;
    }
    motor_left->setSpeed(map_speed(speed));
}

void BuggyMotors::set_speed_right(int8_t speed)
{
    if (speed == 0) {
        brake_right();
        return;
    }
    motor_right->setSpeed(map_speed(speed));
}

void BuggyMotors::set_speed(int8_t speed)
{
    motorspeed = speed;
    set_speed_left(speed);
    set_speed_right(speed);
}

void BuggyMotors::set_turnfactor(float turnfactor)
{
    this->turnfactor = turnfactor;
}

void BuggyMotors::forward()
{
    set_speed(motorspeed);
    forward_left();
    forward_right();
}

void BuggyMotors::backward()
{
    set_speed(motorspeed);
    backward_left();
    backward_right();
}

void BuggyMotors::brake()
{
    brake_left();
    brake_right();
}

void BuggyMotors::rotate_left()
{
    set_speed(motorspeed);
    backward_left();
    forward_right();
}

void BuggyMotors::rotate_right()
{
    set_speed(motorspeed);
    forward_left();
    backward_right();
}

void BuggyMotors::forward_turn_left()
{
    forward_turn_left(turnfactor);
}

void BuggyMotors::forward_turn_left(float turnfactor)
{
    set_speed_right(motorspeed);
    set_speed_left(int(motorspeed * turnfactor));
    forward();
}

void BuggyMotors::forward_turn_right()
{
    forward_turn_right(turnfactor);
}

void BuggyMotors::forward_turn_right(float turnfactor)
{
    set_speed_right(int(motorspeed * turnfactor));
    set_speed_left(motorspeed);
    forward();
}

void BuggyMotors::backward_turn_left()
{
    backward_turn_left(turnfactor);
}

void BuggyMotors::backward_turn_left(float turnfactor)
{
    set_speed_right(motorspeed);
    set_speed_left(int(motorspeed * turnfactor));
    backward();
}

void BuggyMotors::backward_turn_right()
{
    backward_turn_right(turnfactor);
}

void BuggyMotors::backward_turn_right(float turnfactor)
{
    set_speed_right(int(motorspeed * turnfactor));
    set_speed_left(motorspeed);
    backward();
}

void BuggyMotors::release()
{
    motor_left->run(AdafruitDCMotor::kRelease);
    motor_right->run(AdafruitDCMotor::kRelease);
}
