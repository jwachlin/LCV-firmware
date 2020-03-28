/**
 * @file lcv_flow_sensor.cpp
 * @brief Driver to flow sensor for Low Cost Ventilator
 */

#include <Arduino.h>
#include <string.h>

#include "LCV.h"
#include "lcv_i2c_interface.h"

#include "lcv_flow_sensor.h"

bool flow_sensor_init(void)
{
    bool success = true;
    digitalWrite(FLOW_METER_POWER_PIN, HIGH);
    delay(150);

    // Setup first read 
    uint8_t cmd = SFM3300_MEAS_CMD_BYTE_2;
    if(!i2c_write_registers(SFM3300_I2C_ADDRESS, 
    SFM3300_MEAS_CMD_BYTE_1, &cmd, 1, 20))
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
    debug_println("setup 3");
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
    uint8_t cmd = SFM3300_MEAS_CMD_BYTE_2;
    debug_println("setup 0");
    if(!i2c_write_registers(SFM3300_I2C_ADDRESS, 
    SFM3300_MEAS_CMD_BYTE_1, &cmd, 1, 20))
    {
        success = false;
    }
    delay(2);

    uint8_t data[3];
    debug_println("setup 1");
    if(!i2c_read_registers(SFM3300_I2C_ADDRESS, 0, data, 3, 20))  // TODO sends extra byte
    {
        success = false;
    }

    // TODO check CRC 
    uint8_t read_crc = data[2];

    debug_println("setup 2");
    uint32_t raw_rate = data[0] | (data[1] << 8);
    *flow_rate = (float) (raw_rate - SFM3300_OFFSET_FLOW) / SFM3300_SCALE_FACTOR_FLOW;
    return success;
}

bool read_serial_number(uint32_t * sn)
{
    bool success = true;
    uint8_t cmd = SFM3300_READ_SN_BYTE_2;
    if(!i2c_write_registers(SFM3300_I2C_ADDRESS, 
    SFM3300_READ_SN_BYTE_1, &cmd, 1, 20))
    {
        success = false;
    }
    delay(2);

    uint8_t data[6];
    if(!i2c_read_registers(SFM3300_I2C_ADDRESS, 0, &data[0], 6, 20)) // TODO sends extra byte
    {
        success = false;
    }

    // TODO check CRC 
    uint8_t read_crc_1 = data[2];
    uint8_t read_crc_2 = data[5];

    uint8_t assembled_sn[4] = {data[0], data[1], data[3], data[4]};
    memcpy(sn, assembled_sn, 4);

    return success;
}