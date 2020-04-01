/**
 * @file lcv_hmi.h
 * @brief Human-machine interface for Low Cost Ventilator
 */

#ifndef LCV_HMI_h
#define LCV_HMI_h

#include "LCV.h"

/*
*   Display Layout
*           Columns 1   .    .    .    .    .   20
            1                   10
*   Row 1:  Vent: ON/OFF        V: x.xxl
            1                   
*   Row 2:  PEEP: xx cmH20 
            1
*   Row 3:  PIP:  xx cmH20
            1
*   Row 4:  Settings...
*/

#define SCREEN_BUFFER_SIZE  (80)

#define HMI_ENABLE_ROW      (1)
#define HMI_ENABLE_ROW      (1)

typedef enum
{
    STAGE_NONE=0,
    STAGE_BPM=1,
    STAGE_PEEP=2,
    STAGE_PIP=3
} SETTINGS_INPUT_STAGE;

bool hmi_init(void);
float read_portion_knob(void);
bool is_button_start_on(void);
bool is_pushbotton_pressed(void);
void hmi_task(lcv_state_t * settings);
void update_main_buffer(lcv_state_t * settings);
void handle_hmi_input(void);
void update_full_screen_fast(char * buff);
bool update_alarm_buffer(void);

#endif