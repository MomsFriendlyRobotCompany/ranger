#include <ipc/ipc.hpp>
#include <ulog/ulog.h>
#include <signal.h> // alarm
#include <stdio.h> // printf
#include <stdlib.h>    // rand
#include <sys/types.h> // pid (type int)
#include <unistd.h>    // sleep, getpid()
#include "common.h"
#include "gaussian_noise.h"
#include <foxglove_msgs.pb.h>
#include <math.h>

using namespace ipc;
using namespace std;


constexpr float ANGLE_NEG_120_RAD = -120.0f * M_PI / 180.0f;
constexpr float ANGLE_120_RAD = 120.0f * M_PI / 180.0f;
constexpr float ANGLE_240_RAD = 240.0f * M_PI / 180.0f;
constexpr float ANGLE_45_RAD = M_PI / 4.0f;
constexpr float ANGLE_135_RAD = 3.0f * M_PI / 4.0f;
constexpr float ANGLE_225_RAD = 5.0f * M_PI / 4.0f;
constexpr float ANGLE_315_RAD = 7.0f * M_PI / 4.0f;

constexpr size_t SCAN_SIZE{127};

class LidarGenerator: public GaussianNoise {
  float buffer[SCAN_SIZE];
  
  public:
  LidarGenerator(float stddev): GaussianNoise(stddev) {}

  void generate(float range, float width, float height) {
    memset(buffer, 0, SCAN_SIZE*sizeof(float));
    const float dangle = (2.0f * M_PI) / SCAN_SIZE;
    float angle = 0.0f;
    const float rw = width / 2.0f;
    const float rh = height / 2.0f;
    float x,y;
    for (int i=0; i < SCAN_SIZE; ++i) {
      if ((angle <= ANGLE_45_RAD) || (angle > ANGLE_315_RAD)) {
        x = rw;
        y = rw * sin(angle);
      }
      else if ((angle > ANGLE_45_RAD) && (angle <= ANGLE_135_RAD)) {
        x = rh * cos(angle);
        y = rh;
      }
      else if ((angle > ANGLE_135_RAD) && (angle <= ANGLE_225_RAD)) {
        x = -rw;
        y = rw * sin(angle);
      }
      else if ((angle > ANGLE_225_RAD) && (angle <= ANGLE_315_RAD)) {
        x = rh * cos(angle);
        y = -rh;
      }
      buffer[i] = sqrtf(x*x + y*y) + _distribution(_generator);
      angle += dangle;
    }
  }

  float get(size_t i) { return buffer[i]; }
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
  printf("| LIDAR\n");
  printf("| Process ID: %d\n", pid);
  printf("========================\n");
  // printf("timestamp: %d\n", (int) get_timestamp());


  signal(SIGALRM, signal_func);
  ualarm(10000, 10000); // alarm initial and repeat
  signal(SIGTERM, signal_func);

  LidarGenerator lidar(3.0f);
  inetaddr_t addr = inet_sockaddr(LOCAL_LIDAR_PORT); // defaults to 0.0.0.0

  // SerialPort sport;
  SocketUDP s;
  s.open(100); // 1 msec timeout

  uint32_t start_time = 0;

  size_t cnt = 0;
  
  foxglove::LaserScan laser_scan;
  laser_scan.set_start_angle(0.0);
  laser_scan.set_end_angle(2 * M_PI);

  while (run) {
    if (read_imu) {
      // const float range = 5000;
      // const float width = 3000;
      // const float height = 4000;

      // laser_scan_t msg

      // Set timestamp (current time)
      google::protobuf::Timestamp* timestamp = laser_scan.mutable_timestamp();
      auto now = std::chrono::system_clock::now();
      auto seconds = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();
      auto nanos = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch() % std::chrono::seconds(1)).count();
      timestamp->set_seconds(seconds);
      timestamp->set_nanos(nanos);

      // Set frame_id
      laser_scan.set_frame_id("laser_frame");

      // Set pose (origin at (0, 0, 0), no rotation)
      foxglove::Pose* pose = laser_scan.mutable_pose();
      pose->mutable_position()->set_x(0.0);
      pose->mutable_position()->set_y(0.0);
      pose->mutable_position()->set_z(0.0);
      pose->mutable_orientation()->set_x(0.0);
      pose->mutable_orientation()->set_y(0.0);
      pose->mutable_orientation()->set_z(0.0);
      pose->mutable_orientation()->set_w(1.0); // Identity quaternion

      laser_scan.clear_ranges();
      lidar.generate(5000, 3000, 4000);
      for (size_t i=0; i < SCAN_SIZE; ++i) {
        float range = lidar.get(i);
        laser_scan.add_ranges(range);
      }


      LOGGER_MULTI_TOKEN(laser_scan.start_angle(), laser_scan.end_angle());
      LOGGER_TOKEN(laser_scan.ranges(50));

      string msg;
      laser_scan.SerializeToString(&msg);
      LOGGER_TOKEN(msg.size());

      s.sendto((const uint8_t*)&msg, msg.size(), addr);
    }
    // else sensors::sleep_ms(1000);
    ::sleep(1);
  }

  printf("imu oooout!!\n");

  return 0;
}