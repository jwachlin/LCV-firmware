/**
 * @file lcv_i2c_interface.cpp
 * @brief I2C interface for Low Cost Ventilator
 */

#include <Arduino.h>
#include "Wire.h"

#include "LCV.h"
#include "lcv_i2c_interface.h"

bool i2c_interface_init(void)
{
    Wire.begin(50000);
}

bool i2c_read_registers(uint8_t address, uint8_t register, uint8_t * data, uint32_t length, uint32_t timeout_ms)
{
    bool success = true;
    Wire.beginTransmission(address);
    Wire.write(register);
    Wire.endTransmission();

    Wire.requestFrom(address, length);
    uint32_t start_time = millis();
    while(Wire.available())
    {
        *(data++) = Wire.read(); // TODO is this blocking?
        if(millis() > start_time + timeout_ms)
        {
            success = false;
            break;
        }
    }
    return success;
}

bool i2c_write_registers(uint8_t address, uint8_t register, uint8_t * data, uint32_t length, uint32_t timeout_ms)
{
    Wire.beginTransmission(address);
    Wire.write(register);
    for(uint32_t i = 0; i < length; i++)
    {
        Wire.write(data[i]);
    }
    Wire.endTransmission();
    // TODO does anything here possibly block?
    return true;
}