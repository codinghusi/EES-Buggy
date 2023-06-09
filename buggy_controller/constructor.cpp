#include "buggy_controller.h"
#include <wiringPi.h>

Buggy_Controller::Buggy_Controller(void (*ultrasonicHandler)(), void (*gyroHandler)(), int8_t speed):
    motors(speed)
{
    // buggy_motors->sayHello();
    wiringPiSetup();
    ultrasonic_sensor.config(ultrasonicHandler);
    gyro_sensor.init();
    gyro_sensor.setupInterrupt(GYRO_INTERRUPT_PIN, gyroHandler);

    // --- Calibrate Gyro ---
    std::cout << std::endl;
    std::cout << "Calibrating gyro..." << std::endl;
    std::cout << "Please hold your breath." << std::endl;
    while (!gyro_sensor.calibrateDrift(2s))
    {
        std::cout << "Don't move!" << std::endl;
    }
    gyro_sensor.gyroscope.x = 0;
    gyro_sensor.gyroscope.y = 0;
    gyro_sensor.gyroscope.z = 0;
    std::cout << "OK." << std::endl;

}
