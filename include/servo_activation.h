#pragma once // this prevents multiple inclusions

void start_servo_serial();
bool read_pwm_serial1(float pwm[4]);
void control_input_to_servos(float pwm[4]); 