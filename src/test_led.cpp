#include <Arduino.h>

#ifndef LED_BUILTIN // Pin 13 has an LED connected on most Arduino boards:
// This means “if LED_BUILTIN has not been defined yet...”
#define LED_BUILTIN 13
#endif

// put function declarations here:
int myFunction(int, int);

void setup() {
  // put your setup code here, to run once:
  int result = myFunction(2, 3);
  pinMode(LED_BUILTIN, OUTPUT);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LED_BUILTIN, HIGH);
  delay(3000); // Wait for 3000 millisecond(s) or 3 second(s)
  digitalWrite(LED_BUILTIN, LOW);
  delay(3000); // Wait for 3000 millisecond(s) or 3 second(s)
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}