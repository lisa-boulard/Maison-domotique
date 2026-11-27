Rapport
[Rapport_maison_domotique.pdf](https://github.com/user-attachments/files/35515013/Rapport_maison_domotique.pdf)



# Projet Domotique sur Carte STM32 (STM32CubeIDE)

Projet de système embarqué domotique développé en **C/C++** sous **STM32CubeIDE** (configuration matérielle via **STM32CubeMX** et couche **HAL / LL**).

Ce dépôt contient le firmware, la configuration matérielle et la documentation du système domotique permettant la supervision, l'automatisation et le contrôle d'équipements domestiques (éclairage, climatisation/chauffage, sécurité/accès, mesures environnementales et interface utilisateur/communication).

---

## Fonctionnalités principales

- **Acquisition de capteurs** : température, humidité, luminosité, présence/mouvement ou ouverture (`ADC`, `I2C`, `SPI`, `GPIO` avec interruptions).
- **Actionneurs & régulation** : pilotage d'éclairages (PWM / relais), volets ou ventilation, et régulation automatique (ex. seuil de température ou luminosité avec hystérésis).
- **Sécurité & alertes** : détection d'événements critiques (intrusion, seuil dépassé) avec signalisation locale (LED, buzzer) et journalisation.
- **Interface & communication** : afficheur local (OLED / LCD / LEDs), boutons poussoirs et/ou liaison série/sans-fil (`UART`, `I2C`, `SPI`, `BLE` / `Wi-Fi` selon module externe).

---

## Architecture du dépôt
.
├── README.md
├── .mxproject / *.ioc          # Configuration matérielle STM32CubeMX (horloges, pins, périphériques)
├── Core/
│   ├── Inc/                    # Fichiers d'en-tête applicatifs (.h)
│   └── Src/                    # Code source applicatif (.c) et main.c
├── Drivers/
│   ├── CMSIS/                  # Définitions cœur ARM Cortex-M
│   └── STM32xx_HAL_Driver/     # Pilotes HAL fournis par STMicroelectronics
├── Middlewares/                # Piles logicielles optionnelles (FreeRTOS, FATFS, etc.)
└── docs/                       # Schémas de câblage, chronogrammes, photos du montage et notes

---

## Prérequis et environnement

- **STM32CubeIDE** (incluant STM32CubeMX, la chaîne de compilation `arm-none-eabi-gcc` et les outils de debug/flash).
- **Carte de développement STM32** (ex. Nucleo / Discovery) équipée d'un sondeur **ST-LINK** intégré ou externe.
- **Capteurs et actionneurs domotiques** (à compléter selon votre montage : capteur temp./humidité, capteur luminosité/PIR, LEDs/relais, bouton poussoir, écran ou module de communication) et alimentation adaptée.
- **Terminal série** (moniteur intégré à STM32CubeIDE, PuTTY, Tera Term ou minicom) pour les logs de debug via `UART`.

---

## Auteurs

- **Auteurs** : *Lisa* (à compléter avec votre nom / binôme)
- **Contexte** : Projet domotique sur STM32 — STM32CubeIDE
