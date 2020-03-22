/**
 * @file LCV.h
 * @brief Firmware for Low Cost Ventilator
 */

#ifndef LCV_h
#define LCV_h

#include <stdint.h>
#include <Arduino.h>

#define LCV_DEBUG		(1)

#ifdef LCV_DEBUG
#define debug_print(x)			Serial.print((x))
#else
#define debug_print(x)
#endif

#ifdef LCV_DEBUG
#define debug_println(x)			Serial.println((x))
#else
#define debug_println(x)
#endif

// Blower motor control
#define BLDC_SPEED_CONTROL_PIN						(A0)
#define BLDC_NTC_PIN								(A5)
#define BLDC_ENABLE_PIN								(9)
#define BLDC_READY_PIN								(10)

// Pressure sensing
#define NUM_PRESSURE_SENSORS						(3)
#define PRESSURE_ACCURACY							(0.04)
#define PRESSURE_0_PIN								(A1)
#define PRESSURE_1_PIN								(A2)
#define PRESSURE_2_PIN								(A3)

// Human-machine interface
#define CONTROL_POT_PIN								(A4)
#define CONTROL_PUSHBUTTON_PIN						(5)
#define CONTROL_START_PIN							(6)

// FRAM high-speed non-volatile storage
#define FRAM_CS_PIN									(13)

// Mass flow meter 
#define FLOW_METER_POWER_PIN						(4)

// Alarm and safety control
#define WATCHDOG_PIN								(11)
#define BUZZER_PIN									(12)


bool initialize_hardware(void);
void lcv_task(void);

#endif