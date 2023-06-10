#include "motor_controller.h"
#include <iostream>
#include "../../config.h"
#include <chrono>
#include <thread>

MotorController::MotorController(uint8_t motorLeftPort, uint8_t motorRightPort, uint8_t speed): motors(new Buggy_Motors(motorLeftPort, motorRightPort, speed))
{
    setSpeed(speed);
}

MotorController::~MotorController()
{
    delete motors;
}

void MotorController::slower(float factor)
{
    setTemporarySpeed(speed * factor);
}

void MotorController::setTemporarySpeed(float speed)
{
    motors->setSpeed(speed);
}

void MotorController::resetTemporarySpeed()
{
    motors->setSpeed(this->speed);
}

void MotorController::setSpeed(uint8_t speed)
{
    this->speed = speed;
    motors->set_speed(speed);
}

void MotorController::setTargetAngle(Angle targetAngle)
{
    this->targetAngle = targetAngle;
}

void MotorController::drive()
{
    drive(currentAngle);
}

void MotorController::forwards()
{
    drivingForward = true;
    motors->forward();
}

void MotorController::backwards()
{
    drivingForward = false;
    motors->backward();
}

void MotorController::driveRelative(Angle targetAngle)
{
    drive(currentAngle + targetAngle);
}

void MotorController::drive(Angle targetAngle)
{
    std::unique_lock<std::mutex> lock(motorMtx);
    resetTemporarySpeed();
    state = State::FORWARD;
    this->startAngle = currentAngle;
    setTargetAngle(targetAngle); 
}

void MotorController::rotateRelative(Angle targetAngle)
{
    rotate(currentAngle - targetAngle); // subtracting because it goes anti clockwise (against intuition)
}

void MotorController::rotate(Angle targetAngle)
{
    std::unique_lock<std::mutex> lock(motorMtx);
    state = State::ROTATING;
    setTargetAngle(targetAngle);
    setTemporarySpeed(ROTATION_SPEED); 
}

void MotorController::brake(bool doLock)
{
    if (doLock) {
        std::unique_lock<std::mutex> lock(motorMtx);
    }
    state = State::STOPPED;
    motors->brake();
}

void MotorController::release()
{
    brake();
    motors->release();
}

void MotorController::setCurrentAngle(Angle currentAngle)
{
    this->currentAngle = currentAngle;
}

void MotorController::correct()
{
    std::unique_lock<std::mutex> lock(motorMtx);

    if (state == State::STOPPED)
    {
        return;
    }

    const float epsilon = ROTATION_EPSILON;
    float diff = angle_difference(targetAngle, currentAngle).get();

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
            if (drivingForward) {
                motors->forward_turn_right(correction);
            } else {
                motors->backward_turn_left(correction);
            }
        }
        else {
            if (drivingForward) {
                motors->forward_turn_left(correction);
            } else { 
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
            resetTemporarySpeed();
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
