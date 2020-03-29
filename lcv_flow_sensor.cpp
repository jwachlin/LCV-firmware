/**
 * @file lcv_flow_sensor.cpp
 * @brief Driver to flow sensor for Low Cost Ventilator
 */

#include <Arduino.h>
#include <Wire.h>
#include <string.h>

#include "LCV.h"
#include "lcv_i2c_interface.h"

#include "lcv_flow_sensor.h"

//CRC
#define POLYNOMIAL 0x131 //P(x)=x^8+x^5+x^4+1 = 100110001

bool SMF3000_CheckCrc(uint8_t * data, uint8_t nbrOfBytes, uint8_t checksum)
{
uint8_t crc = 0;
uint8_t byteCtr;
//calculates 8-Bit checksum with given polynomial
for (byteCtr = 0; byteCtr < nbrOfBytes; ++byteCtr)
{ crc ^= *(data+byteCtr);
    for (uint8_t bit = 8; bit > 0; --bit)
    { if (crc & 0x80) crc = (crc << 1) ^ POLYNOMIAL;
    else crc = (crc << 1);
    }
}
if (crc != checksum) return false;
else return true;
}

bool flow_sensor_init(void)
{
    bool success = true;
    digitalWrite(FLOW_METER_POWER_PIN, HIGH);
    delay(150);

    // Setup first read 
    uint8_t cmd = SFM3300_MEAS_CMD_BYTE_2;
    if(!i2c_write_registers(SFM3300_I2C_ADDRESS, 
        SFM3300_MEAS_CMD_BYTE_1, &cmd, 1, 10))
    {
        success = false;
    }

    delay(1);

    // First read is invalid
    float dummy;
    if(!measure_flow_slm(&dummy))
    {
        success = false;
    }
    return success;
}

void power_cycle_flow_sensor(void)
{
    digitalWrite(FLOW_METER_POWER_PIN, LOW);
    delay(50);
    digitalWrite(FLOW_METER_POWER_PIN, HIGH);
}

bool measure_flow_slm(float * flow_rate)
{
    bool success = true;

    uint8_t data_to_send[2] = {SFM3300_MEAS_CMD_BYTE_1, SFM3300_MEAS_CMD_BYTE_2};
    if(!i2c_write_data(SFM3300_I2C_ADDRESS, data_to_send, 2, 10))
    {
        success = false;
    }

    delay(2);

    uint8_t data[3];
    if(!i2c_read_data(SFM3300_I2C_ADDRESS, data, 3, 10))
    {
        success = false;
    }

    // check CRC 
    uint8_t read_crc = data[2];
    if(!SMF3000_CheckCrc(data, 2, read_crc))
    {
        success = false;
    }

    uint32_t raw_rate = data[1] | (data[0] << 8);
    *flow_rate = (float) ((int32_t) raw_rate - (int32_t) SFM3300_OFFSET_FLOW) / SFM3300_SCALE_FACTOR_FLOW;
    return success;
}

bool read_serial_number(uint32_t * sn)
{
    bool success = true;
    uint8_t data_to_send[2] = {SFM3300_READ_SN_BYTE_1, SFM3300_READ_SN_BYTE_2};
    if(!i2c_write_data(SFM3300_I2C_ADDRESS, data_to_send, 2, 10))
    {
        success = false;
    }

    delay(2);

    uint8_t data[6];
    if(!i2c_read_data(SFM3300_I2C_ADDRESS, data, 6, 10))
    {
        success = false;
    }

    // check CRC 
    uint8_t read_crc_1 = data[2];
    uint8_t read_crc_2 = data[5];
    if(!SMF3000_CheckCrc(&data[0], 2, read_crc_1))
    {
        success = false;
    }

    if(!SMF3000_CheckCrc(&data[3], 2, read_crc_2))
    {
        success = false;
    }

    uint8_t assembled_sn[4] = {data[4], data[3], data[1], data[0]};
    memcpy(sn, assembled_sn, 4);

    return success;
}