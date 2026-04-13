#include <Arduino.h>
#include <Servo.h>
#include "led_test.h"
#include "motor_wifi_rec.h"
#include <DShot.h>


Servo esc_motor;  // for PWM
DShot esc_dshot(&Serial3, DShotType::DShot600); // for DShot
const int MOTOR_PIN = 3; // change to your actual pin
String msg = "";

void start_serial() {
  // put your setup code here, to run once:
  delay(500);
  Serial.begin(115200);
  delay(500);
  Serial7.begin(115200);  // RX1=0, TX1=1
}

void start_esc() {
  // put your setup code here, to run once:
  delay(500);
  Serial.begin(115200);
  delay(500);
  Serial7.begin(115200);  // RX7, TX7

  //activate LED
  est_led();

  // run motor
  esc_motor.attach(MOTOR_PIN); // motor GPIO PWM pin
  esc_motor.writeMicroseconds(1000); // send min signal to arm ESC
  delay(1200); // wait for ESC to arm
}

void start_dshot_esc() {
  // put your setup code here, to run once:
  delay(500);
  Serial.begin(115200);
  delay(500);
  Serial7.begin(115200);  // RX7, TX7

  //activate LED
  est_led();

  // run motor
  // arm ESC for 2 seconds
  unsigned long start = millis(); // start time in milliseconds
  while (millis() - start < 2000) {
      esc_dshot.sendThrottle(0, false);
      delayMicroseconds(500);
  }

  Serial.println("ESC armed and ready");
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

void dshot_changeDirection(bool reverse, int throttle) {
    // stop motor first
    unsigned long start = millis();
    while (millis() - start < 500) {
        esc_dshot.sendThrottle(0, false);
        delayMicroseconds(500);
    }

    // set direction by sending appropriate throttle range
    // 1048 is stop/neutral
    // 1049-2047 is forward
    // 1-1047 is reverse
    // clamp based on direction
    if (reverse) {
        throttle = throttle * 1047; // invert throttle for reverse
        throttle = constrain(throttle, 1, 1047);    // reverse range
    } 
    else {
        throttle = (throttle * 998) + 1049; // forward throttle range
        throttle = constrain(throttle, 1049, 2047); // forward range
    }

    esc_dshot.sendThrottle(throttle, false);
    //Serial.printf("Direction %s, speed: %d\n", reverse ? "REVERSED" : "FORWARD", lastThrottle);
    
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

void dshot_motor_wifi_recursion() {
  while (true) {
      if (Serial7.available()) {
            String msg = Serial7.readStringUntil('\n');
            msg.trim();
            
            int commaIndex = msg.indexOf(',');
            if (commaIndex != -1) {
                int direction = msg.substring(0, commaIndex).toInt();
                int value = msg.substring(commaIndex + 1).toInt();

                // LED Trigger
                light_blink_trigger(value);

                // keep sending throttle to keep motor running
                dshot_changeDirection(direction, value); // direction: 1 for reverse, 0 for forward
                          
                Serial7.printf("Direction: %d, Throttle: %d\n", direction, value);
            }
        } 
        delayMicroseconds(500);   
      }
}  


