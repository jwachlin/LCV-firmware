/**
 * @file lcv_i2c_interface.cpp
 * @brief I2C interface for Low Cost Ventilator
 */

#include <Arduino.h>
#include <Wire.h>

#include "LCV.h"
#include "lcv_i2c_interface.h"

bool i2c_interface_init(void)
{
    Wire.begin();

    //SERCOM2->I2C.CTRLA = 0;
    SERCOM2->I2CM.BAUD.bit.BAUD = 6520800;//SERCOM_FREQ_REF / ( 2 * 20000) - 1 ; // SERCOM_FREQ_REF = 48M

    return true;
}

bool i2c_read_registers(uint8_t address, uint8_t reg, 
uint8_t * data, uint32_t length, uint32_t timeout_ms)
{
    bool success = true;
    uint32_t start_time = millis();
    
    Wire.beginTransmission(address);
    Wire.write(reg);
    Wire.endTransmission();
    
    Wire.requestFrom(address, length);

    int32_t count = 0;
    while(count < length)
    {
        if(Wire.available())
        {
            *(data+count) = Wire.read();
            count++;
        }
        if( (millis()-start_time) > timeout_ms )
        {
            success = false;
            break;
        }
    }

    return success;
}

bool i2c_write_registers(uint8_t address, uint8_t reg, 
uint8_t * data, uint32_t length, uint32_t timeout_ms)
{
    bool success = true;
    uint32_t start_time = millis();
    
    Wire.beginTransmission(address);
    Wire.write(reg);
    for(int32_t i = 0; i < length; i++)
    {
        Wire.write(reg);
    }
    Wire.endTransmission();

    return success;
}