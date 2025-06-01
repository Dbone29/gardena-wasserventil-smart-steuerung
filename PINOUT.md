# ESP32 Pinout für Gardena Ventil-Steuerung

## Verwendete Pins

| ESP32 Pin | GPIO | Funktion | Beschreibung |
|-----------|------|----------|--------------|
| 23 | GPIO16 | VALVE_PIN | Steuert das Relais für das Gardena-Ventil |
| Onboard LED | GPIO2 | LED_STATUS | Status-LED (zeigt Ventilzustand an) |

## Anschlussschema

### Relais-Modul
- **VCC** → ESP32 5V (oder externe 5V Versorgung)
- **GND** → ESP32 GND
- **IN** → ESP32 GPIO16
- **COM** → 24V AC/DC Versorgung (+)
- **NO** → Gardena Ventil (+)
- **NC** → Nicht verbunden

### Gardena Ventil
- **Ventil (+)** → Relais NO
- **Ventil (-)** → 24V AC/DC Versorgung (-)

### Stromversorgung
- **ESP32** → 5V USB oder externes Netzteil
- **Gardena Ventil** → 24V AC (oder 9V DC je nach Ventiltyp)

## Wichtige Hinweise

1. **Relais-Typ**: Verwenden Sie ein Relais-Modul mit Optokoppler für galvanische Trennung
2. **Ventil-Spannung**: Prüfen Sie die Spezifikationen Ihres Gardena-Ventils (meist 24V AC oder 9V DC)
3. **Stromversorgung**: ESP32 und Ventil benötigen separate Stromversorgungen
4. **Sicherheit**: Installieren Sie eine Sicherung in der Ventil-Stromversorgung

## Schaltplan

```
ESP32                    Relais-Modul              Gardena Ventil
                                                   
GPIO16 -------- IN       COM -------- 24V AC (+)
                         NO  -------- Ventil (+)
5V ------------ VCC      NC  -------- (nicht verbunden)
GND ----------- GND      
                                      Ventil (-) ---- 24V AC (-)
                         
GPIO2 --------- Status LED (onboard)
```

## Optionale Erweiterungen

### Wasserdurchflusssensor
- **GPIO** → GPIO34 (Analog Input)
- **Sensor** → YF-S201 oder ähnlich

### Bodenfeuchtesensor
- **GPIO** → GPIO35 (Analog Input)
- **Sensor** → Kapazitiver Bodenfeuchtesensor

### Externer Taster
- **GPIO** → GPIO25 (mit internem Pull-up)
- **Funktion** → Manuelles Öffnen/Schließen des Ventils

### Display (OLED/LCD)
- **SDA** → GPIO21
- **SCL** → GPIO22
- **VCC** → 3.3V
- **GND** → GND