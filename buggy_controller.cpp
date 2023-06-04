#include "buggy_controller.h"
#include "terminal.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <chrono>
#include <thread>
#include <wiringPi.h>

Buggy_Controller::Buggy_Controller(void (*ultrasonicHandler)(), unsigned speed)
{
    buggy_motors = new Buggy_Motors(speed);
    buggy_motors->sayHello();
    wiringPiSetup();
    ultrasonic_sensor = new HCSR04();
    ultrasonic_sensor->config(ultrasonicHandler);
}

void Buggy_Controller::ultrasonic_handling(){
    ultrasonic_sensor->chronometryInterrupt();
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