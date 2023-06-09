#include "../buggy_controller.h"

void Buggy_Controller::runOver()
{
    using namespace std::chrono_literals;
    motors.motors->backward();
    std::this_thread::sleep_for(200ms);
    motors.motors->forward();
    std::this_thread::sleep_for(1s);
    motors.motors->brake();
}
