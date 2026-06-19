#include <Arduino.h>
#include <Servo.h>
#include "led_test.h"
#include "motor_rec.h"
#include <DShot.h>


void start_serial() {
  // put your setup code here, to run once:
  delay(500);
  Serial.begin(115200);
  delay(500);
  Serial1.begin(115200);  // RX1=0, TX1=1
}




