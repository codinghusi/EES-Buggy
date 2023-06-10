#include <thread>
#include <chrono>
#include <signal.h>
#include <iostream>
#include "buggy_controller/buggy_controller.h"
#include "terminal/terminal.h"
#include "config.h"

void ultrasonicHandler();
void gyroHandler();

Buggy_Controller buggy(motorLeftPort, motorRightPort, ultrasonicHandler, gyroHandler, 50);

void signalHandler(int signum)
{
    std::cout << "Strg-C Programmende" << std::endl;
    buggy.release();
	disableRawMode();
    exit(signum);
}

int main()
{
  	signal(SIGINT, signalHandler);

    std::thread th1(&Buggy_Controller::keyboard_control, &buggy);
    std::thread th2(&Buggy_Controller::ultrasonic_control, &buggy);
    std::thread th3(&Buggy_Controller::gyro_control, &buggy);

    th1.join();
    th2.join();
    th3.join();
    return 0;
}

void ultrasonicHandler(){
    buggy.ultrasonic_handling();
}

void gyroHandler() {
    buggy.gyro_handling();
}
