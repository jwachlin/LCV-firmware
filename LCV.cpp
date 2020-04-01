/**
 * @file LCV.cpp
 * @brief Firmware for Low Cost Ventilator
 */

#include <Arduino.h>

#include "LCV.h"
#include "lcv_fram.h"
#include "lcv_i2c_interface.h"
#include "lcv_lcd.h"
#include "lcv_pressure_sensing.h"
#include "lcv_hmi.h"
#include "lcv_blower_control.h"
#include "lcv_alarm_monitoring.h"
#include "lcv_flow_sensor.h"

#define LOOP_RATE_HZ		(50)

static lcv_state_t state;

bool initialize_hardware(void)
{
	#ifdef LCV_DEBUG
	Serial.begin(115200);
	#endif 	

	delay(200); // Wait for sensors to boot

	// monitor setup
	motor_init();
	disable_motor();
	alarm_monitoring_init();

	debug_println("Setting up FRAM");
	if(fram_init())
	{
		debug_println("FRAM storage setup successful");
	}
	else
	{
		debug_println("FRAM storage setup failure");
	}

	debug_println("Setting up I2C");
	if(i2c_interface_init())
	{
		debug_println("I2C setup successful");
	}
	else
	{
		debug_println("I2C setup failure");
	}

	debug_println("Setting up pressure sensors");
	if(pressure_sensor_interface_init())
	{
		debug_println("Pressure sensor successful");
	}
	else
	{
		debug_println("Pressure sensor setup failure");
	}
	

	// I2C hardware must be setup
	debug_println("Setting up LCD");
	if(lcd_init())
	{
		debug_println("LCD setup success");
	}
	else
	{
		debug_println("LCD setup failure");
	}
	
	debug_println("Setting up flow sensor");
	flow_sensor_init(); // TODO detect issues with flow sensor

	uint32_t flow_sensor_sn = 0;
	if(read_serial_number(&flow_sensor_sn))
	{
		debug_print("Serial Number: ");
		debug_println(flow_sensor_sn);
	}
	else
	{
		debug_println("Serial number unavailable");
		debug_println("Check connection to pressure sensor");
	}

	debug_println("Setting up HMI");
	if(hmi_init())
	{
		debug_println("HMI setup successful");
	}
	else
	{
		debug_println("HMI setup failure");
	}

	// Initialize 
	state.enable = 0;
	state.tidal_volume_liter = 0;
	state.peep_cm_h20 = 40;
	state.pip_cm_h20 = 8;
	state.breath_per_min = 30;

	delay(3000);
}

void lcv_task(void)
{
	// TODO setup actual task
	static uint32_t last_loop_time = 0;
	while(millis() - last_loop_time < (1000/LOOP_RATE_HZ))
	{
	}

	float tidal_volume_l = 0.0;
	if(get_tidal_volume(&tidal_volume_l))
	{
		state.tidal_volume_liter = tidal_volume_l;
	}

	state.enable = is_button_start_on();
	hmi_task(&state);
	motor_task(&state);
	alarm_monitoring_task();

	last_loop_time = millis();
}

void update_settings(lcv_state_t new_settings)
{
	state.breath_per_min = new_settings.breath_per_min;
	state.peep_cm_h20 = new_settings.peep_cm_h20;
	state.pip_cm_h20 = new_settings.pip_cm_h20;
}