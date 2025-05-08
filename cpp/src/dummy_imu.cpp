#include <ipc/ipc.hpp>
#include <ipc/time.hpp>
#include <ulog/ulog.h>
#include <signal.h> // alarm
#include <stdio.h> // printf
#include <stdlib.h>    // rand
#include <sys/types.h> // pid (type int)
#include <unistd.h>    // sleep, getpid()
#include "common.h"
#include "gaussian_noise.h"
#include <sensor_msgs.pb.h>

using namespace ipc;
using namespace std;

class ImuGenerator: public GaussianNoise {
  public:
  ImuGenerator(float stddev=0.001f): GaussianNoise(stddev) {}

  sample_t get() {
    sample_t ret = {
      _distribution(_generator),
      _distribution(_generator),
      _distribution(_generator)
    };
    return ret;
  }
};

bool run      = true;
bool read_imu = false;

// #define ULOG_F_COLOR 1 << 0       // Enable color output (default=on)
// #define ULOG_F_NUMBER 1 << 1      // Enable line number output (default=off)
// #define ULOG_F_TIME 1 << 2        // Enable time output (default=on)
// #define ULOG_F_PROCESS_ID 1 << 3  // Enable process id output (default=on)
// #define ULOG_F_LEVEL 1 << 4       // Enable log level output (default=on)
// #define ULOG_F_FILE_LINE 1 << 5   // Enable file line output (default=on)
// #define ULOG_F_FUNCTION 1 << 6    // Enable function name output (default=on)

void signal_func(int sig) {
  if (sig == SIGALRM) read_imu = true;
  else if (sig == SIGTERM) run = false;
}

int main() {
  // https://en.cppreference.com/w/c/io/setbuf
  // If buffer is null, equivalent to setvbuf(stream, NULL, _IONBF, 0),
  // which turns off buffering.
  setbuf(stdout, NULL); // don't buffer STDOUT
  logger_format_set(ULOG_GLOBAL, ULOG_F_COLOR|ULOG_F_FUNCTION);

  pid_t pid = getpid();
  printf("========================\n");
  printf("| IMU\n");
  printf("| Process ID: %d\n", pid);
  printf("========================\n");
  // printf("timestamp: %d\n", (int) get_timestamp());


  signal(SIGALRM, signal_func);
  ualarm(10000, 10000); // alarm initial and repeat
  signal(SIGTERM, signal_func);

  ImuGenerator gn(0.003f);
  inetaddr_t addr = inet_sockaddr(LOCAL_IMU_PORT); // defaults to 0.0.0.0

  // SerialPort sport;
  SocketUDP s;
  s.open(100); // 1 msec timeout

  uint32_t start_time = 0;

  kevin::HighResolutionImu imu;

  size_t cnt = 0;
  uint64_t sec;
  uint32_t nanos;
  while (run) {
    if (read_imu) {
      time_ns(sec, nanos);

      // read IMU
      kevin::Header* hdr = imu.mutable_header();
      hdr->set_frame_id("laser_frame");

      kevin::Timestamp* ts = hdr->mutable_timestamp();
      ts->set_seconds(sec);
      ts->set_nanos(nanos);

      // imu_t imu;
      sample_t i = gn.get();
      kevin::Vector3* a = imu.mutable_linear_acceleration();
      a->set_x(i.x);
      a->set_y(i.y);
      a->set_z(i.z + 1.0f);

      i = gn.get();
      kevin::Vector3* m = imu.mutable_magnetic_field();
      m->set_x(i.x + 40.0f);
      m->set_y(i.y + 40.0f);
      m->set_z(i.z + 40.0f);

      i = gn.get();
      kevin::Vector3* g = imu.mutable_angular_velocity();
      g->set_x(i.x);
      g->set_y(i.y);
      g->set_z(i.z);

      kevin::Quaternion* q = imu.mutable_orientation();
      q->set_x(0.0);
      q->set_y(0.0);
      q->set_z(0.0);
      q->set_w(1.0); // Identity quaternion
      
      imu.set_temperature(25.5);
      imu.set_pressure(1013);

      LOGGER_MULTI_TOKEN(imu.pressure(),imu.temperature());
      LOGGER_TOKEN(imu.linear_acceleration().x());
      LOGGER_TOKEN(imu.linear_acceleration().y());
      LOGGER_TOKEN(imu.linear_acceleration().z());
      // LOGGER_MULTI_TOKEN(imu.m.x,imu.m.y,imu.m.z);
      
      string msg;
      imu.SerializeToString(&msg);
      LOGGER_TOKEN(msg.size());

      s.sendto((const uint8_t*)msg.data(), msg.size(), addr);
    }
    // else sensors::sleep_ms(1000);
    ::sleep(1);
  }

  printf("imu oooout!!\n");

  return 0;
}