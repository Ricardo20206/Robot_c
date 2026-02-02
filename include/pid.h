#ifndef PID_H
#define PID_H

typedef struct {
    float kp;
    float ki;
    float kd;
    float prev_error;
    float integral;
    float out_min;   /* saturation sortie (ex. 0) */
    float out_max;   /* saturation sortie (ex. 100) */
} PID;

void pid_init(PID *pid, float kp, float ki, float kd);
void pid_set_output_limits(PID *pid, float min, float max);
float pid_compute(PID *pid, float setpoint, float measured, float dt_sec);

#endif
