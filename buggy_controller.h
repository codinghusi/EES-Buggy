#include "motorhatlib/motor_controller.h"
#include "hcsr04/hcsr04.h"
#include "helper/pid/pid.h"
#include "mpu6050/mpu6050.h"

#define GYRO_INTERRUPT_PIN 22


class Buggy_Controller
{
private:
    MotorController motors;
    HCSR04 ultrasonic_sensor;
    MPU6050 gyro_sensor;
    PID correction{1, 1, 1, -1000, 1000};
    float target_angle = 0;
    float motor_relation = 1.f;
    bool prevent_forward;

public:
    Buggy_Controller(void (*ultrasonicHandler)(), void (*gyroHandler)(), unsigned speed = 100);
    void keyboard_control();
    void ultrasonic_handling();
    void ultrasonic_control();
    void gyro_handling();
    void gyro_control();
    void release();
};

void handler();
