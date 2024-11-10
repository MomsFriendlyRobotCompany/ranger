#include <stdio.h>
#include <unistd.h>     // sleep, getpid()
#include <sys/types.h>  // pid (type int)


// #include <gciSensors.hpp>
#include "ipc.hpp"
#include "signals.hpp"
#include "colors.hpp"
#include "common.h"

constexpr int MAX_MSG_SIZE = 6000;

using namespace ipc;

volatile bool run = true;
SocketUDP s;

struct SharedMemory_t {
  bool ok;
  pose_t pose;
  imu_t imu;
  lidar_t lidar;
} sm;

void signal_func(int i) {
  run = false;
  if (i == SIGINT) printf("\nCtrl-c main\n");
  else if (i == SIGTERM) printf("\nTerminate main\n");
  else if (i == SIGKILL) printf("\nKill main\n");
}


void process(uint16_t id, message_t& msg, inetaddr_t& client_addr) {
  message_t reply;

  switch (id) {
    case SET_IMU:
      sm.imu = ipc_unpack<imu_t>(msg);
      // printf("SET_IMU: %d\n", (int)sm.imu.timestamp);
      break;
    case SET_POSE:
      // printf("SET_POSE\n");
      sm.pose = ipc_unpack<pose_t>(msg);
      break;
    case SET_LIDAR:
      sm.lidar = ipc_unpack<lidar_t>(msg);
      // printf("%sSET_LIDAR%s  %d\n", FG_CYAN, FG_RESET, (int)sm.lidar.timestamp);
      break;
    case GET_IMU:
      reply = ipc_pack<imu_t>(GET_IMU,sm.imu);
      s.sendto(reply, client_addr);
      break;
    case GET_POSE:
      reply = ipc_pack<pose_t>(GET_POSE,sm.pose);
      s.sendto(reply, client_addr);
      break;
  }
}

int main() {
  setbuf(stdout, NULL); // don't buffer STDOUT
  printf("hello\n");

  signal(SIGINT, signal_func);
  signal(SIGTERM, signal_func);
  signal(SIGKILL, signal_func);

  Buffer_t<10> buff;
  buff.memset(22);
  Buffer_t<10> b2;
  b2 = buff;

  if (b2[5] == 22) printf("good\n");
  else printf("bad\n");

  return 0;

  pid_t pid = getpid();
  printf(">> process[%d]\n", pid);

  // uint8_t buffer[BUFFER_SIZE];

  // defaults to 0.0.0.0
  inetaddr_t addr = inet_sockaddr(MEMORY_PORT);

  // s.open(); // no timeout
  s.open(100); // timeout
  s.bind(addr);
  uint8_t buffer[MAX_MSG_SIZE];
  uint32_t loop_cnt = 0;
  uint64_t start_time = time_us();

  while (run) {
    inetaddr_t client_addr;
    memset(buffer, 0, MAX_MSG_SIZE);
    size_t num = s.recvfrom(buffer, MAX_MSG_SIZE, &client_addr);

    if (num > 0) {
      message_t m = get_msg(buffer, MAX_MSG_SIZE);
      uint16_t id = get_id(m);
      process(id, m, client_addr);

      if (loop_cnt++ == 100) {
        loop_cnt = 0;
        printf("-----------------------------\n");
        printf("Accels: %8.3f %8.3f %8.3f g\n", sm.imu.a.x, sm.imu.a.y, sm.imu.a.z);
        printf("Gyros: %8.3f %8.3f %8.3f dps\n", sm.imu.g.x, sm.imu.g.y, sm.imu.g.z);
        printf("Temperature: %5.1f C\n", sm.imu.temperature);
        printf("Timestamp: %lu usec  Time: %6.3f\n", sm.imu.timestamp_us, time_sec(start_time));
      }
    }

  }

  printf("Exiting ...\n");

  return 0;
}