#include "buggy_controller.h"
#include <chrono>

void Buggy_Controller::ultrasonic_control()
{
    using namespace std::chrono_literals;
    do
    {
        if (ultrasonic_sensor.get_waitforecho())
            continue;
        if (ultrasonic_sensor.get_distanceresult() < 10)
        {
            if(!prevent_forward) motors.brake();
            prevent_forward = true;
            ultrasonic_sensor.bremslicht_ein();
        }
        else
        {
            prevent_forward = false;
            ultrasonic_sensor.bremslicht_aus();
        }
        std::this_thread::sleep_for(20ms);
        ultrasonic_sensor.distance_messung();
    } while (1);
}
