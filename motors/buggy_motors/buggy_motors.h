/**
 * Dieser Code ist im Abschlussprojekt des Moduls "Einführung in Embedded Software" entstanden.
 * Hannah Lehnen, Timur Gönül und Gerrit Weiermann
 * Gruppe 18, Slot D
*/

#pragma once
#include "../adafruit/motorhat/adafruitmotorhat.h"
#include <math.h>
#include "../../config.h"

template<typename T>
T map(T x, T in_min, T in_max, T out_min, T out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

class BuggyMotors
{
  float turnfactor;
  uint8_t motorspeed;
  AdafruitMotorHAT hat;
  std::shared_ptr<AdafruitDCMotor> motor_left, motor_right;
  uint8_t start_speed = MOTORS_SPEED_OFFSET;

  inline void forward_left() { motor_left->run(AdafruitDCMotor::kForward); }
  inline void forward_right() { motor_right->run(AdafruitDCMotor::kBackward); }
  inline void backward_left() { motor_left->run(AdafruitDCMotor::kBackward); }
  inline void backward_right() { motor_right->run(AdafruitDCMotor::kForward); }
  inline void brake_left() { motor_left->run(AdafruitDCMotor::kBrake); }
  inline void brake_right() { motor_right->run(AdafruitDCMotor::kBrake); }
  inline float map_speed(uint8_t speed) { return map<uint8_t>(abs(speed), 0, 100, start_speed, 255); }

  public:

  /**
   * @brief Construct a new Buggy Motors object
   * @param motor_left_port number of the left motor on the motor hat
   * @param motor_right_port number of the right motor on the motor hat
   * @param speed initial speed of both motors
   * @param turnfactor factor for turning speed (0.0 - 1.0)
  */
  BuggyMotors(uint8_t motor_left_port, uint8_t motor_right_port, uint8_t speed = 100, float turnfactor = 0.9);

  ~BuggyMotors();

  /**
   * @brief Wiggle (rotate) left and right
  */
  void say_hello();

  /**
   * @brief Set the speed of its left motor. Doesn't start the motor.
   * @param motorspeed speed of its left motor (0 - 100). 0 = stop, 1 = speed_offset, 100 = full speed.
  */
  void set_speed_left(int8_t motorspeed);

  /**
   * @brief Set the speed of the right motor. Doesn't start the motor.
   * @param motorspeed speed of its right motor (0 - 100). 0 = stop, 1 = speed_offset, 100 = full speed.
  */
  void set_speed_right(int8_t motorspeed);

  /**
   * @brief Set the speed of both motors. Doesn't start the motors.
   * @param motorspeed speed of both motors (0 - 100). 0 = stop, 1 = speed_offset, 100 = full speed.
  */
  void set_speed(int8_t motorspeed);

  /**
   * @brief Set the default turnfactor
   * @param turnfactor factor for turning speed (0.0 - 1.0). temporary_speed = speed * turnfactor.
  */
  void set_turnfactor(float turnfactor);

  /**
   * @brief Start driving forward with previously set speed.
  */
  void forward();

  /**
   * @brief Start driving backward with previously set speed.
  */
  void backward();

  /**
   * @brief Stop both motors.
  */
  void brake();

  /**
   * @brief Rotate left with previously set speed (backward left, forward right)
  */
  void rotate_left();
  
  /**
   * @brief Rotate right with previously set speed (forward left, backward right)
  */
  void rotate_right();

  /**
   * @brief Drive forward and turn left with previously set speed and turnfactor (forward left slowed, forward right)
  */
  void forward_turn_left();

  /**
   * @brief Drive forward and turn left with previously set speed and given turnfactor (forward left slowed, forward right)
   * @param turnfactor factor for turning speed (0.0 - 1.0). temporary_speed = speed * turnfactor.
  */
  void forward_turn_left(float turnfactor);

  /**
   * @brief Drive forward and turn left with previously set speed and turnfactor (forward left, forward right slowed)
  */
  void forward_turn_right();

  /**
   * @brief Drive forward and turn right with previously set speed and given turnfactor (forward left, forward right slowed)
   * @param turnfactor factor for turning speed (0.0 - 1.0). temporary_speed = speed * turnfactor.
  */
  void forward_turn_right(float turnfactor);

  /**
   * @brief Drive backward and turn left with previously set speed and turnfactor (backward left slowed, backward right)
  */
  void backward_turn_left();

  /**
   * @brief Drive backward and turn left with previously set speed and given turnfactor (backward left slowed, backward right)
   * @param turnfactor factor for turning speed (0.0 - 1.0). temporary_speed = speed * turnfactor.
  */
  void backward_turn_left(float turnfactor);

  /**
   * @brief Drive backward and turn right with previously set speed and turnfactor (backward left, backward right slowed)
  */
  void backward_turn_right();

  /**
   * @brief Drive backward and turn right with previously set speed and given turnfactor (backward left, backward right slowed)
   * @param turnfactor factor for turning speed (0.0 - 1.0). temporary_speed = speed * turnfactor.
  */
  void backward_turn_right(float turnfactor);

  /**
   * @brief Stop both motors. And release access.
  */
  void release();
};
