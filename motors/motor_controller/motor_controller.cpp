#include "motor_controller.h"
#include <iostream>
#include "../../config.h"
#include <chrono>
#include <thread>

MotorController::MotorController(uint8_t motor_left_port, uint8_t motor_right_port, uint8_t speed) : motors(new BuggyMotors(motor_left_port, motor_right_port, speed))
{
    set_speed(speed);
}

MotorController::~MotorController()
{
    delete motors;
}

void MotorController::slower(float factor)
{
    set_temporary_speed(speed * factor);
}

void MotorController::set_temporary_speed(float speed)
{
    motors->set_speed(speed);
}

void MotorController::reset_temporary_speed()
{
    motors->set_speed(this->speed);
}

void MotorController::set_speed(uint8_t speed)
{
    this->speed = speed;
    motors->set_speed(speed);
}

void MotorController::set_target_angle(Angle target_angle)
{
    this->target_angle = target_angle;
}

void MotorController::drive()
{
    drive(current_angle);
}

void MotorController::forwards()
{
    driving_forward = true;
    motors->forward();
}

void MotorController::backwards()
{
    driving_forward = false;
    motors->backward();
}

void MotorController::drive_relative(Angle target_angle)
{
    drive(current_angle + target_angle);
}

void MotorController::drive(Angle target_angle)
{
    std::unique_lock<std::mutex> lock(motor_mtx);
    reset_temporary_speed();
    state = State::FORWARD;
    this->start_angle = current_angle;
    set_target_angle(target_angle);
}

void MotorController::rotate_relative(Angle target_angle)
{
    rotate(current_angle - target_angle); // subtracting because it goes anti clockwise (against intuition)
}

void MotorController::rotate(Angle target_angle)
{
    std::unique_lock<std::mutex> lock(motor_mtx);
    state = State::ROTATING;
    set_target_angle(target_angle);
    set_temporary_speed(ROTATION_SPEED);
}

void MotorController::brake(bool doLock)
{
    if (doLock) {
        std::unique_lock<std::mutex> lock(motor_mtx);
    }
    state = State::STOPPED;
    motors->brake();
}

void MotorController::release()
{
    brake();
    motors->release();
}

void MotorController::set_current_angle(Angle current_angle)
{
    this->current_angle = current_angle;
}

void MotorController::correct()
{
    std::unique_lock<std::mutex> lock(motor_mtx);

    if (state == State::STOPPED)
    {
        return;
    }

    const float epsilon = ROTATION_EPSILON;
    float diff = angle_difference(target_angle, current_angle).get();

    if (state == State::FORWARD) {
        float error = abs(diff);
        float correction;
        if (error >= 5.f) {
            correction = 0.f;
        } else if (error >= 1.f) {
            correction = 0.1f;
        } else {
            correction = 1.f;
        }
        if (diff < 0)
        {
            if (driving_forward)
            {
                motors->forward_turn_right(correction);
            }
            else
            {
                motors->backward_turn_left(correction);
            }
        }
        else {
            if (driving_forward)
            {
                motors->forward_turn_left(correction);
            }
            else
            {
                motors->backward_turn_right(correction);
            }
        }
    }
    else if (state == State::ROTATING) {

        if (diff < 0) {
            motors->rotate_right();
        }
        else {
            motors->rotate_left();
        }

        std::cout << "diff: " << diff << std::endl;

        if (abs(diff) < ROTATION_EPSILON) {
            reset_temporary_speed();
            brake(false);
        }
        else if (abs(diff) < ROTATION_NEARING_EPSILON) {
            using namespace std::chrono_literals;
            motors->brake();
            std::this_thread::sleep_for(10ms);
            std::cout << "nearing" << std::endl;
        } 
    }
}
