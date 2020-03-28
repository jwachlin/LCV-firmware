/**
 * @file lcv_pressure_sensing.cpp
 * @brief Driver for interfacing with pressure sensors
 */

#include <Arduino.h>

#include "LCV.h"
#include "lcv_pressure_sensing.h"

static bool get_single_pressure_sensor(uint8_t index, float * pressure_psi)
{
    int32_t pin = 0;
    switch (index)
    {
    case 0:
        pin = PRESSURE_0_PIN;
        break;
    case 1:
        pin = PRESSURE_1_PIN;
        break;
    case 2:
        pin = PRESSURE_2_PIN;
        break;
    
    default:
        return false;
    }

    uint32_t raw_adc = analogRead(pin);

    // Scale to section of full scale range
    float adc_portion_fsr = (raw_adc / 4095.0) * (PRESSURE_SENSOR_VOLTAGE / 3.3);

    *pressure_psi = adc_portion_fsr * PRESSURE_SENSOR_FSR_PSI;
    return true;
}

bool pressure_sensor_interface_init(void)
{
    bool success = true;
    // Set to 12-bit ADC
    analogReadResolution(12);
    // Set to default 3.3V reference
    analogReference(AR_DEFAULT);

    float pressure_values[NUM_PRESSURE_SENSORS];
    int32_t i;
    for(i = 0; i < NUM_PRESSURE_SENSORS; i++)
    {
        get_single_pressure_sensor(i, &pressure_values[i]);
    }
    float total_difference = abs(pressure_values[0] - pressure_values[1]);
    total_difference += abs(pressure_values[0] - pressure_values[2]);
    
    // TODO what is the threshold
    if(total_difference > 0.2)
    {
        success = false;
    }

    return success;
}


int32_t get_voted_pressure_psi(float * pressure_psi)
{
    float pressure_values[NUM_PRESSURE_SENSORS];
    int32_t i;
    for(i = 0; i < NUM_PRESSURE_SENSORS; i++)
    {
        get_single_pressure_sensor(i, &pressure_values[i]);
    }

    // Vote, if any 2 are within the error range, it is OK
    // TODO
}