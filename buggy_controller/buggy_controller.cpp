#include "buggy_controller.h"

#include <chrono>
#include <thread>
#include <wiringPi.h>
#include <iostream>
#include "../motors/adafruit/motorhat/adafruitmotorhat.h"
#include "../terminal/terminal.h"
#include "../config.h"

Buggy_Controller::Buggy_Controller(uint8_t motorLeftPort, uint8_t motorRightPort, void (*ultrasonicHandler)(), void (*gyroHandler)(), int8_t speed) : motors(motorLeftPort, motorRightPort, speed)
{
    wiringPiSetup();
    ultrasonic_sensor.config(ultrasonicHandler);
    gyro_sensor.init();
    gyro_sensor.setupInterrupt(GYRO_INTERRUPT_PIN, gyroHandler);

    // --- Calibrate Gyro ---
    std::cout << std::endl;
    std::cout << "Calibrating gyro..." << std::endl;
    std::cout << "Please hold your breath." << std::endl;
    while (!gyro_sensor.calibrate_drift(2s))
    {
        std::cout << "Don't move!" << std::endl;
    }
    gyro_sensor.gyroscope.x = 0;
    gyro_sensor.gyroscope.y = 0;
    gyro_sensor.gyroscope.z = 0;
    std::cout << "OK." << std::endl;
    buggy_motors->say_hello();
}

void Buggy_Controller::release(){
    motors.release();
}

void Buggy_Controller::keyboard_control()
{
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
        if(prevent_forward && c != 's') c='b';
        switch (c)
        {
        case 'A':
        case 'w':
            motors.forwards();
            motors.drive();
            break;
        case 's':
            motors.backwards();
            motors.drive();
            break;
        case 'a':
            motors.forwards();
            motors.drive_relative(90);
            break;
        case 'd':
            motors.forwards();
            motors.drive_relative(-90);
            break;
        case 'q':
            motors.rotate_relative(-90);
            break;
        case 'e':
            motors.rotate_relative(90);
            break;
        case 'c':
            std::cout << "Circumnavigate no gyro" << std::endl;
            circumnavigateNoGyro();
            break;
        case 'C':
            std::cout << "Circumnavigate with gyro" << std::endl;
            circumnavigateGyro();
            break;
        case 'r':
            std::cout << "Rectangle no gyro" << std::endl;
            rectangleNoGyro();
            break;
        case 'R':
            std::cout << "Rectangle with gyro" << std::endl;
            rectangleGyro();
            break;
        case 'o':
            std::cout << "Slalom vanilla" << std::endl;
            slalomMotors();
            break;
        case 'O':
            std::cout << "Slalom with gyrosensor" << std::endl;
            slalomGyro();
            break;
        case 'p':
            std::cout << "Slalom with ultrasonic" << std::endl;
            slalomUltrasonic();
            break;

        case 'u': 
            std::cout << "run over" << std::endl;
            runOver();
            break;

        case '+':
            if (motors.get_speed() + 5 <= 100) {
                motors.set_speed(motors.get_speed() + 5);
            }
            std::cout << "Speed: " << (int) motors.get_speed() << std::endl;
            break;

        case '-':
            if (motors.get_speed() - 5 > 0) {
                motors.set_speed(motors.get_speed() - 5);
            }
            std::cout << "Speed: " << (int) motors.get_speed() << std::endl;
            break;

        // reset gyroscope
        case 'k':
            gyro_sensor.gyroscope.z = 0;
            break;

        case 27:
            continue;
        case '[':
            continue;

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
        uint8_t stop_distance = 10 + 0.2 * motors.get_speed();
        if (ultrasonic_sensor.get_distanceresult() < stop_distance)
        {
            if(!prevent_forward) motors.brake();
            prevent_forward = true;
            ultrasonic_sensor.brake_light_on();
        }
        else
        {
            prevent_forward = false;
            ultrasonic_sensor.brake_light_off();
        }
        std::this_thread::sleep_for(20ms);
        ultrasonic_sensor.distance_measurement();
    } while (1);
}

// TODO: namings! chronometryInterrupt vs. interruptTriggered

void Buggy_Controller::ultrasonic_handling(){
    ultrasonic_sensor.chronometryInterrupt();
}

void Buggy_Controller::gyro_handling(){
    gyro_sensor.interruptTriggered();
}

void Buggy_Controller::gyro_control() {
    while(true) {
        motors.set_current_angle(Angle(gyro_sensor.gyroscope.z));
        motors.correct();
        std::this_thread::sleep_for(20ms);
    }
}


