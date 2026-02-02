#include "motor.h"
#include "hal.h"
#include <stdio.h>

/* ================================
   VARIABLES INTERNES (simulation)
   ================================ */

static float motor_speed_left  = 0.0f;   /* vitesse simulée moteur gauche */
static float motor_speed_right = 0.0f;   /* vitesse simulée moteur droit */

static int motor_left_state  = 0;         /* 0 = arrêt, 1 = marche */
static int motor_right_state = 0;

static unsigned int motor_print_count = 0;  /* throttle affichage PWM */

/* ================================
   INITIALISATION
   ================================ */

void motor_init(void)
{
    motor_speed_left  = 0.0f;
    motor_speed_right = 0.0f;

    motor_left_state  = 0;
    motor_right_state = 0;

    printf("[MOTOR] Initialisation moteurs\n");
}

/* ================================
   COMMANDE PWM (simulation)
   ================================ */

void motor_set_pwm_left(float pwm)
{
    if (pwm < 0.0f) pwm = 0.0f;
    if (pwm > 100.0f) pwm = 100.0f;

    motor_speed_left += 0.1f * (pwm - motor_speed_left);
    motor_left_state = (pwm > 0.0f) ? 1 : 0;

    motor_print_count++;
    if ((motor_print_count % 5) == 0)
        printf("[MOTOR] PWM gauche = %.2f | Vitesse = %.2f\n", pwm, motor_speed_left);
}

void motor_set_pwm_right(float pwm)
{
    if (pwm < 0.0f) pwm = 0.0f;
    if (pwm > 100.0f) pwm = 100.0f;

    motor_speed_right += 0.1f * (pwm - motor_speed_right);
    motor_right_state = (pwm > 0.0f) ? 1 : 0;

    if ((motor_print_count % 5) == 0)
        printf("[MOTOR] PWM droit  = %.2f | Vitesse = %.2f\n", pwm, motor_speed_right);
}

/* ================================
   LECTURE VITESSE (PID)
   ================================ */

float motor_get_speed_left(void)
{
    return motor_speed_left;
}

float motor_get_speed_right(void)
{
    return motor_speed_right;
}

/* ================================
   COMMANDES SIMPLES (FSM)
   ================================ */

void motor_forward(void)
{
    motor_left_state  = 1;
    motor_right_state = 1;

    printf("[MOTOR] Avance\n");
}

void motor_stop(void)
{
    /* Ne pas imprimer si déjà à l'arrêt */
    if (motor_left_state != 0 || motor_right_state != 0)
        printf("[MOTOR] STOP\n");

    motor_left_state  = 0;
    motor_right_state = 0;
    motor_speed_left  = 0.0f;
    motor_speed_right = 0.0f;
}

