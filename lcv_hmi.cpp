/**
 * @file lcv_hmi.cpp
 * @brief Human-machine interface for Low Cost Ventilator
 */

#include <Arduino.h>

#include "LCV.h"
#include "lcv_hmi.h"

bool hmi_init(void)
{
    pinMode(CONTROL_PUSHBUTTON_PIN, INPUT_PULLDOWN);
    pinMode(CONTROL_START_PIN, INPUT_PULLDOWN);
}

float read_portion_knob(void)
{
    uint32_t raw_adc = analogRead(CONTROL_POT_PIN);

    float portion_range = (raw_adc / 4095.0);
    return portion_range;
}

bool is_button_start_on(void)
{
    return (digitalRead(CONTROL_START_PIN) == HIGH);
}

bool is_pushbotton_pressed(void)
{
    return (digitalRead(CONTROL_PUSHBUTTON_PIN) == HIGH);