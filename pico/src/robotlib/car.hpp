#pragma once

#include "picolib/picolib.hpp" // servo/esc

constexpr uint16_t CAR_STEERING_PIN = 1;
constexpr uint16_t CAR_MOTOR_PIN = 2;

class Car {
  Servo steer;
  ESC motor;

public:
  Car() {
    steer.init(CAR_STEERING_PIN);
    motor.init(CAR_MOTOR_PIN);
  }

  ~CAR() {
    motor.stop();
  }

  void control(uint16_t steering, uint16_t throttle) {
    ;
  }
};