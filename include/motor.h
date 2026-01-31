#ifndef MOTOR_H
#define MOTOR_H

typedef enum {
    MOTOR_STOP = 0,
    MOTOR_FORWARD,
    MOTOR_BACKWARD
} MotorState;

void motor_init(void);
void motor_set_left(MotorState state);
void motor_set_right(MotorState state);

#endif
