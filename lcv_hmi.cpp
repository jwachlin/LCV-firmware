/**
 * @file lcv_hmi.cpp
 * @brief Human-machine interface for Low Cost Ventilator
 */

#include <Arduino.h>
#include <string.h>

#include "LCV.h"
#include "lcv_lcd.h"
#include "lcv_alarm_monitoring.h"
#include "lcv_hmi.h"

#define ALARM_SCREEN_TOGGLE_MS      (2000)

static char alarm_screen_buffer[SCREEN_BUFFER_SIZE];

static char main_screen_buffer[SCREEN_BUFFER_SIZE];

static char current_screen_buffer[SCREEN_BUFFER_SIZE];
static char last_screen_buffer[SCREEN_BUFFER_SIZE];

static uint8_t screen_buffer_reformatted[SCREEN_BUFFER_SIZE];

static SETTINGS_INPUT_STAGE stage = STAGE_NONE;
static lcv_state_t settings_input;

static const lcv_state_t lower_settings_range = {.enable = 0, .tidal_volume_liter = 0.1,
                    .peep_cm_h20 = 10, .pip_cm_h20 = 3, .breath_per_min = 6};

static const lcv_state_t upper_settings_range = {.enable = 0, .tidal_volume_liter = 2.5,
                    .peep_cm_h20 = 60, .pip_cm_h20 = 30, .breath_per_min = 60};

static void update_screen(void)
{
    static bool first_cycle = true;

    // Clear any trailing 0s from string creation as those are special characters on the LCD
    for(int32_t i = 0; i < SCREEN_BUFFER_SIZE; i++)
    {
        if(current_screen_buffer[i] < 0x07)
        {
            current_screen_buffer[i] = 0x20; // ASCII space
        }
    }

    if(first_cycle)
    {
        update_full_screen_fast(current_screen_buffer);
    }

    // Only update things that changed for reduced overhead
    for(int32_t i = 0; i < SCREEN_BUFFER_SIZE; i++)
    {
        if(last_screen_buffer[i] != current_screen_buffer[i])
        {
            set_character_index(i, &current_screen_buffer[i]);
        }
    }

    for(int32_t i = 0; i < SCREEN_BUFFER_SIZE; i++)
    {
        last_screen_buffer[i] = current_screen_buffer[i];
    }

    first_cycle = false;
}

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

void hmi_task(lcv_state_t * settings)
{
    static uint32_t last_change_time = 0;
    static bool main_page = true;

    handle_hmi_input();

    uint32_t current_time = millis();

    if(!main_page)
    {
        if(!update_alarm_buffer())
        {
            main_page = true;
        }
    }

    if(main_page)
    {
        update_main_buffer(settings);
    }

    update_screen();

    if((current_time - last_change_time) > ALARM_SCREEN_TOGGLE_MS)
    {
        main_page = !main_page;
        last_change_time = current_time;
    }
}

void update_main_buffer(lcv_state_t * settings)
{
    for(int32_t i = 0; i < SCREEN_BUFFER_SIZE; i++)
    {
        main_screen_buffer[i] = 0x20; // ASCII space
    }

    // Update info
    // TODO doesn't seem to support floats
    if(settings->enable)
    {
        snprintf(&main_screen_buffer[0],9,"VENT:ON");
    }
    else
    {
        snprintf(&main_screen_buffer[0],9,"VENT:OFF");
    }

    uint32_t tidal_volume_ml = 1000.0 * settings->tidal_volume_liter;
    snprintf(&main_screen_buffer[10],10, "V:%iml", tidal_volume_ml);

    snprintf(&main_screen_buffer[20],20, "PEEP:%icmH20", settings->peep_cm_h20);

    snprintf(&main_screen_buffer[40],13, "PIP:%icmH20", settings->pip_cm_h20);

    snprintf(&main_screen_buffer[52],7, "BPM:%i", settings->breath_per_min);

    // Fill in settings input display
    switch (stage)
    {
    case STAGE_NONE:
        break;

    case STAGE_BPM:
        sprintf(&main_screen_buffer[60], "SET BPM:%i", settings_input.breath_per_min);
        break;

    case STAGE_PEEP:
        sprintf(&main_screen_buffer[60], "SET PEEP:%icmH20", settings_input.peep_cm_h20);
        break;

    case STAGE_PIP:
        sprintf(&main_screen_buffer[60], "SET PIP:%icmH20", settings_input.pip_cm_h20);
        break;
        
    default:
        break;
    }
    // copy to main buffer for update
    memcpy(current_screen_buffer, main_screen_buffer, SCREEN_BUFFER_SIZE);
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

void update_full_screen_fast(char * buff)
{
    // Reformat data in 20 byte chunks to match what screen needs
    memcpy(&screen_buffer_reformatted[0], &buff[0], 20);
    memcpy(&screen_buffer_reformatted[20], &buff[40], 20);
    memcpy(&screen_buffer_reformatted[40], &buff[20], 20);
    memcpy(&screen_buffer_reformatted[60], &buff[60], 20);
    set_screen(&screen_buffer_reformatted[0]);
}

bool update_alarm_buffer(void)
{
    bool any_alarms = (get_alarm_bitfield() != 0);

    for(int32_t i = 0; i < SCREEN_BUFFER_SIZE; i++)
    {
        alarm_screen_buffer[i] = 0x20; // ASCII space
    }

    snprintf(&alarm_screen_buffer[0],9,"ERRORS:");

    if(check_alarm(ALARM_FLOW_SENSOR))
    {
        snprintf(&alarm_screen_buffer[10],10,"FLOW");
    }

    if(check_alarm(ALARM_PRESSURE_SENSOR))
    {
        snprintf(&alarm_screen_buffer[20],10,"PRES SNS");
    }

    if(check_alarm(ALARM_MOTOR_ERROR))
    {
        snprintf(&alarm_screen_buffer[30],10,"MOT FAIL");
    }

    if(check_alarm(ALARM_MOTOR_TEMP))
    {
        snprintf(&alarm_screen_buffer[40],10,"MOT TEMP");
    }

    if(check_alarm(ALARM_SETTINGS_LOAD))
    {
        snprintf(&alarm_screen_buffer[50],10,"SETT LOAD");
    }

    // Copy to screen buffer
    memcpy(current_screen_buffer, alarm_screen_buffer, SCREEN_BUFFER_SIZE);

    return any_alarms;
}