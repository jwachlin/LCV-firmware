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

static float flow_slm = 0.0;

bool initialize_hardware(void)
{
	disable_motor();

	#ifdef LCV_DEBUG
	Serial.begin(115200);
	#endif 
	delay(1000);
	

	delay(1000); // Wait for sensors to boot

	// monitor setup
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
	
}

void lcv_task(void)
{
	static float flow_volume = 0.0;
	static float tidal_volume = 0.0;
	static float filtered_rate = 0.0;
	static float last_filtered_rate = 0.0;
	static bool rising = true;
	static uint32_t last_time = 0;

	// TODO setup actual task, for now test sensors only
	delay(10);
	if(measure_flow_slm(&flow_slm))
	{
		//debug_print("Flow rate, slm: ");
		//debug_println(flow_slm);
	}
	else
	{
		debug_println("Flow sensor read failure");
	}

	uint32_t current_time = millis();
	
	float alpha = 0.7;
	filtered_rate = (alpha)*filtered_rate + (1.0-alpha)*flow_slm;
	float dt = 0.001 * (current_time - last_time);
	flow_volume += flow_slm * (1.0/60.0) * 0.01;
	tidal_volume += abs(flow_slm) * (1.0/60.0) * 0.01 * 0.5;
	
	if(rising && filtered_rate > 0.0 && last_filtered_rate <= 0.0)
	{	
		rising = false;
	}
	else if(!rising && filtered_rate < 0.0 && last_filtered_rate >= 0.0)
	{
		if(tidal_volume > 0.1)
		{
			debug_print("Tidal volume: ");
			debug_print(tidal_volume);
			debug_println("l");
			debug_print("Net volume: ");
			debug_print(flow_volume);
			debug_println("l");
		}
		
		flow_volume = 0.0;
		tidal_volume = 0.0;
		rising = true;
	}

	last_filtered_rate = filtered_rate;
	last_time = current_time;
}