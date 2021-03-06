/**
 * @file lcv_blower_control.h
 * @brief Blower motor control for Low Cost Ventilator
 */

#ifndef LCV_BLOWER_CONTROL_h
#define LCV_BLOWER_CONTROL_h

#define R25             (1.0) // TODO
#define R100            (1.0) // TODO

void motor_init(void);
void enable_motor(void);
void disable_motor(void);
bool is_motor_ready(void);
void set_motor_speed_rpm(float speed_rpm);
void set_motor_speed_portion(float speed_portion);
float get_motor_temperature_c(void);
void motor_task(lcv_state_t * state);

#endif