#pragma once

#include <stdint.h>
#include "../../utils/angle/angle.h"
#include "../buggy_motors/buggy_motors.h"
#include <mutex>

class MotorController
{
public:
    enum class State {
        STOPPED,
        FORWARD,
        ROTATING
    };

private:
    uint8_t speed;
    bool driving_forward = true;

    Angle start_angle = 0.f;
    Angle target_angle = 0.f;
    Angle current_angle = 0.f;
    std::mutex motor_mtx;
    State state = State::STOPPED;
    State prev_state = State::STOPPED;

public:
    Buggy_Motors *motors;

    MotorController(uint8_t motor_left_port, uint8_t motor_right_port, uint8_t speed);
    ~MotorController();

    void set_speed(uint8_t speed);
    void slower(float factor);
    void set_temporary_speed(float speed);
    void reset_temporary_speed();
    uint8_t get_speed() { return speed; }
    void set_target_angle(Angle target_angle);
    void forwards();
    void backwards();
    void drive();
    void drive_relative(Angle target_angle);
    void drive(Angle target_angle);
    void rotate(Angle target_angle);
    void rotate_relative(Angle target_angle);
    void brake(bool doLock = true);
    void release();
    State get_state() { return state; }

    // call periodically
    void set_current_angle(Angle currentAngle);

    // call periodically
    void correct();
};
