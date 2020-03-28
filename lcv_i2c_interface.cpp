/**
 * @file lcv_i2c_interface.cpp
 * @brief I2C interface for Low Cost Ventilator
 */

#include <Arduino.h>
#include <I2C_DMAC.h>

#include "LCV.h"
#include "lcv_i2c_interface.h"

bool i2c_interface_init(void)
{
    I2C.begin(50000, REG_ADDR_8BIT, PIO_SERCOM_ALT);
    return true;
}

bool i2c_read_registers(uint8_t address, uint8_t reg, 
uint8_t * data, uint32_t length, uint32_t timeout_ms)
{
    bool success = true;
    uint32_t start_time = millis();
    debug_println("setup 4");
    I2C.readBytes(address, reg, data, length);
    debug_println("setup 5");
    while(I2C.readBusy)
    {
        debug_println(".");
        if((millis() - start_time) > timeout_ms)
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
    I2C.writeBytes(address, reg, data, length);
    while(I2C.writeBusy)
    {
        if((millis() - start_time) > timeout_ms)
        {
            success = false;
            break;
        }
    }
    return success;
}