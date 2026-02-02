# Portage STM32 (CubeIDE)

Ce dossier contient l’implémentation de la **HAL** pour une cible STM32. Le reste du projet (`main.c`, `motor.c`, `sensor.c`, `pid.c`) reste inchangé.

## Intégration dans CubeIDE

1. Créer un projet **STM32CubeIDE** pour ta carte (ex. STM32F4, F1, G0).
2. Copier dans le projet :
   - `hal_stm32.c` → Core/Src/ (ou Drivers/BSP)
   - Les dossiers `../src`, `../include` (ou ajouter les chemins include).
3. Dans **Project → Properties → C/C++ Build → Settings → Symboles** : ajouter `USE_STM32`.
4. Exclure ou ne pas compiler `hal.c` (PC) ; compiler `hal_stm32.c` à la place.
5. Adapter les broches dans `hal_stm32.c` (TRIG, ECHO, PWM, direction moteurs, ADC) selon ton câblage.

## Broches à configurer (exemple)

| Fonction        | Broche  | Remarque              |
|-----------------|---------|------------------------|
| Moteur G PWM    | TIMx_CH | Ex. PA8 (TIM1_CH1)     |
| Moteur D PWM    | TIMx_CH | Ex. PA9 (TIM1_CH2)     |
| Moteur G dir    | GPIO    | Ex. PA0                |
| Moteur D dir    | GPIO    | Ex. PA1                |
| HC-SR04 TRIG    | GPIO    | Ex. PA2                |
| HC-SR04 ECHO    | GPIO    | Ex. PA3 (EXTI ou input)|
| IR (ADC)        | ADC_CH  | Ex. PA4 (ADC1_IN4)     |

## Build

- **PC** : `make` (utilise `src/hal.c`).
- **STM32** : build CubeIDE (utilise `stm32/hal_stm32.c`).
