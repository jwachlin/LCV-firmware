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

	//#ifdef LCV_DEBUG
	Serial.begin(115200);
	//#endif 
	delay(1000);
	for(int i = 0; i < 3; i++)
	{
		delay(1000);
		Serial.println("boot");
	}	

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
	// TODO setup actual task, for now test sensors only
	delay(1000);
	if(measure_flow_slm(&flow_slm))
	{
		debug_print("Flow rate, slm: ");
		debug_println(flow_slm);
	}
	else
	{
		debug_println("Flow sensor read failure");
	}
	
}