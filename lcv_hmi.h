/**
 * @file lcv_hmi.h
 * @brief Human-machine interface for Low Cost Ventilator
 */

#ifndef LCV_HMI_h
#define LCV_HMI_h

bool hmi_init(void);
float read_portion_knob(void);
bool is_button_start_on(void);
bool is_pushbotton_pressed(void);

#endif