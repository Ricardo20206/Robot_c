# FreeRTOS — Tâches et timers

Version du robot avec **FreeRTOS** : tâches dédiées (capteur, contrôle) et boucle de contrôle à période fixe.

## Activation sous STM32CubeIDE

1. Ouvrir **CubeMX** (`.ioc` du projet).
2. **Middleware** → **FREERTOS** → **Enabled** (CMSIS_V1 ou V2).
3. **Tasks and Queues** : créer les tâches (ou utiliser le code fourni dans `robot_tasks.c`).
4. Générer le code.
5. Inclure `robot_tasks.c` / `robot_tasks.h` et appeler `robot_tasks_init()` depuis `main.c` après `HAL_Init()`.

## Architecture des tâches

| Tâche        | Priorité | Période  | Rôle                                      |
|-------------|----------|----------|-------------------------------------------|
| SensorTask  | 1        | 50 ms    | Lit la distance, envoie dans la file.     |
| ControlTask | 2        | 100 ms   | FSM + PID, commande moteurs.              |

## Files (queues)

- **distance_queue** : `int` (distance en cm), taille 1. La tâche contrôle consomme avec timeout 100 ms.

## Build

- **Sans FreeRTOS** : compiler `main.c` (boucle unique).
- **Avec FreeRTOS** : définir `USE_FREERTOS`, compiler `main_freertos.c` et `robot_tasks.c` à la place de `main.c`.
