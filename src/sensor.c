#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "sensor.h"
#include "hal.h"
#include "config.h"

#define OBSTACLE_THRESHOLD_CM 20
#define HCSR04_CM_PER_US      0.0343f  /* 1 us = 0.0343 cm (aller-retour) */
#define HCSR04_TIMEOUT_CM     400

static int sensor_initialized = 0;

void sensor_init(void) {
    if (!sensor_initialized) {
        srand((unsigned)time(NULL));
        sensor_initialized = 1;
#if SENSOR_MODE == SENSOR_MODE_SIM
        printf("[SENSOR] Init capteurs (simulation)\n");
#elif SENSOR_MODE == SENSOR_MODE_HCSR04
        printf("[SENSOR] Init HC-SR04\n");
#elif SENSOR_MODE == SENSOR_MODE_IR
        printf("[SENSOR] Init capteur IR\n");
#endif
    }
}

int sensor_get_distance_cm(void) {
#if SENSOR_MODE == SENSOR_MODE_SIM
    return rand() % 100;  /* 0 à 99 cm (simulation) */
#elif SENSOR_MODE == SENSOR_MODE_HCSR04
    hal_hcsr04_trigger();
    hal_delay_us(12);
    unsigned long pulse_us = hal_hcsr04_echo_pulse_us();
    if (pulse_us == 0) return HCSR04_TIMEOUT_CM;
    /* distance = (pulse_us * 0.0343) / 2 = pulse_us / 58.2 */
    float cm = (float)pulse_us * HCSR04_CM_PER_US * 0.5f;
    return (int)(cm > 400.0f ? 400.0f : cm);
#elif SENSOR_MODE == SENSOR_MODE_IR
    unsigned int adc = hal_adc_read_distance();
    /* Formule type IR : plus ADC bas = plus proche (ex. 1024 - adc puis scale) */
    int cm = 4000 / (adc + 1);
    if (cm > 400) cm = 400;
    return cm;
#else
    return rand() % 100;
#endif
}

int sensor_obstacle_detected(void) {
    return sensor_get_distance_cm() < OBSTACLE_THRESHOLD_CM;
}

/* Position ligne : -100 (gauche) .. 0 (centré) .. +100 (droite) */
int sensor_get_line_position(void) {
#if defined(LINE_SENSOR_COUNT) && LINE_SENSOR_COUNT >= 3
    int values[8];
    int count = LINE_SENSOR_COUNT;
    if (count > 8) count = 8;
    sensor_read_line_raw(values, count);
    /* Poids centré : -2 -1 0 +1 +2 pour 5 capteurs -> position -100..+100 */
    int sum = 0, n = 0;
    int center = (count - 1) / 2;
    for (int i = 0; i < count; i++) {
        if (values[i]) {
            sum += (i - center) * 50;
            n++;
        }
    }
    if (n == 0) return 0;  /* pas de ligne détectée : garder cap */
    return sum / (n > 0 ? n : 1);
#else
    (void)0;
    return 0;
#endif
}

void sensor_read_line_raw(int *values, int count) {
    /* Simulation : une seule cellule "ligne" à une position aléatoire */
    int line_cell = rand() % count;
    for (int i = 0; i < count; i++)
        values[i] = (i == line_cell) ? 1 : 0;
}
