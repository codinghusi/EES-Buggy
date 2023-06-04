#include "motorhatlib/buggy_motors.h"
#include "hcsr04.h"

class Buggy_Controller
{
private:
    Buggy_Motors* buggy_motors;
    HCSR04* ultrasonic_sensor;
    bool prevent_forward;

public:
    Buggy_Controller(void (*ultrasonicHandler)(), unsigned speed = 100);
    void keyboard_control();
    void ultrasonic_handling();
    void ultrasonic_control();
    void release();
};

void handler();
