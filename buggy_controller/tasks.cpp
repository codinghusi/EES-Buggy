#include "buggy_controller.h"

void BuggyController::run_over()
{
    using namespace std::chrono_literals;
    motors.forwards();
    motors.drive();
    while (!prevent_forward)
        ;

    motors.rotateRelative(90);
    while (motors.getState() != MotorController::State::STOPPED)
        ;

    std::this_thread::sleep_for(100ms);

    motors.rotateRelative(90);
    while (motors.getState() != MotorController::State::STOPPED)
        ;

    motors.drive();
    std::this_thread::sleep_for(200ms);

    motors.brake();
    std::this_thread::sleep_for(100ms);


    motors.backwards();
    motors.drive();
    motors.setTemporarySpeed(100);
    std::this_thread::sleep_for(1000ms);
    motors.brake();
    motors.resetTemporarySpeed();
}

void BuggyController::circumnavigate_no_gyro()
{
    using namespace std::chrono_literals;
    std::chrono::milliseconds rotationtime = std::chrono::milliseconds(745);
    std::chrono::milliseconds forwardtime = std::chrono::milliseconds(1000);
    std::chrono::milliseconds dodgetime = std::chrono::milliseconds(2000);
    circumnavigate_right ? motors.motors->rotateRight() : motors.motors->rotateLeft();
    std::this_thread::sleep_for(rotationtime);
    motors.motors->forward();
    std::this_thread::sleep_for(forwardtime);
    circumnavigate_right ? motors.motors->rotateLeft() : motors.motors->rotateRight();
    std::this_thread::sleep_for(rotationtime);
    motors.motors->forward();
    std::this_thread::sleep_for(dodgetime);
    circumnavigate_right ? motors.motors->rotateLeft() : motors.motors->rotateRight();
    std::this_thread::sleep_for(rotationtime);
    motors.motors->forward();
    std::this_thread::sleep_for(forwardtime);
    circumnavigate_right ? motors.motors->rotateRight() : motors.motors->rotateLeft();
    std::this_thread::sleep_for(rotationtime);
    motors.motors->brake();
    circumnavigate_right = !circumnavigate_right;
}

void BuggyController::circumnavigate_gyro()
{
    using namespace std::chrono_literals;
    std::chrono::milliseconds forwardtime = std::chrono::milliseconds(2500);
    std::chrono::milliseconds dodgetime = std::chrono::milliseconds(800);
    int16_t angle = circumnavigate_right ? 90 : -90;
    motors.rotateRelative(angle);
    while (motors.getState() != MotorController::State::STOPPED)
        ;
    motors.forwards();
    motors.drive();
    std::this_thread::sleep_for(dodgetime);
    motors.rotateRelative(-angle);
    while (motors.getState() != MotorController::State::STOPPED)
        ;
    motors.forwards();
    motors.drive();
    std::this_thread::sleep_for(forwardtime);
    motors.rotateRelative(-angle);
    while (motors.getState() != MotorController::State::STOPPED)
        ;
    motors.forwards();
    motors.drive();
    std::this_thread::sleep_for(dodgetime);
    motors.rotateRelative(angle);
    while (motors.getState() != MotorController::State::STOPPED)
        ;
    circumnavigate_right = !circumnavigate_right;
}

void BuggyController::rectangle_no_gyro()
{
    using namespace std::chrono_literals;
    std::chrono::milliseconds rotationtime = std::chrono::milliseconds(745);
    std::chrono::milliseconds forwardtime = std::chrono::milliseconds(1500);
    motors.motors->forward();
    std::this_thread::sleep_for(forwardtime);
    motors.motors->rotateRight();
    std::this_thread::sleep_for(rotationtime);
    motors.motors->forward();
    std::this_thread::sleep_for(forwardtime);
    motors.motors->rotateRight();
    std::this_thread::sleep_for(rotationtime);
    motors.motors->forward();
    std::this_thread::sleep_for(forwardtime);
    motors.motors->rotateRight();
    std::this_thread::sleep_for(rotationtime);
    motors.motors->forward();
    std::this_thread::sleep_for(forwardtime);
    motors.motors->rotateRight();
    std::this_thread::sleep_for(rotationtime);
    motors.motors->brake();
}

void BuggyController::rectangle_gyro()
{
    // reset gyroscope
    gyro_sensor.gyroscope.z = 0;
    
    std::chrono::milliseconds forwardtime = std::chrono::milliseconds(1500);
    motors.forwards();
    std::this_thread::sleep_for(forwardtime);
    motors.rotate(90);
    while (motors.getState() != MotorController::State::STOPPED)
        ;
    motors.forwards();
    std::this_thread::sleep_for(forwardtime);
    motors.rotate(180);
    while (motors.getState() != MotorController::State::STOPPED)
        ;
    motors.forwards();
    std::this_thread::sleep_for(forwardtime);
    motors.rotate(270);
    while (motors.getState() != MotorController::State::STOPPED)
        ;
    motors.forwards();
    std::this_thread::sleep_for(forwardtime);
    motors.rotate(0);
    while (motors.getState() != MotorController::State::STOPPED)
        ;
}

void BuggyController::slalom_motors()
{
    uint8_t leftspeed = 19;
    uint8_t rightspeed = 22;
    motors.motors->setSpeedLeft(leftspeed);
    motors.motors->setSpeedRight(rightspeed);
    motors.motors->forward();
    std::this_thread::sleep_for(200ms);
    circumnavigate_no_gyro();
    motors.motors->setSpeedLeft(leftspeed);
    motors.motors->setSpeedRight(rightspeed);
    motors.motors->forward();
    std::this_thread::sleep_for(5000ms);
    circumnavigate_no_gyro();
}

void BuggyController::slalom_ultrasonic()
{
    uint8_t leftspeed = 19;
    uint8_t rightspeed = 22;
    motors.motors->setSpeedLeft(leftspeed);
    motors.motors->setSpeedRight(rightspeed);
    motors.motors->forward();
    while (!prevent_forward)
        ;
    circumnavigate_no_gyro();
    motors.motors->setSpeedLeft(leftspeed);
    motors.motors->setSpeedRight(rightspeed);
    motors.motors->forward();
    while (!prevent_forward)
        ;
    circumnavigate_no_gyro();
}

void BuggyController::slalomGyro()
{
    motors.forwards();
    motors.drive();
    while (!prevent_forward)
        ;
    circumnavigate_gyro();
    motors.forwards();
    motors.drive();
    while (!prevent_forward)
        ;
    circumnavigate_gyro();
}
