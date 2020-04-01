/**
 * @file lcv_alarm_monitoring.cpp
 * @brief Alarm and safety monitoring
 */

#include <Arduino.h>

#include "LCV.h"
#include "lcv_alarm_monitoring.h"

static volatile uint32_t alarm_bitfield = 0;

void alarm_monitoring_init(void)
{
    pinMode(BUZZER_PIN, OUTPUT);
    digitalWrite(BUZZER_PIN, LOW);
    alarm_bitfield = 0;
}

void alarm_monitoring_task(void)
{
    if(get_alarm_bitfield() != 0)
    {
        // set alarm light, buzzer
        digitalWrite(BUZZER_PIN, HIGH);
    }
    else
    {
        digitalWrite(BUZZER_PIN, LOW);
    }
}

void set_alarm(ALARM_TYPE_INDEX alarm_type, bool set)
{
    if((uint32_t) alarm_type > 31)
    {
        return;
    }

    if(set)
    {
        alarm_bitfield |= (1 << (uint32_t) alarm_type);
    }
    else
    {
        alarm_bitfield &= ~(1 << (uint32_t) alarm_type);
    }
    
}

bool check_alarm(ALARM_TYPE_INDEX alarm_type)
{
    if((uint32_t) alarm_type > 31)
    {
        return false;
    }

    return (alarm_bitfield & (1 << (uint32_t) alarm_type));
}

uint32_t get_alarm_bitfield(void)
{
    return alarm_bitfield;
}
