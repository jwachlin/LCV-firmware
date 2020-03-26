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

}

bool i2c_read_registers(uint8_t address, uint8_t register, uint8_t * data, uint32_t length, uint32_t timeout_ms)
{

}

bool i2c_write_registers(uint8_t address, uint8_t register, uint8_t * data, uint32_t length, uint32_t timeout_ms)
{

}