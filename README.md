# 🤖 Robot Éviteur d'Obstacles / Suiveur de Ligne

Projet en C modulaire : évitement d'obstacles, **PID vitesse moteur**, **capteurs réels (HC-SR04 / IR)**, **portage STM32 (CubeIDE)**, **FreeRTOS**, et **suiveur de ligne** complet.

## 🗂️ Étapes du projet (1 → 5)

| Étape | Contenu |
| ----- | ------- |
| **1. PID vitesse** | Régulation vitesse moteurs avec période (dt), saturation sortie, anti-windup (`pid.c` / `pid.h`) |
| **2. Capteur réel** | Modes SIM / HC-SR04 / IR dans `config.h` ; HAL `hal_micros`, `hal_hcsr04_*`, `hal_adc_*` ; `sensor.c` branché sur le mode |
| **3. Portage STM32** | Dossier `stm32/` : `hal_stm32.c` pour CubeIDE, broches à adapter (voir `stm32/README_STM32.md`) |
| **4. FreeRTOS** | Dossier `freertos/` : tâches Sensor + Control, file distance, `main_freertos.c` (voir `freertos/README_FREERTOS.md`) |
| **5. Suiveur de ligne** | `sensor_get_line_position()`, `sensor_read_line_raw()`, PID ligne dans `main.c` ; activable via `LINE_FOLLOW_ENABLED` dans `config.h` |

## 📦 Prérequis

- **Compilateur C** : GCC (MinGW sur Windows), Clang, ou MSVC
- **OS** : Windows (natif), Linux ou macOS (avec adaptation des appels temps réel)
- **Matériel (pour simulation)** : Aucun — le programme tourne entièrement en console

## 📋 Description

Le robot fonctionne selon une **machine à états** à trois étapes :

1. **AVANCER** — Suivi de ligne (si activé) + régulation vitesse ; détection obstacle à &lt; 20 cm
2. **ARRÊT** — Le robot s'arrête pendant 2 s
3. **TOURNER** — Rotation sur place pendant 1,5 s, puis retour en avant

Le cycle se répète indéfiniment dans une boucle infinie.

### Schéma de la machine à états

```
                    ┌─────────────────────────────────────┐
                    │                                     │
                    ▼                                     │
              ┌───────────┐     distance < 20 cm    ┌───────────┐
              │  FORWARD  │ ─────────────────────▶  │   STOP    │
              │  (avancer)│                         │  (arrêt)  │
              └───────────┘                         └───────────┘
                    ▲                                     │
                    │                                     │ 2 s
                    │                                     ▼
                    │                               ┌───────────┐
                    └──── 1,5 s ────────────────────│   TURN    │
                              (tourner)             │ (rotation)│
                                                    └───────────┘
```

## 🏗️ Structure du projet

```
Robot_c/
├── include/
│   ├── config.h    # SENSOR_MODE (SIM/HCSR04/IR), LINE_FOLLOW_ENABLED, LINE_SENSOR_COUNT
│   ├── hal.h       # HAL temps, moteurs, HC-SR04, ADC
│   ├── motor.h     # PWM, vitesse, stop/forward
│   ├── pid.h       # PID avec dt, saturation sortie
│   └── sensor.h    # Distance, obstacle, ligne (position + raw)
├── src/
│   ├── main.c      # FSM + PID vitesse + PID ligne (si activé)
│   ├── motor.c     # Moteurs (simulation ou PWM via HAL)
│   ├── sensor.c    # Capteurs (SIM / HC-SR04 / IR, ligne)
│   ├── hal.c       # HAL PC (Windows/Linux)
│   └── pid.c       # Régulateur PID
├── stm32/          # Portage STM32 CubeIDE
│   ├── hal_stm32.c
│   └── README_STM32.md
├── freertos/       # Version FreeRTOS (tâches, file)
│   ├── main_freertos.c
│   ├── robot_tasks.c / .h
│   └── README_FREERTOS.md
├── Makefile
└── README.md
```

## ⚙️ Fonctionnalités

| Module    | Rôle                                                                 |
| --------- | -------------------------------------------------------------------- |
| **motor** | Contrôle des deux moteurs (gauche/droite) avec états STOP, FORWARD, BACKWARD |
| **sensor**| Retourne la distance en cm vers l'obstacle le plus proche            |

> **Note :** Actuellement, moteurs et capteur sont **simulés** (printf et valeurs aléatoires). Le code est conçu pour faciliter le remplacement par de vrais drivers matériels.

## 🔧 Compilation

### PC (Windows / Linux / macOS)

```bash
gcc -Wall -Wextra -std=c11 -I include -o robot \
  src/main.c src/motor.c src/sensor.c src/hal.c src/pid.c
```

Sous Windows : `robot.exe` à la place de `robot`.

> Sous Linux/macOS, remplacer `Sleep()` et `GetTickCount()` par des équivalents (`usleep`, `gettimeofday`).

## 🚀 Exécution

```bash
# Windows
robot.exe

# Linux / macOS
./robot
```

Le programme affiche en continu l'état actuel et les commandes moteur dans la console. Appuyez sur `Ctrl+C` pour quitter.

### Exemple de sortie console

```
[MOTOR] Initialisation moteurs
Etat: FORWARD
[MOTOR] Moteur gauche = 1
[MOTOR] Moteur droit = 1
Etat: FORWARD
[MOTOR] Moteur gauche = 1
[MOTOR] Moteur droit = 1
Etat: STOP
[MOTOR] Moteur gauche = 0
[MOTOR] Moteur droit = 0
Etat: TURN
[MOTOR] Moteur gauche = 1
[MOTOR] Moteur droit = 2
Etat: FORWARD
[MOTOR] Moteur gauche = 1
[MOTOR] Moteur droit = 1
...
```

> Valeurs moteur : 0 = STOP, 1 = FORWARD, 2 = BACKWARD

## 📐 Paramètres configurables

| Paramètre | Fichier | Description |
| --------- | ------- | ----------- |
| `SENSOR_MODE` | config.h | SIM / HCSR04 / IR |
| `LINE_FOLLOW_ENABLED` | config.h | 1 = suivi de ligne + évitement |
| `LINE_SENSOR_COUNT` | config.h | Nombre de capteurs IR ligne (ex. 5) |
| Seuil obstacle 20 cm, durée STOP 2000 ms, TURN 1500 ms | main.c | FSM évitement |

## 🔌 Adapter pour du matériel réel

- **config.h** : Passer `SENSOR_MODE` à `SENSOR_MODE_HCSR04` ou `SENSOR_MODE_IR`.
- **stm32/** : Utiliser `hal_stm32.c` dans un projet CubeIDE (symbole `USE_STM32`), adapter broches.
- **freertos/** : Lier `main_freertos.c` + `robot_tasks.c` avec FreeRTOS (symbole `USE_FREERTOS`).

## 📝 Licence

Projet éducatif — libre d'utilisation et de modification.
