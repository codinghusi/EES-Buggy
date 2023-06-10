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
  ;

  inline void forward_left() { motor_left->run(AdafruitDCMotor::kForward); }
  inline void forward_right() { motor_right->run(AdafruitDCMotor::kBackward); }
  inline void backward_left() { motor_left->run(AdafruitDCMotor::kBackward); }
  inline void backward_right() { motor_right->run(AdafruitDCMotor::kForward); }
  inline void brake_left() { motor_left->run(AdafruitDCMotor::kBrake); }
  inline void brake_right() { motor_right->run(AdafruitDCMotor::kBrake); }
  inline float map_speed(uint8_t speed) { return map<uint8_t>(abs(speed), 0, 100, start_speed, 255); }

  public:
  BuggyMotors(uint8_t motor_left_port, uint8_t motor_right_port, uint8_t speed = 100, float turnfactor = 0.9);
  ~BuggyMotors();
  void say_hello();
  void set_speed_left(int8_t motorspeed);
  void set_speed_right(int8_t motorspeed);
  void set_speed(int8_t motorspeed);
  void set_turnfactor(float turnfactor);
  void forward();
  void backward();
  void brake();
  void rotate_left();
  void rotate_right();
  void forward_turn_left();
  void forward_turn_left(float turnfactor);
  void forward_turn_right();
  void forward_turn_right(float turnfactor);
  void backward_turn_left();
  void backward_turn_left(float turnfactor);
  void backward_turn_right();
  void backward_turn_right(float turnfactor);
  void release();
};
