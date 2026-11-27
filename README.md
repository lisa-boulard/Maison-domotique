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

> *Adaptez les noms de dossiers et de fichiers selon votre structure exacte générée par STM32CubeIDE.*

```text
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
```

---

## Prérequis et environnement

- **STM32CubeIDE** (incluant STM32CubeMX, la chaîne de compilation `arm-none-eabi-gcc` et les outils de debug/flash).
- **Carte de développement STM32** (ex. Nucleo / Discovery) équipée d'un sondeur **ST-LINK** intégré ou externe.
- **Capteurs et actionneurs domotiques** (à compléter selon votre montage : capteur temp./humidité, capteur luminosité/PIR, LEDs/relais, bouton poussoir, écran ou module de communication) et alimentation adaptée.
- **Terminal série** (moniteur intégré à STM32CubeIDE, PuTTY, Tera Term ou minicom) pour les logs de debug via `UART`.

---

## Prise en main et compilation

1. **Cloner le dépôt** :
   ```bash
   git clone <url-de-votre-depot>
   cd <nom-du-depot>
   ```
2. **Importer le projet dans STM32CubeIDE** :
   - Ouvrir **STM32CubeIDE** et sélectionner votre *Workspace*.
   - Aller dans `File > Import... > General > Existing Projects into Workspace` (ou ouvrir directement le fichier `.project` / `.ioc`).
3. **Vérifier la configuration matérielle (`.ioc`)** :
   - Ouvrir le fichier `*.ioc` pour vérifier l'affectation des broches (`GPIO`), les horloges (`Clock Configuration`) et les périphériques (`ADC`, `TIM`, `I2C`, `SPI`, `UART`).
   - Régénérer le code si nécessaire (`Ctrl + S` dans l'éditeur `.ioc`).
4. **Compiler et flasher** :
   - Compiler le projet (`Project > Build All` ou `Ctrl + B`).
   - Connecter la carte STM32 en USB et lancer le flashage/débogage (`Run > Debug` ou `F11`).

---

## Plan de développement et tests

- **1. Configuration matérielle & GPIO** : initialisation des horloges, test des boutons poussoirs (anti-rebond / interruptions) et des LEDs/relais.
- **2. Lecture des capteurs** : acquisition périodique des grandeurs physiques et filtrage/validation des mesures.
- **3. Logique domotique & automatismes** : mise en place des modes (manuel, automatique, éco, alarme) et des seuils de commande.
- **4. Sorties, affichage & communication** : pilotage PWM/actionneurs, retour d'état visuel/sonore et télémétrie via `UART` ou bus dédié.
- **5. Validation globale** : tests d'intégration, scénarios nominaux et cas limites (coupure/reconnexion capteur, dépassement de seuil, réactivité).

---

## Câblage et brochage (à compléter)

| Signal / Fonction | Périphérique STM32 | Broche MCU | Description / Remarques |
| :--- | :--- | :--- | :--- |
| Bouton utilisateur | `GPIO` (EXTI) | `PAx` | Commande locale / changement de mode |
| Éclairage / Actionneur | `TIMx` (PWM) / `GPIO` | `PAx` | Commande LED / relais / variateur |
| Capteur environnemental | `I2C1` / `ADC1` | `PBx` / `PAx` | Température, humidité ou luminosité |
| Console / Debug | `USARTx` (TX/RX) | `PAx` / `PAx` | Logs série (ex. 115200 bauds, 8N1) |

---

## Auteurs

- **Auteurs** : *Lisa* (à compléter avec votre nom / binôme)
- **Contexte** : Projet domotique sur STM32 — STM32CubeIDE