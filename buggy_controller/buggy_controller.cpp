#include "buggy_controller.h"

#include <chrono>
#include <thread>
#include <wiringPi.h>
#include <iostream>
#include "../motors/adafruit/motorhat/adafruitmotorhat.h"
#include "../terminal/terminal.h"
#include "../config.h"

BuggyController::BuggyController(uint8_t motorLeftPort, uint8_t motorRightPort, void (*ultrasonicHandler)(), void (*gyroHandler)(), int8_t speed) : motors(motorLeftPort, motorRightPort, speed)
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

void BuggyController::release(){
    motors.release();
}

void BuggyController::keyboard_control()
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
            motors.driveRelative(90);
            break;
        case 'd':
            motors.forwards();
            motors.driveRelative(-90);
            break;
        case 'q':
            motors.rotateRelative(-90);
            break;
        case 'e':
            motors.rotateRelative(90);
            break;
        case 'c':
            std::cout << "Circumnavigate no gyro" << std::endl;
            circumnavigate_no_gyro();
            break;
        case 'C':
            std::cout << "Circumnavigate with gyro" << std::endl;
            circumnavigate_gyro();
            break;
        case 'r':
            std::cout << "Rectangle no gyro" << std::endl;
            rectangle_no_gyro();
            break;
        case 'R':
            std::cout << "Rectangle with gyro" << std::endl;
            rectangle_gyro();
            break;
        case 'o':
            std::cout << "Slalom vanilla" << std::endl;
            slalom_motors();
            break;
        case 'O':
            std::cout << "Slalom with gyrosensor" << std::endl;
            slalomGyro();
            break;
        case 'p':
            std::cout << "Slalom with ultrasonic" << std::endl;
            slalom_ultrasonic();
            break;

        case 'u': 
            std::cout << "run over" << std::endl;
            run_over();
            break;

        case '+':
            if (motors.getSpeed() + 5 <= 100) {
                motors.setSpeed(motors.getSpeed() + 5);
            }
            std::cout << "Speed: " << (int) motors.getSpeed() << std::endl;
            break;

        case '-':
            if (motors.getSpeed() - 5 > 0) {
                motors.setSpeed(motors.getSpeed() - 5);
            }
            std::cout << "Speed: " << (int) motors.getSpeed() << std::endl;
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

void BuggyController::ultrasonic_control()
{
    using namespace std::chrono_literals;
    do
    {
        if (ultrasonic_sensor.get_waitforecho())
            continue;
        uint8_t stop_distance = 10 + 0.2 * motors.getSpeed();
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

void BuggyController::ultrasonic_handling(){
    ultrasonic_sensor.chronometryInterrupt();
}

void BuggyController::gyro_handling(){
    gyro_sensor.interruptTriggered();
}

void BuggyController::gyro_control() {
    while(true) {
        motors.setCurrentAngle(Angle(gyro_sensor.gyroscope.z));
        motors.correct();
        std::this_thread::sleep_for(20ms);
    }
}


