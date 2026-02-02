/**
 * FreeRTOS — Tâches robot (capteur, contrôle FSM + PID).
 * À compiler avec USE_FREERTOS et à lier avec FreeRTOS.
 */
#ifdef USE_FREERTOS

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include <stdio.h>
#include "hal.h"
#include "pid.h"
#include "motor.h"
#include "sensor.h"

#define LOOP_PERIOD_MS 100
#define SENSOR_PERIOD_MS 50
#define TARGET_SPEED  60.0f
#define QUEUE_DISTANCE_LEN 1

static QueueHandle_t distance_queue;
static TaskHandle_t task_sensor;
static TaskHandle_t task_control;

typedef enum {
    STATE_FORWARD,
    STATE_STOP,
    STATE_TURN
} robot_state_t;

static robot_state_t state = STATE_FORWARD;
static robot_state_t prev_state = (robot_state_t)-1;
static unsigned long state_timer = 0;
static PID pid_left, pid_right;

static int time_elapsed(unsigned long start, unsigned long delay_ms) {
    return (hal_millis() - start) >= delay_ms;
}

static void SensorTask(void *pv) {
    (void)pv;
    int dist;
    for (;;) {
        dist = sensor_get_distance_cm();
        xQueueOverwrite(distance_queue, &dist);
        vTaskDelay(pdMS_TO_TICKS(SENSOR_PERIOD_MS));
    }
}

static void ControlTask(void *pv) {
    (void)pv;
    int distance_cm = 0;

    for (;;) {
        xQueueReceive(distance_queue, &distance_cm, pdMS_TO_TICKS(LOOP_PERIOD_MS));

        if (state != prev_state) {
            prev_state = state;
            switch (state) {
                case STATE_FORWARD: printf("Etat: FORWARD\n"); break;
                case STATE_STOP:    printf("Etat: STOP\n");    break;
                case STATE_TURN:   printf("Etat: TURN\n");    break;
            }
        }

        switch (state) {
            case STATE_FORWARD:
                if (distance_cm < 20) {
                    state = STATE_STOP;
                    state_timer = hal_millis();
                }
                break;
            case STATE_STOP:
                motor_stop();
                if (time_elapsed(state_timer, 2000)) {
                    state = STATE_TURN;
                    state_timer = hal_millis();
                }
                break;
            case STATE_TURN:
                hal_motor_left_forward();
                hal_motor_right_backward();
                if (time_elapsed(state_timer, 1500)) {
                    state = STATE_FORWARD;
                    state_timer = hal_millis();
                }
                break;
        }

        if (state == STATE_FORWARD) {
            float speed_left  = motor_get_speed_left();
            float speed_right = motor_get_speed_right();
            float dt_sec = LOOP_PERIOD_MS / 1000.0f;
            float pwm_left  = pid_compute(&pid_left,  TARGET_SPEED, speed_left,  dt_sec);
            float pwm_right = pid_compute(&pid_right, TARGET_SPEED, speed_right, dt_sec);
            motor_set_pwm_left(pwm_left);
            motor_set_pwm_right(pwm_right);
            hal_motor_left_forward();
            hal_motor_right_forward();
        }

        vTaskDelay(pdMS_TO_TICKS(LOOP_PERIOD_MS));
    }
}

void robot_tasks_init(void) {
    hal_init();
    motor_init();
    sensor_init();
    pid_init(&pid_left,  0.8f, 0.0f, 0.0f);
    pid_init(&pid_right, 0.8f, 0.0f, 0.0f);
    pid_set_output_limits(&pid_left,  0.0f, 100.0f);
    pid_set_output_limits(&pid_right, 0.0f, 100.0f);

    distance_queue = xQueueCreate(QUEUE_DISTANCE_LEN, sizeof(int));
    if (distance_queue == NULL) return;

    xTaskCreate(SensorTask,  "Sensor",  128, NULL, 1, &task_sensor);
    xTaskCreate(ControlTask, "Control", 256, NULL, 2, &task_control);
}

#endif /* USE_FREERTOS */
