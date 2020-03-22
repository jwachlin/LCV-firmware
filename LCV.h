/**
 * @file LCV.h
 * @brief Firmware for AR Cart 
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

// Pins for blower motor control
#define BLDC_SPEED_CONTROL_PIN						(A0)
#define BLDC_NTC_PIN								(A5)
#define BLDC_ENABLE_PIN								(9)
#define BLDC_READY_PIN								(10)

// Pins for pressure sensing
#define PRESSURE_1_PIN								(A1)
#define PRESSURE_2_PIN								(A2)
#define PRESSURE_3_PIN								(A3)

// Pins for HMI
#define CONTROL_POT_PIN								(A4)
#define CONTROL_PUSHBUTTON_PIN						(5)
#define CONTROL_START_PIN							(6)

// Pins for FRAM 
#define FRAM_CS_PIN									(13)

// Pins for flow meter 
#define FLOW_METER_POWER_PIN						(4)

// Alarm and safety control
#define WATCHDOG_PIN								(11)
#define BUZZER_PIN									(12)


bool initialize_hardware(void);

#endif