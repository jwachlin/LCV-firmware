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
    // Setup I2C at <50khz for LCD

    Wire.begin();                                     // Set-up the I2C port
    sercom2.disableWIRE();                            // Disable the I2C SERCOM

    GCLK->PCHCTRL[23].bit.CHEN = 0; // 23 is SERCOM2
    while(GCLK->PCHCTRL[23].bit.CHEN != 0);
	GCLK->PCHCTRL[23].bit.GEN = 0x04;			// Generic clock generator 4
	GCLK->PCHCTRL[23].bit.CHEN = 1;
	while(GCLK->PCHCTRL[23].bit.CHEN != 1);

    // TODO what is GCLK4 actually?
    SERCOM2->I2CM.BAUD.bit.BAUD = 630;//48000000 / (50000) - 1;   // Set the I2C clock rate slow
    sercom2.enableWIRE();                             // Enable the I2C SERCOM  

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
        Wire.write(*(data+i));
    }
    Wire.endTransmission();

    return success;
}

bool i2c_read_data(uint8_t address, uint8_t * data, uint32_t length, uint32_t timeout_ms)
{
    bool success = true;
    uint32_t start_time = millis();
    Wire.requestFrom(address, length);
    int32_t count = 0;
    while(count < length)
    {
        if(Wire.available())
        {
            data[count] = Wire.read();
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

bool i2c_write_data(uint8_t address, uint8_t * data, uint32_t length, uint32_t timeout_ms)
{
    bool success = true;

    Wire.beginTransmission(address);
    for(int32_t i = 0; i < length; i++)
    {
        Wire.write(*(data+i));
    }
    Wire.endTransmission();
    return success;
}