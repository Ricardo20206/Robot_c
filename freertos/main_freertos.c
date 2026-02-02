/**
 * Point d'entrée avec FreeRTOS.
 * Utiliser ce fichier à la place de main.c lorsque USE_FREERTOS est défini.
 */
#ifdef USE_FREERTOS

#include "FreeRTOS.h"
#include "task.h"
#include "robot_tasks.h"
#include <stdio.h>

int main(void) {
    robot_tasks_init();
    printf("[FREERTOS] Demarrage scheduler\n");
    vTaskStartScheduler();
    for (;;) { }
    return 0;
}

#endif
