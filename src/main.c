#include <stdio.h>
#include "hal.h"
#include "pid.h"
#include "motor.h"
#include "sensor.h"
#include "config.h"

/* ================================
   CONSTANTES
   ================================ */

#define LOOP_PERIOD_MS 100
#define TARGET_SPEED  60.0f   /* vitesse cible moteurs */
#define LINE_POSITION_SETPOINT 0.0f  /* ligne centrée */

/* ================================
   FSM
   ================================ */

typedef enum {
    STATE_FORWARD,
    STATE_STOP,
    STATE_TURN
} robot_state_t;

static robot_state_t state = STATE_FORWARD;
static robot_state_t prev_state = (robot_state_t)-1;  /* force affichage au 1er tour */

/* ================================
   TEMPS
   ================================ */

static unsigned long millis(void)
{
    return hal_millis();
}

static int time_elapsed(unsigned long start, unsigned long delay_ms)
{
    return (millis() - start) >= delay_ms;
}

/* ================================
   PID (module pid.c)
   ================================ */

static PID pid_left;
static PID pid_right;
#if LINE_FOLLOW_ENABLED
static PID pid_line;   /* PID position ligne (erreur = 0 - position) */
#endif

/* ================================
   MAIN
   ================================ */

int main(void)
{
    hal_init();
    motor_init();
    sensor_init();

    pid_init(&pid_left,  0.8f, 0.0f, 0.0f);
    pid_init(&pid_right, 0.8f, 0.0f, 0.0f);
    pid_set_output_limits(&pid_left,  0.0f, 100.0f);
    pid_set_output_limits(&pid_right, 0.0f, 100.0f);
#if LINE_FOLLOW_ENABLED
    pid_init(&pid_line, 1.2f, 0.0f, 0.1f);  /* Kp ligne, Ki, Kd */
    pid_set_output_limits(&pid_line, -50.0f, 50.0f);  /* correction ±50 */
#endif

    unsigned long state_timer = millis();

    while (1)
    {
        /* Affichage uniquement au changement d'état */
        if (state != prev_state)
        {
            prev_state = state;
            switch (state)
            {
            case STATE_FORWARD: printf("Etat: FORWARD\n"); break;
            case STATE_STOP:    printf("Etat: STOP\n");    break;
            case STATE_TURN:    printf("Etat: TURN\n");   break;
            }
        }

        /* ================================
           FSM
           ================================ */

        switch (state)
        {
        case STATE_FORWARD:
            if (sensor_obstacle_detected())
            {
                state = STATE_STOP;
                state_timer = millis();
            }
            break;

        case STATE_STOP:
            motor_stop();

            if (time_elapsed(state_timer, 2000))
            {
                state = STATE_TURN;
                state_timer = millis();
                hal_motor_left_forward();
                hal_motor_right_backward();
            }
            break;

        case STATE_TURN:
            if (time_elapsed(state_timer, 1500))
            {
                state = STATE_FORWARD;
                state_timer = millis();
            }
            break;
        }

        /* ================================
           PID (SEULEMENT EN AVANCE)
           ================================ */

        if (state == STATE_FORWARD)
        {
            float speed_left  = motor_get_speed_left();
            float speed_right = motor_get_speed_right();
            float dt_sec = LOOP_PERIOD_MS / 1000.0f;

            float pwm_left  = pid_compute(&pid_left,  TARGET_SPEED, speed_left,  dt_sec);
            float pwm_right = pid_compute(&pid_right, TARGET_SPEED, speed_right, dt_sec);

#if LINE_FOLLOW_ENABLED
            int line_pos = sensor_get_line_position();
            float line_correction = pid_compute(&pid_line, LINE_POSITION_SETPOINT, (float)line_pos, dt_sec);
            pwm_left  -= line_correction;  /* ligne à droite -> correction > 0 -> freiner gauche */
            pwm_right += line_correction;
#endif

            motor_set_pwm_left(pwm_left);
            motor_set_pwm_right(pwm_right);

            hal_motor_left_forward();
            hal_motor_right_forward();
        }

        hal_delay_ms(LOOP_PERIOD_MS);
    }

    return 0;
}
