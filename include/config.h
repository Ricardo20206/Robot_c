#ifndef CONFIG_H
#define CONFIG_H

/* Mode capteur distance : SIM (simulation), HCSR04 (ultrason), IR (infrarouge) */
#define SENSOR_MODE_SIM    0
#define SENSOR_MODE_HCSR04 1
#define SENSOR_MODE_IR     2

#define SENSOR_MODE SENSOR_MODE_SIM

/* Suiveur de ligne : 0 = désactivé (évitement seul), 1 = suivi de ligne + évitement */
#define LINE_FOLLOW_ENABLED 1

/* Nombre de capteurs IR sous la ligne (3 à 7 typique) */
#define LINE_SENSOR_COUNT 5

#endif
