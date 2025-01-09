#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

#include "index.h"

#define LED_BLUE 19      // LED azul GPIO 19
#define LED_RED 18       // LED rojo GPIO 16
#define BUZZER_PIN 16     // Zumbador GPIO 5
#define DHTPIN 17         // Sensor de temperatura GPIO 4
#define DHTTYPE DHT11

const char *ssid = "AP_asix";
const char *password = "AP_asix2023";

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
AsyncWebServer server(80);
DHT dht(DHTPIN, DHTTYPE);

float targetTemp = 0;   // Temperatura objetivo
unsigned long timerDuration = 0;  // Tiempo objetivo en ms
unsigned long cookTimerStart = 0;     // Marca de tiempo inicial
bool isCooking = false;           // Estado del temporizador

String mode = "IDLE";

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

float readTemperatureWithRetry() {
  float temp;
  for (int i = 0; i < 5; i++) {  // Reintenta hasta 5 veces
    temp = dht.readTemperature();
    if (!isnan(temp)) return temp;  // Devuelve la lectura válida
    delay(500);  // Espera antes de reintentar
  }
  return NAN;  // Si todas las lecturas fallan
}

void checkTemperature() {
  float currentTemp = readTemperatureWithRetry();
  if (isnan(currentTemp)) return;

  if (currentTemp >= targetTemp && targetTemp > 0) {
    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_BLUE, LOW);
    tone(BUZZER_PIN, 1000);
    updateOLED("ALERTA: Temp alcanzada!");
  } else {
    digitalWrite(LED_BLUE, HIGH);
    digitalWrite(LED_RED, LOW);
    noTone(BUZZER_PIN);
    updateOLED("Temp: " + String(currentTemp) + " C");
  }
}

void handleCookTimer() {
  static unsigned long lastBlinkTime = 0; // Marca de tiempo para el parpadeo
  static bool ledState = false;          // Estado del LED rojo
  static bool alertDone = false;         // Bandera para evitar múltiples pitidos

  if (!isCooking) return;

  unsigned long elapsed = millis() - cookTimerStart;
  unsigned long remaining = timerDuration > elapsed ? timerDuration - elapsed : 0;

  if (remaining == 0) {
    if (!alertDone) {
      alertDone = true;
      isCooking = false; // Detiene el temporizador
      tone(BUZZER_PIN, 1000, 3000); // Pita durante 3 segundos
      updateOLED("Tiempo Finalizado!");
    }

    // Parpadeo no bloqueante del LED rojo
    if (millis() - lastBlinkTime > 500) { // Cambiar estado cada 500 ms
      lastBlinkTime = millis();
      ledState = !ledState;
      digitalWrite(LED_RED, ledState);
    }
  } else {
    // Resetea la bandera si el temporizador aún no ha terminado
    alertDone = false;
    updateOLED("Cuenta atras: " + String(remaining / 1000) + "s");
  }
}



String getHTML() {
  String html = webpage;                                  
  return html;
}

void setup() {
  Serial.begin(9600);
  dht.begin();
  pinMode(LED_BLUE, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(LED_BLUE, LOW);
  digitalWrite(LED_RED, LOW);
  
  if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED no encontrado");
    while (1);
  }
  updateOLED("Esperando...");

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando...");
  }
  Serial.println("Conectado");

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
  request->send(200, "text/html", getHTML());
});

  server.on("/thermostat", HTTP_GET, [](AsyncWebServerRequest *request) {
    if (request->hasParam("temp")) {
      targetTemp = request->getParam("temp")->value().toFloat();
      mode = "TERMOSTATO";
      updateOLED("Modo Termostato: " + String(targetTemp) + "C");
    }
    request->send(200, "text/plain", "Modo Termostato activado");
  });

  server.on("/cook", HTTP_GET, [](AsyncWebServerRequest *request) {
    if (request->hasParam("time")) {
      timerDuration = request->getParam("time")->value().toInt() * 60000;
      cookTimerStart = millis();
      isCooking = true;
      mode = "COOK";
      updateOLED("Modo Cook: " + String(timerDuration / 60000) + " min");
    }
    request->send(200, "text/plain", "Modo Cook activado");
  });

  server.begin();
  updateOLED("Servidor listo");
  Serial.println(WiFi.localIP());
}

void loop() {
  if (mode == "TERMOSTATO") checkTemperature();
  if (mode == "COOK") handleCookTimer();
}
