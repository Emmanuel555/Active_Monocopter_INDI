#pragma once // this prevents multiple inclusions

#include <Arduino.h>

// ---- Configuration (change these to match the wiring) ----
const uint8_t  TOF_NUM_SENSORS             = 5;
const uint8_t  TOF_CE_PINS[TOF_NUM_SENSORS] = {2, 3, 4, 5, 6}; // CE / shutdown, LOW = off
const uint8_t  TOF_DEFAULT_ADDR            = 0x29;            // power-up address of every VL6180X
const uint8_t  TOF_BASE_ADDR               = 0x30;            // sensor i -> 0x30 + i
const uint32_t TOF_I2C_CLOCK_HZ            = 400000;          // drop to 100000 if the bus is flaky
const uint16_t TOF_TIMEOUT_MS              = 100;             // per-reading timeout
const int16_t  TOF_INVALID                 = -1;              // value printed for timeout / bad reading

// ---- Per-sensor calibration: corrected_mm = raw_mm * scale + offset ----
//                                          s0    s1    s2    s3    s4
const float TOF_SCALE[TOF_NUM_SENSORS]     = {5.0f, 2.0f, 1.0f, 1.0f, 2.0f}; // s2 reads ~double, so halve it
const float TOF_OFFSET_MM[TOF_NUM_SENSORS] = {0.0f, -10.0f, -10.0f, 0.0f, 0.0f};

// Starts Wire at TOF_I2C_CLOCK_HZ, readdresses every sensor and prints which
// ones initialised. Returns the number of sensors that came up OK.
uint8_t initSensors();

// Single-shot range of sensor i in mm, or TOF_INVALID on timeout, range-status
// error, sensor not initialised, or out of range.
int16_t readSensor(uint8_t i);

// Prints the CSV header: time_ms,s0_mm,...,s4_mm
void printHeader();

// Reads every sensor one at a time and prints one CSV line.
void printSweep();

// Reads every sensor one at a time and prints all readings (mm) on one
// human-readable line, for the serial monitor. -1 = invalid.
void printReadings();
