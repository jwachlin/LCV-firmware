/**
 * @file lcv_pressure_sensing.h
 * @brief Driver for interfacing with pressure sensors
 */

#ifndef LCV_PRESSURE_SENSING_h
#define LCV_PRESSURE_SENSING_h

#define PRESSURE_SENSOR_VOLTAGE         (5.0)
#define PRESSURE_SENSOR_FSR_PSI         (5.0)

bool pressure_sensor_interface_init(void);
int32_t get_voted_pressure_psi(float * pressure_psi);

#endif