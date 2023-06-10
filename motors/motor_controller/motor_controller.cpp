#include "motor_controller.h"
#include <iostream>

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
    motors->set_speed(speed * factor);
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

    const float epsilon = 5;
    float diff = angleDifference(targetAngle, currentAngle).get();

    if (state == State::FORWARD) {
        float error = abs(angleDifference(targetAngle, currentAngle).get());
        float correction = error >= 1 ? 0 : 1;
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

        if (abs(diff) < epsilon) {
            brake(false);
        }
    }
}
