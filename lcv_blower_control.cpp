/**
 * @file lcv_blower_control.cpp
 * @brief Blower motor control for Low Cost Ventilator
 */

#include <Arduino.h>

#include <math.h>

#include "LCV.h"
#include "lcv_blower_control.h"


void enable_motor(void)
{
    digitalWrite(BLDC_ENABLE_PIN, HIGH);
}

void disable_motor(void)
{
    digitalWrite(BLDC_ENABLE_PIN, LOW);
}

void set_motor_speed_rpm(float speed_rpm)
{
    // Calculate DAC output
    // Motor has range of 500-80000 rpm up to 5V
    // We can only provide 3.3V max from the DAC

    float speed_portion = (speed_portion / 80000.0) * (5.0 / 3.3);
    set_motor_speed_portion(speed_portion);
}

void set_motor_speed_portion(float speed_portion)
{
    uint32_t raw_dac = (uint32_t) (speed_portion * 4095.0);
    analogWrite(BLDC_SPEED_CONTROL_PIN, raw_dac);
}

float get_motor_temperature_c(void)
{
    // Thermistor is B25/100 10kohm
    uint32_t raw_adc = analogRead(BLDC_NTC_PIN);

    float B = 1483.4 * log(R25 / R100);

    // TODO need params on motor
}