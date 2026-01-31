# 🤖 Robot Éviteur d'Obstacles

Projet en C simulant le comportement d'un robot capable de détecter et éviter les obstacles. Le code est modulaire et prêt à être adapté pour un robot physique (Arduino, Raspberry Pi, etc.).

## 📦 Prérequis

- **Compilateur C** : GCC (MinGW sur Windows), Clang, ou MSVC
- **OS** : Windows (natif), Linux ou macOS (avec adaptation des appels temps réel)
- **Matériel (pour simulation)** : Aucun — le programme tourne entièrement en console

## 📋 Description

Le robot fonctionne selon une **machine à états** à trois étapes :

1. **AVANCER** — Le robot se déplace en ligne droite
2. **ARRÊT** — Lorsqu'un obstacle est détecté à moins de 20 cm, le robot s'arrête pendant 500 ms
3. **TOURNER** — Le robot tourne sur place pendant 700 ms, puis repart en avant

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
                    │                                     │ 500 ms
                    │                                     ▼
                    │                               ┌───────────┐
                    └──── 700 ms ───────────────────│   TURN    │
                              (tourner)             │ (rotation)│
                                                    └───────────┘
```

## 🏗️ Structure du projet

```
Robot_c/
├── include/
│   ├── motor.h      # Interface des moteurs (états, prototypes)
│   └── sensor.h     # Interface du capteur de distance
├── src/
│   ├── main.c       # Logique principale et machine à états
│   ├── motor.c      # Implémentation des moteurs (simulation)
│   └── sensor.c     # Implémentation du capteur (simulation)
├── robot.exe        # Exécutable (Windows)
└── README.md
```

## ⚙️ Fonctionnalités

| Module    | Rôle                                                                 |
| --------- | -------------------------------------------------------------------- |
| **motor** | Contrôle des deux moteurs (gauche/droite) avec états STOP, FORWARD, BACKWARD |
| **sensor**| Retourne la distance en cm vers l'obstacle le plus proche            |

> **Note :** Actuellement, moteurs et capteur sont **simulés** (printf et valeurs aléatoires). Le code est conçu pour faciliter le remplacement par de vrais drivers matériels.

## 🔧 Compilation

### Windows (MinGW / MSVC)

```bash
gcc -o robot.exe src/main.c src/motor.c src/sensor.c -I include
```

### Linux / macOS

```bash
gcc -o robot src/main.c src/motor.c src/sensor.c -I include
```

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

| Paramètre | Valeur | Fichier | Description |
| --------- | ------ | ------- | ----------- |
| `OBSTACLE_DISTANCE_CM` | 20 | main.c | Distance seuil (cm) pour considérer un obstacle |
| Durée arrêt | 500 ms | main.c | Temps d'arrêt avant de tourner |
| Durée rotation | 700 ms | main.c | Temps de rotation sur place |

## 🔌 Adapter pour du matériel réel

- **motor.c** : Remplacer les `printf` par des appels GPIO/PWM (ex. Arduino `analogWrite`, Raspberry Pi `wiringPi`)
- **sensor.c** : Remplacer `rand() % 100` par la lecture d'un capteur ultrason (HC-SR04) ou infrarouge

## 📝 Licence

Projet éducatif — libre d'utilisation et de modification.
