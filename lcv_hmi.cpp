/**
 * @file lcv_hmi.cpp
 * @brief Human-machine interface for Low Cost Ventilator
 */

#include <Arduino.h>
#include <string.h>

#include "LCV.h"
#include "lcv_lcd.h"
#include "lcv_hmi.h"

bool hmi_init(void)
{
    pinMode(CONTROL_PUSHBUTTON_PIN, INPUT_PULLDOWN);
    pinMode(CONTROL_START_PIN, INPUT_PULLDOWN);
    return true;
}

float read_portion_knob(void)
{
    uint32_t raw_adc = analogRead(CONTROL_POT_PIN);

    float portion_range = (raw_adc / 4095.0);
    return portion_range;
}

bool is_button_start_on(void)
{
    return (digitalRead(CONTROL_START_PIN) == HIGH);
}

bool is_pushbotton_pressed(void)
{
    return (digitalRead(CONTROL_PUSHBUTTON_PIN) == HIGH);
}

bool display_status(lcv_settings_t * settings)
{
    static char buffer[SCREEN_BUFFER_SIZE];
    static char last_buffer[SCREEN_BUFFER_SIZE];
    static bool first_cycle = true;

    for(int32_t i = 0; i < SCREEN_BUFFER_SIZE; i++)
    {
        buffer[i] = 0x20; // ASCII space
    }

    // Update info
    // TODO doesn't seem to support floats
    if(settings->enable)
    {
        sprintf(&buffer[0], "VENT: ON");
    }
    else
    {
        sprintf(&buffer[0], "VENT: OFF");
    }

    uint32_t tidal_volume_ml = 1000.0 * settings->tidal_volume_liter;
    sprintf(&buffer[10], "V: %i ml", tidal_volume_ml);

    sprintf(&buffer[20], "PEEP: %i cmH20", settings->peep_cm_h20);

    sprintf(&buffer[40], "PIP: %i cmH20", settings->pip_cm_h20);

    // Clear any trailing 0s from string creation as those are special characters on the LCD
    for(int32_t i = 0; i < SCREEN_BUFFER_SIZE; i++)
    {
        if(buffer[i] < 0x07)
        {
            buffer[i] = 0x20; // ASCII space
        }
    }

    if(first_cycle)
    {
        set_string(1,1,&buffer[0],SCREEN_BUFFER_SIZE);
    }

    // Only update things that changed for reduced overhead
    for(int32_t i = 0; i < SCREEN_BUFFER_SIZE; i++)
    {
        if(last_buffer[i] != buffer[i])
        {
            set_character_index(i+1, &buffer[i]);
        }
    }
    set_character(1,1,"V");

    for(int32_t i = 0; i < SCREEN_BUFFER_SIZE; i++)
    {
        last_buffer[i] = buffer[i];
    }

    first_cycle = false;
}