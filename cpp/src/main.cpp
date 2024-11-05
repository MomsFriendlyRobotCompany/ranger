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
      printf("SET_IMU: %d\n", (int)sm.imu.timestamp);
      break;
    case SET_POSE:
      printf("SET_POSE\n");
      sm.pose = ipc_unpack<pose_t>(msg);
      break;
    case SET_LIDAR:
      sm.lidar = ipc_unpack<lidar_t>(msg);
      printf("%sSET_LIDAR%s  %d\n", FG_CYAN, FG_RESET, (int)sm.lidar.timestamp);
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

  pid_t pid = getpid();
  printf(">> process[%d]\n", pid);

  // uint8_t buffer[BUFFER_SIZE];

  // defaults to 0.0.0.0
  inetaddr_t addr = inet_sockaddr(MEMORY_PORT);

  // s.open(); // no timeout
  s.open(100); // timeout
  s.bind(addr);
  uint8_t buffer[MAX_MSG_SIZE];

  while (run) {
    inetaddr_t client_addr;
    memset(buffer, 0, MAX_MSG_SIZE);
    size_t num = s.recvfrom(buffer, MAX_MSG_SIZE, &client_addr);

    if (num > 0) {
      message_t m = get_msg(buffer, MAX_MSG_SIZE);
      uint16_t id = get_id(m);
      process(id, m, client_addr);
    }
  }

  printf("Exiting ...\n");

  return 0;
}