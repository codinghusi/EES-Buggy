#include "buggy_controller.h"

void Buggy_Controller::runOver()
{
    using namespace std::chrono_literals;
    motors.motors->backward();
    std::this_thread::sleep_for(200ms);
    motors.motors->forward();
    std::this_thread::sleep_for(1s);
    motors.motors->brake();
}

void Buggy_Controller::circumnavigateNoGyro()
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

void Buggy_Controller::circumnavigateGyro()
{
    using namespace std::chrono_literals;
    std::chrono::milliseconds forwardtime = std::chrono::milliseconds(1000);
    std::chrono::milliseconds dodgetime = std::chrono::milliseconds(2000);
    uint8_t angle = circumnavigate_right ? 90 : -90;
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

void Buggy_Controller::rectangleNoGyro()
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

void Buggy_Controller::rectangleGyro()
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

void Buggy_Controller::slalomMotors()
{
    uint8_t leftspeed = 19;
    uint8_t rightspeed = 22;
    motors.motors->setSpeedLeft(leftspeed);
    motors.motors->setSpeedRight(rightspeed);
    motors.motors->forward();
    std::this_thread::sleep_for(200ms);
    circumnavigateNoGyro();
    motors.motors->setSpeedLeft(leftspeed);
    motors.motors->setSpeedRight(rightspeed);
    motors.motors->forward();
    std::this_thread::sleep_for(5000ms);
    circumnavigateNoGyro();
}

void Buggy_Controller::slalomUltrasonic()
{
    uint8_t leftspeed = 19;
    uint8_t rightspeed = 22;
    motors.motors->setSpeedLeft(leftspeed);
    motors.motors->setSpeedRight(rightspeed);
    motors.motors->forward();
    while (!prevent_forward)
        ;
    circumnavigateNoGyro();
    motors.motors->setSpeedLeft(leftspeed);
    motors.motors->setSpeedRight(rightspeed);
    motors.motors->forward();
    while (!prevent_forward)
        ;
    circumnavigateNoGyro();
}

void Buggy_Controller::slalomGyro()
{
    motors.forwards();
    motors.drive();
    while (!prevent_forward)
        ;
    circumnavigateGyro();
    motors->forward();
    motors.drive();
    while (!prevent_forward)
        ;
    circumnavigateGyro();
}
