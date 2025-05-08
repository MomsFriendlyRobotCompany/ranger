#include <stdio.h>
#include <sys/types.h> // pid (type int)
#include <unistd.h>    // sleep, getpid()
#include <thread>
#include <string>
#include <ipc/ipc.hpp>
#include "common.h"
#include <sensor_msgs.pb.h> // highresimu
#include <foxglove_msgs.pb.h> // lidar

constexpr int MAX_MSG_SIZE = 100;

using namespace ipc;
using namespace std;

bool run = true;
// SocketUDP sock_;

struct SharedMemory_t {
  bool ok;
  // pose_t pose;
  // imu_t imu;
  // lidar_t lidar;
} sm;

void signal_func(int i) {
  run = false;
  if (i == SIGINT) printf("\nCtrl-c main\n");
  else if (i == SIGTERM) printf("\nTerminate main\n");
  else if (i == SIGKILL) printf("\nKill main\n");
}

void lidar_thread() {
  SocketUDP sock;
  sock.open(1000);
  sock.bind(LOCAL_LIDAR_PORT);

  while (run) {
    sock.recv();
  }
}

int main() {
  setbuf(stdout, NULL); // don't buffer STDOUT
  printf("hello\n");

  signal(SIGINT, signal_func);
  // signal(SIGTERM, signal_func);
  // signal(SIGKILL, signal_func);

  // Buffer_t<10> buff;
  // buff.memset(22);
  // Buffer_t<10> b2;
  // b2 = buff;

  // if (b2[5] == 22) printf("good\n");
  // else printf("bad\n");

  // return 0;

  pid_t pid = getpid();
  printf(">> process[%d]\n", pid);

  // uint8_t buffer[BUFFER_SIZE];

  // defaults to 0.0.0.0
  inetaddr_t addr = inet_sockaddr(MEMORY_PORT);


  SocketUDP s;
  // s.open(); // no timeout
  bool ok = s.open(1); // timeout
  printf("open: %s\n", ok ? "success":"fail");
  s.bind(addr); 
  uint8_t buffer[MAX_MSG_SIZE];
  uint32_t loop_cnt   = 0;
  uint64_t start_time = time_us();

  while (run) {
    sleep_ms(1000);

    printf(".");
    inetaddr_t client_addr;
    // memset(buffer, 0, MAX_MSG_SIZE);
    if (s.recvfrom(buffer, MAX_MSG_SIZE, &client_addr) == 0) continue;
    // s.recvfrom(buffer, MAX_MSG_SIZE, &client_addr);
      // message_t m = get_msg(buffer, MAX_MSG_SIZE);
      // uint16_t id = get_id(m);
      // process(id, m, client_addr);

    // if (loop_cnt++ == 100) {
    //   loop_cnt = 0;
    //   printf("-----------------------------\n");
    //   printf("Accels: %8.3f %8.3f %8.3f g\n", sm.imu.a.x, sm.imu.a.y,
    //           sm.imu.a.z);
    //   printf("Gyros: %8.3f %8.3f %8.3f dps\n", sm.imu.g.x, sm.imu.g.y,
    //           sm.imu.g.z);
    //   printf("Temperature: %5.1f C\n", sm.imu.temperature);
    //   printf("Timestamp: %llu usec  Time: %6.3f\n", sm.imu.timestamp_us,
    //           time_sec(start_time));
    // }
  }

  printf("Exiting ...\n");

  return 0;
}