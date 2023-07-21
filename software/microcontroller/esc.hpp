/**************************************************\
* The MIT License (MIT)
* Copyright (c) 2014 Kevin Walchko
* see LICENSE for full details
\**************************************************/
#pragma once

// #include <gecko2.hpp>
#include <Servo.h>
#include <stdint.h>

/**
BLHeli 15a ESC
Features from pdf manual
- Perfect for 1806, 2204, 2205, brushless motor in QAV180, QAV 210, QAV250 and
other small drone
- Works with 2S to 4S input ‐ Rated at 15A continuous and 25A burst (5s max)
- All N‐FET design with external oscillator for steady performance across
different thermal and voltage conditions

## Timings

Supports standard 1-2 msec RC servo timings at >= 50Hz

- Minimum: 920 - 1050 usec (or 0.920 - 1.05 msec)
- Middle: 1500 usec
- Max 1800 - 2000 usec

## Power ON Sequence

1. Turn power on: 3 short rising tone beeps
2. Set middle throttle: 1 long low tone beep
3. Set zero throttle: 1 long high tone beep
4. Motor is ready to command

*/

// #ifndef BOARD_MOTOR_PINS
// constexpr uint8_t BOARD_MOTOR_PINS[4] = {1, 2, 3, 4};
// #endif

// ----- Motor PWM Levels -----
constexpr int MOTOR_MAX_LEVEL  = 2000;
constexpr int MOTOR_ZERO_LEVEL = 1000;
constexpr int MOTOR_ARM_START  = 1500;

constexpr int MOTOR_FORWARD = 1600;
constexpr int MOTOR_STOP = 1500; // FIXME: value?
constexpr int MOTOR_REVERSE = 1400;

class ESC {
public:

  ESC(): armed(false), motor_val(MOTOR_ZERO_LEVEL) {}

  ~ESC() {
    if (armed) stop();
    armed = false;
    motor.detach();
  }

  bool init(int pin) {
    motor.attach(pin, MOTOR_ZERO_LEVEL, MOTOR_MAX_LEVEL);
    return motor.attached();
  }

  void stop() { if (armed) set(MOTOR_STOP); }
  void forward() { if (armed) set(MOTOR_FORWARD); }
  void reverse() { if (armed) set(MOTOR_REVERSE); }

  /*
  * So we need to sync up the ESC with our
  * controller. First forward full throttle
  * and then full reverse so the ESC knows
  * what our limits are.
  */
  void arm() {
    motor.writeMicroseconds(MOTOR_ARM_START);
    delay(2000);
    motor.writeMicroseconds(MOTOR_ZERO_LEVEL);
    delay(1000);

    this->armed = true;
  }

  // Set the ESC for each motor to a PWM
  void set(const int m0) {
    if (!armed) return;
    motor_val = motor_limit(m0);
    motor.writeMicroseconds(motor_val);
  }

protected:
  Servo motor;
  int motor_val; // FIXME: value??
  bool armed;

  // this is just renaming constrain function with max/min
  // PWM values already in there.
  inline int motor_limit(const int val) {
    // return val >= MOTOR_MAX_LEVEL ? MOTOR_MAX_LEVEL : val <= MOTOR_ZERO_LEVEL
    // ? MOTOR_ZERO_LEVEL : val;
    return constrain(val, MOTOR_ZERO_LEVEL, MOTOR_MAX_LEVEL);
  }
};