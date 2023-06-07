#include "motor_controller.h"

// FIXME: add moving backwards and turning while moving backwards

MotorController::MotorController(Buggy_Motors motors): motors(motors)
{
}

void MotorController::setSpeed(uint8_t speed)
{
    this->speed = speed;
    motors.setSpeed(map<uint8_t>(abs(speed), 0, 100, startSpeed, 255));
}

void MotorController::setTargetAngle(Angle targetAngle)
{
    this->targetAngle = targetAngle;
}

void MotorController::drive()
{
    drive(currentAngle, 0);
}

void MotorController::drive(Angle targetAngle, float anglePerSecond)
{
    state = State::FORWARD;
    this->anglePerSecond = anglePerSecond;
    this->startAngle = currentAngle;
    setTargetAngle(targetAngle);

    motors.forward();

    pid.resetTime();
    delta.start();
}

void MotorController::rotate(Angle targetAngle)
{
    state = State::ROTATING;
    setTargetAngle(targetAngle);

    if (angleDifference(targetAngle, currentAngle).get() > 0)
    {
        motors.rotateRight();
    }
    else
    {
        motors.rotateLeft();
    }
}

void MotorController::brake()
{
    state = State::STOPPED;
    motors.brake();
}

void MotorController::release()
{
    state = State::STOPPED;
    motors.release();
}

void MotorController::setCurrentAngle(Angle currentAngle)
{
    this->currentAngle = currentAngle;
}

void MotorController::correct()
{
    if (state == State::STOPPED)
    {
        return;
    }

    constexpr float epsilon = 5;

    float diff = abs(angleDifference(currentAngle, targetAngle).get());

    if (state == State::FORWARD)
    {
        if (diff < epsilon)
        {
            anglePerSecond = 0;
            targetAngle = currentAngle;
        }
        float wipTargetAngle = startAngle.get() + (angleDifference(targetAngle, startAngle).get() / anglePerSecond) * delta.getElapsedSeconds();
        float error = angleDifference(wipTargetAngle, currentAngle).get();
        float correction = pid.calculate(error) / 100.f;
        if (angleDifference(wipTargetAngle, currentAngle).get() > 0)
        {
            motors.forwardTurnRight(correction);
        }
        else
        {
            motors.forwardTurnLeft(-correction);
        }
    }
    else if (state == State::ROTATING)
    {
        if (diff < epsilon)
        {
            brake();
            return;
        }
    }
}
