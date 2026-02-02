#include <stdio.h>
#include "hal.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <sys/time.h>
#endif

void hal_init(void) {
    printf("[HAL] Init hardware PC\n");
}

void hal_motor_left_forward(void)  { printf("[HAL] Left motor forward\n"); }
void hal_motor_left_stop(void)     { printf("[HAL] Left motor stop\n"); }
void hal_motor_right_forward(void) { printf("[HAL] Right motor forward\n"); }
void hal_motor_right_backward(void){ printf("[HAL] Right motor backward\n"); }
void hal_motor_right_stop(void)    { printf("[HAL] Right motor stop\n"); }

unsigned long hal_millis(void) {
#ifdef _WIN32
    return GetTickCount();
#else
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (unsigned long)(tv.tv_sec * 1000UL + tv.tv_usec / 1000);
#endif
}

unsigned long hal_micros(void) {
#ifdef _WIN32
    LARGE_INTEGER freq, count;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&count);
    return (unsigned long)(count.QuadPart * 1000000UL / freq.QuadPart);
#else
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (unsigned long)(tv.tv_sec * 1000000UL + tv.tv_usec);
#endif
}

void hal_delay_ms(unsigned long ms) {
#ifdef _WIN32
    Sleep(ms);
#else
    usleep(ms * 1000);
#endif
}

void hal_delay_us(unsigned long us) {
#ifdef _WIN32
    unsigned long target = hal_micros() + us;
    while (hal_micros() < target) { }
#else
    usleep(us);
#endif
}

void hal_hcsr04_trigger(void) {
    /* PC : simulation (sur STM32 : GPIO TRIG = 1, delay 10 us, GPIO TRIG = 0) */
#ifdef _WIN32
    (void)0;
#else
    (void)0;
#endif
}

unsigned long hal_hcsr04_echo_pulse_us(void) {
    /* PC : simulation (sur STM32 : attendre front montant ECHO, mesurer jusqu'à front descendant) */
    return 0;  /* 0 = pas de mesure / timeout ; en sim on utilisera rand dans sensor.c */
}

unsigned int hal_adc_read_distance(void) {
    /* PC : simulation (sur STM32 : lire ADC canal distance IR) */
    return 512;
}
