#include <stdio.h>
#include <unistd.h>     // sleep, getpid()
#include <sys/types.h>  // pid (type int)
#include <stdlib.h> // rand
 #include<signal.h> // alarm

#include <ipc.hpp>
#include "common.h"

using namespace ipc;


volatile bool read_lidar = false;
volatile bool run = true;
// void signal_func(int sig) { read_lidar = true; }

void signal_func(int sig) {
  if (sig == SIGALRM) read_lidar = true;
  else if (sig == SIGTERM) run = false;
}

int main() {
  setbuf(stdout, NULL); // don't buffer STDOUT
  pid_t pid = getpid();
  printf("========================\n");
  printf("| Lidar\n");
  printf("| Process ID: %d\n", pid);
  printf("========================\n");
  // printf("timestamp: %d\n", (int) get_timestamp());

  inetaddr_t addr = inet_sockaddr(MEMORY_PORT); // defaults to 0.0.0.0

  SocketUDP s;
  s.open(1); // 1 msec timeout

  signal(SIGALRM, signal_func);
  ualarm(100000, 100000); // alarm initial and repeat
  signal(SIGTERM, signal_func);

  uint64_t start_time = ipc::time_us();

  while(run) {
    if (read_lidar) {
      read_lidar = false;
      lidar_t lidar;
      lidar.timestamp_us = ipc::time_us() - start_time;
      for (size_t i=99; i > -1; --i) {
        lidar.points[i].x = 123.456;
        lidar.points[i].y = -123.456;
      }
      message_t msg = ipc_pack<lidar_t>(SET_LIDAR, lidar);
      s.sendto(msg, addr);
      // sleep_ms(10);
      // printf("%d\r", (int)time);
    }
    else sleep_ms(1);
  }

  return 0;
}