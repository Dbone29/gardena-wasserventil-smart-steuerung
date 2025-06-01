# Teileliste für Gardena Wasserventil Smart-Steuerung

## Benötigte Komponenten

### Hauptkomponenten

| Bauteil | Beschreibung | Ungefährer Preis | Bezugsquelle |
|---------|--------------|------------------|--------------|
| **ESP32 Development Board** | ESP32-WROOM-32 DevKit C oder ähnlich | 8-15 € | Amazon, AZ-Delivery, Reichelt |
| **Relais-Modul** | 5V 1-Kanal Relais mit Optokoppler (für 230V/10A) | 3-5 € | Amazon, AZ-Delivery |
| **Gardena Bewässerungsventil** | 24V AC Magnetventil (z.B. Gardena 1278-20) | 25-35 € | Baumarkt, Amazon |
| **Netzteil 24V AC** | Trafo 230V → 24V AC, mind. 20VA | 15-25 € | Conrad, Reichelt |
| **USB-Netzteil** | 5V/1A für ESP32 | 5-10 € | Amazon |

### Verbindungsmaterial

| Bauteil | Beschreibung | Ungefährer Preis |
|---------|--------------|------------------|
| **Dupont-Kabel** | Female-Female, 10-20cm, Set | 3-5 € |
| **Klemmen/Wago-Klemmen** | Für sichere Verbindungen | 5-8 € |
| **Gehäuse** | Wasserdichtes Gehäuse IP65 | 10-20 € |
| **Kabelverschraubungen** | PG7/PG9 für Gehäuse | 3-5 € |

### Optionale Komponenten

| Bauteil | Beschreibung | Ungefährer Preis | Verwendung |
|---------|--------------|------------------|------------|
| **Bodenfeuchtesensor** | Kapazitiver Sensor | 5-10 € | Automatische Bewässerung |
| **Durchflusssensor** | YF-S201 oder ähnlich | 8-15 € | Wasserverbrauch messen |
| **OLED Display** | 0.96" I2C Display | 5-8 € | Lokale Statusanzeige |
| **Taster** | Wasserdichter Taster | 2-4 € | Manuelle Bedienung |
| **Buck Converter** | 24V → 5V Step-Down | 3-5 € | Gemeinsame Stromversorgung |

## Gesamtkosten

### Minimalkonfiguration
- ESP32 Board: 10 €
- Relais-Modul: 4 €
- Gardena Ventil: 30 €
- 24V Netzteil: 20 €
- USB-Netzteil: 7 €
- Verbindungsmaterial: 10 €
- **Gesamt: ca. 81 €**

### Komfort-Konfiguration (mit Gehäuse)
- Minimalkonfiguration: 81 €
- Wasserdichtes Gehäuse: 15 €
- Kabelverschraubungen: 4 €
- **Gesamt: ca. 100 €**

### Vollausstattung (alle Optionen)
- Komfort-Konfiguration: 100 €
- Bodenfeuchtesensor: 7 €
- Durchflusssensor: 12 €
- OLED Display: 6 €
- Taster: 3 €
- Buck Converter: 4 €
- **Gesamt: ca. 132 €**

## Spezifikationen der Hauptkomponenten

### ESP32 Development Board
- Prozessor: Dual-Core 240MHz
- WLAN: 802.11 b/g/n
- GPIO: Min. 25 nutzbare Pins
- Spannung: 5V USB oder 3.3V

### Relais-Modul Anforderungen
- Steuerspannung: 5V
- Optokoppler-Isolation
- Schaltstrom: Min. 2A bei 24V AC
- LED-Statusanzeige

### Gardena Ventil Kompatibilität
Folgende Gardena Ventile sind kompatibel:
- Gardena 1278-20 (24V AC)
- Gardena 1251-20 (9V DC mit Batteriefach)
- Gardena Bewässerungsventil 24V (Art. 1278)

## Bezugsquellen

### Online-Shops
- **AZ-Delivery**: ESP32, Sensoren, Displays
- **Amazon**: Schnelle Lieferung, große Auswahl
- **Reichelt**: Professionelle Komponenten
- **Conrad**: Netzteile, Gehäuse

### Lokale Geschäfte
- **Baumarkt**: Gardena Ventile, Gehäuse
- **Elektronik-Fachhandel**: Spezialkomponenten

## Hinweise

1. **Gardena Ventil-Typ**: Prüfen Sie vor dem Kauf, ob Sie ein 24V AC oder 9V DC Ventil benötigen
2. **Netzteil-Leistung**: 20VA reicht für ein Ventil, bei mehreren Ventilen entsprechend mehr
3. **Gehäuse-Größe**: Mindestens 120x80x55mm für alle Komponenten
4. **Kabel-Querschnitt**: 0.75mm² für Ventil-Anschluss ausreichend

## Spar-Tipps

- ESP32 und Elektronikkomponenten im Set kaufen
- Generische Relais-Module statt Markenprodukte
- Vorhandene USB-Netzteile verwenden
- Bei mehreren Ventilen: Gemeinsames 24V Netzteil mit höherer Leistung