#pragma once

#include <stdint.h>
#include "../../angle/angle.h"
#include "../buggy_motors/buggy_motors.h"
#include <mutex>

class MotorController
{
public:
    enum class State {
        STOPPED,
        FORWARD,
        ROTATING
    };

private:
    uint8_t speed;
    bool driving_forward = true;

    Angle start_angle = 0.f;
    Angle target_angle = 0.f;
    Angle current_angle = 0.f;
    std::mutex motor_mtx;
    State state = State::STOPPED;
    State prev_state = State::STOPPED;

public:
    BuggyMotors *motors;

    /**
     * @brief Construct a new Motor Controller object
     * @param motor_left_port number of the left motor on the motor hat
    */
    MotorController(uint8_t motor_left_port, uint8_t motor_right_port, uint8_t speed);
    ~MotorController();

    /**
     * @brief Set the speed of both motors. Doesn't start the motors.
    */
    void set_speed(uint8_t speed);

    /**
     * @brief Set a temporary_speed = speed * factor
    */
    void slower(float factor);

    /**
     * @brief Set a temporary_speed = speed. get_speed() won't be affected
    */
    void set_temporary_speed(float speed);

    /**
     * @brief Reset temporary_speed to get_speed()
    */
    void reset_temporary_speed();

    /**
     * @brief Get the speed of both motors
    */
    uint8_t get_speed() { return speed; }

    /**
     * @brief Set the target angle for driving
    */
    void set_target_angle(Angle target_angle);

    /**
     * @brief Set the direction of driving to forward. Doesn't start the motors.
    */
    void forwards();

    /**
     * @brief Set the direction of driving to backward. Doesn't start the motors.
    */
    void backwards();

    /**
     * @brief Start driving to previously set target angle
    */
    void drive();

    /**
     * @brief Start driving to current_angle + target_angle
     * @param target_angle angle to drive to. Sets the target_angle = current_angle - target_angle.
    */
    void drive_relative(Angle target_angle);

    /**
     * @brief Start driving to absolute target_angle
     * @param target_angle angle to drive to. Sets the target angle.
    */
    void drive(Angle target_angle);

    /**
     * @brief Rotate to absolute target_angle around itself
    */
    void rotate(Angle target_angle);

    /**
     * @brief Rotate to relative target_angle around itself (current_angle - target_angle)
    */
    void rotate_relative(Angle target_angle);

    /**
     * @brief Stop both motors
     * @param doLock if true, the mutex will be locked. Set to false if you already locked the mutex.
    */
    void brake(bool doLock = true);

    /**
     * @brief Release the motors
    */
    void release();

    /**
     * @brief Get the current state of the motor controller
    */
    State get_state() { return state; }

    /**
     * @brief Blocks this thread until the rotation has been finished
    */
    void wait_for_rotation();

    /**
     * @return difference of target_angle - current_angle
    */
    float error_to_target_angle();

    /**
     * @brief Set the current angle. Call this oeriodically before correct() with gyro value.
    */
    void set_current_angle(Angle currentAngle);

    /**
     * @brief Adjust the motors speed to target its correct angle.
    */
    void correct();
};
