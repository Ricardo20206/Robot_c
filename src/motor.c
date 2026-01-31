#include <stdio.h>
#include "motor.h"

void motor_init(void) {
    printf("[MOTOR] Initialisation moteurs\n");
}

void motor_set_left(MotorState state) {
    printf("[MOTOR] Moteur gauche = %d\n", state);
}

void motor_set_right(MotorState state) {
    printf("[MOTOR] Moteur droit = %d\n", state);
}
