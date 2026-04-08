#include <Arduino.h>
#include <Servo.h>
#include "led_test.h"
#include "motor_wifi_rec.h"


Servo esc_motor;  // create servo object to control a motor (ESC)
const int MOTOR_PIN = 3; // change to your actual pin

String msg = "";

void start_serial() {
  // put your setup code here, to run once:
  delay(1000);
  Serial.begin(115200);
  delay(1000);
  Serial7.begin(115200);  // RX1=0, TX1=1
}

void start_esc() {
  // put your setup code here, to run once:
  delay(1000);
  Serial.begin(115200);
  delay(1000);
  Serial7.begin(115200);  // RX1=0, TX1=1

  //activate LED
  est_led();

  // run motor
  esc_motor.attach(MOTOR_PIN); // motor GPIO PWM pin
  esc_motor.writeMicroseconds(1000); // send min signal to arm ESC
  delay(1200); // wait for ESC to arm
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

void motor_wifi_recursion() {
  while (true) {
    if (Serial7.available()) {
          int value = Serial7.parseInt();
          
          // clamp to valid range
          value = constrain(value, 1000, 2000);

          // LED Trigger
          light_blink_trigger(value);
          
          esc_motor.writeMicroseconds(value);
          Serial7.print("Motor set to: ");
          Serial7.println(value);
      }
  }  
}
  


