/**
 * Dieser Code ist im Abschlussprojekt des Moduls "Einführung in Embedded Software" entstanden.
 * Hannah Lehnen, Timur Gönül und Gerrit Weiermann
 * Gruppe 18, Slot D
*/

#include <signal.h>
#include <iostream>
#include "buggy_controller/buggy_controller.h"
#include "terminal/terminal.h"
#include "config.h"

void ultrasonic_handler();
void gyro_handler();

BuggyController buggy(MOTOR_LEFT_PORT, MOTOR_RIGHT_PORT, ultrasonic_handler, gyro_handler, INITIAL_BUGGY_SPEED);

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
    buggy.start();
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
