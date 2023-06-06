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
#include "helper/angle_difference/angle_difference.h"

Buggy_Controller::Buggy_Controller(void (*ultrasonicHandler)(), void (*gyroHandler)(), unsigned speed)
{
    buggy_motors = new Buggy_Motors(speed);
    // buggy_motors->sayHello();
    wiringPiSetup();
    ultrasonic_sensor = new HCSR04();
    ultrasonic_sensor->config(ultrasonicHandler);
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
    ultrasonic_sensor->chronometryInterrupt();
}

void Buggy_Controller::gyro_handling(){
    gyro_sensor.interruptTriggered();
}

void Buggy_Controller::release(){
    buggy_motors->release();
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
            buggy_motors->forward();
            break;
        case 's':
            buggy_motors->backward();
            break;
        case 'a':
            buggy_motors->forwardTurnLeft();
            break;
        case 'd':
            buggy_motors->forwardTurnRight();
            break;
        case 'q':
            buggy_motors->rotateLeft();
            break;
        case 'e':
            buggy_motors->rotateRight();
            break;

        default:
            buggy_motors->brake();
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
        if (ultrasonic_sensor->get_waitforecho())
            continue;
        if (ultrasonic_sensor->get_distanceresult() < 50)
        {
            if(!prevent_forward) buggy_motors->brake();
            prevent_forward = true;
            ultrasonic_sensor->bremslicht_ein();
        }
        else
        {
            prevent_forward = false;
            ultrasonic_sensor->bremslicht_aus();
        }
        std::this_thread::sleep_for(20ms);
        ultrasonic_sensor->distance_messung();
    } while (1);
}

void Buggy_Controller::gyro_control() {
    while(true) {
        float error = angleDifference(gyro_sensor.gyroscope.z, target_angle);
        // float relation = correction.calculate(error) / 1000.f;
        float relation = error / 180.f;
        buggy_motors->setSpeedLeft(70 * (1.f + relation));
        buggy_motors->setSpeedRight(70 * (1.f - relation));

        std::cout << "Error: " << error << ", Correction: " << relation << std::endl;
        std::cout << "Gyro: " << gyro_sensor.gyroscope.z << std::endl;

        std::this_thread::sleep_for(100ms);
    }
}
