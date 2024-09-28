
#pragma once

#include <gciSensors.hpp>
#include <gcigps.hpp>
#include <squaternion.hpp>

#include "flags.hpp"

using namespace LSM6DSOX;
using namespace BMP390;
using namespace LIS3MDL;
// using namespace gci::sensors;
using gci::sensors::vecf_t;

// different reoccuring timers
constexpr uint32_t TIMER_1HZ   = (1 << 0);
constexpr uint32_t TIMER_5HZ   = (1 << 1);
constexpr uint32_t TIMER_10HZ  = (1 << 2);
constexpr uint32_t TIMER_25HZ  = (1 << 3);
constexpr uint32_t TIMER_50HZ  = (1 << 4);
constexpr uint32_t TIMER_100HZ = (1 << 5);

// different sensors
// TODO: group sensors that can be read together to reduce bits
//   accel/gyro
//   mag(slower is better for heading, rely on gyros for fast est?)
//   pressure/temperature
//   pos/vel/attitude (filtered)
constexpr uint32_t STATUS_AG      = (1 << 0); // accel/gyro
constexpr uint32_t STATUS_PT      = (1 << 1); // pressure/temperature
constexpr uint32_t STATUS_MAGS    = (1 << 2);
constexpr uint32_t STATUS_GPS     = (1 << 3);
constexpr uint32_t STATUS_BATTERY = (1 << 4);
constexpr uint32_t STATUS_PVA     = (1 << 5); // pos/vel/attitude (filtered)
// constexpr uint16_t STATUS_ACCELS = (1 << 1);
// constexpr uint16_t STATUS_GYROS = (1 << 2);
// constexpr uint16_t STATUS_PRESS = (1 << 6);
// constexpr uint16_t STATUS_TEMP = (1 << 7);
// constexpr uint16_t STATUS_POS = (1 << 4);
// constexpr uint16_t STATUS_VEL = (1 << 5);

// constexpr uint16_t SET_BITS(const uint16_t val, const uint16_t mask) { return
// val | mask; } constexpr uint16_t CLEAR_BITS(const uint16_t val, const
// uint16_t mask) { return val & ~mask; }

struct SharedMemory_t {
  lsm6dsox_t imu;
  lis3mdl_t mags;
  bmp390_t press_temp;
  vecf_t pos; // from KF
  vecf_t vel; // from KF
  Quaternion q; // from KF
  gga_t gps;
  float battery;

  // int32_t timer_100HZ_ms{-10};
  // int32_t timer_10HZ_ms{0};
  // int32_t timer_1HZ_ms{-1000};

  BitFlag timers;
  BitFlag sensors;
};


SharedMemory_t memory;
