#ifndef MOTOR_H
#define MOTOR_H

void motor_init(void);
void motor_set_pwm_left(float pwm);
void motor_set_pwm_right(float pwm);
float motor_get_speed_left(void);
float motor_get_speed_right(void);
void motor_forward(void);
void motor_stop(void);

#endif
