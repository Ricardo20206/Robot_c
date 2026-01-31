#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#include "motor.h"
#include "sensor.h"

#define OBSTACLE_DISTANCE_CM 20

typedef enum {
    STATE_FORWARD,
    STATE_STOP,
    STATE_TURN
} RobotState;
DWORD state_start_time = 0;
int time_elapsed(DWORD start, DWORD delay_ms) {
    return (GetTickCount() - start) >= delay_ms;
}


int main(void) {
    srand((unsigned)time(NULL));

    motor_init();
    RobotState state = STATE_FORWARD;
    
    while (1) {
        int distance = sensor_get_distance_cm();
    
        switch (state) {
    
            case STATE_FORWARD:
                motor_set_left(MOTOR_FORWARD);
                motor_set_right(MOTOR_FORWARD);
                printf("Etat: FORWARD\n");
    
                if (distance < OBSTACLE_DISTANCE_CM) {
                    state = STATE_STOP;
                    state_start_time = GetTickCount();
                }
                break;
    
            case STATE_STOP:
                motor_set_left(MOTOR_STOP);
                motor_set_right(MOTOR_STOP);
                printf("Etat: STOP\n");
    
                if (time_elapsed(state_start_time, 500)) {
                    state = STATE_TURN;
                    state_start_time = GetTickCount();
                }
                break;
    
            case STATE_TURN:
                motor_set_left(MOTOR_FORWARD);
                motor_set_right(MOTOR_BACKWARD);
                printf("Etat: TURN\n");
    
                if (time_elapsed(state_start_time, 700)) {
                    state = STATE_FORWARD;
                }
                break;
        }
    
        Sleep(50); // juste pour ne pas saturer la console
    }
    

    return 0;
}
