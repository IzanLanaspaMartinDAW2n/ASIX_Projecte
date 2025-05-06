#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <HTTPClient.h> 
#include "index.h"
#include <vector>

// Pines y configuraciones
#define LED_BLUE 19         // LED azul GPIO 19
#define LED_RED 18          // LED rojo GPIO 18
#define BUZZER_PIN 16       // Zumbador GPIO 16
#define DHTPIN 17           // Sensor de temperatura GPIO 17
#define DHTTYPE DHT11

// Credenciales WiFi
const char *ssid = "AP_asix";
const char *password = "AP_asix2023";

// Pantalla OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Servidor asíncronoString cameraIP
AsyncWebServer server(80);

// Sensor de temperatura
DHT dht(DHTPIN, DHTTYPE);

// 
std::vector<String> logs;

// Variables globales
String cameraIP = "";            // Dirección IP de la cámara (configurable)
float targetTemp = 0;            // Temperatura objetivo
unsigned long timerDuration = 0; // Duración del temporizador (en ms)
unsigned long cookTimerStart = 0; 
bool isCooking = false;          // Estado del temporizador
String mode = "IDLE";            // Estado actual: IDLE, TERMOSTATO, COOK

// -----------------------------------------------------------------------------
// Función para actualizar la pantalla OLED con un mensaje
// -----------------------------------------------------------------------------
void updateOLED(String message) {
  oled.clearDisplay();
  oled.setTextSize(1);
  oled.setTextColor(WHITE);
  oled.setCursor(0, 10);
  oled.println("COOKTECH");
  oled.setCursor(0, 30);
  oled.println(message);
  oled.display();
}

// -----------------------------------------------------------------------------
// Función para leer la temperatura con varios reintentos
// -----------------------------------------------------------------------------
float readTemperatureWithRetry() {
  float temp;
  for (int i = 0; i < 5; i++) {
    temp = dht.readTemperature();
    if (!isnan(temp)) {
      return temp;  // Devuelve la lectura válida
    }
    delay(500);     // Espera antes de reintentar
  }
  return NAN;       // Si todas las lecturas fallan
}

float readHumidityWithRetry() {
  float hum;
  for (int i = 0; i < 5; i++) {
    hum = dht.readHumidity();
    if (!isnan(hum)) return hum;
    delay(500);
  }
  return NAN;
}


// -----------------------------------------------------------------------------
// Función que controla el "Modo Termostato"
// -----------------------------------------------------------------------------
void checkTemperature() {
  float currentTemp = readTemperatureWithRetry();
  if (isnan(currentTemp)) return; // Si falla la lectura, no hace nada

  if (currentTemp >= targetTemp && targetTemp > 0) {
    // Temperatura objetivo alcanzada o superada
    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_BLUE, LOW);
    buzz(1000, 500); // Alerta sonora mientras se mantenga alta
    updateOLED("ALERTA: Temp alcanzada!");
  } else {
    // Aún no se llega a la temperatura objetivo
    digitalWrite(LED_BLUE, HIGH);
    digitalWrite(LED_RED, LOW);
    noTone(BUZZER_PIN);
    updateOLED("Temp: " + String(currentTemp) + " C");
  }
}

// -----------------------------------------------------------------------------
// Función que controla el "Modo COOK" (temporizador)
// -----------------------------------------------------------------------------
void handleCookTimer() {
  static unsigned long lastBlinkTime = 0; 
  static bool ledState = false;          
  static bool alertDone = false;        

  if (!isCooking) return; // Solo opera si está en modo cocción

  unsigned long elapsed = millis() - cookTimerStart;
  unsigned long remaining = (timerDuration > elapsed) ? (timerDuration - elapsed) : 0;

  if (remaining == 0) {
    // Se agotó el temporizador
    if (!alertDone) {
      alertDone = true;
      isCooking = false; 
      playMelody(); // Pita 3 segundos
      updateOLED("Tiempo Finalizado!");
      logs.push_back("[COOK] Fin del temporizador");
    }
    // Parpadeo del LED rojo para indicar fin de cocción
    if (millis() - lastBlinkTime > 500) { 
      lastBlinkTime = millis();
      ledState = !ledState;
      digitalWrite(LED_RED, ledState);
    }
  } else {
    // Aún está contando el tiempo
    alertDone = false;
    updateOLED("Cuenta atras: " + String(remaining / 1000) + "s");
  }
}

// -----------------------------------------------------------------------------
// Devuelve la página web principal con los formularios
// -----------------------------------------------------------------------------
String getHTML() {
  float t = readTemperatureWithRetry();
  float h = readHumidityWithRetry();
  String html = webpage;
  html.replace("%TEMPERATURE%", isnan(t) ? "--" : String(t,1));
  html.replace("%HUMIDITY%",    isnan(h) ? "--" : String(h,1));
  return html;
}

void buzz(uint16_t freq, uint16_t ms) {
  tone(BUZZER_PIN, freq, ms);
  delay(ms);
  noTone(BUZZER_PIN);
}

const uint16_t melody[] = {262, 294, 330, 349};
void playMelody() {
  for (uint8_t i = 0; i < 4; i++) {
    buzz(melody[i], 200);
    delay(80);
  }
}

// -----------------------------------------------------------------------------
// SETUP: Inicializa WiFi, OLED, servidor y endpoints
// -----------------------------------------------------------------------------
void setup() {
  Serial.begin(9600);
  dht.begin();
  
  // Configurar pines como salida
  pinMode(LED_BLUE, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(LED_BLUE, LOW);
  digitalWrite(LED_RED, LOW);

  // Inicializar pantalla OLED
  if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED no encontrado");
    while (1);
  }
  updateOLED("Esperando...");

  // Conexión WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando...");
  }
  Serial.println("Conectado a WiFi");
  logs.push_back("[WIFI] Conectado a red " + String(ssid));
  updateOLED("Conectado a WiFi");

  // -----------------------------------------------------------------------------
  // Rutas / Endpoints del servidor
  // -----------------------------------------------------------------------------
  
  // Página de inicio
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/html", getHTML());
  });

  // Modo Termostato
  server.on("/thermostat", HTTP_GET, [](AsyncWebServerRequest *request) {
    if (request->hasParam("temp")) {
      targetTemp = request->getParam("temp")->value().toFloat();
      mode = "TERMOSTATO";
      updateOLED("Modo Termostato: " + String(targetTemp) + "C");
      logs.push_back("[THERMO] Objetivo: " + String(targetTemp, 1) + "C");
    }
    request->send(200, "text/plain", "Modo Termostato activado");
  });

  // Modo Cocción
  server.on("/cook", HTTP_GET, [](AsyncWebServerRequest *request) {
    if (request->hasParam("time")) {
      timerDuration = request->getParam("time")->value().toInt() * 60000; 
      cookTimerStart = millis();
      isCooking = true;
      mode = "COOK";
      updateOLED("Modo Cook: " + String(timerDuration / 60000) + " min");
      logs.push_back("[COOK] Timer: " + String(timerDuration / 60000) + " min");
    }
    request->send(200, "text/plain", "Modo Cook activado");
  });

  // Configurar la IP de la cámara dinámicamente
  server.on("/set_camera", HTTP_GET, [](AsyncWebServerRequest *request) {
    if (request->hasParam("ip")) {
      cameraIP = request->getParam("ip")->value();
      logs.push_back("[CAM] IP configurada: " + cameraIP);
      updateOLED("IP Camara: " + cameraIP);
      Serial.println("IP Cámara configurada: " + cameraIP);
    }
    request->redirect("/");
  });

  // Endpoint para capturar una foto desde la cámara configurada
 server.on("/camera", HTTP_GET, [](AsyncWebServerRequest *request) {
    if (cameraIP == "") {
        request->redirect("/");
        return;
    }

    // Construir la URL para capturar la imagen
    String captureURL = "http://" + cameraIP + "/capture";

    HTTPClient http;
    http.begin(captureURL);
    int httpCode = http.GET(); // Realizar la solicitud GET a la cámara

    if (httpCode == 200) {
        // Longitud del contenido (si está disponible)
        int len = http.getSize();
        WiFiClient *client = http.getStreamPtr();

        if (len > 0) {
            // Crear una respuesta con longitud fija
            AsyncWebServerResponse *response = request->beginResponse(
                "image/jpeg", len, 
                [client, len](uint8_t *buffer, size_t maxLen, size_t index) -> size_t {
                    size_t available = len - index;
                    if (available > maxLen) {
                        available = maxLen;
                    }
                    size_t bytesRead = client->readBytes(buffer, available);
                    return bytesRead;
                }
            );

            // Enviar la imagen como respuesta
            response->addHeader("Content-Disposition", "inline; filename=capture.jpg");
            request->send(response);
        } else {
            // Si no se conoce el tamaño, leer y reenviar en tiempo real (chunked)
            AsyncWebServerResponse *response = request->beginChunkedResponse(
                "image/jpeg",
                [client](uint8_t *buffer, size_t maxLen, size_t index) -> size_t {
                    if (!client->available()) {
                        return 0; // Finaliza la transmisión
                    }
                    return client->read(buffer, maxLen);
                }
            );
            response->addHeader("Content-Disposition", "inline; filename=capture.jpg");
            request->send(response);
        }
    } else {
        // Si la cámara no respondió con éxito
        request->redirect("/");
    }
    logs.push_back("[CAM] Foto servida desde " + cameraIP);
    http.end(); // Finaliza la conexión HTTP
});

server.on("/logs", HTTP_GET, [](AsyncWebServerRequest *request){
  String out = "<html><head><style>body{font-family:sans-serif;padding:20px;}li{margin:6px 0;}</style></head><body><h2>LOGS</h2><ul>";
  for (auto &l : logs) out += "<li>" + l + "</li>";
  out += "</ul><a href='/'>volver</a></body></html>";
  request->send(200, "text/html", out);
});


  // Iniciar servidor
  server.begin();
  updateOLED("Servidor listo");
  Serial.print("IP del ESP32: ");
  Serial.println(WiFi.localIP());
}

// -----------------------------------------------------------------------------
// LOOP: Manejo de modos TERMO o COOK
// -----------------------------------------------------------------------------
void loop() {
  if (mode == "TERMOSTATO") {
    checkTemperature();
  }
  if (mode == "COOK") {
    handleCookTimer();
  }
}
