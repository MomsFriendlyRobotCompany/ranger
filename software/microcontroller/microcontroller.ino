
#if defined(ARDUINO_ITSYBITSY_M0)
  constexpr int MOTOR_PIN = 8;
  constexpr int STEERING_PIN = 9;
#elif defined(ARDUINO_ITSYBITSY_M4)
  constexpr int MOTOR_PIN = 8;
  constexpr int STEERING_PIN = 9;
#elif defined(ARDUINO_QTPY_M0) // ADAFRUIT_QTPY_M0
  // constexpr int BOARD_LED_PIN           = 13;
  // constexpr int BOARD_ANALOG_BATTERY    = 15;
  // constexpr bool HAS_NEOPIXEL           = false;
  
  // SCK / A8 / D8 - Hardware SPI clock pin. 
  // Also analog/digital GPIO 8. Can act as PWM output.
  constexpr int MOTOR_PIN = 8;
  // MI / A9 / D9 -  Hardware SPI MISO microcontroller 
  // in serial out pin. Also analog/digital GPIO 9. Can 
  // act as PWM output.
  constexpr int STEERING_PIN = 9;
#endif

#include "esc.hpp"
#include <Servo.h>

ESC esc;
Servo steering;

void setup() {
  Serial.begin(1000000);
  delay(100);
  
  esc.init(MOTOR_PIN);
  steering.attach(STEERING_PIN);
  steering.write(90);
  delay(100);
}

void loop() {
  // serial ascii input
  if (Serial.available() > 0) {
    int inByte = Serial.read();
    
    if (inByte == 'f') { // add
      esc.forward();
    }
    else if (inByte == 'r') { // decrease
      esc.reverse();
    }
    else if (inByte == 's') { // stop
      esc.stop();
    }
  }

  delay(20); // 50Hz
}
