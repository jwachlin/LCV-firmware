/**
 * @file lcv_hmi.cpp
 * @brief Human-machine interface for Low Cost Ventilator
 */

#include <Arduino.h>
#include <string.h>

#include "LCV.h"
#include "lcv_lcd.h"
#include "lcv_hmi.h"

static char buffer[SCREEN_BUFFER_SIZE];
static char last_buffer[SCREEN_BUFFER_SIZE];

static uint8_t screen_buffer_reformatted[SCREEN_BUFFER_SIZE];

static SETTINGS_INPUT_STAGE stage = STAGE_NONE;
static lcv_settings_t settings_input;

static const lcv_settings_t lower_settings_range = {.enable = 0, .tidal_volume_liter = 0.1,
                    .peep_cm_h20 = 10, .pip_cm_h20 = 3, .breath_per_min = 6};

static const lcv_settings_t upper_settings_range = {.enable = 0, .tidal_volume_liter = 2.5,
                    .peep_cm_h20 = 60, .pip_cm_h20 = 30, .breath_per_min = 60};

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
    
    static bool first_cycle = true;

    for(int32_t i = 0; i < SCREEN_BUFFER_SIZE; i++)
    {
        buffer[i] = 0x20; // ASCII space
    }

    // Update info
    // TODO doesn't seem to support floats
    if(settings->enable)
    {
        sprintf(&buffer[0], "VENT:ON");
    }
    else
    {
        sprintf(&buffer[0], "VENT:OFF");
    }

    uint32_t tidal_volume_ml = 1000.0 * settings->tidal_volume_liter;
    sprintf(&buffer[10], "V:%iml", tidal_volume_ml);

    sprintf(&buffer[20], "PEEP:%icmH20", settings->peep_cm_h20);

    sprintf(&buffer[40], "PIP:%icmH20", settings->pip_cm_h20);

    sprintf(&buffer[52], "BPM:%i", settings->breath_per_min);

    // Fill in settings input display
    switch (stage)
    {
    case STAGE_NONE:
        break;

    case STAGE_BPM:
        sprintf(&buffer[60], "SET BPM:%i", settings_input.breath_per_min);
        break;

    case STAGE_PEEP:
        sprintf(&buffer[60], "SET PEEP:%icmH20", settings_input.peep_cm_h20);
        break;

    case STAGE_PIP:
        sprintf(&buffer[60], "SET PIP:%icmH20", settings_input.pip_cm_h20);
        break;
        
    default:
        break;
    }

    // Clear any trailing 0s from string creation as those are special characters on the LCD
    for(int32_t i = 0; i < SCREEN_BUFFER_SIZE; i++)
    {
        if(buffer[i] < 0x07)
        {
            buffer[i] = 0x20; // ASCII space
        }
    }

    update_full_screen_fast();
    /*

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

    first_cycle = false;*/
}

void handle_hmi_input(void)
{
    static bool last_button_status = false;
    // Check for stage change 
    bool new_button_status = is_pushbotton_pressed();

    if(!last_button_status && new_button_status)
    {
        debug_println("Settings mode stage change");
        switch (stage)
        {
        case STAGE_NONE:
            stage = STAGE_BPM;
            break;

        case STAGE_BPM:
            stage = STAGE_PEEP;
            break;

        case STAGE_PEEP:
            stage = STAGE_PIP;
            break;

        case STAGE_PIP:
            // Save settings 
            update_settings(settings_input);
            stage = STAGE_NONE;
            break;
        
        default:
            stage = STAGE_NONE;
            break;
        }
    }

    // Handle the stage

    float knob_portion = read_portion_knob();

    switch (stage)
    {
    case STAGE_NONE:
        break;

    case STAGE_BPM:
        settings_input.breath_per_min = (int32_t) lower_settings_range.breath_per_min + 
        knob_portion * (upper_settings_range.breath_per_min - lower_settings_range.breath_per_min);
        break;

    case STAGE_PEEP:
        settings_input.peep_cm_h20 = (int32_t) lower_settings_range.peep_cm_h20 + 
        knob_portion * (upper_settings_range.peep_cm_h20 - lower_settings_range.peep_cm_h20);
        break;

    case STAGE_PIP:
        settings_input.pip_cm_h20 = (int32_t) lower_settings_range.pip_cm_h20 + 
        knob_portion * (upper_settings_range.pip_cm_h20 - lower_settings_range.pip_cm_h20);
        break;
        
    default:
        stage = STAGE_NONE;
        break;
    }

    last_button_status = new_button_status;
}

void update_full_screen_fast(void)
{
    // Reformat data in 20 byte chunks to match what screen needs
    memcpy(&screen_buffer_reformatted[0], &buffer[0], 20);
    memcpy(&screen_buffer_reformatted[20], &buffer[40], 20);
    memcpy(&screen_buffer_reformatted[40], &buffer[20], 20);
    memcpy(&screen_buffer_reformatted[60], &buffer[60], 20);
    set_screen(&screen_buffer_reformatted[0]);
}