#include <Arduino.h>
#include "led_test.h"

// put function declarations here:
int myFunction(int, int);
String msg = "";

void setup() {
  // put your setup code here, to run once:
  int result = myFunction(2, 3);
  est_led();
  delay(1000);
  Serial.begin(115200);
  delay(1000);
  Serial7.begin(115200);  // RX1=0, TX1=1
}

void loop() {
  // put your main code here, to run repeatedly:
  light_blink();
  while (Serial7.available()) {
    char c = Serial7.read(); // read a byte for characters 

    if (c == '\n') {
      //Serial.println(msg);   // print full message
      Serial7.println(msg);  // echo back full message
      msg = "";              // reset buffer
    } 
    else {
      msg += c;              // build string - formatted text
    }

    //Serial.write(c);   // echo to USB serial
    //Serial7.write(c);  // echo back
  }
  
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