#include "../buggy_controller.h"


void Buggy_Controller::rectangle()
{
    using namespace std::chrono_literals;
#if (defined(MOTORS) || defined(ULTRASONIC)) && !defined(GYRO)
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
#endif
#ifdef GYRO
    std::chrono::milliseconds forwardtime = std::chrono::milliseconds(1500);
    motors.forwards();
    std::this_thread::sleep_for(forwardtime);
    motors.rotate(90);
    while (motors.state != MotorController::State::STOPPED);
    motors.forwards();
    std::this_thread::sleep_for(forwardtime);
    motors.rotate(180);
    while (motors.state != MotorController::State::STOPPED);
    motors.forwards();
    std::this_thread::sleep_for(forwardtime);
    motors.rotate(270);
    while (motors.state != MotorController::State::STOPPED);
    motors.forwards();
    std::this_thread::sleep_for(forwardtime);
    motors.rotate(0);
    while (motors.state != MotorController::State::STOPPED);
#endif
}
