/*
 * ESC Test
 * ----------------------------------------
 * MIT License 2021 (c) Kevin Walchko
 * I have recreated this many times, here it
 * is for Arduino (again) using an old Uno.
 */

#include <Servo.h>

Servo esc;
Servo steering;

int pos = 90;
int steer = 90;
int cnt = 0;
constexpr int incr {5};
//int incr = 5;

// Adafruit QT Py controller ----------------------------- 
// SCK / A8 / D8 - Hardware SPI clock pin. 
// Also analog/digital GPIO 8. Can act as PWM output.
constexpr int pwmPin {8}; // D8/SCK

// MI / A9 / D9 -  Hardware SPI MISO microcontroller 
// in serial out pin. Also analog/digital GPIO 9. Can 
// act as PWM output.
constexpr int steeringPin {9};

void reboot(){
    /*
     * So we need to sync up the ESC with our
     * controller. First forward full throttle
     * and then full reverse so the ESC knows
     * what our limits are.
     */
    Serial.println("forward");
    esc.write(180);
    delay(2000);

    Serial.println("reverse");
    esc.write(0);
    delay(1000);
    
    Serial.println("stop");
    esc.write(90);
}

void setup() {
    Serial.begin(9600);
    delay(100);
    
    esc.attach(pwmPin);
    delay(100);
    reboot();

    steering.attach(steeringPin);
    delay(100);
    
}

int pwmLimit(const int val){
    return val >= 180 ? 180 : val <= 0 ? 0 : val;
}
// auto pwmLimit = [](const int val){return val >= 180 ? 180 : val <= 0 ? 0 : val;};
// #define pwmLimit(val)(val >= 180 ? 180 : val <= 0 ? 0 : val)

void loop() {
    // serial ascii input
    if (Serial.available() > 0) {
        int inByte = Serial.read();
        
        if (inByte == 'a') { // add
            pos = pwmLimit(pos + incr);
        }
        else if (inByte == 'd') { // decrease
            pos = pwmLimit(pos - incr);
        }
        else if (inByte == 's') { // stop
            pos = 90;
        }
        else if (inByte == 'p') { // pwm
            // p,motor,steering
            inByte = Serial.read();
            pos = pwmLimit(inByte);
            inByte = Serial.read();
            steer = pwmLimit(inByte);
        }
        else if (inByte == 'r') { // reboot
            pos = 90;
            steer = 90;
            reboot();
            delay(1000);
        }
    }

    // debug print out
    if ((cnt++ % 25) == 0){
        Serial.println(pos);  // ascii print
        // cnt = 0;
    }
    
    esc.write(pos);
    steering.write(steer);
    delay(20); // 50Hz

}
