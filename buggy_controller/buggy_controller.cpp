#include "buggy_controller.h"

#include <chrono>
#include <thread>
#include <wiringPi.h>
#include <iostream>
#include "../motors/adafruit/motorhat/adafruitmotorhat.h"

void Buggy_Controller::release(){
    motors.release();
}
