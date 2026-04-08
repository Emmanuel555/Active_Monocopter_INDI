#include <Arduino.h>
#include "led_test.h"
#include "motor_wifi_rec.h"


// put function declarations here:
int myFunction(int, int);

void setup() {
  // put your setup code here, to run once:
  int result = myFunction(2, 3);
  start_esc();
}

void loop() {
  // put your main code here, to run repeatedly:
  motor_wifi_recursion();
}
      
// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}

/* void setup() {
    Serial.begin(115200);     // USB debug
    Serial1.begin(115200);    // UART with ESP32
}

void loop() {
    if (Serial1.available()) {
        char c = Serial1.read();
        Serial1.write(c);     // echo back
    }
} */