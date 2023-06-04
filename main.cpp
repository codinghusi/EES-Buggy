#include <thread>
#include <chrono>
#include <signal.h>
#include <iostream>
#include "buggy_controller.h"
#include "terminal.h"

void ultrasonicHandler();

Buggy_Controller buggy(ultrasonicHandler);

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

    th1.join();
    th2.join();
    return 0;
}

void ultrasonicHandler(){
    buggy.ultrasonic_handling();
}
