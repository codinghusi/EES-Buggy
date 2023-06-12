/**
 * Dieser Code ist im Abschlussprojekt des Moduls "Einführung in Embedded Software" entstanden.
 * Hannah Lehnen, Timur Gönül und Gerrit Weiermann
 * Gruppe 18, Slot D
*/

#include "buggy_controller.h"

void BuggyController::say_hello()
{
    motors.motors->say_hello();
}

void BuggyController::run_over()
{
    using namespace std::chrono_literals;
    using namespace std::this_thread;
    // searching for obstacle
    motors.forwards();
    motors.drive();
    wait_for_collision();

    // think about how to avoid obstacle
    motors.rotate_relative(90);
    motors.wait_for_rotation();
    sleep_for(100ms);

    // do more thinking
    motors.rotate_relative(90);
    motors.wait_for_rotation();

    motors.drive();
    sleep_for(200ms);

    motors.brake();
    sleep_for(100ms);

    // well... just run it over
    motors.backwards();
    motors.drive();
    motors.set_temporary_speed(100);
    sleep_for(1000ms);
    motors.brake();

    motors.reset_temporary_speed();    
}

void BuggyController::circumnavigate_no_gyro()
{
    using namespace std::chrono_literals;
    using namespace std::this_thread;
    
    auto rotationtime = 745ms;
    auto forwardtime = 1s;
    auto dodgetime = 2s;

    // rotate
    circumnavigate_right ? motors.motors->rotate_right() : motors.motors->rotate_left();
    sleep_for(rotationtime);
    
    // drive forward
    motors.motors->forward();
    sleep_for(forwardtime);
    
    // rotate opposite direction
    circumnavigate_right ? motors.motors->rotate_left() : motors.motors->rotate_right();
    sleep_for(rotationtime);
    
    // drive forward
    motors.motors->forward();
    sleep_for(dodgetime);
    
    // rotate opposite direction
    circumnavigate_right ? motors.motors->rotate_left() : motors.motors->rotate_right();
    sleep_for(rotationtime);
    motors.motors->forward();

    // drive forward
    sleep_for(forwardtime);
    circumnavigate_right ? motors.motors->rotate_right() : motors.motors->rotate_left();
    
    // rotate opposite direction
    sleep_for(rotationtime);
    motors.motors->brake();
    
    // next time drive slalom the other way around
    circumnavigate_right = !circumnavigate_right;
}

void BuggyController::circumnavigate_gyro()
{
    using namespace std::this_thread;
    using namespace std::chrono_literals;

    auto forwardtime = 2200ms;
    int16_t angle = circumnavigate_right ? 45 : -45;

    // absolute gyro values fix some additive errors
    gyro_sensor.reset_gyroscope();
    motors.set_current_angle(Angle(0));

    // In case it's set to backwards()
    motors.forwards();

    motors.rotate(angle);
    motors.wait_for_rotation();

    motors.drive();
    sleep_for(forwardtime);
    
    motors.rotate(-angle);
    motors.wait_for_rotation();

    motors.drive();
    sleep_for(forwardtime);
    
    motors.rotate(0);
    motors.wait_for_rotation();

    // next time drive slalom the other way around
    circumnavigate_right = !circumnavigate_right;
}

void BuggyController::rectangle_no_gyro()
{
    using namespace std::chrono_literals;
    using namespace std::this_thread;
    auto rotationtime = 745ms;
    auto forwardtime = 1500ms;

    motors.motors->forward();
    sleep_for(forwardtime);

    motors.motors->rotate_right();
    sleep_for(rotationtime);

    motors.motors->forward();
    sleep_for(forwardtime);

    motors.motors->rotate_right();
    sleep_for(rotationtime);
    
    motors.motors->forward();
    sleep_for(forwardtime);
    
    motors.motors->rotate_right();
    sleep_for(rotationtime);
    
    motors.motors->forward();
    sleep_for(forwardtime);
    
    motors.motors->rotate_right();
    sleep_for(rotationtime);
    
    motors.motors->brake();
}

void BuggyController::rectangle_gyro()
{
    using namespace std::this_thread;
    using namespace std::chrono_literals;

    gyro_sensor.reset_gyroscope();
    motors.set_current_angle(Angle(0));

    auto forwardtime = 2000ms;
    motors.forwards();

    motors.drive();
    sleep_for(forwardtime);

    motors.rotate(90);
    motors.wait_for_rotation();
        
    motors.drive();
    sleep_for(forwardtime);
    
    motors.rotate(180);
    motors.wait_for_rotation();

    motors.drive();
    sleep_for(forwardtime);

    motors.rotate(270);
    motors.wait_for_rotation();

    motors.drive();
    sleep_for(forwardtime);

    motors.rotate(0);
    motors.wait_for_rotation();
}

void BuggyController::slalom_motors()
{
    uint8_t leftspeed = 19;
    uint8_t rightspeed = 22;

    motors.motors->set_speed_left(leftspeed);
    motors.motors->set_speed_right(rightspeed);
    motors.motors->forward();
    sleep_for(200ms);
    
    circumnavigate_no_gyro();
    
    motors.motors->set_speed_left(leftspeed);
    motors.motors->set_speed_right(rightspeed);
    motors.motors->forward();
    std::this_thread::sleep_for(5000ms);
    circumnavigate_no_gyro();
}

void BuggyController::slalom_ultrasonic()
{
    // because we have no gyro stabilization
    uint8_t leftspeed = 19;
    uint8_t rightspeed = 22;

    motors.motors->set_speed_left(leftspeed);
    motors.motors->set_speed_right(rightspeed);
    motors.motors->forward();
    wait_for_collision();

    circumnavigate_no_gyro();

    motors.motors->set_speed_left(leftspeed);
    motors.motors->set_speed_right(rightspeed);
    motors.motors->forward();
    wait_for_collision();

    circumnavigate_no_gyro();
}

void BuggyController::slalom_gyro()
{
    motors.set_temporary_speed(25);
    motors.forwards();

    motors.drive();
    wait_for_collision();

    circumnavigate_gyro();
    
    motors.drive();
    wait_for_collision();

    circumnavigate_gyro();

    motors.reset_temporary_speed();
}
