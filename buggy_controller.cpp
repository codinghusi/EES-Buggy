#include "buggy_controller.h"
#include "terminal.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <chrono>
#include <thread>
#include <wiringPi.h>
#include <iostream>
#include "motorhatlib/motor_controller.h"

Buggy_Controller::Buggy_Controller(void (*ultrasonicHandler)(), void (*gyroHandler)(), int8_t speed):
    motors(speed)
{
    // buggy_motors->sayHello();
    wiringPiSetup();
    ultrasonic_sensor.config(ultrasonicHandler);
    gyro_sensor.init();
    gyro_sensor.setupInterrupt(GYRO_INTERRUPT_PIN, gyroHandler);

    // --- Calibrate Gyro ---
    std::cout << std::endl;
    std::cout << "Calibrating gyro..." << std::endl;
    std::cout << "Please hold your breath." << std::endl;
    while (!gyro_sensor.calibrateDrift(1s))
    {
        std::cout << "Don't move!" << std::endl;
    }
    std::cout << "OK." << std::endl;

}

void Buggy_Controller::ultrasonic_handling(){
    ultrasonic_sensor.chronometryInterrupt();
}

void Buggy_Controller::gyro_handling(){
    gyro_sensor.interruptTriggered();
}

void Buggy_Controller::release(){
    motors.release();
}

void Buggy_Controller::keyboard_control(){
    enableRawMode();
    char c;
    while (read(STDIN_FILENO, &c, 1) == 1)
    {
        if (iscntrl(c))
        {
            printf("%d\n", c);
        }
        else
        {
            printf("%d ('%c')\n", c, c);
        }
        if(prevent_forward && c == 'w') c='b';
        switch (c)
        {
        case 'w':
            motors.forwards();
            motors.drive();
            break;
        case 's':
            motors.backwards();
            motors.drive();
            break;
        case 'a':
            motors.drive(90, 10);
            break;
        case 'd':
            motors.drive(-90, 10);
            break;
        case 'q':
            motors.rotateRelative(-90);
            break;
        case 'e':
            motors.rotateRelative(90);
            break;

        default:
            motors.brake();
            break;
        }
    }
    disableRawMode();
}

void Buggy_Controller::ultrasonic_control()
{
    using namespace std::chrono_literals;
    do
    {
        if (ultrasonic_sensor.get_waitforecho())
            continue;
        if (ultrasonic_sensor.get_distanceresult() < 50)
        {
            if(!prevent_forward) motors.brake();
            prevent_forward = true;
            ultrasonic_sensor.bremslicht_ein();
        }
        else
        {
            prevent_forward = false;
            ultrasonic_sensor.bremslicht_aus();
        }
        std::this_thread::sleep_for(20ms);
        ultrasonic_sensor.distance_messung();
    } while (1);
}

void Buggy_Controller::gyro_control() {
    while(true) {
        motors.setCurrentAngle(Angle(gyro_sensor.gyroscope.z));
        motors.correct();

        std::this_thread::sleep_for(20ms);
    }
}

void Buggy_Controller::slalom()
{
#ifdef MOTORS
    motors.motors->forward();
    std::this_thread::sleep_for(200ms);
    circumnavigate();
    motors.motors->forward();
    std::this_thread::sleep_for(200ms);
    circumnavigate();
#endif
#ifdef ULTRASONIC
    motors.motors->forward();
    while (!prevent_forward)
        ;
    circumnavigate();
    motors.motors->forward();
    while (!prevent_forward)
        ;
    circumnavigate();
#endif
#ifdef GYRO
#endif
}

void Buggy_Controller::rectangle()
{
    using namespace std::chrono_literals;
#if defined(MOTORS) || defined(ULTRASONIC)
    std::chrono::milliseconds rotationtime = std::chrono::milliseconds(200);
    std::chrono::milliseconds forwardtime = std::chrono::milliseconds(200);
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
#endif
}

void Buggy_Controller::runOver()
{
    using namespace std::chrono_literals;
    motors.motors->backward();
    std::this_thread::sleep_for(200ms);
    motors.motors->forward();
    std::this_thread::sleep_for(1s);
    motors.motors->brake();
}

void Buggy_Controller::circumnavigate()
{
    using namespace std::chrono_literals;
#if defined(MOTORS) || defined(ULTRASONIC)
    std::chrono::milliseconds rotationtime = std::chrono::milliseconds(200);
    std::chrono::milliseconds forwardtime = std::chrono::milliseconds(200);
    circumnavigate_right ? motors.motors->rotateRight() : motors.motors->rotateLeft();
    std::this_thread::sleep_for(rotationtime);
    motors.motors->forward();
    std::this_thread::sleep_for(forwardtime);
    circumnavigate_right ? motors.motors->rotateLeft() : motors.motors->rotateRight();
    std::this_thread::sleep_for(rotationtime);
    motors.motors->forward();
    std::this_thread::sleep_for(forwardtime);
    circumnavigate_right ? motors.motors->rotateLeft() : motors.motors->rotateRight();
    std::this_thread::sleep_for(rotationtime);
    motors.motors->forward();
    std::this_thread::sleep_for(forwardtime);
    circumnavigate_right ? motors.motors->rotateRight() : motors.motors->rotateLeft();
    std::this_thread::sleep_for(rotationtime);
    motors.motors->brake();
#endif
#ifdef GYRO
#endif
    circumnavigate_right = !circumnavigate_right;
}