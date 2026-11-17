#include <Arduino.h>
// #include "led_test.h"
// #include "servo_activation.h"
#include "tof_sensors.h"

void setup() {
  // put your setup code here, to run once:
  // start_servo_serial();
  // est_led();

  Serial.begin(115200);
  while (!Serial && millis() < 3000) {} // wait up to 3 s for the serial monitor so boot messages aren't lost

  initSensors();   // prints which sensors came up OK
}

void loop() {
  // put your main code here, to run repeatedly:
  // float pwm[5];
  // if (read_pwm_serial1(pwm)) {
  //   control_input_to_servos(pwm);
  //   Serial.printf("left hinge, right hinge, left rotor, right rotor, go/stop: %f, %f, %f, %f, %f\n", pwm[0], pwm[1], pwm[2], pwm[3], pwm[4]);
  //   light_dshot_blink_trigger();
  // }

  printReadings(); // all 5 readings on one line; use printSweep() instead for CSV
  delay(20);
}
