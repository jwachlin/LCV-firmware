/**
 * @file lcv_flow_sensor.h
 * @brief Driver to flow sensor for Low Cost Ventilator
 */

#ifndef LCV_FLOW_SENSOR_h
#define LCV_FLOW_SENSOR_h

#define SFM3300_I2C_ADDRESS             (0x40)

#define SFM3300_MEAS_CMD_BYTE_1         (0x10)
#define SFM3300_MEAS_CMD_BYTE_2         (0x00)
#define SFM3300_READ_SN_BYTE_1          (0x31)
#define SFM3300_READ_SN_BYTE_2          (0xAE)
#define SFM3300_SOFT_RESET_BYTE_1       (0x20)
#define SFM3300_SOFT_RESET_BYTE_2       (0x00)

#define SFM3300_OFFSET_FLOW             (32768)
#define SFM3300_SCALE_FACTOR_FLOW       (120)       // in 1/slm

bool flow_sensor_init(void);
void power_cycle_flow_sensor(void);
bool measure_flow_slm(float * flow_rate);
bool read_serial_number(uint32_t * sn);

#endif