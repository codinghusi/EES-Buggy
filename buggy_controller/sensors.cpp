#include "buggy_controller.h"


// interrupts
// TODO: namings! chronometryInterrupt vs. interruptTriggered

void Buggy_Controller::ultrasonic_handling(){
    ultrasonic_sensor.chronometryInterrupt();
}

void Buggy_Controller::gyro_handling(){
    gyro_sensor.interruptTriggered();
}

void Buggy_Controller::gyro_control() {
    while(true) {
        motors.setCurrentAngle(Angle(gyro_sensor.gyroscope.z));
        motors.correct();
        std::this_thread::sleep_for(20ms);
    }
}


