#include <Arduino.h>
#include "wifi_rec.h"

String msg = "";

void start_serial() {
  // put your setup code here, to run once:
  delay(1000);
  Serial.begin(115200);
  delay(1000);
  Serial7.begin(115200);  // RX1=0, TX1=1
}

void wifi_recursion() {
  // put your main code here, to run repeatedly:
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

