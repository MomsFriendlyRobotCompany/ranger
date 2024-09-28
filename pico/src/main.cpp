/**/

#include <stdio.h>

#include "pico/multicore.h" // multicore_launch_core1()
#include "picolib/picolib.hpp"
// #include "messaging/mavlink_comm.hpp"
// #include "ins_nav/ins_nav.hpp"
// #include "sensor_funcs.hpp"

#include "defs.hpp"
// #include "led.hpp"
// #include "main_core_1.hpp"
#include "memory.hpp"
// #include "states.hpp"
#include "car.hpp"
#include "esc.hpp"

#include <gciSensors.hpp>
#include <gcigps.hpp>
#include <squaternion.hpp>
#include <yivo.hpp>

using namespace std;
using namespace LSM6DSOX;
using namespace BMP390;
using namespace LIS3MDL;
using namespace gci::sensors;
using namespace PA1010D;

gci::NEMA gps_parser;
gciLIS3MDL mag;
gciLSM6DSOX imu;
gciBMP390 bmp;
gciPA1010D gps;
// yivo::Parser yivo;

TwoWire tw;
Serial Serial0;
Serial Serial1;
ADC adc;
SPI spi;
WatchDog wdog; // not sure how useful this is

// extern SharedMemory_t memory;
// extern SystemState_t sysstate;
Mutex sm_mutex;

// Servo m0;
// Servo m1;
// Servo m2;
// Servo m3;

Car car;

bool callback_100hz(struct repeating_timer *t) {
  memory.timers += TIMER_100HZ;
  return true;
}

// bool callback_50hz(struct repeating_timer *t) {
//   memory.timers += TIMER_50HZ;
//   return true;
// }

// bool callback_10hz(struct repeating_timer *t) {
//   memory.timers += TIMER_10HZ;
//   return true;
// }

bool callback_1hz(struct repeating_timer *t) {
  memory.timers += TIMER_1HZ;
  return true;
}

int main() {
  stdio_init_all();
  wait_for_usb();

  uint8_t chip = rp2040_chip_version();
  uint8_t rom  = rp2040_rom_version();
  printf(">> Chip Ver: %zu  ROM Ver: %zu\n", chip, rom);

  if (wdog.caused_reboot()) {
    printf("*** Watchdog Rebooted ***\n");
  }

  // wdog.enable(WATCHDOG_RESET);

  uint speed = tw.init(I2C_400KHZ, I2C_PORT, i2c_sda, i2c_scl);

  printf(">> i2c instance: %u buad: %u\n", I2C_PORT, speed);
  printf(">> i2c SDA: %u SCL: %u\n", i2c_sda, i2c_scl);

  // gpio_init(LED_PIN);
  // gpio_set_dir(LED_PIN, GPIO_OUT);

  bool ok = adc.init(ADC_BATT_PIN);
  if (ok == false) printf("*** Error ADC battery ***\n");

  Serial0.init(115200, 0, UART0_TX_PIN, UART0_RX_PIN);

  while (true) {
    uint8_t err = imu.init(ACCEL_RANGE_4_G, GYRO_RANGE_2000_DPS, RATE_104_HZ);
    if (err == 0) break;
    printf("*** imu error: %d ***\n", err);
    sleep_ms(1000);
  }

  while (true) {
    int err = bmp.init(
      ODR_100_HZ,
      IIR_FILTER_COEFF_7);
    if (err == 0) break;
    printf("*** bmp error: %d ***\n", err);
    sleep_ms(1000);
  }

  while (true) {
    int err = mag.init(RANGE_4GAUSS, ODR_155HZ);
    if (err == 0) break;
    printf("*** mag error: %d ***\n", err);
    sleep_ms(1000);
  }

  // Blink LED /////////////////////
  // led_init();

  // GPS ///////////////////////////

  // gciPA1010D gps(PA_ADDR, i2c_port); // default is 0, so don't need to do this
  // char init_command[] = "$PMTK314,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29\r\n";
  // gps.write(init_command, sizeof(init_command));
  gps.write(PMTK::GGA, sizeof(PMTK::GGA));
  // Serial1.init(9600, 1, UART1_TX_PIN, UART1_RX_PIN);
  // Serial1.write(GCI_GGA, sizeof(GCI_GGA));
  // Serial1.write(GCI_UPDATE_1HZ, sizeof(GCI_UPDATE_1HZ));
  // Serial1.write(GCI_NOANTENNA, sizeof(GCI_NOANTENNA));
  // Serial1.write(GCI_BAUD_115200, sizeof(GCI_BAUD_115200));
  // uint baud = Serial1.set_baud(115200);
  // printf("/-- UART is reset to %u baud --/\n", baud);

  // Servo /////////////////////////
  // m0.init(PWM_M0);
  // m1.init(PWM_M1);
  // m2.init(PWM_M2);
  // m3.init(PWM_M3);

  // accel, gyro, mag, press
  struct repeating_timer timer_100hz;
  add_repeating_timer_ms(-10, callback_100hz, NULL, &timer_100hz);

  // gps
  struct repeating_timer timer_1hz;
  add_repeating_timer_ms(-1000, callback_1hz, NULL, &timer_1hz);

  sleep_ms(100);

  // multicore_launch_core1(main_core_1);

  while (1) {
    // if (sysstate.current_state == SS_BOOT) {
    //   // calibrate
    // }
    // if (sysstate.telemetry == true) {
    //   if (memory.timers.is_set(TIMER_100HZ)) {
    //     // handle_ins_sensors();
    //     // run_nav_filter();
    //   }
    //   if (memory.timers.is_set(TIMER_10HZ)) {
    //     // handle_battery();
    //   }
    //   if (memory.timers.is_set(TIMER_1HZ)) {
    //     handle_gps(); // maybe move to core1?
    //     // handle_health();
    //   }
    // }

    // update_state(sysstate);

    wdog.touch();
  }
}