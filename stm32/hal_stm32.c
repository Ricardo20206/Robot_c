/**
 * HAL STM32 — Implémentation pour STM32CubeIDE.
 * Remplace hal.c lors du build cible STM32.
 * Adapter les broches (GPIO, TIM, ADC) selon le hardware.
 */
#ifdef USE_STM32

#include "hal.h"
#include <stdint.h>

/* Inclure les en-têtes de ta carte (à adapter) */
#if defined(STM32F4xx)
#include "stm32f4xx_hal.h"
#elif defined(STM32F1xx)
#include "stm32f1xx_hal.h"
#elif defined(STM32G0xx)
#include "stm32g0xx_hal.h"
#else
#include "main.h"
#endif

/* Broches à adapter selon ton câblage */
#define MOTOR_LEFT_DIR_PORT   GPIOA
#define MOTOR_LEFT_DIR_PIN   GPIO_PIN_0
#define MOTOR_RIGHT_DIR_PORT  GPIOA
#define MOTOR_RIGHT_DIR_PIN   GPIO_PIN_1
#define HCSR04_TRIG_PORT     GPIOA
#define HCSR04_TRIG_PIN      GPIO_PIN_2
#define HCSR04_ECHO_PORT     GPIOA
#define HCSR04_ECHO_PIN      GPIO_PIN_3

extern TIM_HandleTypeDef htim2;   /* PWM moteur gauche */
extern TIM_HandleTypeDef htim3;   /* PWM moteur droit */
extern ADC_HandleTypeDef hadc1;   /* ADC distance IR (optionnel) */

void hal_init(void) {
    /* GPIO et PWM initialisés par CubeMX / main.c */
}

void hal_motor_left_forward(void) {
    HAL_GPIO_WritePin(MOTOR_LEFT_DIR_PORT, MOTOR_LEFT_DIR_PIN, GPIO_PIN_SET);
}
void hal_motor_left_stop(void) {
    HAL_GPIO_WritePin(MOTOR_LEFT_DIR_PORT, MOTOR_LEFT_DIR_PIN, GPIO_PIN_RESET);
}
void hal_motor_right_forward(void) {
    HAL_GPIO_WritePin(MOTOR_RIGHT_DIR_PORT, MOTOR_RIGHT_DIR_PIN, GPIO_PIN_SET);
}
void hal_motor_right_backward(void) {
    HAL_GPIO_WritePin(MOTOR_RIGHT_DIR_PORT, MOTOR_RIGHT_DIR_PIN, GPIO_PIN_RESET);
}
void hal_motor_right_stop(void) {
    HAL_GPIO_WritePin(MOTOR_RIGHT_DIR_PORT, MOTOR_RIGHT_DIR_PIN, GPIO_PIN_RESET);
}

unsigned long hal_millis(void) {
    return (unsigned long)HAL_GetTick();
}

unsigned long hal_micros(void) {
    uint32_t ms = HAL_GetTick();
    /* Option : DWT cycle counter pour us précis (à activer dans SystemInit) */
    return ms * 1000UL;
}

void hal_delay_ms(unsigned long ms) {
    HAL_Delay(ms);
}

void hal_delay_us(unsigned long us) {
    uint32_t start = hal_micros();
    while ((hal_micros() - start) < us) { }
}

void hal_hcsr04_trigger(void) {
    HAL_GPIO_WritePin(HCSR04_TRIG_PORT, HCSR04_TRIG_PIN, GPIO_PIN_SET);
    hal_delay_us(12);
    HAL_GPIO_WritePin(HCSR04_TRIG_PORT, HCSR04_TRIG_PIN, GPIO_PIN_RESET);
}

unsigned long hal_hcsr04_echo_pulse_us(void) {
    unsigned long t0 = hal_micros();
    while (HAL_GPIO_ReadPin(HCSR04_ECHO_PORT, HCSR04_ECHO_PIN) == GPIO_PIN_RESET) {
        if ((hal_micros() - t0) > 30000) return 0;
    }
    t0 = hal_micros();
    while (HAL_GPIO_ReadPin(HCSR04_ECHO_PORT, HCSR04_ECHO_PIN) == GPIO_PIN_SET) {
        if ((hal_micros() - t0) > 30000) return 0;
    }
    return hal_micros() - t0;
}

unsigned int hal_adc_read_distance(void) {
    uint32_t adc_val = 0;
    HAL_ADC_Start(&hadc1);
    if (HAL_ADC_PollForConversion(&hadc1, 10) == HAL_OK)
        adc_val = HAL_ADC_GetValue(&hadc1);
    HAL_ADC_Stop(&hadc1);
    return (unsigned int)adc_val;
}

#endif /* USE_STM32 */
