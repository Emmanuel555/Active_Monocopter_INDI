#include <Arduino.h>
#include "led_test.h"

#ifndef LED_BUILTIN // Pin 13 has an LED connected on most Arduino boards:
// This means “if LED_BUILTIN has not been defined yet...”
#define LED_BUILTIN 13
#endif

void est_led() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);  
}

void light_blink() {
  // put your main code here, to run repeatedly:
  digitalWrite(LED_BUILTIN, HIGH);
  delay(3000); // Wait for 3000 millisecond(s) or 3 second(s)
  digitalWrite(LED_BUILTIN, LOW);
  delay(3000); // Wait for 3000 millisecond(s) or 3 second(s)
}

void light_blink_trigger(int trigger) {
  if (trigger < 1100) {
    digitalWrite(LED_BUILTIN, LOW);
  } 
  else {
    digitalWrite(LED_BUILTIN, HIGH);
  }
}
