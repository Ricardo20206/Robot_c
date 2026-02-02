#include "pid.h"

void pid_init(PID *pid, float kp, float ki, float kd) {
    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;
    pid->prev_error = 0.0f;
    pid->integral = 0.0f;
    pid->out_min = 0.0f;
    pid->out_max = 100.0f;
}

void pid_set_output_limits(PID *pid, float min, float max) {
    pid->out_min = min;
    pid->out_max = max;
}

#define INTEGRAL_MAX 500.0f
#define INTEGRAL_MIN (-500.0f)

float pid_compute(PID *pid, float setpoint, float measured, float dt_sec) {
    float error = setpoint - measured;

    if (dt_sec <= 0.0f) dt_sec = 0.001f;

    pid->integral += error * dt_sec;
    if (pid->integral > INTEGRAL_MAX) pid->integral = INTEGRAL_MAX;
    if (pid->integral < INTEGRAL_MIN) pid->integral = INTEGRAL_MIN;

    float derivative = (error - pid->prev_error) / dt_sec;
    pid->prev_error = error;

    float out = pid->kp * error
              + pid->ki * pid->integral
              + pid->kd * derivative;

    if (out < pid->out_min) out = pid->out_min;
    if (out > pid->out_max) out = pid->out_max;
    return out;
}
