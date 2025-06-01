#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include <time.h>
#include <Preferences.h>

// WiFi-Konfiguration
const char* default_ssid = "YOUR_WIFI_SSID";
const char* default_password = "YOUR_WIFI_PASSWORD";

// Pin-Definitionen
const int VALVE_PIN = 16;  // GPIO16 für Ventil-Relais
const int LED_STATUS = 2;  // Onboard LED für Status

// Webserver auf Port 80
WebServer server(80);

// Preferences für persistente Speicherung
Preferences preferences;

// Globale Variablen
bool valveState = false;
unsigned long valveOnTime = 0;
unsigned long scheduledDuration = 0;
String lastActivation = "Nie";

// NTP-Server für Zeitstempel
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 3600;  // GMT+1 für Deutschland
const int daylightOffset_sec = 3600;  // Sommerzeit

// HTML-Seite
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>
<head>
  <title>Gardena Ventil Steuerung</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    body { 
      font-family: Arial; 
      text-align: center; 
      margin: 0px auto; 
      padding: 20px;
      background-color: #f0f0f0;
    }
    .container {
      max-width: 600px;
      margin: 0 auto;
      background-color: white;
      padding: 20px;
      border-radius: 10px;
      box-shadow: 0 0 10px rgba(0,0,0,0.1);
    }
    h1 { color: #333; }
    .status {
      font-size: 1.2em;
      padding: 10px;
      margin: 10px;
      border-radius: 5px;
    }
    .on { background-color: #4CAF50; color: white; }
    .off { background-color: #f44336; color: white; }
    button {
      padding: 15px 32px;
      font-size: 16px;
      margin: 10px;
      cursor: pointer;
      border: none;
      border-radius: 5px;
      transition: 0.3s;
    }
    .btn-on { background-color: #4CAF50; color: white; }
    .btn-off { background-color: #f44336; color: white; }
    .btn-timer { background-color: #2196F3; color: white; }
    button:hover { opacity: 0.8; }
    input[type="number"] {
      padding: 10px;
      font-size: 16px;
      width: 80px;
      margin: 5px;
    }
    .info {
      margin-top: 20px;
      padding: 15px;
      background-color: #e7f3ff;
      border-radius: 5px;
    }
  </style>
</head>
<body>
  <div class="container">
    <h1>Gardena Bewässerungsventil</h1>
    <div id="status" class="status">Status: Lädt...</div>
    
    <div>
      <button class="btn-on" onclick="toggleValve(true)">Ventil Öffnen</button>
      <button class="btn-off" onclick="toggleValve(false)">Ventil Schließen</button>
    </div>
    
    <div style="margin-top: 20px;">
      <h3>Timer-Funktion</h3>
      <input type="number" id="duration" min="1" max="240" value="30">
      <label>Minuten</label><br>
      <button class="btn-timer" onclick="startTimer()">Timer Starten</button>
    </div>
    
    <div class="info">
      <h3>Informationen</h3>
      <p><strong>Letzte Aktivierung:</strong> <span id="lastActivation">-</span></p>
      <p><strong>Verbleibende Zeit:</strong> <span id="remainingTime">-</span></p>
      <p><strong>WiFi Signal:</strong> <span id="rssi">-</span> dBm</p>
    </div>
  </div>

  <script>
    function updateStatus() {
      fetch('/status')
        .then(response => response.json())
        .then(data => {
          const statusDiv = document.getElementById('status');
          statusDiv.textContent = 'Status: ' + (data.valve ? 'OFFEN' : 'GESCHLOSSEN');
          statusDiv.className = 'status ' + (data.valve ? 'on' : 'off');
          
          document.getElementById('lastActivation').textContent = data.lastActivation;
          document.getElementById('rssi').textContent = data.rssi;
          
          if (data.remainingMinutes > 0) {
            document.getElementById('remainingTime').textContent = data.remainingMinutes + ' Minuten';
          } else {
            document.getElementById('remainingTime').textContent = '-';
          }
        })
        .catch(err => console.error('Error:', err));
    }

    function toggleValve(state) {
      fetch('/valve', {
        method: 'POST',
        headers: {'Content-Type': 'application/json'},
        body: JSON.stringify({state: state})
      })
      .then(response => response.json())
      .then(data => {
        updateStatus();
      })
      .catch(err => console.error('Error:', err));
    }

    function startTimer() {
      const duration = document.getElementById('duration').value;
      fetch('/timer', {
        method: 'POST',
        headers: {'Content-Type': 'application/json'},
        body: JSON.stringify({duration: parseInt(duration)})
      })
      .then(response => response.json())
      .then(data => {
        updateStatus();
      })
      .catch(err => console.error('Error:', err));
    }

    // Update status every 5 seconds
    setInterval(updateStatus, 5000);
    updateStatus();
  </script>
</body>
</html>
)rawliteral";

void setup() {
  Serial.begin(115200);
  Serial.println("Gardena Ventil Steuerung startet...");

  // Pin-Konfiguration
  pinMode(VALVE_PIN, OUTPUT);
  pinMode(LED_STATUS, OUTPUT);
  digitalWrite(VALVE_PIN, LOW);
  digitalWrite(LED_STATUS, LOW);

  // Preferences initialisieren
  preferences.begin("gardena", false);
  
  // WiFi verbinden
  connectToWiFi();

  // Zeit konfigurieren
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  // Webserver-Routen einrichten
  setupWebServer();
  
  server.begin();
  Serial.println("HTTP server gestartet");
}

void loop() {
  server.handleClient();
  
  // Timer-Überprüfung
  if (valveState && scheduledDuration > 0) {
    unsigned long elapsed = (millis() - valveOnTime) / 1000 / 60; // in Minuten
    if (elapsed >= scheduledDuration) {
      setValveState(false);
      scheduledDuration = 0;
    }
  }
  
  // Status-LED
  digitalWrite(LED_STATUS, valveState ? HIGH : LOW);
  
  delay(100);
}

void connectToWiFi() {
  // Versuche gespeicherte Credentials zu laden
  String ssid = preferences.getString("ssid", default_ssid);
  String password = preferences.getString("password", default_password);
  
  WiFi.begin(ssid.c_str(), password.c_str());
  Serial.print("Verbinde mit WiFi");
  
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi verbunden!");
    Serial.print("IP-Adresse: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nWiFi-Verbindung fehlgeschlagen!");
    // Hier könnte ein AP-Modus für Konfiguration gestartet werden
  }
}

void setupWebServer() {
  // Hauptseite
  server.on("/", HTTP_GET, []() {
    server.send(200, "text/html", index_html);
  });
  
  // Status-Endpoint
  server.on("/status", HTTP_GET, []() {
    StaticJsonDocument<200> doc;
    doc["valve"] = valveState;
    doc["lastActivation"] = lastActivation;
    doc["rssi"] = WiFi.RSSI();
    
    if (valveState && scheduledDuration > 0) {
      unsigned long elapsed = (millis() - valveOnTime) / 1000 / 60;
      doc["remainingMinutes"] = scheduledDuration - elapsed;
    } else {
      doc["remainingMinutes"] = 0;
    }
    
    String response;
    serializeJson(doc, response);
    server.send(200, "application/json", response);
  });
  
  // Ventil-Steuerung
  server.on("/valve", HTTP_POST, []() {
    if (server.hasArg("plain")) {
      StaticJsonDocument<100> doc;
      DeserializationError error = deserializeJson(doc, server.arg("plain"));
      
      if (!error) {
        bool state = doc["state"];
        setValveState(state);
        scheduledDuration = 0; // Timer zurücksetzen
        
        server.send(200, "application/json", "{\"success\":true}");
      } else {
        server.send(400, "application/json", "{\"error\":\"Invalid JSON\"}");
      }
    }
  });
  
  // Timer-Funktion
  server.on("/timer", HTTP_POST, []() {
    if (server.hasArg("plain")) {
      StaticJsonDocument<100> doc;
      DeserializationError error = deserializeJson(doc, server.arg("plain"));
      
      if (!error) {
        int duration = doc["duration"];
        if (duration > 0 && duration <= 240) { // Max 4 Stunden
          setValveState(true);
          valveOnTime = millis();
          scheduledDuration = duration;
          
          server.send(200, "application/json", "{\"success\":true}");
        } else {
          server.send(400, "application/json", "{\"error\":\"Invalid duration\"}");
        }
      } else {
        server.send(400, "application/json", "{\"error\":\"Invalid JSON\"}");
      }
    }
  });
  
  // WiFi-Konfiguration
  server.on("/wifi", HTTP_POST, []() {
    if (server.hasArg("plain")) {
      StaticJsonDocument<200> doc;
      DeserializationError error = deserializeJson(doc, server.arg("plain"));
      
      if (!error) {
        String ssid = doc["ssid"];
        String password = doc["password"];
        
        preferences.putString("ssid", ssid);
        preferences.putString("password", password);
        
        server.send(200, "application/json", "{\"success\":true,\"message\":\"WiFi config saved. Restarting...\"}");
        delay(1000);
        ESP.restart();
      }
    }
  });
}

void setValveState(bool state) {
  valveState = state;
  digitalWrite(VALVE_PIN, state ? HIGH : LOW);
  
  if (state) {
    // Zeitstempel aktualisieren
    struct tm timeinfo;
    if (getLocalTime(&timeinfo)) {
      char buffer[30];
      strftime(buffer, 30, "%d.%m.%Y %H:%M:%S", &timeinfo);
      lastActivation = String(buffer);
    } else {
      lastActivation = "Zeit nicht verfügbar";
    }
  }
  
  Serial.print("Ventil: ");
  Serial.println(state ? "OFFEN" : "GESCHLOSSEN");
}