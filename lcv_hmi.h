/**
 * @file lcv_hmi.h
 * @brief Human-machine interface for Low Cost Ventilator
 */

#ifndef LCV_HMI_h
#define LCV_HMI_h

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

typedef struct 
{
    uint8_t enable : 1;
    float tidal_volume_liter;
    int32_t peep_cm_h20;
    int32_t pip_cm_h20;
    int32_t breath_per_min;
} lcv_settings_t;

bool hmi_init(void);
float read_portion_knob(void);
bool is_button_start_on(void);
bool is_pushbotton_pressed(void);
bool display_status(lcv_settings_t * settings);
void handle_hmi_input(void);
void update_full_screen_fast(void);

#endif