#include <Arduino.h>
#include "led_test.h"
#include "servo_activation.h"

void setup() {
  // put your setup code here, to run once:
  start_servo_serial();
  est_led();
}

void loop() {
  // put your main code here, to run repeatedly:
  float pwm[5];
  if (read_pwm_serial1(pwm)) {
    control_input_to_servos(pwm);
    Serial.printf("left hinge, right hinge, left rotor, right rotor, go/stop: %f, %f, %f, %f, %f\n", pwm[0], pwm[1], pwm[2], pwm[3], pwm[4]);
    light_dshot_blink_trigger();
  }
}  
      
