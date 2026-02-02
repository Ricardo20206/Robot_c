#ifndef SENSOR_H
#define SENSOR_H

void sensor_init(void);
int sensor_get_distance_cm(void);
int sensor_obstacle_detected(void);

/* Suiveur de ligne : position -100 (ligne à gauche) .. 0 (centré) .. +100 (ligne à droite) */
int sensor_get_line_position(void);
/* Lecture brute des capteurs ligne (0 = noir, 1 = blanc) ; count = LINE_SENSOR_COUNT */
void sensor_read_line_raw(int *values, int count);

#endif
