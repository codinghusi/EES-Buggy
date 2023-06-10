#include <thread>
#include <chrono>
#include <signal.h>
#include <iostream>
#include "buggy_controller/buggy_controller.h"
#include "terminal/terminal.h"
#include "config.h"

void ultrasonic_handler();
void gyro_handler();

Buggy_Controller buggy(MOTOR_LEFT_PORT, MOTOR_RIGHT_PORT, ultrasonic_handler, gyro_handler, INITIAL_BUGGY_SPEED);

void signal_handler(int signum)
{
    std::cout << "Strg-C Programmende" << std::endl;
    buggy.release();
	disable_raw_mode();
    exit(signum);
}

int main()
{
    signal(SIGINT, signal_handler);

    std::thread th1(&Buggy_Controller::keyboard_control, &buggy);
    std::thread th2(&Buggy_Controller::ultrasonic_control, &buggy);
    std::thread th3(&Buggy_Controller::gyro_control, &buggy);

    th1.join();
    th2.join();
    th3.join();
    return 0;
}

void ultrasonic_handler()
{
    buggy.ultrasonic_handling();
}

void gyro_handler()
{
    buggy.gyro_handling();
}
