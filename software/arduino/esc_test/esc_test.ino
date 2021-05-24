/*
 * ESC Test
 * ----------------------------------------
 * MIT License 2021 (c) Kevin Walchko
 * I have recreated this many times, here it
 * is for Arduino (again) using an old Uno.
 */

#include <Servo.h>

Servo esc;
int pos = 90;
int cnt = 0;
int incr = 5;

void setup() {
    Serial.begin(9600);
    delay(100);
    
    esc.attach(9);
    delay(100);

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

void loop() {
    // serial ascii input
    if (Serial.available() > 0) {
        int inByte = Serial.read();
        if (inByte == 'a') {
            pos = pos >= 180 ? 180 : pos + incr;
        }
        else if (inByte == 'd') {
            pos = pos <= 0 ? 0 : pos - incr;
        }
        else if (inByte == 's') {
            pos = 90;
        }
    }

    // debug print out
    if ((cnt++ % 25) == 0){
        Serial.println(pos);  // ascii print
        // Serial.write(pos); // binary write byte
        // Serial.write(buf, len); // binary write array
    }
    
    esc.write(pos);
    delay(20); // 50Hz

}
