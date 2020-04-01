/**
 * @file lcv_alarm_monitoring.h
 * @brief Alarm and safety monitoring
 */

#ifndef LCV_ALARM_MONITORING_h
#define LCV_ALARM_MONITORING_h

typedef enum
{
    ALARM_FLOW_SENSOR = 0,
    ALARM_LCD = 1,
    ALARM_PRESSURE_SENSOR = 2,
    ALARM_MOTOR_ERROR = 3,
    ALARM_MOTOR_TEMP = 4,
    ALARM_SETTINGS_LOAD = 5
} ALARM_TYPE_INDEX;

void alarm_monitoring_init(void);
void alarm_monitoring_task(void);
void set_alarm(ALARM_TYPE_INDEX alarm_type, bool set);
bool check_alarm(ALARM_TYPE_INDEX alarm_type);
uint32_t get_alarm_bitfield(void);

#endif