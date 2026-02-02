#ifndef HAL_H
#define HAL_H

void hal_init(void);
void hal_motor_left_forward(void);
void hal_motor_left_stop(void);
void hal_motor_right_forward(void);
void hal_motor_right_backward(void);
void hal_motor_right_stop(void);

unsigned long hal_millis(void);
unsigned long hal_micros(void);
void hal_delay_ms(unsigned long ms);
void hal_delay_us(unsigned long us);

/* HC-SR04 : trigger = sortie, echo = entrée (pulse width = temps aller-retour) */
void hal_hcsr04_trigger(void);
unsigned long hal_hcsr04_echo_pulse_us(void);

/* IR : lecture ADC (0..1023 ou 0..4096 selon cible), à convertir en cm */
unsigned int hal_adc_read_distance(void);

#endif
