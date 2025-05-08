#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#if 1
  #include <pigpio.h>
#else
  // these are fake interfaces for testing on my linux desktop
  #include <thread>

using namespace std;

void gpioSetSignalFunc(int sig, void f(int)) {
  struct sigaction sigIntHandler;
  sigIntHandler.sa_handler = f;
  sigemptyset(&sigIntHandler.sa_mask);
  sigIntHandler.sa_flags = 0;
  sigaction(sig, &sigIntHandler, NULL);
}
void gpioServo(int, int) {}
void gpioTerminate() {}
void time_sleep(float msec) {
  this_thread::sleep_for(chrono::milliseconds(int(1000 * msec)));
}
int gpioInitialise() { return 0; }
#endif
/*
gcc -Wall -pthread -o servo_demo servo_demo.c -lpigpio

sudo ./servo_demo          # Send servo pulses to GPIO 4.
sudo ./servo_demo 23 24 25 # Send servo pulses to GPIO 23, 24, 25.

The PWM and servo pulses are timed using the DMA and PWM peripherals
hardware timed servo pulses on any of GPIO 0-31
ALL GPIO are identified by their Broadcom number.

http://abyz.me.uk/rpi/pigpio/cif.html#gpioServo
*/

bool run            = true;
const int NUM_GPIO  = 1;
const int MIN_WIDTH = 1000; // 1000 usec or 1 msec
const int MAX_WIDTH = 2000; // 2000 usec
const int STEP_SIZE = 10;

int width[NUM_GPIO];
int used[NUM_GPIO] = {18}; //{22,23,24,25}; // BCM pins used

// int randint(int from, int to)
// {
//    return (random() % (to - from + 1)) + from;
// }

void stop(int signum) {
  run = false;

  printf("<< caught signal: %d >>\n", signum);
}

void init() {
  gpioServo(used[0], 1000);
  time_sleep(2.0);
  gpioServo(used[0], 1500);
  time_sleep(2.0);
  gpioServo(used[0], 0);
}

int main(int argc, char *argv[]) {
  int i, g;

  if (gpioInitialise() < 0) return -1;

  gpioSetSignalFunc(SIGINT, stop);

  printf("Sending servos pulses to GPIO");
  printf(", control C to stop.\n");

  init();

  while (run) {
    for (g = 0; g < NUM_GPIO; g++) {
      gpioServo(used[g], width[g]);

      printf(">> pin %d: %d\n", used[g], width[g]);
      width[g] += STEP_SIZE;

      if ((width[g] < MIN_WIDTH) || (width[g] > 1500)) {
        printf("** pin %d reset to min\n", used[g]);
        width[g] = MIN_WIDTH;
      }
    }

    time_sleep(0.5);
  }

  printf("\n*** shutting off everything ***\n");

  for (g = 0; g < NUM_GPIO; g++) {
    gpioServo(used[g], 0);
  }

  gpioTerminate();

  return 0;
}
