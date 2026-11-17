#include <Arduino.h>
#include <Wire.h>
#include <VL6180X.h>
#include "tof_sensors.h"

static VL6180X sensors[TOF_NUM_SENSORS];
static bool    sensorOk[TOF_NUM_SENSORS] = {false};

// Returns true if a device ACKs at addr on Wire.
static bool i2cPresent(uint8_t addr) {
  Wire.beginTransmission(addr);
  return Wire.endTransmission() == 0;
}

// Brings up one sensor (its CE must already be HIGH and it must be the only
// device at 0x29). Returns true if it answers at its new address.
static bool initOne(uint8_t i) {
  if (!i2cPresent(TOF_DEFAULT_ADDR)) {
    return false; // nothing at 0x29: wiring / CE / power problem
  }

  sensors[i].setBus(&Wire);
  sensors[i].init();             // Pololu init() returns void, so verify below
  sensors[i].configureDefault();
  sensors[i].setAddress(TOF_BASE_ADDR + i);
  sensors[i].setTimeout(TOF_TIMEOUT_MS);

  // Confirm it moved to the new address and identifies as a VL6180X (0xB4).
  if (!i2cPresent(TOF_BASE_ADDR + i)) {
    return false;
  }
  return sensors[i].readReg(VL6180X::IDENTIFICATION__MODEL_ID) == 0xB4;
}

uint8_t initSensors() {
  Wire.begin();
  Wire.setClock(TOF_I2C_CLOCK_HZ);

  // Hold every sensor in shutdown so they don't all answer at 0x29.
  for (uint8_t i = 0; i < TOF_NUM_SENSORS; i++) {
    pinMode(TOF_CE_PINS[i], OUTPUT);
    digitalWrite(TOF_CE_PINS[i], LOW);
  }
  delay(10);

  uint8_t okCount = 0;
  for (uint8_t i = 0; i < TOF_NUM_SENSORS; i++) {
    digitalWrite(TOF_CE_PINS[i], HIGH);
    delay(2); // boot time after leaving shutdown

    sensorOk[i] = initOne(i);

    if (sensorOk[i]) {
      okCount++;
      Serial.printf("# sensor %u (CE pin %u): OK at 0x%02X\n",
                    i, TOF_CE_PINS[i], TOF_BASE_ADDR + i);
    } else {
      // Put it back in shutdown: if it is stuck at 0x29 it would otherwise
      // clash with the next sensor being brought up.
      digitalWrite(TOF_CE_PINS[i], LOW);
      Serial.printf("# sensor %u (CE pin %u): FAILED, skipping\n",
                    i, TOF_CE_PINS[i]);
    }
  }

  Serial.printf("# %u/%u sensors initialised\n", okCount, TOF_NUM_SENSORS);
  return okCount;
}

int16_t readSensor(uint8_t i) {
  if (i >= TOF_NUM_SENSORS || !sensorOk[i]) {
    return TOF_INVALID;
  }

  // Single-shot: starts one measurement and blocks until it finishes (or times out).
  uint8_t range = sensors[i].readRangeSingleMillimeters();
  if (sensors[i].timeoutOccurred()) {
    return TOF_INVALID;
  }

  // Error code is the top 4 bits of RESULT__RANGE_STATUS; 0 = valid reading.
  uint8_t status = sensors[i].readReg(VL6180X::RESULT__RANGE_STATUS) >> 4;
  if (status != 0 || range == 255) {
    return TOF_INVALID;
  }

  // Apply per-sensor calibration (see TOF_SCALE / TOF_OFFSET_MM in tof_sensors.h).
  return (int16_t)lroundf(range * TOF_SCALE[i] + TOF_OFFSET_MM[i]);
}

void printHeader() {
  Serial.print("time_ms");
  for (uint8_t i = 0; i < TOF_NUM_SENSORS; i++) {
    Serial.printf(",s%u_mm", i);
  }
  Serial.println();
}

void printSweep() {
  int16_t ranges[TOF_NUM_SENSORS];
  uint32_t t = millis(); // time at start of sweep

  // Strictly sequential: never have two sensors ranging at once.
  for (uint8_t i = 0; i < TOF_NUM_SENSORS; i++) {
    ranges[i] = readSensor(i);
  }

  Serial.print(t);
  for (uint8_t i = 0; i < TOF_NUM_SENSORS; i++) {
    Serial.print(',');
    Serial.print(ranges[i]);
  }
  Serial.println();
}

void printReadings() {
  int16_t ranges[TOF_NUM_SENSORS];
  uint32_t t = millis(); // time at start of sweep

  // Strictly sequential: never have two sensors ranging at once.
  for (uint8_t i = 0; i < TOF_NUM_SENSORS; i++) {
    ranges[i] = readSensor(i);
  }

  Serial.printf("t=%lu ms", t);
  for (uint8_t i = 0; i < TOF_NUM_SENSORS; i++) {
    Serial.printf("  s%u: %d", i, ranges[i]);
  }
  Serial.println();
}
