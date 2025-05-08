#include <ipc.hpp>
#include <signal.h> // alarm
#include <stdio.h>
#include <stdlib.h>    // rand
#include <sys/types.h> // pid (type int)
#include <unistd.h>    // sleep, getpid()

#include "colors.hpp"
#include "common.h"
#include "serialcomm.hpp"

#include <common/messages.hpp>
#include <gciSensors.hpp>

using namespace ipc;
using namespace std;

using namespace LSM6DSOX;
using namespace BMP390;
using namespace sensors;

// using vec_t = gci::sensors::vec_t;
// using quat_t = gci::sensors::quat_t;
// using imu_t =  gci::sensors::imu_t;

gciLSM6DSOX accel(1);
gciBMP390 bmp(1);
sensors::compfilter_t qcf(0.01f);

volatile bool run      = true;
volatile bool read_imu = false;

float random(float min, float max) {
  return (float)rand() / RAND_MAX * (max - min) + min;
}

void signal_func(int sig) {
  if (sig == SIGALRM) read_imu = true;
  else if (sig == SIGTERM) run = false;
}

int main() {
  // https://en.cppreference.com/w/c/io/setbuf
  // If buffer is null, equivalent to setvbuf(stream, NULL, _IONBF, 0),
  // which turns off buffering.
  setbuf(stdout, NULL); // don't buffer STDOUT
  pid_t pid = getpid();
  printf("========================\n");
  printf("| IMU\n");
  printf("| Process ID: %d\n", pid);
  printf("========================\n");
  // printf("timestamp: %d\n", (int) get_timestamp());

  inetaddr_t addr = inet_sockaddr(MEMORY_PORT); // defaults to 0.0.0.0

  SerialPort sport;
  SocketUDP s;
  s.open(1); // 1 msec timeout

  while (true) {
    uint8_t err = accel.init(ACCEL_RANGE_4_G, GYRO_RANGE_2000_DPS, RATE_208_HZ);
    if (err == 0) break;
    printf("accel error %d\n", int(err));
    sensors::sleep_ms(1000);
  }

  signal(SIGALRM, signal_func);
  ualarm(10000, 10000); // alarm initial and repeat
  signal(SIGTERM, signal_func);

  uint32_t start_time = 0;

  while (run) {
    if (read_imu) {
      lsm6dsox_t i = accel.read();
      if (i.ok == false) continue;
      read_imu = false;
      if (start_time == 0) start_time = i.timestamp_us;

      // read IMU
      imu_t imu;
      imu.a = i.a;
      imu.g = i.g;
      // imu.m = i.m;
      imu.temperature  = i.temperature;
      imu.q            = quat_t{1, 0, 0, 0};
      imu.timestamp_us = i.timestamp_us - start_time;
      // imu.timestamp = i.timestamp;
      message_t msg = ipc_pack<imu_t>(SET_IMU, imu);
      s.sendto(msg, addr);
      // printf("%d\r", (int)imu.timestamp);
    }
    else sensors::sleep_ms(1000);
  }

  printf("imu oooout!!\n");

  return 0;
}