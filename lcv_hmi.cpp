/**
 * @file lcv_hmi.cpp
 * @brief Human-machine interface for Low Cost Ventilator
 */

#include <Arduino.h>

#include "LCV.h"
#include "lcv_hmi.h"

float read_portion_knob(void)
{
    uint32_t raw_adc = analogRead(CONTROL_POT_PIN);

    float portion_range = (raw_adc / 4095.0);
    return portion_range;
}