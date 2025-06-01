# Gardena Wasserventil Smart-Steuerung

ESP32-basierte WLAN-Steuerung für Gardena Bewässerungsventile mit Web-Interface.

## Features

- **Web-Interface**: Einfache Steuerung über Browser (Smartphone/PC)
- **Timer-Funktion**: Automatisches Schließen nach einstellbarer Zeit (1-240 Minuten)
- **Status-Anzeige**: Echtzeit-Status des Ventils
- **WLAN-Konfiguration**: Speicherung der WLAN-Zugangsdaten
- **Zeitstempel**: Protokollierung der letzten Aktivierung
- **Responsive Design**: Optimiert für mobile Geräte

## Hardware-Anforderungen

- ESP32 Entwicklungsboard
- Relais-Modul (5V, mit Optokoppler)
- Gardena Bewässerungsventil (24V AC oder 9V DC)
- Netzteil für Ventil (24V AC oder 9V DC)
- USB-Netzteil für ESP32 (5V)
- Verbindungskabel

## Software-Anforderungen

- [PlatformIO](https://platformio.org/) IDE
- ArduinoJson Library (wird automatisch installiert)

## Installation

1. Repository klonen:
```bash
git clone https://github.com/yourusername/gardena-wasserventil-smart-steuerung.git
cd gardena-wasserventil-smart-steuerung
```

2. WLAN-Zugangsdaten in `src/main.cpp` anpassen:
```cpp
const char* default_ssid = "YOUR_WIFI_SSID";
const char* default_password = "YOUR_WIFI_PASSWORD";
```

3. Mit PlatformIO kompilieren und auf ESP32 hochladen:
```bash
pio run -t upload
```

4. Seriellen Monitor öffnen um die IP-Adresse zu sehen:
```bash
pio device monitor
```

## Verkabelung

Siehe [PINOUT.md](PINOUT.md) für detaillierte Anschlussinformationen.

### Schnellübersicht:
- **GPIO16** ’ Relais IN
- **GPIO2** ’ Status LED (onboard)
- **Relais COM** ’ 24V AC (+)
- **Relais NO** ’ Ventil (+)
- **Ventil (-)** ’ 24V AC (-)

## Bedienung

1. Nach dem Start verbindet sich der ESP32 mit dem konfigurierten WLAN
2. IP-Adresse aus dem seriellen Monitor notieren
3. Browser öffnen und IP-Adresse eingeben
4. Web-Interface bietet:
   - **Ventil Öffnen/Schließen**: Manuelle Steuerung
   - **Timer**: Ventil für bestimmte Zeit öffnen
   - **Status**: Aktueller Zustand und Informationen

## API-Endpoints

- `GET /` - Web-Interface
- `GET /status` - JSON Status-Informationen
- `POST /valve` - Ventil steuern (`{"state": true/false}`)
- `POST /timer` - Timer starten (`{"duration": minutes}`)
- `POST /wifi` - WLAN konfigurieren (`{"ssid": "...", "password": "..."}`)

## Sicherheitshinweise

  **Wichtig**: 
- Arbeiten Sie nur mit Kleinspannung (24V AC/DC)
- Verwenden Sie ein Relais mit Optokoppler für galvanische Trennung
- Installieren Sie eine Sicherung in der Ventil-Stromversorgung
- Schützen Sie die Elektronik vor Feuchtigkeit

## Troubleshooting

### ESP32 verbindet sich nicht mit WLAN
- SSID und Passwort in `main.cpp` überprüfen
- Seriellen Monitor für Fehlermeldungen prüfen
- 2.4GHz WLAN verwenden (5GHz wird nicht unterstützt)

### Ventil öffnet/schließt nicht
- Stromversorgung des Ventils prüfen (24V AC oder 9V DC)
- Relais-Verbindungen überprüfen
- Mit Multimeter Schaltvorgang am Relais testen

### Web-Interface nicht erreichbar
- IP-Adresse aus seriellem Monitor verwenden
- Firewall-Einstellungen prüfen
- ESP32 und Client im gleichen Netzwerk?

## Erweiterungsmöglichkeiten

- Zeitgesteuerte Bewässerung (Cron-Jobs)
- Wetterbasierte Steuerung
- Bodenfeuchtesensor Integration
- MQTT Support für Smart Home Integration
- Mehrere Ventile steuern
- Wasserdurchflussmessung

## Lizenz

Dieses Projekt steht unter der MIT-Lizenz.

## Beiträge

Pull Requests sind willkommen! Für größere Änderungen bitte erst ein Issue erstellen.