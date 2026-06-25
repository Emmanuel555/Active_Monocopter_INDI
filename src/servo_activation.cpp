#include <Arduino.h>
#include <Servo.h>
#include <string.h>
#include "led_test.h"
#include "servo_activation.h"

Servo left_hinge, right_hinge, left_rotor, right_rotor;

void start_servo_serial() {
  // put your setup code here, to run once:
  delay(500);
  Serial.begin(115200);
  delay(500);
  Serial1.begin(115200);  // RX1=0, TX1=1
  delay(500);

  // servos attached
  left_hinge.attach(3);
  right_hinge.attach(4);
  left_rotor.attach(5, 900, 2100);
  right_rotor.attach(6, 900, 2100);

  //servos initialized
  left_hinge.write(90);
  right_hinge.write(90);
  left_rotor.writeMicroseconds(1500);
  right_rotor.writeMicroseconds(1500);

}

// Non-blocking read of a framed packet from the ESP32: [0xAA sync][5 little-endian floats].
// pwm[4] is the stop flag: nonzero forces all servos/rotors to their neutral/safe position.
// Drains the entire Serial1 buffer on every call so that if multiple packets have queued up
// (e.g. the sender outpaces loop()), only the newest one is kept and stale ones are discarded.
// Call every loop() iteration. Returns true and fills pwm[5] once a full packet has arrived.
bool read_pwm_serial1(float pwm[5]) {
  static const uint8_t SYNC_BYTE = 0xAA;
  static bool in_packet = false;
  static uint8_t buf[5 * sizeof(float)];
  static uint8_t idx = 0;

  bool got_packet = false;

  while (Serial1.available()) {
    uint8_t b = Serial1.read();

    if (!in_packet) {
      if (b == SYNC_BYTE) {
        in_packet = true;
        idx = 0;
      }
      continue;
    }

    buf[idx++] = b;
    if (idx == sizeof(buf)) {
      memcpy(pwm, buf, sizeof(buf));
      in_packet = false;
      got_packet = true;
    }
  }
  return got_packet;
}

// pwm[0]=left_hinge, pwm[1]=right_hinge (degrees, 0-180); pwm[2]=left_rotor, pwm[3]=right_rotor (us, 900-2100).
// pwm[4]=stop flag: nonzero forces hinges to 90 deg and rotors to 1500us, ignoring pwm[0..3].
void control_input_to_servos(float pwm[5]) {
  if (pwm[4] != 0.0f) {
    left_hinge.write(90);
    right_hinge.write(90);
    left_rotor.writeMicroseconds(1500);
    right_rotor.writeMicroseconds(1500);
    return;
  }
  left_hinge.write(constrain((int)pwm[0], 0, 180));
  right_hinge.write(constrain((int)pwm[1], 0, 180));
  left_rotor.writeMicroseconds(constrain((int)pwm[2], 900, 2100));
  right_rotor.writeMicroseconds(constrain((int)pwm[3], 900, 2100));
}








