#include <stdio.h>
#include <unistd.h>     // sleep, getpid()
#include <sys/types.h>  // pid (type int)
#include <ipc.hpp>
#include <stdlib.h> // rand
#include <signal.h> // alarm

#include "common.h"
#include "colors.hpp"
#include "serialcomm.hpp"

using namespace ipc;

volatile bool run = true;
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
  uint64_t start_time = get_timestamp();

  signal(SIGALRM, signal_func);
  ualarm(10000, 10000); // alarm initial and repeat
  signal(SIGTERM, signal_func);

  while(run) {
    if (read_imu) {
      read_imu = false;
      // read IMU
      imu_t imu;
      imu.a = vec_t{
        random(-1,1),
        random(-1,1),
        random(-1,1)
      };
      imu.g = vec_t{
        random(-.1,.1),
        random(-.1,.1),
        random(-.1,.1)
      };
      imu.m = vec_t{
        random(-10,10),
        random(-10,10),
        random(-10,10)
      };
      imu.q = quat_t{1,0,0,0};
      imu.timestamp = (uint32_t)(get_timestamp() - start_time);
      message_t msg = ipc_pack<imu_t>(SET_IMU,imu);
      s.sendto(msg, addr);
      // printf("%d\r", (int)imu.timestamp);
    }
    else sleep_ms(1000);
  }

  printf("imu oooout!!\n");

  return 0;
}