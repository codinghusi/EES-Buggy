#include <thread>
#include <chrono>
#include "buggy_motors.h"

Buggy_Motors::Buggy_Motors(uint8_t motor_left_port, uint8_t motor_right_port, uint8_t speed, float turnfactor)
{
    motor_left = hat.getMotor(motor_left_port);
    motor_right = hat.getMotor(motor_right_port);
    set_speed(speed);
    set_turnfactor(turnfactor);
}

Buggy_Motors::~Buggy_Motors()
{
    release();
}

void Buggy_Motors::say_hello()
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

void Buggy_Motors::set_speed_left(int8_t speed)
{
    if (speed == 0) {
        brake_left();
        return;
    }
    motor_left->set_speed(map_speed(speed));
}

void Buggy_Motors::set_speed_right(int8_t speed)
{
    if (speed == 0) {
        brake_right();
        return;
    }
    motor_right->set_speed(map_speed(speed));
}

void Buggy_Motors::set_speed(int8_t speed)
{
    motorspeed = speed;
    set_speed_left(speed);
    set_speed_right(speed);
}

void Buggy_Motors::set_turnfactor(float turnfactor)
{
    this->turnfactor = turnfactor;
}

void Buggy_Motors::forward()
{
    set_speed(motorspeed);
    forward_left();
    forward_right();
}

void Buggy_Motors::backward()
{
    set_speed(motorspeed);
    backward_left();
    backward_right();
}

void Buggy_Motors::brake()
{
    brake_left();
    brake_right();
}

void Buggy_Motors::rotate_left()
{
    set_speed(motorspeed);
    backward_left();
    forward_right();
}

void Buggy_Motors::rotate_right()
{
    set_speed(motorspeed);
    forward_left();
    backward_right();
}

void Buggy_Motors::forward_turn_left()
{
    forward_turn_left(turnfactor);
}

void Buggy_Motors::forward_turn_left(float turnfactor)
{
    set_speed_right(motorspeed);
    set_speed_left(int(motorspeed * turnfactor));
    forward();
}

void Buggy_Motors::forward_turn_right()
{
    forward_turn_right(turnfactor);
}

void Buggy_Motors::forward_turn_right(float turnfactor)
{
    set_speed_right(int(motorspeed * turnfactor));
    set_speed_left(motorspeed);
    forward();
}

void Buggy_Motors::backward_turn_left()
{
    backward_turn_left(turnfactor);
}

void Buggy_Motors::backward_turn_left(float turnfactor)
{
    set_speed_right(motorspeed);
    set_speed_left(int(motorspeed * turnfactor));
    backward();
}

void Buggy_Motors::backward_turn_right()
{
    backward_turn_right(turnfactor);
}

void Buggy_Motors::backward_turn_right(float turnfactor)
{
    set_speed_right(int(motorspeed * turnfactor));
    set_speed_left(motorspeed);
    backward();
}

void Buggy_Motors::release()
{
    motor_left->run(AdafruitDCMotor::kRelease);
    motor_right->run(AdafruitDCMotor::kRelease);
}
