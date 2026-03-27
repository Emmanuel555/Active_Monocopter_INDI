#include <Arduino.h>
#include "led_test.h"

// put function declarations here:
int myFunction(int, int);

void setup() {
  // put your setup code here, to run once:
  int result = myFunction(2, 3);
  est_led();
}

void loop() {
  // put your main code here, to run repeatedly:
  light_blink();
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}