#include "buggy_controller.h"

#include <chrono>
#include <thread>
#include <wiringPi.h>
#include <iostream>
#include "../motors/adafruit/motorhat/adafruitmotorhat.h"
#include "../terminal/terminal.h"
#include "../config.h"

BuggyController::BuggyController(uint8_t motor_left_port, uint8_t motor_right_port, void (*ultrasonic_handler)(), void (*gyro_handler)(), int8_t speed, uint16_t socketport) : motors(motor_left_port, motor_right_port, speed), socket(socketport)
{
    wiringPiSetup();
    ultrasonic_sensor.config(ultrasonic_handler);
    gyro_sensor.init();
    gyro_sensor.setup_interrupt(GYRO_INTERRUPT_PIN, gyro_handler);
    // gyro_sensor.standby_except_gyro_z();

    // --- Calibrate Gyro ---
    std::cout << std::endl;
    std::cout << "Calibrating gyro..." << std::endl;
    std::cout << "Please hold your breath." << std::endl;
    while (!gyro_sensor.calibrate_drift(1s, GYRO_CALIBRATION_ERROR)) 
    {
        std::cout << "Don't move!" << std::endl;
    }
    std::cout << "OK." << std::endl;
    gyro_sensor.reset_gyroscope();
    socket.create();
}

BuggyController::~BuggyController()
{
    socket.close();
}

void BuggyController::start()
{
    std::thread th1(&BuggyController::keyboard_control, &(*this));
    std::thread th2(&BuggyController::ultrasonic_control, &(*this));
    std::thread th3(&BuggyController::gyro_control, &(*this));

    th1.join();
    th2.join();
    th3.join();
}

void BuggyController::release()
{
    motors.release();
}

void BuggyController::wait_for_collision()
{
    using namespace std::chrono_literals;
    using namespace std::this_thread;
    while (!prevent_forward)
    {
        sleep_for(10ms);
    }
}

void BuggyController::execute_command(char command)
{
    // Feedback for pressed key
    if (iscntrl(command))
    {
        std::cout << (int)command << std::endl;
    }
    else
    {
        std::cout << (int)command << " ('" << (char)command << "')" << std::endl;
    }

    // Prevent forward movement if ultrasonic sensor detects an obstacle
    // Allow moving backwards
    if (prevent_forward && command != 's')
    {
        command = 'b';
    }

    // Handling key input
    switch (command)
    {
    // Basic movement WASD, Turning with Q and E
    case 'w':
        motors.forwards();
        motors.drive();
        break;

    case 's':
        motors.backwards();
        motors.drive();
        break;

    case 'a':
        motors.forwards();
        motors.drive_relative(90);
        break;

    case 'd':
        motors.forwards();
        motors.drive_relative(-90);
        break;

    case 'q':
        motors.rotate_relative(-90);
        break;

    case 'e':
        motors.rotate_relative(90);
        break;

    // Configuration
    case '+':
        if (motors.get_speed() + 5 <= 100)
        {
            motors.set_speed(motors.get_speed() + 5);
        }
        std::cout << "Speed: " << (int)motors.get_speed() << std::endl;
        break;

    case '-':
        if (motors.get_speed() - 5 > 0)
        {
            motors.set_speed(motors.get_speed() - 5);
        }
        std::cout << "Speed: " << (int)motors.get_speed() << std::endl;
        break;

    case 'k':
        gyro_sensor.reset_gyroscope();
        std::cout << "Reset gyro to 0" << std::endl;
        break;

    // Debugging
    case 'g':
        std::cout << "gyroscope.z = " << gyro_sensor.get_gyroscope().z << std::endl;
        break;

    // Routines:
    case 'c':
        std::cout << "Circumnavigate no gyro" << std::endl;
        circumnavigate_no_gyro();
        break;

    case 'C':
        std::cout << "Circumnavigate with gyro" << std::endl;
        circumnavigate_gyro();
        break;

    case 'r':
        std::cout << "Rectangle no gyro" << std::endl;
        rectangle_no_gyro();
        break;

    case 'R':
        std::cout << "Rectangle with gyro" << std::endl;
        rectangle_gyro();
        break;

    case 'o':
        std::cout << "Slalom vanilla" << std::endl;
        slalom_motors();
        break;

    case 'O':
        std::cout << "Slalom with gyrosensor" << std::endl;
        slalom_gyro();
        break;

    case 'p':
        std::cout << "Slalom with ultrasonic" << std::endl;
        slalom_ultrasonic();
        break;

    case 'u':
        std::cout << "Run over (be careful!)" << std::endl;
        run_over();
        break;

    case 'h':
        std::cout << "say hello" << std::endl;
        motors.motors->say_hello();

    default:
        std::cout << "Couldn't detect key." << std::endl;
    case 'b':
        std::cout << "Brake" << std::endl;
        motors.brake();
        break;
    }
}

void BuggyController::keyboard_control()
{
    enable_raw_mode();
    char c;
    while (read(STDIN_FILENO, &c, 1) == 1)
    {
        std::lock_guard<std::mutex> lock(execute_command_mtx);
        execute_command(c);
    }
    disable_raw_mode();
}

void BuggyController::ultrasonic_control()
{
    using namespace std::chrono_literals;
    using namespace std::this_thread;
    while (true)
    {
        if (ultrasonic_sensor.is_waiting_for_echo())
        {
            continue;
        }

        float stop_distance = 10 + 0.2 * motors.get_speed();
        if (ultrasonic_sensor.get_distance_result() < stop_distance)
        {
            if (!prevent_forward)
            {
                motors.brake();
            }
            prevent_forward = true;
            ultrasonic_sensor.brake_light_on();
        }
        else
        {
            prevent_forward = false;
            ultrasonic_sensor.brake_light_off();
        }
        sleep_for(20ms);
        ultrasonic_sensor.distance_measurement();
    }
}

void BuggyController::ultrasonic_handling()
{
    ultrasonic_sensor.chronometry_interrupt();
}

void BuggyController::gyro_handling()
{
    gyro_sensor.interrupt_triggered();
}

void BuggyController::gyro_control()
{
    using namespace std::this_thread;
    while (true)
    {
        motors.set_current_angle(Angle(gyro_sensor.get_gyroscope().z));
        motors.correct();
        sleep_for(20ms);
    }
}

void BuggyController::socket_control()
{
    char c;
    while (true)
    {
        c = socket.read();
        std::lock_guard<std::mutex> lock(execute_command_mtx);
        execute_command(c);
    }
}