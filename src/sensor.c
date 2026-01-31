#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "sensor.h"

int sensor_get_distance_cm(void) {
    return rand() % 100;  // 0 à 99 cm
}
