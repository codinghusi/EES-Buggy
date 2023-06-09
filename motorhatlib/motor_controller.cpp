#include "motor_controller.h"
#include <iostream>

// FIXME: add moving backwards and turning while moving backwards

MotorController::MotorController(uint8_t speed): motors(new Buggy_Motors(speed))
{
    setSpeed(speed);
}

MotorController::~MotorController()
{
    delete motors;
}

void MotorController::slower(float factor) {
    motors->setSpeed(speed * factor);
}

void MotorController::setSpeed(uint8_t speed)
{
    std::cout << "set speed to " << (int) speed << std::endl;
    this->speed = speed;
    motors->setSpeed(speed);
}

void MotorController::setTargetAngle(Angle targetAngle)
{
    this->targetAngle = targetAngle;
}

void MotorController::drive()
{
    drive(currentAngle, 0);
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

void MotorController::driveRelative(Angle targetAngle, float anglePerSecond) {
    std::cout << (currentAngle + targetAngle).get() << std::endl;
    drive(currentAngle + targetAngle, anglePerSecond);
}

void MotorController::drive(Angle targetAngle, float anglePerSecond)
{
    std::unique_lock<std::mutex> lock(motorMtx);

    state = State::FORWARD;
    this->anglePerSecond = anglePerSecond;
    this->startAngle = currentAngle;
    setTargetAngle(targetAngle);
    pid.resetTime();
    delta.start();
}

void MotorController::rotateRelative(Angle targetAngle)
{
    rotate(currentAngle + targetAngle);
}

void MotorController::rotate(Angle targetAngle)
{
    std::unique_lock<std::mutex> lock(motorMtx);

    state = State::ROTATING;
    setTargetAngle(targetAngle);

    // if (angleDifference(targetAngle, currentAngle).get() < 0)
    // {
    //     motors->rotateRight();
    // }
    // else
    // {
    //     motors->rotateLeft();
    // }
}

void MotorController::brake()
{
    std::unique_lock<std::mutex> lock(motorMtx);

    state = State::STOPPED;
    motors->brake();
}

void MotorController::release()
{
    std::unique_lock<std::mutex> lock(motorMtx);

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

    // constexpr float nearingEpsilon = 10;
    const float nearingEpsilon = 90;
    const float epsilon = 5;



    float diff = angleDifference(targetAngle, currentAngle).get();
    float diffAbs = abs(diff);

    // std::cout << "diff: " << diff << ", curr: " << currentAngle.get() << ", target: " << targetAngle.get() << std::endl;


    if (state == State::FORWARD)
    {
        // setSpeed(50);
        anglePerSecond = 50.f;
        // float wipTargetAngle = startAngle.get() + (angleDifference(targetAngle, startAngle).get() / anglePerSecond) * delta.getElapsedSeconds();
        Angle wipTargetAngle = targetAngle;
        // float wipTargetAngle = startAngle.get() + 
        float error = abs(angleDifference(wipTargetAngle, currentAngle).get());
        float correction = pid.calculate(error);
        std::cout << "speed: " << (int)speed << "error: " << error << " correction: " << correction << std::endl;
        if (diff < 0)
        {
            if (drivingForward) {
                motors->forwardTurnRight(correction);
            } else {
                motors->backwardTurnLeft(correction);
            }
        }
        else
        {
            if (drivingForward) {
                motors->forwardTurnLeft(correction);
            } else { 
                motors->backwardTurnRight(correction);
            }
        }
    }
    else if (state == State::ROTATING)
    {
        if (diff < 0) {
            motors->rotateRight();
        }
        else {
            motors->rotateLeft();
        }

        if (diffAbs < epsilon) {
            lock.unlock();
            // slower(1);
            brake();
            lock.lock();
        }
        else if (diffAbs < nearingEpsilon) {
            // lock.unlock();
            // slower(0);
            // std::cout << "nearingEpsilon" << std::endl;
            // lock.lock();
        }
    }
}
